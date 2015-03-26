#include "KCode/KPlotManager.h"

using namespace std;

//how to run:
//root -l 'KPlotDriverSyst.C+("tree_main","input_final_syst.txt","btagSFunc",1)'
void KPlotDriverSyst(string dir, string inFile, string syst_opt, int syst_val, bool doPrint = false){
	KPlotManager k(inFile,dir);
	k.SetPrint(doPrint);
	
	//additional options for RPV
	OptionMap* option = k.GetOption();
	option->Set<int>(syst_opt,syst_val);
	
	//k.ListOptions();
	k.DrawPlots();
}

void AllSyst(bool do_rpv=0){
	string inFile = "input_final_syst.txt";
	if(do_rpv) inFile = "input_final_lqd321_syst.txt";

	cout << "central:" << endl;
	KDriverSyst("tree_main",inFile,"btagSFunc",0);
	cout << "btagUp:" << endl;
	KDriverSyst("tree_main",inFile,"btagSFunc",1);
	cout << "btagDown:" << endl;
	KDriverSyst("tree_main",inFile,"btagSFunc",-1);
	cout << "mistagUp:" << endl;
	KDriverSyst("tree_main",inFile,"mistagSFunc",1);
	cout << "mistagDown:" << endl;
	KDriverSyst("tree_main",inFile,"mistagSFunc",-1);
}