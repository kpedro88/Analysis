float btagSF(double x, int isb, int do_unc=0){

	//Using payloads for EPS13
	//https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-pt_WITHttbar_payload_EPS13.txt
	float ptmin[16] = {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600};
	float ptmax[16] = {30, 40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 600, 800};

	//Tagger: CSVL within 20 < pt < 800 GeV, abs(eta) < 2.4, x = pt
	float SFb = 0.997942*((1.+(0.00923753*x))/(1.+(0.0096119*x)));
	float SFb_error[] = {
		0.033299,
		0.0146768,
		0.013803,
		0.0170145,
		0.0166976,
		0.0137879,
		0.0149072,
		0.0153068,
		0.0133077,
		0.0123737,
		0.0157152,
		0.0175161,
		0.0209241,
		0.0278605,
		0.0346928,
		0.0350099
	};
  
	double SFunc = 0.;
	for (int i=0; i<15; ++i){
		if (x > ptmin[i] && x < ptmax[i+1]){
			SFunc = SFb_error[i];
			break;
		}
	}
	
	double unc = 0;
	
	if (isb) unc = SFunc;
	else unc = 2*SFunc;
	
	return SFb + do_unc*unc;
}
