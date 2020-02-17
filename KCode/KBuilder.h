#ifndef KBUILDER_H
#define KBUILDER_H

//custom headers
#include "KLooper.h"
#include "KMap.h"
#include "KParser.h"
#include "KBase.h"
#include "KSelection.h"

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
		KBuilder() : KLooper(), localOpt(new OptionMap()), globalOpt(new OptionMap()) {}
		KBuilder(KBase* base) : 
			KLooper(base->GetLocalOpt(),base->GetGlobalOpt()),
			localOpt(base->GetLocalOpt() ? base->GetLocalOpt() : new OptionMap()), 
			globalOpt(base->GetGlobalOpt() ? base->GetGlobalOpt() : new OptionMap())
		{
			AddBase(base);
		}
		//destructor
		virtual ~KBuilder() {}

		//functions for histo creation
		virtual void Loop() {
			if(fChain == 0) return;
			if(MyBases.size() == 0) return;

			//check if we have a reason to loop
			bool all_special = true;
			//assume all bases have same list of histos
			for(auto& hit : MyBases[0]->GetTable()){
				if(hit.second->khtmp) all_special &= hit.second->khtmp->IsSpecial();
			}
			if(all_special) return;
			
			//check for branches to enable/disable
			vector<string> disable_branches;
			globalOpt->Get("disable_branches",disable_branches);
			DisableBranches(disable_branches);
			vector<string> enable_branches;
			globalOpt->Get("enable_branches",enable_branches);
			EnableBranches(enable_branches);
			//check for any necessary branches
			for(auto& base : MyBases){
				base->GetSelection()->CheckBranches();
			}
			
			//loop over ntuple tree
			Long64_t nentries = fChain->GetEntries();
			int maxevents = 0;
			if(globalOpt->Get("maxevents",maxevents) && maxevents < nentries) nentries = maxevents;
			Long64_t jentry = 0;
			unsigned firstevent = 0;
			if(globalOpt->Get("firstevent",firstevent) && firstevent < nentries) jentry = firstevent;
			bool debugloop = globalOpt->Get("debugloop",false);
			Long64_t nbytes = 0, nb = 0;
			for (; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(debugloop && jentry % 10000 == 0) cout << MyBases[0]->GetName() << " " << jentry << "/" << nentries << endl;
				
				for(auto& base : MyBases){
					base->GetSelection()->DoSelection();
				}
			}
			
			//final steps
			if(globalOpt->Get("debugcut",false)) {
				for(auto& base : MyBases){
					cout << base->GetName() << endl;
					base->GetSelection()->PrintEfficiency(nentries,sqrt((double)nentries));
				}
			}
			
			//finalize histos
			for(auto& base : MyBases){
				base->SetBuilt();
				KSelector* Histo = base->GetSelection()->Get<KSelector*>("Histo");
				Histo->Finalize(NULL);
			}
		}
		virtual void AddBase(KBase* base){
			MyBases.push_back(base);
		}

	public:
		//member variables
		vector<KBase*> MyBases;
		OptionMap* localOpt;
		OptionMap* globalOpt;
};

//---------------------------------------------------------------
//extension of base class for data - has default intlumi
class KBaseData : public KBase {
	public:
		//constructors
		KBaseData() : KBase() { localOpt->Set<double>("intlumi",0.0); }
		KBaseData(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) { 
			if(!localOpt->Has("intlumi")) localOpt->Set<double>("intlumi",0.0);
			SetLooper();
		}
		virtual KLooper* MakeLooper() { return new KBuilder(this); }
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
REGISTER_SET(KBaseData,base,data);

//--------------------------------------------------------------------------------
//extension of base class for MC - has error band calc, default cross section & norm type
class KBaseMC : public KBase {
	public:
		//constructors
		KBaseMC() : KBase() { 
			localOpt->Set<string>("normtype","MC");
		}
		KBaseMC(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) {
			if(!localOpt->Has("normtype")) localOpt->Set<string>("normtype","MC");
			SetLooper();
		}
		virtual KLooper* MakeLooper() { return new KBuilder(this); }
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
REGISTER_SET(KBaseMC,base,mc);

#endif
