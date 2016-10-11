#ifndef KSKIMMER_H
#define KSKIMMER_H

#ifndef NtupleClass_cxx
#define NtupleClass_cxx

//custom headers
#include "NtupleClass.h"
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

using namespace std;

void NtupleClass::Loop() {}

class KSkimmer : public NtupleClass {
	public :
		//constructor
		KSkimmer(KBase* MyBase_) : 
			NtupleClass(MyBase_->GetTree()), MyBase(MyBase_), nEventHist(MyBase->GetNEventHist()), nEventNegHist(MyBase->GetNEventNegHist()), globalOpt(MyBase->GetGlobalOpt()), nentries(0)
		{
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
		}
		//destructor
		virtual ~KSkimmer() {}
		//functions
		virtual void Loop(){
			if (fChain == 0) {
				cout << "Error: fChain is null!" << endl;
				return;
			}

			//loop over ntuple tree
			nentries = fChain->GetEntries();
			int maxevents = 0;
			if(globalOpt->Get("maxevents",maxevents) && maxevents < nentries) nentries = maxevents;
			//if(nentries>10000) nentries = 10000;
			
			//create nEventProc if necessary
			if(!nEventHist){
				nEventHist = new TH1F("nEventProc","",1,0,1);
				nEventHist->SetBinContent(1,nentries);
			}
			bool fillNeg = false;
			int negevents = 0;
			if(!nEventNegHist){
				nEventNegHist = new TH1F("nEventNeg","",1,0,1);
				fillNeg = true;
			}
			
			for(unsigned s = 0; s < theSelections.size(); s++){
				theSelections[s]->CheckBranches();
			}
			//ROOT voodoo (apparently, branch addresses can sometimes get screwed up by excessive enabling/disabling?)
			Init(fChain);
			
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Skimming " << jentry << "/" << nentries << endl;
				
				if(GetEventSign()==-1) ++negevents;
				
				for(unsigned s = 0; s < theSelections.size(); s++){
					theSelections[s]->DoSelection();
				}
			}
			
			if(fillNeg) nEventNegHist->SetBinContent(1,negevents);
			
			//final steps
			for(unsigned s = 0; s < theSelections.size(); s++){
				theSelections[s]->PrintEfficiency(nEventHist);
				theSelections[s]->Finalize(nEventHist,nEventNegHist);
			}
		}
		void AddSelection(KSelection<KSkimmer>* sel) {
			sel->SetLooper(this); //also sets looper for selectors, variation, variators
			theSelections.push_back(sel);
		}
		//implemented in KSkimmerSelectors for future reasons
		int GetEventSign();
		
		//member variables
		KBase* MyBase;
		TH1F *nEventHist, *nEventNegHist;
		vector<KSelection<KSkimmer>*> theSelections;
		OptionMap* globalOpt;
		Long64_t nentries;

};

//fake implementations of unneeded KBase classes
void KBase::Build() {}
void KBaseData::Build() {}
void KBaseMC::Build() {}

#endif
#endif