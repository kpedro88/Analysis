#include "KCode/KPlotManager.h"

using namespace std;

//how to run:
//root -l 'KPlotDriverShapeSyst.C+("tree_main_JESUp","input_final_dd_shapesyst_datacard.txt","datacard_dd_JESUp")'
void KPlotDriverShapeSyst(string dir, string inFile, string oname, bool doPrint = false){
	KPlotManager k(inFile,dir);
	k.SetPrint(doPrint);
	
	//additional options for shape syst
	OptionMap* option = k.GetOption();
	option->Set<string>("rootfile",oname);
	
	//k.ListOptions();
	k.DrawPlots();
}