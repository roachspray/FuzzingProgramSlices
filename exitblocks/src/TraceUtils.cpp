/*
 * TraceUtils.cpp
 *
 * Static helper methods for dealing with certain types of
 * trace files. Mostly just those specified in early
 * versions of libtrays. It is possible that we should 
 * jack this out and make it an interface or something to
 * implement for implementations of libtrays. I.e., the
 * two would be tied and thus reasonable for any changing
 * of libtrays trace methods.
 *
 */


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <map>
#include <set>
#include <vector>

#include "TraceUtils.h"

/*
 * origTraceFile - path to original trace log file 
 * sampleTraceFile - path to sample trace you diff against original
 * precedingBlockCount - how many blocks prior to diverge to include
 * sameCount - used to return # of initial trace entries are the same
 *
 * returns a vector of maps string to string... the last two are the
 * diverging blocks (i.e., second to last is reached by orig and last
 * is reached by sample). The number leading up to that will be 
 * dictated by precedingBlockCount
 *
 */
std::vector<std::map<std::string, std::string>>
TraceUtils::DiffTraceForDivergentBlocks(std::string origTraceFile,
  std::string sampleTraceFile, unsigned precedingBlockCount,
  unsigned *sameCount)
{
	std::vector<std::map<std::string, std::string>> vRet;
	unsigned scRet = 0;

	std::ifstream origHandle(origTraceFile);
	std::ifstream sampleHandle(sampleTraceFile);

	std::string predLine;
	std::vector<std::string> predLines;	// May blow up memory, but fuck it.
	std::string origLine;
	std::string sampleLine;

	while (1) {
		std::getline(origHandle, origLine);
		std::getline(sampleHandle, sampleLine);	

		std::string origCD;
		std::string origName;
		std::string origNum;
		std::string origAddr;
		std::string sampCD;
		std::string sampName;
		std::string sampNum;
		std::string sampAddr;

		std::stringstream origConv(origLine);
		origConv >> origCD >> origName >> origNum >> origAddr;
		std::stringstream sampConv(sampleLine);
		sampConv >> sampCD >> sampName >> sampNum >> sampAddr;

		// Must split because we will have different block addresses displayed
		if (origCD == sampCD && \
		  origName == sampName && \
		  origNum == sampNum) {
			predLine = origLine;
			predLines.push_back(origLine);
			++scRet;
			continue;
		}
		break;
	}
	if (sameCount != NULL) {
		*sameCount = scRet;
	}

	unsigned numPredLines = predLines.size();
	// Reduce if we wanted more than we can get..
	if (precedingBlockCount > numPredLines) {
		precedingBlockCount = numPredLines;
	}
	unsigned numToChuck = numPredLines - precedingBlockCount;
	
	auto pit = predLines.begin();
	while (numToChuck > 0) {
		++pit;
		--numToChuck;
	}

	std::string nCallDepth;
	std::string funcName;
	std::string blkNumI;
    std::string blkAddr;
	while (precedingBlockCount > 0) {	
		predLine = *pit;
		std::stringstream conv(predLine);
		conv >> nCallDepth >> funcName >> blkNumI >> blkAddr;
		std::map<std::string, std::string> m;
		m["depth"] = nCallDepth;
		m["function"] = funcName;
		m["block"] = blkNumI; 
		m["blockaddr"] = "0x" + blkAddr;
		vRet.push_back(m);	

		++pit;
		--precedingBlockCount;
	}
	std::stringstream c2(origLine);
	c2 >> nCallDepth >> funcName >> blkNumI >> blkAddr;
	std::map<std::string, std::string> m2;
	m2["depth"] = nCallDepth;
	m2["function"] = funcName;
	m2["block"] = blkNumI; 
	m2["blockaddr"] = "0x" + blkAddr;
	vRet.push_back(m2);

	std::stringstream c3(sampleLine);
	c3 >> nCallDepth >> funcName >> blkNumI >> blkAddr;
	std::map<std::string, std::string> m3;
	m3["depth"] = nCallDepth;
	m3["function"] = funcName;
	m3["block"] = blkNumI; 
	m3["blockaddr"] = "0x"+blkAddr;
	vRet.push_back(m3);

	return vRet;	
}

std::set<TraceEntry> 
TraceUtils::ParseTraceFile(std::string traceFile)
{
	std::ifstream fileHandle(traceFile);
	std::string entryFnBlkNm;
	std::set<TraceEntry> traceEntries;
	unsigned long count = 0;
	unsigned long lineno = 0;

	while (std::getline(fileHandle, entryFnBlkNm)) {
		++lineno;
		if (entryFnBlkNm.find("#", 0) == 0) {
			continue;
		}


		std::stringstream conv(entryFnBlkNm);
		unsigned long nCallDepth;
		std::string funcName;
		unsigned blkNumI;
		std::string blkAddr;
		conv >> nCallDepth >> funcName >> blkNumI >> blkAddr;
		if (conv.fail()) {
			std::cout << "Issue parsing trace file line num "  \
			  << lineno << "..continuing\n"; 
			continue;	
		}
		blkAddr = "0x" + blkAddr;
		uint64_t blkAddrI = std::stoul(blkAddr, nullptr, 16);
		++count;
/*
		std::cout << "Entry: " << count << ","  \
		  << nCallDepth << "," << funcName<< "," <<blkNumI<< "\n";
 */
		TraceEntry t(count, nCallDepth, funcName, blkNumI, blkAddrI);
		traceEntries.insert(t);
	}
	fileHandle.close();
	return traceEntries;
}
std::vector<const TraceEntry *>
TraceUtils::TraceByFunction(std::string fnName, std::set<TraceEntry> allTraces)
{
	std::vector<const TraceEntry *> prj;

	for (auto i = allTraces.begin(); i != allTraces.end(); ++i) {
		const TraceEntry *t = &*i;
		if (t->getFunction() == fnName) {
			prj.push_back(t);
		}
	}
	return prj;
}
