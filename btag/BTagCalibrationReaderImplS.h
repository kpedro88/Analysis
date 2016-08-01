#ifndef BTagCalibrationReaderImplS_H
#define BTagCalibrationReaderImplS_H

#include <string>
#include <vector>
#include <utility>

#include "BTagEntryS.h"
#include "BTagCalibrationS.h"

class BTagCalibrationReaderImplS
{
friend class BTagCalibrationReaderS;
	
public:
  struct TmpEntry {
    float etaMin;
    float etaMax;
    float ptMin;
    float ptMax;
    float discrMin;
    float discrMax;
    TF1 func;
  };

private:
  BTagCalibrationReaderImplS(BTagEntryS::OperatingPoint op, const std::string & sysType) : op_(op), sysType_(sysType), tmpData_(3), useAbsEta_(3, true) {}
  BTagCalibrationReaderImplS(BTagEntryS::OperatingPoint op, const std::string & sysType, const std::vector<std::string> & otherSysTypes) :
    op_(op), sysType_(sysType), tmpData_(3), useAbsEta_(3, true)
  {
    for (const std::string & ost : otherSysTypes) {
      if (otherSysTypeReaders_.count(ost)) {
        std::cerr << "ERROR in BTagCalibrationS: "
              << "Every otherSysType should only be given once. Duplicate: "
              << ost;
        throw std::exception();
      }
      otherSysTypeReaders_[ost] = new BTagCalibrationReaderImplS(op, ost);
    }
  }

  void load(const BTagCalibrationS & c, BTagEntryS::JetFlavor jf, std::string measurementType)
  {
    if (tmpData_[jf].size()) {
      std::cerr << "ERROR in BTagCalibrationS: "
            << "Data for this jet-flavor is already loaded: "
            << jf;
      throw std::exception();
    }
  
    BTagEntryS::Parameters params(op_, measurementType, sysType_);
    const std::vector<BTagEntryS> &entries = c.getEntries(params);
  
    for (unsigned ibe = 0; ibe < entries.size(); ++ibe) {
      const BTagEntryS& be(entries[ibe]);
      if (be.params.jetFlavor != jf) {
        continue;
      }
  
      TmpEntry te;
      te.etaMin = be.params.etaMin;
      te.etaMax = be.params.etaMax;
      te.ptMin = be.params.ptMin;
      te.ptMax = be.params.ptMax;
      te.discrMin = be.params.discrMin;
      te.discrMax = be.params.discrMax;
  
      if (op_ == BTagEntryS::OP_RESHAPING) {
        te.func = TF1("", be.formula.c_str(),
                      be.params.discrMin, be.params.discrMax);
      } else {
        te.func = TF1("", be.formula.c_str(),
                      be.params.ptMin, be.params.ptMax);
      }
  
      tmpData_[be.params.jetFlavor].push_back(te);
      if (te.etaMin < 0) {
        useAbsEta_[be.params.jetFlavor] = false;
      }
    }
  
    for (typename std::map<std::string, BTagCalibrationReaderImplS*>::iterator p = otherSysTypeReaders_.begin(); p != otherSysTypeReaders_.end(); ++p) {
      p->second->load(c, jf, measurementType);
    }
  }

  double eval(BTagEntryS::JetFlavor jf, float eta, float pt, float discr) const
  {
    bool use_discr = (op_ == BTagEntryS::OP_RESHAPING);
    if (useAbsEta_[jf] && eta < 0) {
      eta = -eta;
    }
  
    // search linearly through eta, pt and discr ranges and eval
    // future: find some clever data structure based on intervals
    const std::vector<TmpEntry> &entries = tmpData_.at(jf);
    for (unsigned i=0; i<entries.size(); ++i) {
      const TmpEntry &e = entries.at(i);
      if (
        e.etaMin <= eta && eta < e.etaMax                   // find eta
        && e.ptMin < pt && pt <= e.ptMax                    // check pt
      ){
        if (use_discr) {                                    // discr. reshaping?
          if (e.discrMin <= discr && discr < e.discrMax) {  // check discr
            return e.func.Eval(discr);
          }
        } else {
          return e.func.Eval(pt);
        }
      }
    }
  
    return 0.;  // default value
  }

  double eval_auto_bounds(const std::string & sys, BTagEntryS::JetFlavor jf, float eta, float pt, float discr) const
  {
    std::pair<float, float> sf_bounds = min_max_pt(jf, eta, discr);
    float pt_for_eval = pt;
    bool is_out_of_bounds = false;
  
    if (pt < sf_bounds.first) {
      pt_for_eval = sf_bounds.first + .0001;
      is_out_of_bounds = true;
    } else if (pt > sf_bounds.second) {
      pt_for_eval = sf_bounds.second - .0001;
      is_out_of_bounds = true;
    }
  
    // get central SF (and maybe return)
    double sf = eval(jf, eta, pt_for_eval, discr);
    if (sys == sysType_) {
      return sf;
    }
  
    // get sys SF (and maybe return)
    if (!otherSysTypeReaders_.count(sys)) {
      std::cerr << "ERROR in BTagCalibrationS: "
          << "sysType not available (maybe not loaded?): "
          << sys;
      throw std::exception();
    }
    double sf_err = otherSysTypeReaders_.at(sys)->eval(jf, eta, pt_for_eval, discr);
    if (!is_out_of_bounds) {
      return sf_err;
    }
  
    // double uncertainty on out-of-bounds and return
    sf_err = sf + 2*(sf_err - sf);
    return sf_err;
  }

  std::pair<float, float> min_max_pt(BTagEntryS::JetFlavor jf, float eta, float discr) const
  {
    bool use_discr = (op_ == BTagEntryS::OP_RESHAPING);
    if (useAbsEta_[jf] && eta < 0) {
      eta = -eta;
    }
  
    const std::vector<TmpEntry> &entries = tmpData_.at(jf);
    float min_pt = -1., max_pt = -1.;
    for (unsigned i=0; i<entries.size(); ++i) {
      const TmpEntry &e = entries.at(i);
      if (
        e.etaMin <= eta && eta < e.etaMax                   // find eta
      ){
        if (min_pt < 0.) {                                  // init
          min_pt = e.ptMin;
          max_pt = e.ptMax;
          continue;
        }
  
        if (use_discr) {                                    // discr. reshaping?
          if (e.discrMin <= discr && discr < e.discrMax) {  // check discr
            min_pt = min_pt < e.ptMin ? min_pt : e.ptMin;
            max_pt = max_pt > e.ptMax ? max_pt : e.ptMax;
          }
        } else {
          min_pt = min_pt < e.ptMin ? min_pt : e.ptMin;
          max_pt = max_pt > e.ptMax ? max_pt : e.ptMax;
        }
      }
    }
  
    return std::make_pair(min_pt, max_pt);
  }

  BTagEntryS::OperatingPoint op_;
  std::string sysType_;
  std::vector<std::vector<TmpEntry> > tmpData_;  // first index: jetFlavor
  std::vector<bool> useAbsEta_;                  // first index: jetFlavor
  std::map<std::string, BTagCalibrationReaderImplS*> otherSysTypeReaders_;
};

#endif
