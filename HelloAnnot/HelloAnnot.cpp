#include <fstream>
#include <map>
#include <string>
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

namespace {

//local copy while linking issues are resolved
vector<pair<int,int> > getStringParts(const string& formatted_string, char delim) {
  vector<pair<int,int> > res;
  int i, last_i;
  last_i = -1;
  for (i=0; i<formatted_string.length(); i++) {
    if (formatted_string[i] == delim) {
      res.push_back(make_pair(last_i+1, i-last_i-1));
      last_i = i;
    }
  }
  res.push_back(make_pair(last_i+1, i-last_i-1));
  return res;
}

bool partStringEqual(const string& formatted_string, pair<int,int> start_len_pair, const string compare_to) {
  return !formatted_string.compare(start_len_pair.first, start_len_pair.second, compare_to);
}

string partSubstring(const string& formatted_string, pair<int,int> start_len_pair) {
  return formatted_string.substr(start_len_pair.first, start_len_pair.second);
}
//end of local copy

bool isGroup(const string& formatted_string) {
  vector<pair<int, int> > parts = getStringParts(formatted_string, ':');
  return partStringEqual(formatted_string, parts[1], "group");
}

bool isFunctionFreq(const string& formatted_string) {
  vector<pair<int, int> > parts = getStringParts(formatted_string, ':');
  return partStringEqual(formatted_string, parts[1], "call_freq");
}

string getGroupName(const string& formatted_string) {
  vector<pair<int, int> > parts = getStringParts(formatted_string, ':');
  return partSubstring(formatted_string, parts[2]);
}

string getFunctFreqParams(const string& formatted_string) {
  vector<pair<int, int> > parts = getStringParts(formatted_string, ':');
  if (parts.size() < 4) {
    errs() << "HelloAnnot: " << "Function frequency annotation expected, but not received.\n";
  }
  return formatted_string.substr(parts[3].first);
}

struct Hello : public ModulePass {
  static char ID;
  Hello() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    ifstream loom_inst_basic_policy;
    ofstream loom_inst_policy;
    loom_inst_basic_policy.open ("basic.policy");
    loom_inst_policy.open ("loom_inst.policy");
    char c;
    //Copy base of our loom policy into a new loom policy
    while (!loom_inst_basic_policy.eof()) {
      loom_inst_basic_policy.get(c);
      loom_inst_policy << c;
    }
    string ta_prefix = "TA_ASSERT";
    string ta_instrument_anno = "TA_INSTRUMENT";
    bool functions_instrumented = false;
    
    map<string, string> group_to_param;

    //Look through global constants to look for function annotations
    auto global_annos = M.getNamedGlobal("llvm.global.annotations");
    if (global_annos) {
      auto a = cast<ConstantArray>(global_annos->getOperand(0));
      for (unsigned int i=0; i<a->getNumOperands(); i++) {
	      auto e = cast<ConstantStruct>(a->getOperand(i));
      	if (auto fn = dyn_cast<Function>(e->getOperand(0)->getOperand(0))) {
          auto anno = cast<ConstantDataArray>(cast<GlobalVariable>(
      	  e->getOperand(1)->getOperand(0))->getOperand(0))->getAsCString();

      	  //Check whether it starts with our prefix
      	  string std_anno = anno.str();
      	  if (std_anno.compare(0, ta_prefix.length(), ta_prefix) == 0) {
            functions_instrumented = true;
            if (isGroup(std_anno)) {
              //Skip, since we lack information contained in function_freq
            } else {
              if (isFunctionFreq(std_anno)) {
                errs() << "HelloAnnot: found call_freq: " << getFunctFreqParams(std_anno) << "\n";
                group_to_param[getGroupName(std_anno)] = getFunctFreqParams(std_anno);
              }
              fn->addFnAttr(ta_instrument_anno, anno);
            }
          }
        }
      }
      
      for (unsigned int i=0; i<a->getNumOperands(); i++) {
	      auto e = cast<ConstantStruct>(a->getOperand(i));
      	if (auto fn = dyn_cast<Function>(e->getOperand(0)->getOperand(0))) {
          auto anno = cast<ConstantDataArray>(cast<GlobalVariable>(
      	  e->getOperand(1)->getOperand(0))->getOperand(0))->getAsCString();

      	  //Look for the group annotations we skipped the first time
      	  string std_anno = anno.str();
      	  if (std_anno.compare(0, ta_prefix.length(), ta_prefix) == 0) {
            if (isGroup(std_anno)) {
              string enhanced_anno = std_anno + ":" + group_to_param[getGroupName(std_anno)];
              errs() << "HelloAnnot: enhanced_anno: " << enhanced_anno << "\n";
              StringRef enhanced_anno_ref(enhanced_anno);
              fn->addFnAttr(ta_instrument_anno, enhanced_anno_ref);
            }
          }
        }
      }

    }
    if (functions_instrumented) {
        loom_inst_policy << "functions:\n";
    }
    
    //Manage function annotations - generate policies
    for (auto cur_fref = M.getFunctionList().begin(),
         end_fref = M.getFunctionList().end();
         cur_fref != end_fref; ++cur_fref) {
      //errs() << "HelloAnnot: funct " << cur_fref->getName() << "\n";
      if (cur_fref->hasFnAttribute(ta_instrument_anno)) {
        errs() << "HelloAnnot: " << cur_fref->getName() << "\n";
        loom_inst_policy << "    - name: " << cur_fref->getName().str() << "\n";
        loom_inst_policy << "      caller: [ entry ]" << "\n";
      }
    }
    
    //Part prepared for variable annotation below, which ended up not being used
    /*map<Value*, DbgDeclareInst*> value_to_dbg_dec;
    bool vars_instrumented = false;

    for (auto cur_f = M.getFunctionList().begin(),
         end_f = M.getFunctionList().end();
         cur_f != end_f; ++cur_f) {
      errs() << "HelloAnnot: going over " << cur_f->getName() << "\n";
      for (auto cur_b = cur_f->getBasicBlockList().begin(),
           end_b = cur_f->getBasicBlockList().end();
           cur_b != end_b; ++cur_b) {
        for (auto cur_i = cur_b->getInstList().begin(),
             end_i = cur_b->getInstList().end();
             cur_i != end_i; ++cur_i) {
          if (auto cur_dbg_dec = dyn_cast<DbgDeclareInst>(cur_i)) {
            errs() << "Found declaration: "
              << cur_dbg_dec->getVariable()->getName().str()
              << " on address " << (void*) cur_dbg_dec->getAddress() << "\n";
            value_to_dbg_dec[cur_dbg_dec->getAddress()] = cur_dbg_dec;
          }
          if (auto cur_call = dyn_cast<CallInst>(cur_i)) {
            errs() << "Found a call: ";
            errs() << cur_call->getCalledFunction()->getName() << "\n";
            if (cur_call->getCalledFunction()->getName() == "llvm.var.annotation") {
              errs() << "Found annotation on value " << (void*) cur_call->getArgOperand(0)->stripPointerCasts() << "\n";
              if (value_to_dbg_dec.find(cur_call->getArgOperand(0)->stripPointerCasts()) != value_to_dbg_dec.end()) {
                errs() << "Should annotate " << value_to_dbg_dec[cur_call->getArgOperand(0)->stripPointerCasts()]->getVariable()->getName().str() << "\n";
                if (auto gv = dyn_cast<ConstantExpr>(cur_call->getArgOperand(1))) {
                  string std_anno = cast<ConstantDataArray>(cast<GlobalVariable>(cast<ConstantExpr>(gv)->getOperand(0))->getInitializer())->getAsCString();
                  //if (std_anno.compare(0, ta_prefix.length(), ta_prefix) == 0) {
                  //            if (!vars_instrumented) {
                  //    loom_inst_policy << "structures:\n"
                  //    vars_instrumented = true;
                  //  }
                  //            fn->addFnAttr(ta_instrument_anno);
                  //            fn->addFnAttr(anno);
                  //  }
                  errs() << "with " << std_anno << "\n";
                }
              }
              //for (unsigned int i=0; i<cur_call->getNumArgOperands(); i++) {
              //    Value *op = cur_call->getArgOperand(i);
              //    errs() << " " << (op->hasName()? op->getName().str() : "noname"); 
              //}
              errs() << "\n";
            }
          }
          //errs() << cur_i->getOpcodeName() << "\n";
        }
      }
    }*/

    //errs() << "HelloAnnot: ";
    //errs().write_escaped(F.getName()) << '\n';
    loom_inst_basic_policy.close();
    loom_inst_policy.close();
    return false;
  }
}; // end of struct Hello
}  // end of anonymous namespace

char Hello::ID = 0;
static RegisterPass<Hello> X("hello_annot", "Generates policies fro Loom and adds som annotations",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);
