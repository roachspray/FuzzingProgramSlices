#ifndef _EXITBLOCKSPASS_H
#define	_EXITBLOCKSPASS_H

#include <list>

#include "BBlockRef.h"

struct ExitBlocksPass : ModulePass {
private:
	std::set<TraceEntry>	traceEntries;
	std::set<std::string>	prunedFunctions;

	std::list<BBlockRef>  keepBlocks;
	std::list<BBlockRef> removeBlocks;

	Module	*mod;

	std::string currentFocusFunctionName;
	Function *currentFocusFunction;

	unsigned parseFocusFuncForBlocks();
	bool setFocusFunc(unsigned d);

public:
	static char ID;
	ExitBlocksPass() : ModulePass(ID) {}

	virtual bool runOnModule(Module &);
//	virtual void getAnalysisUsage(AnalysisUsage &) const;

	void	setTraceFile(std::string f) { }
	void	setFocusFunction(std::string f) { currentFocusFunctionName = f; }

};
#endif
