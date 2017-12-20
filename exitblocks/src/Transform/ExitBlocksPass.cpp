/*
 * Instead of removing blocks, just insert an exit(0)
 * as the first real instruction to those blocks. The
 * idea is that it tells the fuzzer that this is not
 * an interesting path. i think.
 */
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/CallSite.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

// Like i said.. ugly as fuhhhh
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

using namespace llvm;

#include "../TraceUtils.h"
#include "../CommonUtils.h"
#include "ExitBlocksPass.h"

static cl::opt<std::string> TraceLogFile("pskin-log-file",
  cl::desc("Specify trace log file"), cl::init("default_trace.log"),
  cl::Required);


static cl::opt<unsigned> PruneDepth("pskin-depth", 
  cl::desc("Specify the maximum depth to prune"), cl::init(3),
  cl::Required);

bool
ExitBlocksPass::runOnModule(Module &M)
{
	mod = &M;

	traceEntries = TraceUtils::ParseTraceFile(TraceLogFile);	
	if (traceEntries.size() == 0) {
		errs() << "No trace entries found in '" << TraceLogFile << "'\n";
		return false;
	}
	errs() << "Trace file entries: " << traceEntries.size() << "\n";

		
	Constant *exitFnC = mod->getOrInsertFunction("exit",
	  Type::getVoidTy(mod->getContext()),
	  Type::getInt32Ty(mod->getContext()),
	  NULL);
	if (exitFnC == NULL) {
		errs() << "Unable to insert 'void exit(int)' decl\n";
		return false;
	}
	Function *exitFn = cast<Function>(exitFnC);
	errs() << "\n\n\n";
	unsigned dep = PruneDepth;
	unsigned total_removed = 0;
	do {	
		unsigned removed_count = 0;
		if (setFocusFunc(dep) == false) {
			--dep;
			continue;
		}
		keepBlocks.clear();
		removeBlocks.clear();
		parseFocusFuncForBlocks();	
		for (auto rbi = removeBlocks.begin(); rbi != removeBlocks.end();
		  ++rbi) {
			BBlockRef *bbRef = const_cast<BBlockRef *>(&*rbi);
			BasicBlock *bb = bbRef->getBB();

			Instruction *firstInst = bb->getFirstNonPHI();
			Value *a[1];
			a[0] = Constant::getIntegerValue(Type::getInt32Ty(mod->getContext()),
			  APInt(32, 0, false));
			ArrayRef<Value *> fIdxs(a, 1);
			Value *vci = CallInst::Create(exitFn, fIdxs, "", firstInst);
//			vci->dump();
		}
//		currentFocusFunction->dump();
	} while (dep > 0);

	return true;
}

/*
 * For a given depth, find a function of that depth to set as 
 * currentFocusFunctionName. Put function into list so we can iterate 
 * through each.
 */
bool
ExitBlocksPass::setFocusFunc(unsigned dep)
{
	std::string nameofFocusFunction = "";
	Function *tmpFun = NULL;

	for (auto e = traceEntries.begin(); e != traceEntries.end(); ++e) {
		const TraceEntry *t = &*e;
		if (t->getDepth() == dep) { 
			nameofFocusFunction = t->getFunction();

			if (prunedFunctions.end() != prunedFunctions.find(nameofFocusFunction)) {
				nameofFocusFunction = "";
				continue;
			}
			tmpFun = mod->getFunction(nameofFocusFunction);
			if (tmpFun == NULL) {
				// Try for the next of same dep.
				errs() << "setFocusFunc: not found: "  \
				  << nameofFocusFunction << "\n";
				nameofFocusFunction = "";
				continue;
			}

			if (tmpFun->isDeclaration() || tmpFun->isIntrinsic()) {
				errs() << "setFocusFunc: skipping external: "  \
				  << nameofFocusFunction << "\n";
				tmpFun = NULL;
				nameofFocusFunction = "";
				continue;
			}
			break;
		}
	}
	if (nameofFocusFunction != "" && tmpFun != NULL) {
		currentFocusFunctionName = nameofFocusFunction;
		currentFocusFunction = tmpFun;
		prunedFunctions.insert(currentFocusFunctionName);
		return true;
	}
	return false;
}

unsigned
ExitBlocksPass::parseFocusFuncForBlocks()
{
	std::set<unsigned> blkNums;


	for (auto e = traceEntries.begin(); e != traceEntries.end(); ++e) {
		const TraceEntry *t = &*e;
		if (t->getFunction() == currentFocusFunctionName) {
			blkNums.insert(t->getBlock());
			continue;
		}
	}

	unsigned blkCnt = 0;
	for (BasicBlock &BB : *currentFocusFunction) {
		BasicBlock *BBp = &BB;
		++blkCnt;
		if (blkNums.end() != blkNums.find(blkCnt)) {
			keepBlocks.push_back(BBlockRef(BBp, blkCnt));

		} else {
			removeBlocks.push_back(BBlockRef(BBp, blkCnt));
		}
	}
	return blkCnt;
}
char ExitBlocksPass::ID = 0;
static RegisterPass<ExitBlocksPass> X("pskin-exit-blocks",
  "Most basicier");

