#ifndef KBUILDER_H
#define KBUILDER_H

//custom headers
#include "KLooper.h"
#include "KMap.h"
#include "KMath.h"
#include "KParser.h"
#include "KBase.h"
#include "KSelection.h"
#include "../corrections/TriggerEfficiencySextet.cpp"
#include "../corrections/ISRCorrector.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TProfile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TLorentzVector.h>
#include <TMath.h>

//STL headers
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

//---------------------------------------------------------------
//histo builder class - loops over tree to fill histos for a base
class KBuilder : public KLooper {
	public:
		//constructors
		KBuilder() : KLooper(), MyBase(0), localOpt(new OptionMap()), globalOpt(new OptionMap()), MySelection(0) {}
		KBuilder(KBase* MyBase_) : 
			KLooper(MyBase_->GetLocalOpt(),MyBase_->GetGlobalOpt()), MyBase(MyBase_), 
			localOpt(MyBase->GetLocalOpt() ? MyBase->GetLocalOpt() : new OptionMap()), 
			globalOpt(MyBase->GetGlobalOpt() ? MyBase->GetGlobalOpt() : new OptionMap()),
			MySelection(0)
		{}
		//destructor
		virtual ~KBuilder() {}

		//functions for histo creation
		using NtupleClass::Cut;
		virtual bool Cut() { //this implements the set of cuts common between data and MC
			bool goodEvent = true;
		
			return (goodEvent && MySelection->DoSelection());
		}
		virtual double GetWeight() { return 1.; }
		virtual void Loop() {
			if (fChain == 0) return;
			MySelection = MyBase->GetSelection();
			
			//check for branches to enable/disable
			vector<string> disable_branches;
			globalOpt->Get("disable_branches",disable_branches);
			for(unsigned b = 0; b < disable_branches.size(); ++b){
				fChain->SetBranchStatus(disable_branches[b].c_str(),0);
			}
			vector<string> enable_branches;
			globalOpt->Get("enable_branches",enable_branches);
			for(unsigned b = 0; b < enable_branches.size(); ++b){
				fChain->SetBranchStatus(enable_branches[b].c_str(),1);
			}
			//check for any necessary branches
			CheckBranches();
			MySelection->CheckBranches();
			
			//loop over ntuple tree
			Long64_t nentries = fChain->GetEntries();
			int maxevents = 0;
			if(globalOpt->Get("maxevents",maxevents) && maxevents < nentries) nentries = maxevents;
			bool debugloop = globalOpt->Get("debugloop",false);
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(debugloop && jentry % 10000 == 0) cout << MyBase->GetName() << " " << jentry << "/" << nentries << endl;
				
				Cut();
			}
			
			//final steps
			if(globalOpt->Get("debugcut",false)) {
				cout << MyBase->GetName() << endl;
				MySelection->PrintEfficiency(nentries,sqrt((double)nentries));
			}
			
			//finalize histos
			KSelector* Histo = MySelection->Get<KSelector*>("Histo");
			Histo->Finalize(NULL);
		}
		//unimplemented
		virtual void CheckBranches() {}

	public:
		//member variables
		KBase* MyBase;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		KSelection* MySelection;
};

//---------------------------------------------------------------
//extension of base class for data - has default intlumi
class KBaseData : public KBase {
	public:
		//constructors
		KBaseData() : KBase() { localOpt->Set<double>("intlumi",0.0); }
		KBaseData(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) { 
			if(!localOpt->Has("intlumi")) localOpt->Set<double>("intlumi",0.0);
			KBuilder* ltmp = new KBuilder(this);
			SetLooper(ltmp);
		}
		//destructor
		virtual ~KBaseData() {}
		//accessors
		virtual bool IsData() { return true; }
		virtual bool IsMC() { return false; }

		//functions for histo creation
		using KBase::Build;
		virtual void Build(){
			if(!isBuilt) {
				MyLooper->Loop(); //loop over tree to build histograms
				isBuilt = true;
			}
		}
};

//--------------------------------------------------------------------------------
//extension of base class for MC - has error band calc, default cross section & norm type
class KBaseMC : public KBase {
	public:
		//constructors
		KBaseMC() : KBase() { 
			localOpt->Set<string>("normtype","MC");
			localOpt->Set<double>("xsection",0.0);
		}
		KBaseMC(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) {
			if(!localOpt->Has("normtype")) localOpt->Set<string>("normtype","MC");
			if(!localOpt->Has("xsection")) localOpt->Set<double>("xsection",0.0);
			KBuilder* ltmp = new KBuilder(this);
			SetLooper(ltmp);
		}
		//destructor
		virtual ~KBaseMC() {}
		virtual bool IsData() { return false; }
		virtual bool IsMC() { return true; }
		
		//functions for histo creation
		using KBase::Build;
		virtual void Build(){
			if(!isBuilt) {
				MyLooper->Loop(); //loop over tree to build histograms
				isBuilt = true;
			}
		}
};

#endif
