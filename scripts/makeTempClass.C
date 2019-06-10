#include "KCode/KMap.h"

#include <TFile.h>
#include <TTree.h>

#include <string>
using std::string;

void makeTempClass(string fname="", string tname="tree", string cname="TempClass"){
	if(fname.empty()) return;
	TFile* file = KOpen(fname);
	TTree* tree = KGet<TTree>(file,tname);
	tree->MakeClass(cname.c_str());
}
