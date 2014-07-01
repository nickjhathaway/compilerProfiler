
/*

 * main.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway
 */



#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>
#include <stdint.h>
#include "all.h"
#include "profilerRunner.hpp"

using namespace compro;

int simpleAlignmentProfiler(MapStrStr inputCommands) {
	uint32_t maxSize = 50;
	uint32_t minSize = 40;
	uint32_t strNum = 50;
	uint32_t runTimes = 1;
	bool local = false;
	std::string alphStr = "A40,C10,G10,T40";
	std::string alphDelim = ",";
	bool veryVerbose = false;
	bool simple = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(maxSize, "-maxSize", "maxSize");
	setUp.setOption(minSize, "-minSize", "minSize");
	if(minSize > maxSize){
		std::cout << "minSize can't be larger than maxSize, setting minSize to maxSize" << std::endl;
		minSize = maxSize;
	}
	setUp.setOption(simple, "-simple", "simple");
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(runTimes, "-runTimes", "runTimes");
	setUp.setOption(local, "-local", "local");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");

	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "minSize: " << minSize << std::endl;
		std::cout << "maxSize: " << maxSize << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "runTimes: " << runTimes << std::endl;
		std::cout << "local: " << local << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;
	gapScoringParameters <int32_t> gapPars(5,1);
	substituteMatrix scoreMatrix(2,-2);
	alignerTemp <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
	for(uint32_t run = 0; run < runTimes; ++run){
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		alignerObj.alignSeq(randStrings.front(), randStrings.back(), local);
		if(veryVerbose){
			std::for_each(randStrings.begin(),
						randStrings.end(), [&](const std::string & str){ std::cout << str << std::endl;});
		}
		if (setUp.verbose_) {
			std::cout << alignerObj.alignObjectA_ << std::endl;
			std::cout << alignerObj.alignObjectB_ << std::endl;
		}
		for(auto pos : iter::range(randStrings.size())){
			for(auto secondPos : iter::range(randStrings.size())){
				alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], local);
			}
		}
	}
	if(setUp.verbose_){
		setUp.logRunTime(std::cout);
	}else{
		std::cout << setUp.timer_.getRunTime() << std::endl;
	}
	return 0;
}



int fullAlignmentProfiler(MapStrStr inputCommands) {
	uint32_t maxSize = 50;
	uint32_t minSize = 50;
	uint32_t strNum = 50;
	uint32_t runTimes = 1;
	std::string alphStr = "A,C,G,T";
	std::string alphDelim = ",";

	bool veryVerbose = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(maxSize, "-maxSize", "maxSize");
	if(!setUp.setOption(minSize, "-minSize", "minSize")){
		minSize = maxSize;
	}
	if(minSize > maxSize){
		std::cout << "minSize can't be larger than maxSize, setting minSize to maxSize" << std::endl;
		minSize = maxSize;
	}
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(runTimes, "-runTimes", "runTimes");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");
	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "minSize: " << minSize << std::endl;
		std::cout << "maxSize: " << maxSize << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "runTimes: " << runTimes << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;


	//std::cout << compilerUsed << std::endl;
	if(setUp.header_){
		std::cout << "numType\talnType\t"<< getCompilerInfo("\t", true) << "\tminSize\tmaxSize"
				"\tstrNum\trunTimes\talnCount\ttime(sec)" << std::endl;
	}
	{
		/*
		 * double
		 */
		gapScoringParameters <double> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <double> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("double", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){

					for(auto pos : iter::range(randStrings.size())){
						for(auto secondPos : iter::range(randStrings.size())){
							alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
							++alnCount;
						}
					}
				}
			std::cout << "double\tglobal\t"
										<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t"<< timmerdoub.getRunTime() << std::endl;
			}
		}
	{
		/*
		 * float
		 */
		gapScoringParameters <float> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <float> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("float", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			std::cout << "float\tglobal\t" << getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount
								<< "\t" << timmerdoub.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int16_t
		 */
		gapScoringParameters <int16_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int16_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int16_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			std::cout << "int16_t\tglobal\t"
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int32_t
		 */
		gapScoringParameters <int32_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
					alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int32_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			std::cout << "int32_t\tglobal\t"
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int64_t
		 */
		gapScoringParameters <int64_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int64_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int64_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			std::cout << "int64_t\tglobal\t"
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * double
		 */
		gapScoringParameters <double> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <double> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("double", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){

					for(auto pos : iter::range(randStrings.size())){
						for(auto secondPos : iter::range(randStrings.size())){
							alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
							++alnCount;
						}
					}
				}
			std::cout << "double\tlocal\t"
										<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t"<< timmerdoub.getRunTime() << std::endl;
			}
		}
	{
		/*
		 * float
		 */
		gapScoringParameters <float> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <float> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("float", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			std::cout << "float\tlocal\t" << getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount
								<< "\t" << timmerdoub.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int16_t
		 */
		gapScoringParameters <int16_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int16_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int16_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			std::cout << "int16_t\tlocal\t"
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int32_t
		 */
		gapScoringParameters <int32_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
					alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int32_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			std::cout << "int32_t\tlocal\t"
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int64_t
		 */
		gapScoringParameters <int64_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int64_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int64_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			std::cout << "int64_t\tlocal\t"
					<< getCompilerInfo("\t", false) << "\t"
					<< vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t")
					<< "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}

	if(setUp.verbose_){
		setUp.logRunTime(std::cout);
	}
	return 0;
}


int justScoreAlignmentProfiler(MapStrStr inputCommands) {
	uint32_t maxSize = 50;
	uint32_t minSize = 50;
	uint32_t strNum = 50;
	uint32_t runTimes = 1;
	std::string alphStr = "A,C,G,T";
	std::string alphDelim = ",";
	bool veryVerbose = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(maxSize, "-maxSize", "maxSize");
	setUp.processExtra();
	if(!setUp.setOption(minSize, "-minSize", "minSize")){
		minSize = maxSize;
	}
	if(minSize > maxSize){
		std::cout << "minSize can't be larger than maxSize, setting minSize to maxSize" << std::endl;
		minSize = maxSize;
	}
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(runTimes, "-runTimes", "runTimes");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");
	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "minSize: " << minSize << std::endl;
		std::cout << "maxSize: " << maxSize << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "runTimes: " << runTimes << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;


	//std::cout << compilerUsed << std::endl;
	if(setUp.header_){
		std::cout << "numType\talnType\t";
		if(setUp.addExtra_){
			std::cout << setUp.extraColName_ << "\t";
		}
		std::cout<< getCompilerInfo("\t", true) << "\tminSize\tmaxSize"
				"\tstrNum\trunTimes\talnCount\ttime(sec)" << std::endl;
	}
	{
		/*
		 * double
		 */
		gapScoringParameters <double> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <double> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("double", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){

					for(auto pos : iter::range(randStrings.size())){
						for(auto secondPos : iter::range(randStrings.size())){
							alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
							++alnCount;
						}
					}
				}
			std::cout << "double\tglobal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ << "\t";
			}
			std::cout << getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t"<< timmerdoub.getRunTime() << std::endl;
			}
		}
	{
		/*
		 * float
		 */
		gapScoringParameters <float> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <float> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("float", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			std::cout << "float\tglobal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout << getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount
								<< "\t" << timmerdoub.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int16_t
		 */
		gapScoringParameters <int16_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int16_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int16_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			std::cout << "int16_t\tglobal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int32_t
		 */
		gapScoringParameters <int32_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
					alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int32_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			std::cout << "int32_t\tglobal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int64_t
		 */
		gapScoringParameters <int64_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int64_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int64_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			std::cout << "int64_t\tglobal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * double
		 */
		gapScoringParameters <double> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <double> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("double", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){

					for(auto pos : iter::range(randStrings.size())){
						for(auto secondPos : iter::range(randStrings.size())){
							alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
							++alnCount;
						}
					}
				}
			std::cout << "double\tlocal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout
										<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t"<< timmerdoub.getRunTime() << std::endl;
			}
		}
	{
		/*
		 * float
		 */
		gapScoringParameters <float> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <float> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("float", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			std::cout << "float\tlocal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout << getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount
								<< "\t" << timmerdoub.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int16_t
		 */
		gapScoringParameters <int16_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int16_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int16_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			std::cout << "int16_t\tlocal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int32_t
		 */
		gapScoringParameters <int32_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
					alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int32_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			std::cout << "int32_t\tlocal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout
					<< getCompilerInfo("\t", false) << "\t" << vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t") << "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}
	{
		/*
		 * int64_t
		 */
		gapScoringParameters <int64_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		alignerTemp <int64_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int64_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			std::cout << "int64_t\tlocal\t";
			if(setUp.addExtra_){
				std::cout << setUp.extraColInfo_ <<"\t";
			}
			std::cout
					<< getCompilerInfo("\t", false) << "\t"
					<< vectorToString(std::vector<uint32_t>{minSize, maxSize, strNum, runTimes}, "\t")
					<< "\t" << alnCount << "\t" << timmerInt.getRunTime() << std::endl;
		}
	}

	if(setUp.verbose_){
		setUp.logRunTime(std::cout);
	}
	return 0;
}

int randomNumberGeneration(MapStrStr inputCommands) {
	profilerSetUp setUp(inputCommands);
	uint64_t stop = 100;
	bool randDeviceAsWell = false;
	setUp.setOption(randDeviceAsWell, "-randDev", "randDeviceAsWell");
	setUp.setOption(stop, "-stop", "stop");
	setUp.processExtra();
	setUp.finishSetUp(std::cout);
	std::random_device rd;
	std::mt19937 mtGen(rd());
	std::mt19937_64 mtGen64(rd());
	randomGenerator gen;
	std::cout << mtGen.max() << std::endl;
	std::cout << mtGen64.max() << std::endl;
	if(setUp.header_){
		std::cout << "generator\trunTimes\t" ;
		if(setUp.addExtra_){
			std::cout << setUp.extraColName_ << "\t";
		}
		std::cout << getCompilerInfo("\t", true) << "\ttime" << std::endl;
	}
	if(randDeviceAsWell){
		timeTracker timmer("random_device", false);
		for(uint64_t run = 0; run < stop; ++run	){
			rd();
		}
		std::cout << "random_device\t" << stop << "\t";
		if(setUp.addExtra_){
			std::cout << setUp.extraColInfo_ <<"\t";
		}
		std::cout  << getCompilerInfo("\t", false) << "\t" << timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("mt",false);
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen();
		}
		std::cout << "mt19937\t" << stop << "\t";
		if(setUp.addExtra_){
			std::cout << setUp.extraColInfo_ <<"\t";
		}
		std::cout << getCompilerInfo("\t", false) << "\t" << timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("mt64", false);
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen64();
		}
		std::cout << "mt19937_64\t" << stop << "\t";
		if(setUp.addExtra_){
			std::cout << setUp.extraColInfo_ <<"\t";
		}
		std::cout << getCompilerInfo("\t", false) << "\t" << timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("randomGenerator", false);
		for(uint64_t run = 0; run < stop; ++run	){
			gen.unifRand();
		}
		std::cout << "randomGenerator\t" << stop << "\t";
		if(setUp.addExtra_){
			std::cout << setUp.extraColInfo_ <<"\t";
		}
		std::cout << getCompilerInfo("\t", false) << "\t" << timmer.getRunTime() << std::endl;
	}

	return 0;
}
int testingTracker(MapStrStr inputCommands) {
	profilerSetUp setUp(inputCommands);
	uint64_t stop = 100;
	setUp.setOption(stop, "-stop", "stop");
	setUp.processExtra();
	setUp.finishSetUp(std::cout);
	std::random_device rd;
	std::mt19937 mtGen(rd());
	std::cout << std::chrono::high_resolution_clock::period::den << std::endl;
	if(setUp.header_){
		std::cout << "generator\trunTimes\t" ;
		if(setUp.addExtra_){
			std::cout << setUp.extraColName_ << "\t";
		}
		std::cout << getCompilerInfo("\t", true) << "\ttime" << std::endl;
	}
	{
		timeTracker timmer("mt",false);
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen();
		}
		std::cout << "mt19937\t" << stop << "\t";
		if(setUp.addExtra_){
			std::cout << setUp.extraColInfo_ <<"\t";
		}
		std::cout << getCompilerInfo("\t", false) << "\t" << timmer.getRunTime();
		std::cout << std::endl;
	}
	{

		std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen();
		}
		std::cout << "mt19937\t" << stop << "\t";
		if(setUp.addExtra_){
			std::cout << setUp.extraColInfo_ <<"\t";
		}
		std::cout << getCompilerInfo("\t", false);
		auto finish = std::chrono::high_resolution_clock::now();
		std::cout << "\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() /static_cast<double>( std::chrono::high_resolution_clock::period::den);

		std::cout << std::endl;
	}
	{

		timeTracker timmer("mt", false);
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen();
		}
		std::cout << "mt19937\t" << stop << "\t";
		if(setUp.addExtra_){
			std::cout << setUp.extraColInfo_ <<"\t";
		}
		std::cout << getCompilerInfo("\t", false);

		std::cout << "\t" << timmer.getRunTime();
		std::cout << std::endl;
	}


	return 0;
}


/* profiler template
 * int nameOfProgram(MapStrStr inputCommands) {
 *  //programSetUp for easy command line parsing
 * 	programSetUp setUp(inputCommands);
 * 	T variable1 = defaultValue;
 * 	T2 variable2 = defaultValue2
 * 	//setOption takes four options variableToSet, multipleFlags_commandDelimited, nameOfVariable, requried(byDeault false)
 * 	//setOption is also overloaded to set more of the common std libary variable, int types (32_t, 64_t etc), double, std::string etc
 *	setUp.setOption(variable1, "-variable1", "variable1");
 *  setUp.setOption(variable2, "-variable2,-var2", "variable2", true);
 *  //always finish with setUp.finishSetUp(std::cout); so you
 *  //can use -getFlags and have the setUp print warnings
 *  setUp.finishSetUp(std::cout);
 *
 *  //your code here, compilerUsed is a static variable set to clang or gcc so you can use to log which compiler you are using
 *  //compilerProfiler comes with a random number generator, functions for generating random strings, an aligner, a timer to log run times
 *  //can include own code to profile
 *  // once you write your own profiler, just add to profilerRunner with the addFunc function in the initialization seen bellow
 *  // which takes name to call the program at commandline, the function itself, and whether the name is an alias and
 *  // shouldn't be printed to commandline, and whether to make the call lower case to make the calling case insensitive (default)
 *
 * return 0;
 * }
 */

profilerRunner::profilerRunner()
    : programRunner(
          {addFunc("fullAlignmentProfiler", fullAlignmentProfiler, false),
					 addFunc("simpleAlignmentProfiler", simpleAlignmentProfiler, false),
					 addFunc("justScoreAlignmentProfiler", justScoreAlignmentProfiler, false),
					 addFunc("randomNumberGeneration", randomNumberGeneration, false),
					 addFunc("testingTracker", testingTracker, false)
           },
          "profilerRunner") {}

int main(int argc, char* argv[]) {
	//std::cout << __VERSION__ << std::endl;
	//std::cout << getCompilerVersion() << std::endl;
	//std::cout << optimizationLevel << std::endl;
	//std::cout << processorInfo << std::endl;
	//std::cout << TESTMACRO << std::endl;
  profilerRunner proRunner;
  if (argc > 1) {
    return proRunner.run(argc, argv);
  }
  proRunner.listPrograms(std::cout);
  return 0;
}
