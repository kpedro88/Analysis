#ifndef KBUILDER_H
#define KBUILDER_H
#define TreeClass_cxx

//custom headers
#include "KMap.h"
#include "KMath.h"
#include "KParser.h"
#include "KBase.h"
#include "TreeClass.h"
#include "KSelection.h"
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

void TreeClass::Loop() {}

//---------------------------------------------------------------
//histo builder class - loops over tree to fill histos for a base
class KBuilder : public TreeClass {
	public:
		//constructors
		KBuilder() : TreeClass(), MyBase(0), localOpt(0), globalOpt(0), MySelection(0) {
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		KBuilder(KBase* MyBase_, TTree* tree_, KSelection<KBuilder>* sel_) : TreeClass(tree_), MyBase(MyBase_), localOpt(MyBase->GetLocalOpt()), globalOpt(MyBase->GetGlobalOpt()), MySelection(sel_) {
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
			//pass self to selection; also sets looper for selectors, variation, variators
			MySelection->SetLooper(this); 
		}
		//destructor
		virtual ~KBuilder() {}

		//functions for histo creation
		virtual bool Cut() { //this implements the set of cuts common between data and MC
			bool goodEvent = true;
			
			//get RA2 bin (other bins could also be retrieved here)
			if(binner){
				RA2bin = binner->GetBinNumber(NJets,BTags,MHT,HT);
				if(RA2bin==0) goodEvent &= false;
			}
			
			//int filter = -1;
			//if(globalOpt->Get<int>("filter",filter)){
			//	//check appropriate filter conditions
			//}
		
			return (goodEvent && MySelection->DoSelection());
		}
		virtual double Weight() { return 1.; }
		virtual void Loop() {
			if (fChain == 0) return;
			
			//check for RA2 binning
			binner = NULL;
			globalOpt->Get("RA2binner",binner);
			
			//initial loop to get histo variables
			int table_size = MyBase->GetTable().size();
			vars.clear(); vars.reserve(table_size);
			htmp.clear(); htmp.reserve(table_size);
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
			bool debugloop = globalOpt->Get("debugloop",false);
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(debugloop && jentry % 10000 == 0) cout << MyBase->GetName() << " " << jentry << "/" << nentries << endl;
				
				//clear event variables
				RA2bin = 0;
				
				Cut();
			}
			
			//final steps
			if(globalOpt->Get("debugcut",false)) MySelection->PrintEfficiency(MySelection->GetSelectorWidth(),nentries);
			
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

	public:
		//member variables
		KBase* MyBase;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		KSelection<KBuilder>* MySelection;
		vector<vector<string> > vars;
		vector<TH1*> htmp;
		
		//extra variables
		RA2binner* binner;
		unsigned RA2bin;
};

void KBase::Build(){
	if(!isBuilt) {
		if(MyBuilder==0) MyBuilder = new KBuilder(this,tree,MySelection);
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
		KBuilderData(KBase* MyBase_, TTree* tree_, KSelection<KBuilder>* sel_) : KBuilder(MyBase_,tree_,sel_) {}
		//destructor
		virtual ~KBuilderData() {}
		
		//functions for histo creation
		bool Cut(){
			bool goodEvent = true;
			
			//special blinding option for data (disabled by default)
			if(globalOpt->Get("blind",false)){
				//do not look at signal region
				//could make this setting into a double value for variable blinding...
			}
			
			//KBuilder::Cut() comes *last* because it includes histo filling selector
			return (goodEvent && KBuilder::Cut());
		}
};

void KBaseData::Build(){
	if(!isBuilt) {
		if(MyBuilder==0) MyBuilder = new KBuilderData(this,tree,MySelection);
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
		KBuilderMC(KBase* MyBase_, TTree* tree_, KSelection<KBuilder>* sel_) : KBuilder(MyBase_,tree_,sel_) { }
		//destructor
		virtual ~KBuilderMC() {}
		
		//functions for histo creation
		bool Cut(){
			bool goodEvent = true;
			
			//check normalization type here
			string normtype = "";
			localOpt->Get("normtype",normtype);
		
			//KBuilder::Cut() comes *last* because it includes histo filling selector
			return (goodEvent && KBuilder::Cut());
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
		if(MyBuilder==0) MyBuilder = new KBuilderMC(this,tree,MySelection);
		MyBuilder->Loop(); //loop over tree to build histograms
		isBuilt = true;
	}
}

#endif
