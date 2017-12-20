#ifndef __BBLOCKREF_H
#define	__BBLOCKREF_H

/*
 * The idea here is that we have some container for
 * referring to basic blocks. To be honest, I have
 * not investigated any possible existing LLVM data
 * structures that may be of use, instead of doing
 * this. However, it seems to work for now.
 *
 */
class BBlockRef {
	bool removed;
	BasicBlock *blk;
	std::string fnName;
	int idx;

public:

	// FIXME: Bad fnName init.
	BBlockRef(BasicBlock *b, int ix) :
	  removed(false), blk(b), fnName(b->getParent()->getName().str()), idx(ix) { }
	BBlockRef(BasicBlock *b, int ix, unsigned r) : removed(false), blk(b),
	  fnName(b->getParent()->getName().str()), idx(ix) {}

	void setRemoved(bool v) {
		this->removed = v;
	}

	bool isRemoved() const {
		return this->removed;
	}

	BasicBlock *getBB() const {
		return this->blk;
	}

	std::string getFocusName() const {
		return this->fnName;
	}

	int getFocusIdx() const {
		return this->idx;
	}

	/*
	 * XXX: Verify I am even using this...I had it for some 
	 * container.
	 */
	bool operator < (const BBlockRef &rhs) const {
		return (unsigned long)blk < (unsigned long)rhs.getBB();
	}

	/*
	 * XXX: Verify I am even using this...I had it for some 
	 * container.
	 */
	bool operator == (const BBlockRef &rhs) const {
		return blk == rhs.getBB();
	}
};
#endif
