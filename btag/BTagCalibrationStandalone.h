#ifndef BTagEntryS_H
#define BTagEntryS_H

/**
 *
 * BTagEntryS
 *
 * Represents one pt- or discriminator-dependent calibration function.
 *
 * measurement_type:    e.g. comb, ttbar, di-mu, boosted, ...
 * sys_type:            e.g. central, plus, minus, plus_JEC, plus_JER, ...
 *
 * Everything is converted into a function, as it is easiest to store it in a
 * txt or json file.
 *
 ************************************************************/

#include <string>
#include <TF1.h>
#include <TH1.h>


class BTagEntryS
{
public:
  enum OperatingPoint {
    OP_LOOSE=0,
    OP_MEDIUM=1,
    OP_TIGHT=2,
    OP_RESHAPING=3,
  };
  enum JetFlavor {
    FLAV_B=0,
    FLAV_C=1,
    FLAV_UDSG=2,
  };
  struct Parameters {
    OperatingPoint operatingPoint;
    std::string measurementType;
    std::string sysType;
    JetFlavor jetFlavor;
    float etaMin;
    float etaMax;
    float ptMin;
    float ptMax;
    float discrMin;
    float discrMax;

    // default constructor
    Parameters(
      OperatingPoint op=OP_TIGHT,
      std::string measurement_type="comb",
      std::string sys_type="central",
      JetFlavor jf=FLAV_B,
      float eta_min=-99999.,
      float eta_max=99999.,
      float pt_min=0.,
      float pt_max=99999.,
      float discr_min=0.,
      float discr_max=99999.
    );

  };

  BTagEntryS() {}
  BTagEntryS(const std::string &csvLine);
  BTagEntryS(const std::string &func, Parameters p);
  BTagEntryS(const TF1* func, Parameters p);
  BTagEntryS(const TH1* histo, Parameters p);
  ~BTagEntryS() {}
  static std::string makeCSVHeader();
  std::string makeCSVLine() const;
  static std::string trimStr(std::string str);

  // public, no getters needed
  std::string formula;
  Parameters params;

};

#endif  // BTagEntryS_H


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


class BTagCalibrationS
{
public:
  BTagCalibrationS() {}
  BTagCalibrationS(const std::string &tagger);
  BTagCalibrationS(const std::string &tagger, const std::string &filename);
  ~BTagCalibrationS() {}

  std::string tagger() const {return tagger_;}

  void addEntry(const BTagEntryS &entry);
  const std::vector<BTagEntryS>& getEntries(const BTagEntryS::Parameters &par) const;

  void readCSV(std::istream &s);
  void readCSV(const std::string &s);
  void makeCSV(std::ostream &s) const;
  std::string makeCSV() const;

protected:
  static std::string token(const BTagEntryS::Parameters &par);

  std::string tagger_;
  std::map<std::string, std::vector<BTagEntryS> > data_;

};

#endif  // BTagCalibrationS_H


#ifndef BTagCalibrationReaderS_H
#define BTagCalibrationReaderS_H

/**
 * BTagCalibrationReaderS
 *
 * Helper class to pull out a specific set of BTagEntryS's out of a
 * BTagCalibrationS. TF1 functions are set up at initialization time.
 *
 ************************************************************/

#include <memory>
#include <string>



class BTagCalibrationReaderS
{
public:
  class BTagCalibrationReaderImplS;

  BTagCalibrationReaderS() {}
  BTagCalibrationReaderS(BTagEntryS::OperatingPoint op,
                        const std::string & sysType="central",
                        const std::vector<std::string> & otherSysTypes={});

  void load(const BTagCalibrationS & c,
            BTagEntryS::JetFlavor jf,
            const std::string & measurementType="comb");

  double eval(BTagEntryS::JetFlavor jf,
              float eta,
              float pt,
              float discr=0.) const;

  double eval_auto_bounds(const std::string & sys,
                          BTagEntryS::JetFlavor jf,
                          float eta,
                          float pt,
                          float discr=0.) const;

  std::pair<float, float> min_max_pt(BTagEntryS::JetFlavor jf,
                                     float eta,
                                     float discr=0.) const;

protected:
  std::shared_ptr<BTagCalibrationReaderImplS> pimpl;
};


#endif  // BTagCalibrationReaderS_H


