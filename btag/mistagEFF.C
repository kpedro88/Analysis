double mistagEFF(double x, double eta) {

  //These correspond still to the 2011 payloads
  //https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/MistagFuncs.C

  //Values are available only for jets with pT<670
  if (x>670) x=669;

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
