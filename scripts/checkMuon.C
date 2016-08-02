#include <TFile.h>
#include <TChain.h>
#include <TMath.h>

#include <vector>
#include <string>
#include <sstream>

using namespace std;

namespace KMath {
	double TransverseMass(double pt1, double phi1, double pt2, double phi2){
		double phi1_= atan2( sin(phi1), cos(phi1) );
		phi1_ = phi1_>=0 ? phi1_ : phi1_ + 2*TMath::Pi();
		double phi2_= atan2( sin(phi2), cos(phi2) );
		phi2_ = phi2_>=0 ? phi2_ : phi2_ + 2*TMath::Pi();
		double dphi_= phi1_-phi2_;
		if( dphi_> TMath::Pi() ) dphi_-=2*TMath::Pi();
		if( dphi_<-TMath::Pi() ) dphi_+=2*TMath::Pi();

		return sqrt(2*pt1*pt2*(1-cos(dphi_)));
	}
}

void checkMuon(){
	vector<string> filenames;
	string filepre = "Run2015D-PromptReco-v4.SingleMuon_";
	string filesuff = "_RA2AnalysisTree.root";
	string chainsuff = "/TreeMaker2/PreSelection";
	string treedir = "root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV5";
	vector<int> filerange;
	filerange.push_back(0);
	filerange.push_back(697);
		
	//loop over ranges (inclusive)
	filenames.reserve(filerange[1]-filerange[0]+1);
	for(int f = filerange[0]; f <= filerange[1]; f++){
		stringstream fs;
		fs << filepre << f << filesuff;
		filenames.push_back(fs.str());
	}
	
	//add filenames to chain
	TChain* tree = new TChain("tree_SingleMuon_2015Db");

	for(unsigned f = 0; f < filenames.size(); f++){
		string filename = filenames[f];
		filename = treedir + "/" + filename;
		tree->Add((filename+chainsuff).c_str());
	}
	
	tree->Draw("NJets","NJets>=4 && HT>500 && MHT>200 && @Electrons.size()==0 && DeltaPhi1 > 0.5 && DeltaPhi2 > 0.5 && DeltaPhi3 > 0.3 && DeltaPhi4 > 0.3 && JetID == 1 && @Muons.size()==1 && KMath::TransverseMass(METPt,METPhi,Muons[0].Pt(),Muons[0].Phi()) < 100");
}