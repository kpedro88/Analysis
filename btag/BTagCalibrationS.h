#ifndef BTagCalibrationS_H
#define BTagCalibrationS_H

/**
 * BTagCalibrationS
 *
 * The 'hierarchy' of stored information is this:
 * - by tagger (BTagCalibrationS)
 *   - by operating point or reshape bin
 *     - by jet parton flavor
 *       - by type of measurement
 *         - by systematic
 *           - by eta bin
 *             - as 1D-function dependent of pt or discriminant
 *
 ************************************************************/

#include <map>
#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <exception>

#include "BTagEntryS.h"

class BTagCalibrationS
{
public:
  BTagCalibrationS() {}
  BTagCalibrationS(const std::string &tagger) : tagger_(tagger) {}
  BTagCalibrationS(const std::string &tagger, const std::string &filename) : tagger_(tagger)
  {
    std::ifstream ifs(filename);
    if (!ifs.good()) {
      std::cerr << "ERROR in BTagCalibrationS: "
            << "input file not available: "
            << filename;
      throw std::exception();
    }
    readCSV(ifs);
    ifs.close();
  }
  ~BTagCalibrationS() {}

  std::string tagger() const {return tagger_;}

  void addEntry(const BTagEntryS &entry)
  {
    data_[token(entry.params)].push_back(entry);
  }
  const std::vector<BTagEntryS>& getEntries(const BTagEntryS::Parameters &par) const
  {
    std::string tok = token(par);
    if (!data_.count(tok)) {
      std::cerr << "ERROR in BTagCalibrationS: "
            << "(OperatingPoint, measurementType, sysType) not available: "
            << tok;
      throw std::exception();
    }
    return data_.at(tok);
  }

  void readCSV(const std::string &s)
  {
    std::stringstream buff(s);
    readCSV(buff);
  }
  void readCSV(std::istream &s)
  {
    std::string line;
  
    // firstline might be the header
    std::getline(s,line);
    if (line.find("OperatingPoint") == std::string::npos) {
      addEntry(BTagEntryS(line));
    }
  
    while (std::getline(s,line)) {
      line = BTagEntryS::trimStr(line);
      if (line.empty()) {  // skip empty lines
        continue;
      }
      addEntry(BTagEntryS(line));
    }
  }
  void makeCSV(std::ostream &s) const
  {
    s << tagger_ << ";" << BTagEntryS::makeCSVHeader();
    for (std::map<std::string, std::vector<BTagEntryS> >::const_iterator i = data_.cbegin(); i != data_.cend(); ++i) {
      const std::vector<BTagEntryS> &vec = i->second;
      for (std::vector<BTagEntryS>::const_iterator j = vec.cbegin(); j != vec.cend(); ++j) {
        s << j->makeCSVLine();
      }
    }
  }
  std::string makeCSV() const
  {
    std::stringstream buff;
    makeCSV(buff);
    return buff.str();
  }

protected:
  static std::string token(const BTagEntryS::Parameters &par)
  {
    std::stringstream buff;
    buff << par.operatingPoint << ", "
         << par.measurementType << ", "
         << par.sysType;
    return buff.str();
  }

  std::string tagger_;
  std::map<std::string, std::vector<BTagEntryS> > data_;

};

#endif  // BTagCalibrationS_H
