double GetSMTriggerEff(double MHT, bool realMET) {
  if (realMET) {
    if (MHT<50) return 0.0080;
    else if (MHT<100) return 0.0988;
    else if (MHT<150) return 0.5424;
    else if (MHT<200) return 0.9011;
    else if (MHT<250) return 0.9846;
    else if (MHT<300) return 0.9961;
    else if (MHT<400) return 0.9865;
    else if (MHT<500) return 1.0000;
    else return 1.0000;
  } 
  else {
    if (MHT<50) return 0.0089;
    else if (MHT<100) return 0.0522;
    else if (MHT<150) return 0.2359;
    else if (MHT<200) return 0.5525;
    else if (MHT<250) return 0.7316;
    else if (MHT<300) return 0.8041;
    else if (MHT<400) return 0.8348;
    else if (MHT<500) return 0.8734;
    else return 0.8195;
  }
}
double GetSMTriggerEffStatUncHi(double MHT, bool realMET) {
  if (realMET) {
    if (MHT<50) return 0.0007;
    else if (MHT<100) return 0.0033;
    else if (MHT<150) return 0.0099;
    else if (MHT<200) return 0.0091;
    else if (MHT<250) return 0.0053;
    else if (MHT<300) return 0.0032;
    else if (MHT<400) return 0.0073;
    else if (MHT<500) return 0.0000;
    else return 0.0000;
  } 
  else {
    if (MHT<50) return 0.0001;
    else if (MHT<100) return 0.0004;
    else if (MHT<150) return 0.0017;
    else if (MHT<200) return 0.0047;
    else if (MHT<250) return 0.0078;
    else if (MHT<300) return 0.0111;
    else if (MHT<400) return 0.0134;
    else if (MHT<500) return 0.0220;
    else return 0.0343;
  }
}
double GetSMTriggerEffStatUncLo(double MHT, bool realMET) {
  if (realMET) {
    if (MHT<50) return 0.0006;
    else if (MHT<100) return 0.0032;
    else if (MHT<150) return 0.0099;
    else if (MHT<200) return 0.0099;
    else if (MHT<250) return 0.0075;
    else if (MHT<300) return 0.0089;
    else if (MHT<400) return 0.0000;
    else if (MHT<500) return 0.0135;
    else return 0.0135;
  } 
  else {
    if (MHT<50) return 0.0001;
    else if (MHT<100) return 0.0004;
    else if (MHT<150) return 0.0017;
    else if (MHT<200) return 0.0047;
    else if (MHT<250) return 0.0080;
    else if (MHT<300) return 0.0115;
    else if (MHT<400) return 0.0143;
    else if (MHT<500) return 0.0254;
    else return 0.0000;
  }
}
double GetSignalTriggerEffWeight(double MHT) {
    if (MHT<200) return 0;
    else if (MHT<500) return 0.9887;
    else return 1.0000;
}
double GetSignalTriggerEffStatUncHi(double MHT) {
    if (MHT<200) return 0;
    else if (MHT<500) return 0.0032;
    else return 0.0000;
}
double GetSignalTriggerEffStatUncLo(double MHT) {
    if (MHT<200) return 0;
    else if (MHT<500) return 0.0043;
    else return 0.0135;
}
double GetSignalTriggerEffSystUncHi(double MHT) {
    if (MHT<200) return 0;
    else if (MHT<500) return 0.0081;
    else return 0.0000;
}
double GetSignalTriggerEffSystUncLo(double MHT) {
    if (MHT<200) return 0;
    else if (MHT<500) return 0.0130;
    else return 0.0001;
}

double GetTriggerEffCorr(bool signal, double MHT, bool realMET, int statUnc=0, int systUnc=0){
	double w = 1;
	if(signal) {
		w = GetSignalTriggerEffWeight(MHT);
	
		if(statUnc==1) w += GetSignalTriggerEffStatUncHi(MHT);
		else if(statUnc==-1) w -= GetSignalTriggerEffStatUncLo(MHT);
		
		if(systUnc==1) w += GetSignalTriggerEffSystUncHi(MHT);
		else if(systUnc==-1) w -= GetSignalTriggerEffSystUncLo(MHT);
	}
	else {
		w = GetSMTriggerEff(MHT, realMET);
	
		if(statUnc==1) w += GetSMTriggerEffStatUncHi(MHT, realMET);
		else if(statUnc==-1) w -= GetSMTriggerEffStatUncLo(MHT, realMET);		
	}
	
	return w;
}