#ifndef __CKSUMFIND_H
#define	__CKSUMFIND_H

struct CksumFind : public ModulePass {
	static char ID;

	CksumFind() : ModulePass(ID) {}
	virtual bool runOnModule(Module &);
   	virtual void getAnalysisUsage(AnalysisUsage &) const;

private:
};

#endif
