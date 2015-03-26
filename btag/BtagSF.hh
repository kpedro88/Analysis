#ifndef BtagSF_hh
#define BtagSF_hh

#include "TRandom3.h"
#include <iostream>

class BtagSF{
  
  public:
  
  BtagSF( int seed=0 );
  ~BtagSF();
    
  Bool_t isbtagged(Float_t pt, Float_t eta, Float_t csv, Int_t jetflavor, Bool_t isdata, UInt_t btagsys, UInt_t mistagsys, Bool_t is2012);
  Double_t getEffb(double y);
  Double_t getEffc(double x);
  Double_t getSFb(Float_t pt, UInt_t btagsys, Bool_t is2012);
  Double_t getSFc(Float_t pt, UInt_t btagsys, Bool_t is2012);
  Double_t getSFl(Float_t pt, Float_t eta, UInt_t mistagsys, Bool_t is2012);
  Double_t getMistag(Float_t pt, Float_t eta);

  enum { kNo, kDown, kUp };                     // systematic variations 


 private:
  
  TRandom3* randm;

};


BtagSF::BtagSF( int seed ) {

  randm = new TRandom3(seed);

}

BtagSF::~BtagSF() {

  delete randm;

}


Bool_t BtagSF::isbtagged(Float_t pt, Float_t eta, Float_t csv, Int_t jetflavor, Bool_t isdata, UInt_t btagsys, UInt_t mistagsys, Bool_t is2012)
{
  Bool_t btagged = kFALSE;

  double csvCut = 0.244;

  if(isdata) {
    if(csv>csvCut) btagged = kTRUE;
    else          btagged = kFALSE;
    return btagged;
  }

  Double_t SFb = 0.0;
  Double_t eff_b = getEffb(csvCut);//0.719;

  SFb = getSFb(pt, btagsys, is2012);

  Double_t promoteProb_btag=0; // ~probability to promote to tagged
  Double_t demoteProb_btag=0; // ~probability to demote from tagged

  if(SFb < 1) {
    demoteProb_btag = fabs(1.0 - SFb);
  } else {
    promoteProb_btag = fabs(SFb - 1.0)/((SFb/eff_b) - 1.0);
  }

  if(fabs(jetflavor) == 5) {                // real b-jet
    if(csv > csvCut) {                      // if tagged
      btagged = kTRUE;
      cout << "Here am I " << endl;
      cout << "Random " << randm->Uniform() << endl;
      cout << "demoteProb_btag " << demoteProb_btag<<endl;
      if(demoteProb_btag > 0 && randm->Uniform() > demoteProb_btag) btagged = kTRUE;  // leave it tagged
      else                                                          btagged = kFALSE; // demote it to untagged
    } else {
      btagged = kFALSE;
      if(promoteProb_btag > 0 && randm->Uniform() < promoteProb_btag) btagged = kTRUE;  // promote it to tagged
      else                                                            btagged = kFALSE; // leave it untagged
    }
    return btagged;
  }
  
  // not a real b-jet, apply mistag

  Double_t SFl = 0, eff_l = 0;
  // c or light
  if(fabs(jetflavor) == 4) {
    // SFc = SFb with twice the quoted uncertainty
    SFl = getSFc(pt, btagsys, is2012);
    //    eff_l = 0.192*SFl; // eff_c in MC for CSVM = (-1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724), x = 0.679
    eff_l = SFl*getEffc(csvCut);
  } else {
    SFl = getSFl(pt, eta, mistagsys, is2012);
    eff_l = getMistag(pt, eta);
  }

  Double_t promoteProb_mistag=0; // ~probability to promote to tagged
  Double_t demoteProb_mistag=0; // ~probability to demote from tagged

  if(SFl > 1) {
    promoteProb_mistag = fabs(SFl - 1.0)/((SFl/eff_l) - 1.0);
  }
  else {
    demoteProb_mistag = SFl;
  }

  if(csv > csvCut) {         // if tagged
    btagged = kTRUE;
    if(demoteProb_mistag > 0 && randm->Uniform() > demoteProb_mistag) btagged = kFALSE; // demote it to untagged
    else                                                              btagged = kTRUE;  // leave it tagged
  }
  else {                    // not tagged
    btagged = kFALSE;
    if(promoteProb_mistag > 0 && randm->Uniform() < promoteProb_mistag) btagged = kTRUE;  // promote it to tagged
    else                                                                btagged = kFALSE; // leave it untagged
  }

  return btagged;
}

Double_t BtagSF::getEffb(double y){

  //y == discriminator value
  //This returns btag efficiency measured in 2011 data ttbar events
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2011_Data_and_MC
  double eff = -4.46932526854*y*y*y*y+7.32781975653*y*y*y-3.78459588569*y*y+0.221027515486*y+0.970299300468;
  return eff;
}

Double_t BtagSF::getEffc(double x){

  //x == discriminator value
  //This returns btag efficiency measured in 2011 MC ttbar events
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2011_Data_and_MC
  double eff = -1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724;
  return eff;

}

Double_t BtagSF::getSFb(Float_t pt, UInt_t btagsys, Bool_t is2012){

  //IMPORTANT:  PAYLOADS/UNCERTAINTIES FOR 2011 ARE NOT THE CORRECT ONES!

  // pT dependent scale factors
  // Tagger: CSVL within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
  // for pt > 670 (800) GeV: use the SFb value at 670 (800) GeV with twice the quoted uncertainty for 2011 (2012)
  // for pt < 30 (20) GeV: use the SFb value at 30 (20) GeV with a +/-0.12 absolute uncertainty (twice the quoted uncertainty) for 2011 (2012)
  // i.e SFb(pt<30) = SFb(pt=30) +/- 0.12, so the relative uncertainty is 0.12/SFb(pt=30) for 2011
  

  Float_t x = pt;
  Double_t SFb = 1.0; 
  if(!is2012) {
    if(pt >= 670.0) x = 669.9;
    if(pt < 30.0) x = 30.0;
    SFb = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x))); 
  } else {
    if(pt >= 800.0) x = 799.9;
    if(pt < 20.0) x = 20.0;
    SFb = 0.981149*((1.+(-0.000713295*x))/(1.+(-0.000703264*x)));
  }

  if(btagsys == kNo)        return SFb;

  Double_t SFb_error_2011[] = {0.0295675, 0.0295095, 0.0210867, 0.0219349, 0.0227033, 0.0204062, 0.0185857, 0.0256242, 0.0383341, 0.0409675, 0.0420284, 0.0541299, 0.0578761, 0.0655432};
  Float_t ptmin_2011[] = {30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500};
  Float_t ptmax_2011[] = {40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670};
  Double_t SFb_error_2012[] = SFb_error[] = {
    0.0484285,
    0.0126178,
    0.0120027,
    0.0141137,
    0.0145441,
    0.0131145,
    0.0168479,
    0.0160836,
    0.0126209,
    0.0136017,
    0.019182,
    0.0198805,
    0.0386531,
    0.0392831,
    0.0481008,
    0.0474291 };
  Float_t ptmin_2012[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
  Float_t ptmax_2012[] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

  Double_t SFb_error_x = 0.0;

  UInt_t nbins = is2012 ? 16 : 14;

  for (UInt_t ibin=0; ibin<nbins; ibin++) {
    if(!is2012) {
      if(x >= ptmin_2011[ibin] && x < ptmax_2011[ibin]) SFb_error_x = SFb_error_2011[ibin];
    } else {
      if(x >= ptmin_2012[ibin] && x < ptmax_2012[ibin]) SFb_error_x = SFb_error_2012[ibin];
    }
  }
  if(!is2012) {
    if(pt >= 670.0) SFb_error_x = 2.0*0.0655432;
    if(pt <   30.0) SFb_error_x = 0.12;
  } else {
    if(pt >= 800.0) SFb_error_x = 2.0*0.0717567;
    if(pt <   20.0) SFb_error_x = 2.0*0.0554504;
  }

  Double_t scalefactor = SFb;

  if(btagsys == kDown) scalefactor = (SFb - SFb_error_x);
  if(btagsys == kUp)   scalefactor = (SFb + SFb_error_x);

  return scalefactor;

}

Double_t BtagSF::getSFc(Float_t pt, UInt_t btagsys, Bool_t is2012)
{
  //IMPORTANT:  PAYLOADS/UNCERTAINTIES FOR 2011 ARE NOT THE CORRECT ONES!
  // SFc = SFb with twice the quoted uncertainty

  Float_t x = pt;
  Double_t SFc = 1.0;
  if(!is2012) {
    if(pt >= 670.0) x = 669.9;
    if(pt < 30.0) x = 30.0;
    SFc = 0.6981*((1.+(0.414063*x))/(1.+(0.300155*x)));
  } else {
    if(pt >= 800.0) x = 799.9;
    if(pt < 20.0) x = 20.0;
    SFc = 0.981149*((1.+(-0.000713295*x))/(1.+(-0.000703264*x)));
  }

  if(btagsys == kNo)        return SFc;

  Double_t SFb_error_2011[] = {0.0295675, 0.0295095, 0.0210867, 0.0219349, 0.0227033, 0.0204062, 0.0185857, 0.0256242, 0.0383341, 0.0409675, 0.0420284, 0.0541299, 0.0578761, 0.0655432};
  Float_t ptmin_2011[] = {30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500};
  Float_t ptmax_2011[] = {40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670};
  Double_t SFb_error_2012[] =  SFb_error[] = {
    0.0484285,
    0.0126178,
    0.0120027,
    0.0141137,
    0.0145441,
    0.0131145,
    0.0168479,
    0.0160836,
    0.0126209,
    0.0136017,
    0.019182,
    0.0198805,
    0.0386531,
    0.0392831,
    0.0481008,
    0.0474291 };
  Float_t ptmin_2012[] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
  Float_t ptmax_2012[] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

  Double_t SFc_error_x = 0.0;

  UInt_t nbins = is2012 ? 16 : 14;

  for (UInt_t ibin=0; ibin<nbins; ibin++) {
    if(!is2012) {
      if(x >= ptmin_2011[ibin] && x < ptmax_2011[ibin]) SFc_error_x = 2.0*SFb_error_2011[ibin];
    } else {
      if(x >= ptmin_2012[ibin] && x < ptmax_2012[ibin]) SFc_error_x = 2.0*SFb_error_2012[ibin];
    }

  }
  if(!is2012) {
    if(pt >= 670.0) SFc_error_x = 2.0*2.0*0.0655432;
    if(pt <   30.0) SFc_error_x = 2.0*0.12;
  } else {
    if(pt >= 800.0) SFc_error_x = 2.0*2.0*0.0717567;
    if(pt <   20.0) SFc_error_x = 2.0*2.0*0.0554504;
  }

  Double_t scalefactor = SFc;

  if(btagsys == kDown) scalefactor = (SFc - SFc_error_x);
  if(btagsys == kUp)   scalefactor = (SFc + SFc_error_x);

  return scalefactor;

}

Double_t BtagSF::getSFl(Float_t pt, Float_t eta, UInt_t mistagsys, Bool_t is2012){
  
  //IMPORTANT:  PAYLOADS/UNCERTAINTIES FOR 2011 ARE NOT THE CORRECT ONES!
  
  Float_t x = min(double(pt), is2012 ? 670.0 : 800.0);
  
  Double_t SFl = 0;
  
  if(!is2012) {
    if(fabs(eta) >= 0.0 && fabs(eta) < 0.8) {
      if(mistagsys == kNo)        SFl = ((1.06182+(0.000617034*x))+(-1.5732e-06*(x*x)))+(3.02909e-10*(x*(x*x)));
      else if(mistagsys == kDown) SFl = ((0.972455+(7.51396e-06*x))+(4.91857e-07*(x*x)))+(-1.47661e-09*(x*(x*x)));
      else if(mistagsys == kUp)   SFl = ((1.15116+(0.00122657*x))+(-3.63826e-06*(x*x)))+(2.08242e-09*(x*(x*x)));
    } else if(fabs(eta) >= 0.8 && fabs(eta) < 1.6) {
      if(mistagsys == kNo)        SFl = ((1.111+(-9.64191e-06*x))+(1.80811e-07*(x*x)))+(-5.44868e-10*(x*(x*x)));
      else if(mistagsys == kDown) SFl = ((1.02055+(-0.000378856*x))+(1.49029e-06*(x*x)))+(-1.74966e-09*(x*(x*x)));
      else if(mistagsys == kUp)   SFl = ((1.20146+(0.000359543*x))+(-1.12866e-06*(x*x)))+(6.59918e-10*(x*(x*x)));
    } else if(fabs(eta) >= 1.6 && fabs(eta) < 2.4) {
      if(mistagsys == kNo)        SFl = ((1.08498+(-0.000701422*x))+(3.43612e-06*(x*x)))+(-4.11794e-09*(x*(x*x)));
      else if(mistagsys == kDown) SFl = ((0.983476+(-0.000607242*x))+(3.17997e-06*(x*x)))+(-4.01242e-09*(x*(x*x)));
      else if(mistagsys == kUp)   SFl = ((1.18654+(-0.000795808*x))+(3.69226e-06*(x*x)))+(-4.22347e-09*(x*(x*x)));
    }
  } else {
    if (fabs(eta)>=0.0 && fabs(eta)<0.5){
      if (mistagsys == kNo)  SFl = ((1.04901+(0.00152181*x))+(-3.43568e-06*(x*x)))+(2.17219e-09*(x*(x*x)));
      if( mistagsys == kDown ) SFl = ((0.973773+(0.00103049*x))+(-2.2277e-06*(x*x)))+(1.37208e-09*(x*(x*x)));
      if( mistagsys == kUp ) SFl = ((1.12424+(0.00201136*x))+(-4.64021e-06*(x*x)))+(2.97219e-09*(x*(x*x)));
    } else if(fabs(eta) >= 0.5 && fabs(eta) < 1.0) {
      if(mistagsys == kNo)        SFl = ((0.991915+(0.00172552*x))+(-3.92652e-06*(x*x)))+(2.56816e-09*(x*(x*x)));
      if(mistagsys == kDown) SFl = ((0.921518+(0.00129098*x))+(-2.86488e-06*(x*x)))+(1.86022e-09*(x*(x*x)));
      if(mistagsys == kUp)   SFl = ((1.06231+(0.00215815*x))+(-4.9844e-06*(x*x)))+(3.27623e-09*(x*(x*x)));
    } else if(fabs(eta) >= 1.0 && fabs(eta) < 1.5) {
      if(mistagsys == kNo)   SFl = ((0.962127+(0.00192796*x))+(-4.53385e-06*(x*x)))+(3.0605e-09*(x*(x*x)));
      if(mistagsys == kDown) SFl = ((0.895419+(0.00153387*x))+(-3.48409e-06*(x*x)))+(2.30899e-09*(x*(x*x)));
      if(mistagsys == kUp)   SFl = ((1.02883+(0.00231985*x))+(-5.57924e-06*(x*x)))+(3.81235e-09*(x*(x*x))); 
    }else if(fabs(eta) >= 1.5 && fabs(eta) < 2.4) {
      if(mistagsys == kNo)   SFl = ((1.06121+(0.000332747*x))+(-8.81201e-07*(x*x)))+(7.43896e-10*(x*(x*x)));
      if(mistagsys == kDown) SFl = ((0.983607+(0.000196747*x))+(-3.98327e-07*(x*x)))+(2.95764e-10*(x*(x*x)));
      if(mistagsys == kUp)   SFl = ((1.1388+(0.000468418*x))+(-1.36341e-06*(x*x)))+(1.19256e-09*(x*(x*x)));
    }
  }
  
  return SFl;
  
}

Double_t BtagSF::getMistag(Float_t pt, Float_t eta)
{

  //These correspond still to the 2011 payloads
  //https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/MistagFuncs.C

  Float_t x = min(double(pt), 670.0);

  Double_t eff_l = 0.0;
  
  if(fabs(eta) >= 0.0 && fabs(eta) < 0.5) 
    eff_l = 242534*(((1+(0.0182863*x))+(4.50105e-05*(x*x)))/(1+(108569*x)));
  else if (fabs(eta) >= 0.5 && fabs(eta) < 1.0) 
    eff_l = 129.938*(((1+(0.0197657*x))+(4.73472e-05*(x*x)))/(1+(55.2415*x)));
  else if (fabs(eta) >= 1.0 && fabs(eta) < 1.5)
    eff_l = 592.214*(((1+(0.00671207*x))+(6.46109e-05*(x*x)))/(1+(134.318*x)));
  else if (fabs(eta) >= 1.5 && fabs(eta) < 2.4)
    eff_l = 93329*(((1+(0.0219705*x))+(3.76566e-05*(x*x)))/(1+(18245.1*x)));      
  
  return eff_l;

}

#endif


