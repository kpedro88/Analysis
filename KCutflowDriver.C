#include "KCode/KCutflow.h"

using namespace std;

//how to run:
//root -b -l -q 'KCutflowDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV6/tree_signal/tree_T1tttt_1500_100.root")'
void KCutflowDriver(string fname="", bool printerrors=false, int prcsn=2){
	if(fname.size()==0){
		cout << "Recompiled KCutflowDriver, exiting." << endl;
		return;
	}
	
	cout << fixed << setprecision(prcsn);
	KCutflow k(fname);
	k.PrintEfficiency(printerrors);
}
