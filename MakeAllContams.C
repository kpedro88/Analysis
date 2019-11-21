//custom headers
#include "KCode/KParser.h"

//STL headers
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <exception>
#include <cmath>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"

using namespace std;

void MakeAllContams(string model="", string year="", string indir="", string outdir="", string xrdir="root://cmseos.fnal.gov/"){
	//checks
	if(model.empty()) return;
	if(!xrdir.empty() and xrdir[xrdir.size()-1]!='/') xrdir += "/";
	if(!indir.empty() and indir[indir.size()-1]!='/') indir += "/";
	if(!outdir.empty() and outdir[outdir.size()-1]!='/') outdir += "/";

	//local or remote dir
	string cmd;
	if(indir.substr(0,6)=="/store") cmd = "xrdfs "+xrdir+" ls "+indir;
	else cmd = "ls "+indir;
	
	//select desired files and write output
	string olist = outdir+"/flist_"+model+"_"+year+".root";
	cmd += " | grep RA2bin_signal | grep "+model+" | grep "+year+"_ > "+olist;

	system(cmd.c_str());

	//read output
	vector<string> models;
	ifstream infile(olist.c_str());
	if(infile.is_open()){
		string line;
		while(getline(infile,line)){
			models.push_back(line.substr(line.find_last_of('/')+1,string::npos));
		}
	}
	cout << models.size() << " models" << endl;

	TTree* tree = new TTree("tree","systematics");
	int mMother, mLSP;
	double contam;
	tree->Branch("mMother",&mMother,"mMother/I");
	tree->Branch("mLSP",&mLSP,"mLSP/I");
	tree->Branch("contam",&contam,"contam/D");

	for(const auto& point : models){
		//reset branch values
		mMother = mLSP = 0;
		contam = 0.;

		vector<string> setnames;
		KParser::process(point,'_',setnames);
		mMother = KParser::getOptionValue<int>(setnames[3]);
		mLSP = KParser::getOptionValue<int>(setnames[4]);

		//extract histo name prefix
		vector<string> replaces{"signal_",".root"};
		string prefix = point;
		for(const auto& r : replaces){
			auto ind = prefix.find(r);
			if(ind==string::npos) continue;
			prefix.replace(ind,r.size(),"");
		}

		TFile* file = TFile::Open((xrdir+indir+point).c_str());
		TH1F* nominal = (TH1F*)file->Get((prefix+"_nominal").c_str());
		double nominal_yield = nominal->Integral(0,nominal->GetNbinsX()+1);
		TH1F* SLe = (TH1F*)file->Get((prefix+"_SLe").c_str());
		double SLe_yield = SLe->Integral(0,SLe->GetNbinsX()+1);
		TH1F* SLm = (TH1F*)file->Get((prefix+"_SLm").c_str());
		double SLm_yield = SLm->Integral(0,SLm->GetNbinsX()+1);

		contam = ((SLe_yield+SLm_yield)/nominal_yield)*100;
		tree->Fill();

		file->Close();
	}

	//output tree
	TFile* ofile = TFile::Open((outdir+"tree_contam_"+model+"_MC"+year+"_fast.root").c_str(),"RECREATE");
	ofile->cd();
	tree->Write();
	ofile->Close();
}
