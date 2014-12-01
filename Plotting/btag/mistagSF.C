float mistagSF(double x, double y, int do_unc=0){
	//x= pT
	//y= eta
	//This function is only for CSVL taggger
	//Does not take into account syst+stat uncertainties on SF

	//using payloads for EPS13
	//https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_EPS2013.C
	
	TString meanminmax = "mean";
	if(do_unc==1) meanminmax = "max";
	else if(do_unc==-1) meanminmax = "min";
	
	//Values are available only for jets with pT<1000
	if (x>1000) x = 999;

	float tmpSFl = 1.;
	
	if (TMath::Abs(y)<0.5){
		if( meanminmax == "mean" ) tmpSFl = ((1.01177+(0.0023066*x))+(-4.56052e-06*(x*x)))+(2.57917e-09*(x*(x*x)));
		else if( meanminmax == "min" ) tmpSFl = ((0.977761+(0.00170704*x))+(-3.2197e-06*(x*x)))+(1.78139e-09*(x*(x*x)));
		else if( meanminmax == "max" ) tmpSFl = ((1.04582+(0.00290226*x))+(-5.89124e-06*(x*x)))+(3.37128e-09*(x*(x*x)));
	}
	else if(TMath::Abs(y)>0.5&&TMath::Abs(y)<1.0){
		if( meanminmax == "mean" ) tmpSFl = ((0.975966+(0.00196354*x))+(-3.83768e-06*(x*x)))+(2.17466e-09*(x*(x*x)));
		else if( meanminmax == "min" ) tmpSFl = ((0.945135+(0.00146006*x))+(-2.70048e-06*(x*x)))+(1.4883e-09*(x*(x*x)));
		else if( meanminmax == "max" ) tmpSFl = ((1.00683+(0.00246404*x))+(-4.96729e-06*(x*x)))+(2.85697e-09*(x*(x*x)));
	}
	else if(TMath::Abs(y)>1.0&&TMath::Abs(y)<1.5){
		if( meanminmax == "mean" ) tmpSFl = ((0.93821+(0.00180935*x))+(-3.86937e-06*(x*x)))+(2.43222e-09*(x*(x*x)));
		else if( meanminmax == "min" ) tmpSFl = ((0.911657+(0.00142008*x))+(-2.87569e-06*(x*x)))+(1.76619e-09*(x*(x*x)));
		else if( meanminmax == "max" ) tmpSFl = ((0.964787+(0.00219574*x))+(-4.85552e-06*(x*x)))+(3.09457e-09*(x*(x*x)));
	}
	else if(TMath::Abs(y)>1.5&&TMath::Abs(y)<2.4){
		if (x>850) x = 849; //smaller ptmax in endcap
		
		if( meanminmax == "mean" ) tmpSFl = ((1.00022+(0.0010998*x))+(-3.10672e-06*(x*x)))+(2.35006e-09*(x*(x*x)));
		else if( meanminmax == "min" ) tmpSFl = ((0.970045+(0.000862284*x))+(-2.31714e-06*(x*x)))+(1.68866e-09*(x*(x*x)));
		else if( meanminmax == "max" ) tmpSFl = ((1.03039+(0.0013358*x))+(-3.89284e-06*(x*x)))+(3.01155e-09*(x*(x*x)));
	}
	
	return tmpSFl;
}
