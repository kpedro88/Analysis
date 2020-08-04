#include "TFile.h"
#include "TTree.h"
#include "TH2.h"

#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <exception>
#include <iostream>

#include "KCode/KParser.h"
#include "KCode/KMap.h"

using namespace std;

void makeJESTree(string eosdir, string year, string outname="", bool AK8 = true, bool passed = true, string xrdir="root://cmseos.fnal.gov/"){
	if(eosdir.back()!='/') eosdir += '/';
	if(xrdir.back()!='/') xrdir += '/';
	
	string eosdirclean = eosdir; KParser::clean(eosdirclean);
	string flist = "eosls_"+eosdirclean+".txt";
	string cmd = "eos "+xrdir+" ls "+eosdir+" | grep "+year+" > "+flist;
	auto exit_code = system(cmd.c_str());
	if(exit_code != 0) throw runtime_error("Exit "+to_string(exit_code)+" from eos ls");
	
	ifstream inlist(flist.c_str());
	vector<string> files;
	if(inlist.is_open()){
		string line;
		while(getline(inlist,line)){
			files.push_back(line);
		}
	}
	else throw runtime_error("Could not open file: "+flist);
	
	//keep same format as in MakeAllDCsyst
	int mParent, mChild, alpha;
	double rinv, JESfactor1, JESfactor2, JESfactor;
	TTree* tree = new TTree("tree","systematics");
	tree->Branch("mZprime",&mParent,"mZprime/I");
	tree->Branch("mDark",&mChild,"mDark/I");
	tree->Branch("rinv",&rinv,"rinv/D");
	tree->Branch("alpha",&alpha,"alpha/I");
	tree->Branch("JESfactor1",&JESfactor1,"JESfactor1/D");
	tree->Branch("JESfactor2",&JESfactor2,"JESfactor2/D");
	tree->Branch("JESfactor",&JESfactor,"JESfactor/D");
	
	map<string,double> alpha_vals{
		{"peak",-2},
		{"high",-1},
		{"low",-3},
	};
	int ctr = 0;
	string suff = passed ? "_passed" : "";
	string sAK8 = AK8 ? "AK8" : "";
	for(const auto& fname : files){
		if(ctr % 100 == 0) cout << "Processing " << ctr << " / " << files.size() << endl;
		
		//parse fname to get signal params
		vector<string> setnames;
		KParser::process(fname,'_',setnames);
		mParent = KParser::getOptionValue<int>(setnames[2]);
		mChild = KParser::getOptionValue<int>(setnames[3]);
		rinv = KParser::getOptionValue<double>(setnames[4]);
		alpha = alpha_vals[setnames[5]];
		
		//open file and get histo
		TFile* file = KOpen(xrdir+eosdir+fname);
		TH2* h_JESfactor = KGet<TH2>(file,"JESfactor"+sAK8+suff);
		JESfactor = h_JESfactor->GetMean();
		TH1D* h_tmp = nullptr;
		h_tmp = h_JESfactor->ProjectionX("JESfactor1",1,1);
		JESfactor1 = h_tmp->GetMean();
		h_tmp = h_JESfactor->ProjectionX("JESfactor2",2,2);
		JESfactor2 = h_tmp->GetMean();
		
		tree->Fill();
		
		++ctr;
	}
	
	//name file as eos base dir
	vector<string> dirnames;
	KParser::process(eosdir,'/',dirnames);
	string ofname = outname.empty() ? dirnames.back()+".root" : outname;
	TFile* fout = KOpen(ofname.c_str(),"RECREATE");
	fout->cd();
	tree->Write();
	fout->Close();
	
	//remove txt file
	string rcmd = "rm "+flist;
	system(rcmd.c_str());
}