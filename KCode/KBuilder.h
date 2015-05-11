#ifndef KBUILDER_H
#define KBUILDER_H
#define TreeClass_cxx

//custom headers
#include "KMap.h"
#include "KMath.h"
#include "KParser.h"
#include "KBase.h"
#include "TreeClass.h"
#include "RA2bin.h"

//custom headers for weighting
#include "../btag/GetBtagScale.C"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TProfile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TLorentzVector.h>

//STL headers
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

//---------------------------------------------------------------
//little class to store value & weight pairs for filling histos
class KValue {
	public:
		//constructor
		KValue() : values(0), weights(0) {}
		//accessors
		void Fill(double v, double w=1){
			values.push_back(v);
			weights.push_back(w);
		}
		double & GetValue(int iv) { return values[iv]; }
		double & GetWeight(int iw) { return weights[iw]; }
		int GetSize() { return values.size(); }
		
	protected:
		//member variables
		vector<double> values;
		vector<double> weights;
	
};

void TreeClass::Loop() {}

//---------------------------------------------------------------
//histo builder class - loops over tree to fill histos for a base
class KBuilder : public TreeClass {
	public:
		//constructors
		KBuilder() : TreeClass(), MyBase(0), localOpt(0), globalOpt(0) {
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		KBuilder(KBase* MyBase_, TTree* tree_) : TreeClass(tree_), MyBase(MyBase_), localOpt(MyBase->GetLocalOpt()), globalOpt(MyBase->GetGlobalOpt()) {
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		//destructor
		virtual ~KBuilder() {}
		
		bool CheckDoubleCount(){
			pair<int,int> tmp(RunNum,EvtNum);
			map<pair<int,int>,int>::iterator cmit = countmap.find(tmp);
			if(cmit!=countmap.end()) return true; //returns true if it finds a double count
			else {
				countmap[tmp] = 1;
				return false;
			}
		}

		//functions for histo creation
		virtual bool Cut() { //this implements the set of cuts common between data and MC
			bool goodEvent = true;
			
			//check for double counting
			//if (CheckDoubleCount()) return false;
			
			//get RA2 bin (other bins could also be retrieved here)
			if(binner){
				RA2bin = binner->GetBinNumber(NJets,BTags,MHT,HT);
				if(RA2bin==0) goodEvent &= false;
			}
			
			//int filter = -1;
			//if(globalOpt->Get<int>("filter",filter)){
			//	//check appropriate filter conditions
			//}
		
			return goodEvent;
		}
		virtual double Weight() { return 1.; }
		virtual void Loop() {
			if (fChain == 0) return;
			
			//check for RA2 binning
			binner = NULL;
			globalOpt->Get("RA2binner",binner);
			
			//initial loop to get histo variables
			int table_size = MyBase->GetTable().size();
			vector<vector<string> > vars; vars.reserve(table_size);
			vector<TH1*> htmp; htmp.reserve(table_size);
			HMit sit;
			for(sit = MyBase->GetTable().begin(); sit != MyBase->GetTable().end(); sit++){
				//get histo name
				string stmp = sit->first;
				htmp.push_back(sit->second);
				//split up histo variable names
				vector<string> vars_tmp;
				KParser::process(stmp,'_',vars_tmp);
				vars.push_back(vars_tmp);
			}
			
			//loop over ntuple tree
			Long64_t nentries = fChain->GetEntries();
			bool debugloop = MyBase->GetGlobalOpt()->Get("debugloop",false);
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(debugloop && jentry % 10000 == 0) cout << MyBase->GetName() << " " << jentry << "/" << nentries << endl;
				
				//clear event variables
				RA2bin = 0;
				
				if(!Cut()) continue;
				
				double w = Weight();
				
				for(int h = 0; h < htmp.size(); h++){
					vector<KValue> values(vars[h].size());
					//if(jentry%10000==0) cout << stmp << " TH" << vars.size() << " " << jentry;
				
					for(int i = 0; i < vars.size(); i++){
						//list of cases for histo calculation and filling
						if(vars[h][i]=="RA2bin"){//plot yield vs. bin of RA2 search
							values[i].Fill(RA2bin,w);
						}
						else if(vars[h][i]=="njets"){//jet multiplicity
							values[i].Fill(NJets,w);
						}
						else if(vars[h][i]=="nbjets"){//b-jet multiplicity
							values[i].Fill(BTags,w);
						}
						else if(vars[h][i]=="ht"){//sum of jet pt
							values[i].Fill(HT,w);
						}
						else if(vars[h][i]=="mht"){//missing hadronic energy
							values[i].Fill(MHT,w);
						}
						else if(vars[h][i]=="mindeltaphiN"){//min normalized deltaphi between jets and MET
							values[i].Fill(minDeltaPhiN,w);
						}
						else if(vars[h][i]=="nleptons"){//# leptons (mu or ele)
							values[i].Fill(Leptons,w);
						}
						else if(vars[h][i]=="nelectrons"){//# electrons
							values[i].Fill(ElectronsNum,w);
						}
						else if(vars[h][i]=="nmuons"){//# muons
							values[i].Fill(MuonsNum,w);
						}
						else if(vars[h][i]=="nisotrack"){//# iso tracks
							values[i].Fill(isoTracks,w);
						}
						else if(vars[h][i]=="nvertex"){//# good vertices
							values[i].Fill(NVtx,w);
						}
						else { //if it's a histogram with no known variable or calculation, do nothing
						}
					}
					
					//now fill the histogram
					if(vars[h].size()==1){
						for(int ix = 0; ix < values[0].GetSize(); ix++){
							htmp[h]->Fill(values[0].GetValue(ix), values[0].GetWeight(ix));
						}
					}
					else if(vars[h].size()==2){
						//need to cast in order to use Fill(x,y,w)
						//these three cases allow for various x vs. y comparisons: same # entries per event, or 1 vs. N per event
						if(values[0].GetSize()==values[1].GetSize()) {
							for(int i = 0; i < values[0].GetSize(); i++){
								if(htmp[h]->GetDimension()==1)
									static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
								else if(htmp[h]->GetDimension()==2)
									static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
							}
						}
						else if(values[0].GetSize()==1){
							for(int iy = 0; iy < values[1].GetSize(); iy++){
								if(htmp[h]->GetDimension()==1)
									static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
								else if(htmp[h]->GetDimension()==2)
									static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
							}
						}
						else if(values[1].GetSize()==1){
							for(int ix = 0; ix < values[0].GetSize(); ix++){
								if(htmp[h]->GetDimension()==1)
									static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
								else if(htmp[h]->GetDimension()==2)
									static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
							}
						}
					}
					else { //no support for other # of vars
					}
				}
			}
			
			if(globalOpt->Get("plotoverflow",false)){
				for(int h = 0; h < htmp.size(); h++){
					if(vars[h].size()==2) continue; //not implemented for 2D histos or profiles yet
					
					//temporary histo to calculate error correctly when adding overflow bin to last bin
					TH1* otmp = (TH1*)htmp[h]->Clone();
					otmp->Reset("ICEM");
					int ovbin = htmp[h]->GetNbinsX()+1;
					double err = 0.;
					otmp->SetBinContent(ovbin-1,htmp[h]->IntegralAndError(ovbin,ovbin,err));
					otmp->SetBinError(ovbin-1,err);
					
					//add overflow bin to last bin
					htmp[h]->Add(otmp);
					
					//remove overflow bin from htmp[h] (for consistent integral/yield)
					htmp[h]->SetBinContent(ovbin,0);
					htmp[h]->SetBinError(ovbin,0);
					
					delete otmp;
				}
			}
		}	

	protected:
		//member variables
		KBase* MyBase;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		map<pair<int,int>,int> countmap;
		
		//extra variables
		RA2binner* binner;
		unsigned RA2bin;
};

void KBase::Build(){
	if(!isBuilt) {
		if(MyBuilder==0) MyBuilder = new KBuilder(this,tree);
		MyBuilder->Loop(); //loop over tree to build histograms
		isBuilt = true;
	}
}

//---------------------------------------------------------
//extension of builder class for data - has blinding option
class KBuilderData : public KBuilder {
	public:
		//constructors
		KBuilderData() : KBuilder() { }
		KBuilderData(KBase* MyBase_, TTree* tree_) : KBuilder(MyBase_,tree_) {}
		//destructor
		virtual ~KBuilderData() {}
		
		//functions for histo creation
		bool Cut(){
			bool goodEvent = KBuilder::Cut();
			
			//special blinding option for data (disabled by default)
			if(globalOpt->Get("blind",false)){
				//do not look at signal region
				//could make this setting into a double value for variable blinding...
			}
			
			return goodEvent;
		}
};

void KBaseData::Build(){
	if(!isBuilt) {
		if(MyBuilder==0) MyBuilder = new KBuilderData(this,tree);
		MyBuilder->Loop(); //loop over tree to build histograms
		isBuilt = true;
	}
}

//------------------------------------------------------------------------------------------------------------
//extension of builder class for MC - has weighting (corrections & normalization), extra cuts (fake tau, etc.)
class KBuilderMC : public KBuilder {
	public:
		//constructors
		KBuilderMC() : KBuilder() { }
		KBuilderMC(KBase* MyBase_, TTree* tree_) : KBuilder(MyBase_,tree_) { }
		//destructor
		virtual ~KBuilderMC() {}
		
		//functions for histo creation
		bool Cut(){
			bool goodEvent = KBuilder::Cut();
			
			//check normalization type here
			string normtype = "";
			localOpt->Get("normtype",normtype);
		
			return goodEvent;
		}
		double Weight(){
			double w = 1.;
			
			//check option in case correction types are disabled globally
			//(enabled by default
			//(*disabled* until 2015 data is available)
			/*
			if(globalOpt->Get("pucorr",false)) {
				TH1F* puWeights;
				globalOpt->Get("puWeights",puWeights);
				w *= puWeights->GetBinContent(puWeights->GetXaxis()->FindBin(trueNInteraction));
			}
			
			if(globalOpt->Get("btagcorr",false)) {
				int bSF = 0;
				int mSF = 0;
				globalOpt->Get("btagSFunc",bSF);
				globalOpt->Get("mistagSFunc",mSF);
				//todo: add dependence on # btags
				w *= GetBtagScale(PFJetPt,PFJetEta,PFJetPartonFlavour,bSF,mSF);
			}
			*/
			
			//now do scaling: norm*xsection/nevents
			//should this be a separate function using Scale()?
			int nEventProc = 0;
			double xsection = 0;
			if(localOpt->Get("nEventProc",nEventProc) && nEventProc>0 && localOpt->Get("xsection",xsection)) w *= xsection/nEventProc;
			
			//get norm from options
			double norm = 0;
			//use lumi norm (default)
			if(globalOpt->Get("luminorm",norm)) w *= norm;
			
			return w;
		}

};

void KBaseMC::Build(){
	if(!isBuilt) {
		if(MyBuilder==0) MyBuilder = new KBuilderMC(this,tree);
		MyBuilder->Loop(); //loop over tree to build histograms
		isBuilt = true;
	}
}

#endif
