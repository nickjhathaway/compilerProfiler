/*
 *
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "compro/alignment/alignInfoHolder.hpp"
namespace compro {

void alnInfoLocal::writeInfoSingleLine(std::ostream &indexFile, uint64_t seq1,
                                       uint64_t seq2) const {
  indexFile << seq1 << " " << seq2 << " " << score_ << " " << localAStart_ << " " << localASize_
            << " " << localBStart_ << " " << localBSize_;
  for (const auto &g : gapInfos_) {
    indexFile << " ";
    g.writeInfoNoEnd(indexFile);
  }
  indexFile << std::endl;
}

alnInfoLocal alnInfoLocal::readInfo(std::stringstream& ss,
		std::vector<std::string>& info, std::vector<std::string>& inputGapInfo,
		std::vector<gapInfo> & gInfos, std::string & out){

	uint32_t index = 2;
  while (!ss.eof()) {
    ss >> out;
    info[index] = out;
    ++index;
  }
  if (index >= 7) {
    for (auto i : iter::range<uint32_t>(7, index)) {
      inputGapInfo = tokenizeString(info[i], ",");
      gInfos.emplace_back(gapInfo(std::stoi(inputGapInfo[0]),
                                  std::stoi(inputGapInfo[1]),
                                  std::stoi(inputGapInfo[2])));
    }
  }
  return alnInfoLocal(gInfos, std::stoi(info[3]), std::stoi(info[4]),
                            std::stoi(info[5]), std::stod(info[6]),
                            std::stoi(info[2]), true);
}

void alnInfoGlobal::writeInfoSingleLine(std::ostream &indexFile, uint64_t seq1,
                                        uint64_t seq2) const {
  indexFile << seq1 << " " << seq2 << " " << score_;
  for (const auto &g : gapInfos_) {
    indexFile << " ";
    g.writeInfoNoEnd(indexFile);
  }
  indexFile << std::endl;
}

alnInfoGlobal alnInfoGlobal::readInfo(std::stringstream& ss,
   		std::vector<std::string>& info, std::vector<std::string>& inputGapInfo,
   		std::vector<gapInfo> & gInfos, std::string & out){

	uint32_t index = 2;
  while (!ss.eof()) {
    ss >> out;
    info[index] = out;
    ++index;
  }

  if (index >= 3) {
    for (auto i : iter::range<uint32_t>(3, index)) {
      inputGapInfo = tokenizeString(info[i], ",");
      gInfos.emplace_back(gapInfo(std::stoi(inputGapInfo[0]),
                                  std::stoi(inputGapInfo[1]),
                                  std::stoi(inputGapInfo[2])));
    }
  }
  return alnInfoGlobal(gInfos, std::stod(info[2]), true);
}

}
