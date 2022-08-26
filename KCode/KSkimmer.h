#ifndef KSKIMMER_H
#define KSKIMMER_H

//custom headers
#include "KLooper.h"
#include "KMap.h"
#include "KBase.h"
#include "KSelection.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>
#include <iostream>
#include <exception>

using namespace std;

class KSkimmer : public KLooper {
	public :
		//constructor
		KSkimmer(KBase* MyBase_) : 
			KLooper(MyBase_->GetLocalOpt(),MyBase_->GetGlobalOpt()), MyBase(MyBase_), nEventHist(MyBase->GetNEventHist()), nEventNegHist(MyBase->GetNEventNegHist()), 
			nEventSelName("NEventSelection"), nEventProcName("NEventProc"), nEventNegName("NegativeWeight"), globalOpt(MyBase_->GetGlobalOpt()), nentries(0)
		{
			//check for branches to enable/disable
			vector<string> disable_branches;
			globalOpt->Get("disable_branches",disable_branches);
			DisableBranches(disable_branches);
			vector<string> enable_branches;
			globalOpt->Get("enable_branches",enable_branches);
			EnableBranches(enable_branches);
		}
		//helpers
		void MakeNEventSelection() {
			//make nevents and negative weight selectors, if needed, and put into a selection
			KSelection* nEventSelection = new KSelection(nEventSelName, globalOpt);
			string pmssm_options; globalOpt->Get("pmssm_options",pmssm_options);
			if(!nEventHist){
				KSelector* NEventProc = KSelectorFactory::GetFactory().construct(nEventProcName,nEventProcName,GetNEventOpt(pmssm_options));
				nEventSelection->AddSelector(NEventProc);
			}
			if(!nEventNegHist){
				KSelector* NegativeWeight = nEventNegHist ? nullptr : KSelectorFactory::GetFactory().construct(nEventNegName,nEventNegName,GetNEventOpt(pmssm_options));
				nEventSelection->AddSelector(NegativeWeight);
			}
			if(nEventSelection->empty()) delete nEventSelection;
			else AddSelection(nEventSelection);
		}
		OptionMap* GetNEventOpt(const string& options){
			OptionMap* nEventOpt = new OptionMap();
			if(!options.empty()) KParser::processOption("in:input["+options+"]", nEventOpt);
			nEventOpt->Set<int>("xbins",1);
			nEventOpt->Set<double>("xmin",0);
			nEventOpt->Set<double>("xmax",0);
			nEventOpt->Set<vector<string>>("extend",{"x"});
			return nEventOpt;
		}
		//destructor
		virtual ~KSkimmer() {}
		//functions
		virtual void Loop(){
			if (fChain == 0) {
				throw runtime_error("fChain is null!");
			}

			//loop over ntuple tree
			nentries = fChain->GetEntries();
			int maxevents = 0;
			if(globalOpt->Get("maxevents",maxevents) && maxevents < nentries) nentries = maxevents;
			Long64_t jentry = 0;
			unsigned firstevent = 0;
			if(globalOpt->Get("firstevent",firstevent) && firstevent < nentries) jentry = firstevent;

			for(unsigned s = 0; s < theSelections.size(); s++){
				theSelections[s]->CheckBranches();
			}
			//ROOT voodoo (apparently, branch addresses can sometimes get screwed up by excessive enabling/disabling?)
			Init(fChain);
			
			Long64_t nbytes = 0, nb = 0;
			for (; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Skimming " << jentry << "/" << nentries << endl;

				for(unsigned s = 0; s < theSelections.size(); s++){
					theSelections[s]->DoSelection();
				}
			}

			Finalize();
		}
		void Finalize(); //implemented in KSkimmerSelectors.h
		void AddSelection(KSelection* sel) {
			sel->SetBase(MyBase); //also sets looper for selectors, variation, variators
			theSelections.push_back(sel);
		}
		
		//member variables
		KBase* MyBase;
		THN *nEventHist, *nEventNegHist;
		string nEventSelName, nEventProcName, nEventNegName;
		vector<KSelection*> theSelections;
		OptionMap* globalOpt;
		Long64_t nentries;
};

//-------------------------------------------
//extension of base class for skimmer
class KBaseSkim : public KBase {
	public:
		//constructors
		KBaseSkim() : KBase() {}
		KBaseSkim(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) {
			SetLooper();
		}
		virtual KLooper* MakeLooper() { return new KSkimmer(this); }
};
REGISTER_SET(KBaseSkim,base,skim);

#endif
