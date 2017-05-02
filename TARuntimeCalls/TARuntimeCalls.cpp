#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

namespace {

pair<string, int> getNextStringPart(const string& formatted_string, char delim, int from_pos) {
  string res;
  int i;
  for (i=from_pos; i<formatted_string.length(); i++) {
    if (formatted_string[i] == delim) {
      break;
    }
    res.push_back(formatted_string[i]);
  }
  return make_pair(res, i);
}

int getArgNumFromFormat(string formatted_string) {
  pair<string, int> ta_const = getNextStringPart(formatted_string,':',0);
  pair<string, int> assert_type = getNextStringPart(formatted_string,':',ta_const.second+1);

  if (!assert_type.first.compare("arg_monotonic") || !assert_type.first.compare("arg_uniform")) {
    pair<string, int> arg_num = getNextStringPart(formatted_string,':',assert_type.second+1);
    return stoi(arg_num.first);
  }
  return -1;
}

//Neater version with linking issues
/*int getArgNumFromFormat(string formatted_string) {
  vector<pair<int, int> > parts = Util::getStringParts(formatted_string, ':');
  //ta_const, assert_type, (opt) arg_num
  if (Util::partStringEqual(formatted_string, parts[1], "arg_monotonic")) {
    string arg_num = Util::partSubstring(formatted_string, parts[2]);
    return stoi(arg_num);
  } 
  return -1;
}*/

struct TARuntimeCalls : public ModulePass {
  static char ID;
  TARuntimeCalls() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    string ta_prefix = "TA_ASSERT";
    string ta_instrument_anno = "TA_INSTRUMENT";
    string ta_instrument_exit_anno = "TA_INSTRUMENT_EXIT";
    StringRef ta_inst_anno_ref = StringRef(ta_instrument_anno);
    
    vector<pair<string, pair<string, bool> > > instr_stuff;
    ifstream stuff_to_instrument;
    string fun_name, fun_assert;

    stuff_to_instrument.open("stuff_to_instrument.ta");
    char c;
    while (!stuff_to_instrument.eof()) {
      fun_name.clear();
      fun_assert.clear();
      stuff_to_instrument.get(c);
      if (stuff_to_instrument.eof()) {
        break;
      }
      while (c != ' ') {
        fun_name.push_back(c);
        stuff_to_instrument.get(c);
      }
      stuff_to_instrument.get(c);
      while (!((c == '?')||(c == '!'))) {
        fun_assert.push_back(c);
        stuff_to_instrument.get(c);
      }
      instr_stuff.push_back(make_pair(fun_name, make_pair(fun_assert, (c=='!'))));
      stuff_to_instrument.get(c);
    }

    errs() << "RuntimeCalls: Parsed info from file:\n";
    for (int i=0; i<instr_stuff.size(); i++) {
      errs() << instr_stuff[i].first << " " << instr_stuff[i].second.first
          << (instr_stuff[i].second.second?"T":"F") << "\n";
    }

    //Manage function annotations - generate calls to assertion manager
    for (int k=0; k<instr_stuff.size(); k++) {
      string loom_fun_name = "__loom_call_" + instr_stuff[k].first;
      Function *loom_f = M.getFunction(loom_fun_name);

      if (loom_f) {
        errs() << "RuntimeCalls: " << instr_stuff[k].first << "\n";
                
        //Remove return from the generated function - we should add
	      //our instructions in its place
        BasicBlock &entry_block = loom_f->getEntryBlock();
        Instruction &last_inst = entry_block.back();
        last_inst.eraseFromParent();
        
        IRBuilder<> builder(&entry_block);
        
        //Function is external, so lets declare it
        LLVMContext& context = M.getContext();
        std::vector<llvm::Type *> newValue_args;
        newValue_args.push_back(Type::getInt8PtrTy(context));
        newValue_args.push_back(Type::getInt32Ty(context));
        FunctionType *newValue_type =
              FunctionType::get(Type::getVoidTy(context), newValue_args, false);
        Constant *newValue_fun =
              M.getOrInsertFunction("AssertManager_newValue", newValue_type);
        
        Value *hello_str = builder.CreateGlobalStringPtr(instr_stuff[k].first + ":" + instr_stuff[k].second.first);

        std::vector<llvm::Value *> fun_args;
        fun_args.push_back(hello_str);

        //Figure out which (if any) argument we want to pass along
        int arg_num = getArgNumFromFormat(instr_stuff[k].second.first);
        
        Value* x = ConstantInt::getSigned(Type::getInt32Ty(context), 0);
        if (arg_num >= 0) {
          Function::arg_iterator call_args = loom_f->arg_begin();
          for (int i=0; i<arg_num; i++) {
            call_args++;
          }
          x = &(*call_args);
        }

        fun_args.push_back(x);
        
        builder.CreateCall(newValue_fun, fun_args);
        builder.CreateRetVoid();
      }
      
      loom_fun_name = "__loom_return_" + instr_stuff[k].first;
      loom_f = M.getFunction(loom_fun_name);
      if (loom_f) {
        errs() << "RuntimeCalls: " << instr_stuff[k].first << " exit\n";
        
        //Remove return from the generated function - we should add
	      //our instructions in its place
        BasicBlock &entry_block = loom_f->getEntryBlock();
        Instruction &last_inst = entry_block.back();
        last_inst.eraseFromParent();
        
        IRBuilder<> builder(&entry_block);
        
        //Function is external, so lets declare it
        LLVMContext& context = M.getContext();
        std::vector<llvm::Type *> newValue_args;
        newValue_args.push_back(Type::getInt8PtrTy(context));
        FunctionType *newValue_type =
              FunctionType::get(Type::getVoidTy(context), newValue_args, false);
        Constant *newValue_fun =
              M.getOrInsertFunction("AssertManager_exitFunction", newValue_type);
        
        Value *hello_str = builder.CreateGlobalStringPtr(instr_stuff[k].first + ":" + instr_stuff[k].second.first);

        std::vector<llvm::Value *> fun_args;
        fun_args.push_back(hello_str);

        //Figure out which (if any) argument we want to pass along
        
        builder.CreateCall(newValue_fun, fun_args);
        builder.CreateRetVoid();
      }
    }
    
    return false;
  }
}; // end of struct TARuntimeCalls
}  // end of anonymous namespace

char TARuntimeCalls::ID = 0;
static RegisterPass<TARuntimeCalls> X("runtime_calls", "Temporal Assertions 2nd pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
