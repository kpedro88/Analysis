#include "KCode/KPlotManager.h"

using namespace std;

//how to run:
//root -l 'KPlotDriver.C+("tree","input_main.txt")'
void KPlotDriver(string dir, string inFile, bool doPrint = false){
	KPlotManager k(inFile,dir);
	k.SetPrint(doPrint);
	k.DrawPlots();
}
