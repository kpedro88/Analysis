#include <TFile.h>
#include <TTree.h>

#include <string>
#include <stdexcept>
using std::string;

void makeTempClass(string fname="", string tname="tree", string cname="TempClass"){
	if(fname.size()==0) return;
	TFile* file = TFile::Open(fname.c_str());
	if(!file){
		throw std::runtime_error("Could not open file: "+fname);
	}
	TTree* tree = (TTree*)file->Get(tname.c_str());
	if(!tree){
		throw std::runtime_error("Could not get tree: "+tname);
	}
	tree->MakeClass(cname.c_str());
}
