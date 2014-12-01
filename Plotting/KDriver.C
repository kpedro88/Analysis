#include "KCode/KManager.h"
#include "KCode/KParser.h"

using namespace std;

//how to run:
//root -l 'KDriver.C++("tree","input_main.txt")'
void KDriver(string dir, string inFile, bool doPrint = false){
	KManager k(inFile,dir);
	k.SetPrint(doPrint);
	k.DrawPlots();
}
