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
#include <algorithm>
#include <iostream>
#include <exception>
#include <sstream>
#include <TF1.h>
#include <TH1.h>

namespace {
  // Creates chained step functions like this:
  // "<prevous_bin> : x<bin_high_bound ? bin_value : <next_bin>"
  // e.g. "x<0 ? 1 : x<1 ? 2 : x<2 ? 3 : 4"
  std::string th1ToFormulaLin(const TH1* hist) {
    int nbins = hist->GetNbinsX();
    TAxis const* axis = hist->GetXaxis();
    std::stringstream buff;
    buff << "x<" << axis->GetBinLowEdge(1) << " ? 0. : ";  // default value
    for (int i=1; i<nbins+1; ++i) {
      char tmp_buff[50];
      sprintf(tmp_buff,
              "x<%g ? %g : ",  // %g is the smaller one of %e or %f
              axis->GetBinUpEdge(i),
              hist->GetBinContent(i));
      buff << tmp_buff;
    }
    buff << 0.;  // default value
    return buff.str();
  }
  
  // Creates step functions making a binary search tree:
  // "x<mid_bin_bound ? (<left side tree>) : (<right side tree>)"
  // e.g. "x<2 ? (x<1 ? (x<0 ? 0:0.1) : (1)) : (x<4 ? (x<3 ? 2:3) : (0))"
  std::string th1ToFormulaBinTree(const TH1* hist, int start=0, int end=-1) {
    if (end == -1) {                      // initialize
      start = 0.;
      end = hist->GetNbinsX()+1;
      TH1* h2 = (TH1*) hist->Clone();
      h2->SetBinContent(start, 0);  // kill underflow
      h2->SetBinContent(end, 0);    // kill overflow
      std::string res = th1ToFormulaBinTree(h2, start, end);
      delete h2;
      return res;
    }
    if (start == end) {                   // leave is reached
      char tmp_buff[20];
      sprintf(tmp_buff, "%g", hist->GetBinContent(start));
      return std::string(tmp_buff);
    }
    if (start == end - 1) {               // no parenthesis for neighbors
      char tmp_buff[70];
      sprintf(tmp_buff,
              "x<%g ? %g:%g",
              hist->GetXaxis()->GetBinUpEdge(start),
              hist->GetBinContent(start),
              hist->GetBinContent(end));
      return std::string(tmp_buff);
    }
  
    // top-down recursion
    std::stringstream buff;
    int mid = (end-start)/2 + start;
    char tmp_buff[25];
    sprintf(tmp_buff,
            "x<%g ? (",
            hist->GetXaxis()->GetBinUpEdge(mid));
    buff << tmp_buff
         << th1ToFormulaBinTree(hist, start, mid)
         << ") : ("
         << th1ToFormulaBinTree(hist, mid+1, end)
         << ")";
    return buff.str();
  }

}

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
      OperatingPoint op,
      std::string measurement_type,
      std::string sys_type,
      JetFlavor jf=FLAV_B,
      float eta_min=-99999.,
      float eta_max=99999.,
      float pt_min=0.,
      float pt_max=99999.,
      float discr_min=0.,
      float discr_max=99999.
    ):
      operatingPoint(op),
      measurementType(measurement_type),
      sysType(sys_type),
      jetFlavor(jf),
      etaMin(eta_min),
      etaMax(eta_max),
      ptMin(pt_min),
      ptMax(pt_max),
      discrMin(discr_min),
      discrMax(discr_max)
    {
      std::transform(measurementType.begin(), measurementType.end(),
                     measurementType.begin(), ::tolower);
      std::transform(sysType.begin(), sysType.end(),
                     sysType.begin(), ::tolower);
    }
	Parameters():
      operatingPoint(OP_TIGHT),
      measurementType("comb"),
      sysType("central"),
      jetFlavor(FLAV_B),
      etaMin(-99999.),
      etaMax(99999.),
      ptMin(0.),
      ptMax(99999.),
      discrMin(0.),
      discrMax(99999.)
    {
      std::transform(measurementType.begin(), measurementType.end(),
                     measurementType.begin(), ::tolower);
      std::transform(sysType.begin(), sysType.end(),
                     sysType.begin(), ::tolower);
    }

  };

  BTagEntryS() {}
  BTagEntryS(const std::string &csvLine)
  {
    // make tokens
    std::stringstream buff(csvLine);
    std::vector<std::string> vec;
    std::string token;
    while (std::getline(buff, token, ","[0])) {
      token = BTagEntryS::trimStr(token);
      if (token.empty()) {
        continue;
      }
      vec.push_back(token);
    }
    if (vec.size() != 11) {
      std::cerr << "ERROR in BTagCalibration: "
            << "Invalid csv line; num tokens != 11: "
            << csvLine;
      throw std::exception();
    }
  
    // clean string values
    char chars[] = " \"\n";
    for (unsigned int i = 0; i < strlen(chars); ++i) {
      vec[1].erase(remove(vec[1].begin(),vec[1].end(),chars[i]),vec[1].end());
      vec[2].erase(remove(vec[2].begin(),vec[2].end(),chars[i]),vec[2].end());
      vec[10].erase(remove(vec[10].begin(),vec[10].end(),chars[i]),vec[10].end());
    }
  
    // make formula
    formula = vec[10];
    TF1 f1("", formula.c_str());  // compile formula to check validity
    if (f1.IsZombie()) {
      std::cerr << "ERROR in BTagCalibration: "
            << "Invalid csv line; formula does not compile: "
            << csvLine;
      throw std::exception();
    }
  
    // make parameters
    unsigned op = atoi(vec[0].c_str());
    if (op > 3) {
      std::cerr << "ERROR in BTagCalibration: "
            << "Invalid csv line; OperatingPoint > 3: "
            << csvLine;
      throw std::exception();
    }
    unsigned jf = atoi(vec[3].c_str());
    if (jf > 2) {
      std::cerr << "ERROR in BTagCalibration: "
            << "Invalid csv line; JetFlavor > 2: "
            << csvLine;
      throw std::exception();
    }
    params = BTagEntryS::Parameters(
      BTagEntryS::OperatingPoint(op),
      vec[1],
      vec[2],
      BTagEntryS::JetFlavor(jf),
      atof(vec[4].c_str()),
      atof(vec[5].c_str()),
      atof(vec[6].c_str()),
      atof(vec[7].c_str()),
      atof(vec[8].c_str()),
      atof(vec[9].c_str())
    );
  }
  BTagEntryS(const std::string &func, Parameters p) : formula(func), params(p)
  {
    TF1 f1("", formula.c_str());  // compile formula to check validity
    if (f1.IsZombie()) {
      std::cerr << "ERROR in BTagCalibration: "
            << "Invalid func string; formula does not compile: "
            << func;
      throw std::exception();
    }
  }
  BTagEntryS(const TF1* func, Parameters p): formula(std::string(func->GetExpFormula("p").Data())), params(p)
  {
    if (func->IsZombie()) {
      std::cerr << "ERROR in BTagCalibration: "
            << "Invalid TF1 function; function is zombie: "
            << func->GetName();
      throw std::exception();
    }
  }
  BTagEntryS(const TH1* hist, Parameters p) : params(p)
  {
    int nbins = hist->GetNbinsX();
    TAxis const* axis = hist->GetXaxis();
  
    // overwrite bounds with histo values
    if (params.operatingPoint == BTagEntryS::OP_RESHAPING) {
      params.discrMin = axis->GetBinLowEdge(1);
      params.discrMax = axis->GetBinUpEdge(nbins);
    } else {
      params.ptMin = axis->GetBinLowEdge(1);
      params.ptMax = axis->GetBinUpEdge(nbins);
    }
  
    // balanced full binary tree height = ceil(log(2*n_leaves)/log(2))
    // breakes even around 10, but lower values are more propable in pt-spectrum
    if (nbins < 15) {
      formula = th1ToFormulaLin(hist);
    } else {
      formula = th1ToFormulaBinTree(hist);
    }
  
    // compile formula to check validity
    TF1 f1("", formula.c_str());
    if (f1.IsZombie()) {
      std::cerr << "ERROR in BTagCalibration: "
            << "Invalid histogram; formula does not compile (>150 bins?): "
            << hist->GetName();
      throw std::exception();
    }
  }
  ~BTagEntryS() {}
  static std::string makeCSVHeader()
  {
    return "OperatingPoint, "
           "measurementType, "
           "sysType, "
           "jetFlavor, "
           "etaMin, "
           "etaMax, "
           "ptMin, "
           "ptMax, "
           "discrMin, "
           "discrMax, "
           "formula \n";
  }
  std::string makeCSVLine() const
  {
    std::stringstream buff;
    buff << params.operatingPoint
         << ", " << params.measurementType
         << ", " << params.sysType
         << ", " << params.jetFlavor
         << ", " << params.etaMin
         << ", " << params.etaMax
         << ", " << params.ptMin
         << ", " << params.ptMax
         << ", " << params.discrMin
         << ", " << params.discrMax
         << ", \"" << formula
         << "\" \n";
    return buff.str();
  }
  static std::string trimStr(std::string str)
  {
    size_t s = str.find_first_not_of(" \n\r\t");
    size_t e = str.find_last_not_of (" \n\r\t");
  
    if((std::string::npos == s) || (std::string::npos == e))
      return "";
    else
      return str.substr(s, e-s+1);
  }

  // public, no getters needed
  std::string formula;
  Parameters params;
  
  };

#endif  // BTagEntryS_H