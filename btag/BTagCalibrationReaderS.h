#ifndef BTagCalibrationReader_H
#define BTagCalibrationReader_H

/**
 * BTagCalibrationReaderS
 *
 * Helper class to pull out a specific set of BTagEntryS's out of a
 * BTagCalibrationS. TF1 functions are set up at initialization time.
 *
 ************************************************************/

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include "BTagEntryS.h"
#include "BTagCalibrationS.h"
#include "BTagCalibrationReaderImplS.h"

class BTagCalibrationReaderS
{
public:
  BTagCalibrationReaderS() {}
  BTagCalibrationReaderS(BTagEntryS::OperatingPoint op, const std::string & sysType="central") : pimpl(new BTagCalibrationReaderImplS(op, sysType)) {}
  BTagCalibrationReaderS(BTagEntryS::OperatingPoint op, const std::string & sysType, const std::vector<std::string> & otherSysTypes) : pimpl(new BTagCalibrationReaderImplS(op, sysType, otherSysTypes)) {}

  void load(const BTagCalibrationS & c, BTagEntryS::JetFlavor jf, const std::string & measurementType="comb")
  {
    pimpl->load(c, jf, measurementType);
  }

  double eval(BTagEntryS::JetFlavor jf, float eta, float pt, float discr=0.) const
  {
    return pimpl->eval(jf, eta, pt, discr);
  }

  double eval_auto_bounds(const std::string & sys, BTagEntryS::JetFlavor jf, float eta, float pt, float discr=0.) const
  {
    return pimpl->eval_auto_bounds(sys, jf, eta, pt, discr);
  }

  std::pair<float, float> min_max_pt(BTagEntryS::JetFlavor jf, float eta, float discr=0.) const
  {
    return pimpl->min_max_pt(jf, eta, discr);
  }

protected:
  BTagCalibrationReaderImplS* pimpl;
};


#endif  // BTagCalibrationReader_H
