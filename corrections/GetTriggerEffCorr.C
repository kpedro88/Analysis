#ifndef GETTRIGGEREFFCORR_C
#define GETTRIGGEREFFCORR_C

#define nMHT 4

double GetTriggerEffCorr(bool signal, double MHT, bool realMET, int statUnc=0, int systUnc=0){
	double w = 1;
	if(signal) {
		double central[nMHT] = {0.982, 0.985, 0.995, 1.00};
		double stat_up[nMHT] = {0.004, 0.006, 0.003, 0.00};
		double stat_dn[nMHT] = {0.005, 0.009, 0.007, 0.02};
		double syst_up[nMHT] = {0.02, 0.02, 0.02, 0.02}; 
		double syst_dn[nMHT] = {0.02, 0.02, 0.02, 0.02}; 

		//bin lower edges
		double MHTbins[nMHT+1] = {250,300,350,500,9999};
		
		int bin = -1;
		for(int b = 0; b < nMHT; ++b){
			if(MHT > MHTbins[b] && MHT <= MHTbins[b+1]){
				bin = b;
				break;
			}
		}
		
		if(bin==-1) return 0.;
		
		w = central[bin];
	
		if(statUnc==1) w += stat_up[bin];
		else if(statUnc==-1) w -= stat_dn[bin];
		
		if(systUnc==1) w += syst_up[bin];
		else if(systUnc==-1) w -= syst_dn[bin];
	}
	else {
		w = 1;
		//new SM/fakeMET effs not implemented yet
	}
	
	return w;
}

#endif