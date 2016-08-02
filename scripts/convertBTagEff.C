#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>

#include "btag/BTagCalibrationStandalone.cc"

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

//usage:
//root -l -q 'convertBTagEff.C+("test/tree_signalBtag/tree_T1tttt_1500_100.root")'
//root -l -q 'convertBTagEff.C+("test/tree_signalBtag/tree_T1tttt_1200_800.root")'
//root -l -q 'convertBTagEff.C+("test/tree_signalBtag/tree_T1bbbb_1500_100.root")'
//root -l -q 'convertBTagEff.C+("test/tree_signalBtag/tree_T1bbbb_1000_900.root")'

void convertBTagEff(string fname, bool applySF=true){
	TFile* file = TFile::Open(fname.c_str(),"READ");
	
	string suff = "_btag.root";
	if(applySF) suff = "_btagSF.root";
	string oname = fname.substr(0,fname.size()-5)+suff;
	
	TFile* outfile = TFile::Open(oname.c_str(),"RECREATE");
	outfile->cd();
	
	//initialize btag helper classes
	BTagCalibration calib = BTagCalibration("","btag/CSVv2_mod.csv");
	BTagCalibrationReader reader[3];
	reader[0] = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "central");
	reader[1] = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "up");
	reader[2] = BTagCalibrationReader(&calib, BTagEntry::OP_MEDIUM, "comb", "down");
	string uname[3] = {"central","up","down"};
	
	BTagEntry::JetFlavor jfname[3] = {BTagEntry::FLAV_B,BTagEntry::FLAV_C,BTagEntry::FLAV_UDSG};
	string dname[3] = {"d_eff_b","d_eff_c","d_eff_udsg"};
	string nname[3] = {"n_eff_b","n_eff_c","n_eff_udsg"};
	string hname[3] = {"h_eff_b","h_eff_c","h_eff_udsg"};
	string aname[3] = {"a_eff_b","a_eff_c","a_eff_udsg"};
	int u_max = 3;
	if(!applySF) u_max = 1;
	for(int u = 0; u < u_max; u++){
		for(int i = 0; i < 3; i++){
			TH2* dtmp = (TH2*)file->Get(dname[i].c_str());
			TH1D* dtmp1d = dtmp->ProjectionX((dname[i]+"_1D").c_str());
			TH1F* dtmp1df = new TH1F();
			dtmp1d->Copy(*dtmp1df);
			TH2* ntmp = (TH2*)file->Get(nname[i].c_str());
			TH1D* ntmp1d = ntmp->ProjectionX((nname[i]+"_1D").c_str());
			TH1F* ntmp1df = new TH1F();
			ntmp1d->Copy(*ntmp1df);
			
			TGraphAsymmErrors* etmp = new TGraphAsymmErrors(ntmp1df,dtmp1df);
			etmp->SetName((aname[i]+"_"+uname[u]).c_str());
			TH1F* htmp = (TH1F*)dtmp1df->Clone(hname[i].c_str());
			htmp->Divide(ntmp1df,dtmp1df,1,1,"B");
			
			if(applySF){
				//for(int b = 1; b <= htmp->GetNbinsX(); b++){
				//	htmp->SetBinContent(b,htmp->GetBinContent(b)*reader[u].eval(jfname[i],0,htmp->GetBinCenter(b)));
				//}
				for(int b = 0; b < etmp->GetN(); b++){
					double x, y;
					etmp->GetPoint(b,x,y);
					double SF = reader[u].eval(jfname[i],0,x);
					etmp->SetPoint(b,x,y*SF);
					//cout << x << ", " << y << ", " << y*SF << endl;
					etmp->SetPointEYlow(b, etmp->GetErrorYlow(b)*SF);
					etmp->SetPointEYhigh(b, etmp->GetErrorYhigh(b)*SF);
				}
			}
			
			etmp->Write();
			if((applySF && u==2) || (!applySF && u==0)) htmp->Write();
		}
	}
	file->Close();
	outfile->Close();
}