#include <TFile.h>
#include <TTree.h>

#include <string>
using std::string;

void makeTempClass(string fname="", string tname="tree", string cname="TempClass"){
	if(fname.size()==0) return;
	TFile* file = TFile::Open(fname.c_str());
	TTree* tree = (TTree*)file->Get(tname.c_str());
	tree->MakeClass(cname.c_str());
}
