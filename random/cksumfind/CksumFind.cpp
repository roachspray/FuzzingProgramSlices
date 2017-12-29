#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CallSite.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#include "CksumFind.h"
void
CksumFind::getAnalysisUsage(AnalysisUsage &AU) const
{
	AU.setPreservesCFG();
}
bool
CksumFind::runOnModule(Module &M)
{
	std::vector<Function *> cksumFunctions;

	for (Function &F : M) {
		if (!F.hasName()) {
			continue;
		}
		std::string fn = F.getName().str();
		size_t loc = fn.find("cksum");
		if (loc != std::string::npos) {
			cksumFunctions.push_back(&F);
			continue;
		}
		loc = fn.find("checksum");
		if (loc != std::string::npos) {
			cksumFunctions.push_back(&F);
			continue;
		}
	}
	if (cksumFunctions.empty()) {
		errs() << "No checksum functions found\n";
		return false;
	}
	for (auto it = cksumFunctions.begin(); it != cksumFunctions.end();
	  ++it) {
		Function *f = *it;
		for (auto ui = f->user_begin(); ui != f->user_end(); ++ui) {
			User *u = *ui;
			Instruction *ii = cast<Instruction>(u);
			Function *fp = ii->getParent()->getParent();
			if (fp->hasName()) {
				errs() << "checksum found in function: " << fp->getName().str() << "\n";
			} else {
				errs() << "checksum found in unnamed function\n";
			}
			u->dump();
		}
	}
	return false;
}
char CksumFind::ID = 0;
static RegisterPass<CksumFind> XX("cksum-find", "");
