double btagEFF(double x, int isb){

  //x == discriminator value
  
  double effb = 1.;
  double effc = 1.;
  //double effb_err_max = 0.;
  //This returns btag efficiency measured in 2011 data ttbar events
  //https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2011_Data_and_MC
  effb = -4.46932526854*x*x*x*x+7.32781975653*x*x*x-3.78459588569*x*x+0.221027515486*x+0.970299300468;

  //Functional form for b/c-tagging efficiency on MC:
  //  effb = -1.73338329789*x*x*x*x +  1.26161794785*x*x*x +  0.784721653518*x*x +  -1.03328577451*x +  1.04305075822
  effc = -1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724;

  double eff = 1.;
  if (isb==1)
    eff = effb;
  else
    eff = effc;
  
  return eff;
}
