#ifndef	__TRACEUTILS_H
#define	__TRACEUTILS_H

/* 
 * Intended to represent a single line in a libtrays trace file.
 */
class TraceEntry {
	unsigned long	count;
	unsigned long	depth;
	std::string		function;
	unsigned		block;
	uint64_t		blockaddr;

public:
	TraceEntry(unsigned long c, unsigned long d, std::string f, unsigned b,
	  uint64_t ba) :
	  count(c), depth(d), function(f), block(b), blockaddr(ba) {}

	unsigned long getCount() const { return count; }
	unsigned long getDepth() const { return depth; }
	std::string	getFunction() const { return function; }
	unsigned getBlock() const { return block; }
	uint64_t getBlockAddr() const { return blockaddr; }

	std::string str() const {
		std::stringstream m;
		m << std::hex << blockaddr;
		std::string hv(m.str());
		return function + ":b" + std::to_string(block) + ":d" + \
		  std::to_string(depth) + ":a0x" + hv;
	}
	
	bool operator==(const TraceEntry &rhs) const {
		return count == rhs.getCount();
	}
	bool operator< (const TraceEntry &rhs) const {
		return count < rhs.getCount();
	}
};

/*
 * Static helper functions for dealing with libtrays 
 * trace files. Of course.... these are specific to my
 * version of libtrays, so really, should be some class
 * that is subclassed, etc.
 * *shrug*
 */
class TraceUtils {

public: 
	static std::set<TraceEntry> ParseTraceFile(std::string);
	static std::vector<std::map<std::string, std::string>>
	  DiffTraceForDivergentBlocks(std::string origTraceFile,
	  std::string sampleTraceFile,
	  unsigned precedingBlockCount,
	  unsigned *sameCount);

	static std::vector<const TraceEntry *> TraceByFunction(std::string,
	  std::set<TraceEntry>);
};
#endif
