double GetTriggerEffWeight(double MHT, bool realMET) {
  if (realMET) {
    if (MHT<150) return 0.0389;
    else if (MHT<200) return 0.3208;
    else if (MHT<500) return 0.987;
    else return 1.0;
  } else {
    if (MHT<100) return 0.0389;
    else if (MHT<150) return 0.3208;
    else if (MHT<200) return 0.9127;
    else if (MHT<500) return 0.985;
    else return 1.0;
  }
}

double GetTriggerEffStatUncHi(double MHT, bool realMET) {
  if (realMET) {
    if (MHT<150) return 0;
    else if (MHT<200) return 0;
    else if (MHT<500) return 0.007;
    else return 0.0;
  } else {
    if (MHT<100) return 0;
    else if (MHT<150) return 0;
    else if (MHT<200) return 0.004;
    else if (MHT<500) return 0;
    else return 1.0;
  }
}

double GetTriggerEffStatUncLo(double MHT, bool realMET) {
  if (realMET) {
    if (MHT<150) return 0;
    else if (MHT<200) return 0;
    else if (MHT<500) return 0.012;
    else return 0.025;
  } else {
    if (MHT<150) return 0;
    else if (MHT<200) return 0;
    else if (MHT<500) return 0.006;
    else return 0.021;
  }
}

double GetTriggerEffSystUncHi(double MHT, bool realMET) {
  if (realMET) {
    if (MHT<150) return 0;
    else if (MHT<200) return 0;
    else if (MHT<500) return 0.006;
    else return 0.0;
  } else {
    return 0;
  }
}

double GetTriggerEffSystUncLo(double MHT, bool realMET) {
  if (realMET) {
    if (MHT<150) return 0;
    else if (MHT<200) return 0;
    else if (MHT<500) return 0.008;
    else return 0.07;
  } else {
    return 0;
  }
}

double GetTriggerEffCorr(double MHT, bool realMET, int statUnc=0, int systUnc=0){
	double w = GetTriggerEffWeight(MHT, realMET);
	
	if(statUnc==1) w += GetTriggerEffStatUncHi(MHT, realMET);
	else if(statUnc==-1) w -= GetTriggerEffStatUncLo(MHT, realMET);
	
	if(systUnc==1) w += GetTriggerEffSystUncHi(MHT, realMET);
	else if(systUnc==-1) w -= GetTriggerEffSystUncLo(MHT, realMET);
	
	return w;
}