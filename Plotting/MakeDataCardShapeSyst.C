#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>

#include "KPlotDriverShapeSyst.C"

#include <string>
#include <sstream>

#define nSources 6
#define nLQ 17
#define nRPV 8
#define nSyst 9 //including central

using namespace std;

void MakeDataCardShapeSyst(string fname="datacard_out.root", string dirname="datacards", string syst="", bool do_rpv=0){
	TFile* infile = TFile::Open(fname.c_str());

	string names[] = {"data","Fake #tau","Z + jets","diboson","t#bar{t}","single-t"};
	string pre = "st_";
	string dnames[] = {"data","wjets","zjets","ewk","ttbar","singlet"};
	string dpre = "h_";
	TH1F* histos[nSources];
	
	//get common histos
	for(int i = 0; i < nSources; i++){
		string tmp = pre + names[i];
		TH1F* htmp = (TH1F*)infile->Get(tmp.c_str());
		string dtmp = dpre + dnames[i];
		histos[i] = (TH1F*)htmp->Clone(dtmp.c_str());
	}

	if(do_rpv){
		int masses[] = {200,300,400,500,600,700,800,900};
		TFile* outfiles[nRPV];
		string fpre = dirname + "/muTau_M";
		system(("mkdir -p " + dirname).c_str());
		
		//get RPV histos and make datacard files
		for(int j = 0; j < nRPV; j++){
			stringstream tmp;
			tmp << pre << "Stop" << masses[j];
			TH1F* htmp = (TH1F*)infile->Get((tmp.str()).c_str());
			string dtmp = "h_sig";
			TH1F* hRPV = (TH1F*)htmp->Clone(dtmp.c_str());
			
			//open file
			stringstream ftmp;
			ftmp << fpre << masses[j];
			if(syst.size()>0) ftmp << "_" << syst;
			ftmp << ".root";
			outfiles[j] = new TFile((ftmp.str()).c_str(),"RECREATE");
			outfiles[j]->cd();
			
			hRPV->Write();
			for(int i = 0; i < nSources; i++){
				histos[i]->Write();
			}
			
			outfiles[j]->Close();
		}
	}
	else{
		int masses[] = {200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000};
		TFile* outfiles[nLQ];
		string fpre = dirname + "/muTau_M";
		system(("mkdir -p " + dirname).c_str());
		
		//get LQ histos and make datacard files
		for(int j = 0; j < nLQ; j++){
			stringstream tmp;
			tmp << pre << "LQ" << masses[j];
			TH1F* htmp = (TH1F*)infile->Get((tmp.str()).c_str());
			string dtmp = "h_sig";
			TH1F* hLQ = (TH1F*)htmp->Clone(dtmp.c_str());
			
			//open file
			stringstream ftmp;
			ftmp << fpre << masses[j];
			if(syst.size()>0) ftmp << "_" << syst;
			ftmp << ".root";
			outfiles[j] = new TFile((ftmp.str()).c_str(),"RECREATE");
			outfiles[j]->cd();
			
			hLQ->Write();
			for(int i = 0; i < nSources; i++){
				histos[i]->Write();
			}
			
			outfiles[j]->Close();
		}
	}
}

void AllSyst(bool do_rpv=0){
	string syst[nSyst] = {"","TauESUp","TauESDown","TauERUp","TauERDown","JESUp","JESDown","JERUp","JERDown"};
	
	string inFile, opre, dirname;
	if(do_rpv) { inFile = "input_final_lqd321_dd_shapesyst_datacard.txt"; opre = "datacard_lqd321_dd"; dirname = "datacards_lqd321_muTau"; }
	else { inFile = "input_final_dd_shapesyst_datacard.txt"; opre = "datacard_dd"; dirname = "datacards_muTau"; }
	
	for(int i = 0; i < nSyst; i++){
		string us = "";
		if(syst[i].size()>0) us = "_";
		string dir = "tree_main" + us + syst[i];
		string oname = opre + us + syst[i];
		KPlotDriverShapeSyst(dir,inFile,oname);
		
		string fname = oname + ".root";
		MakeDataCardShapeSyst(fname,dirname,syst[i],do_rpv);
	}

	system(("zip -r " + dirname + ".zip " + dirname + "/").c_str());
}