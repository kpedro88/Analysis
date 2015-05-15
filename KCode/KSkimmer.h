#ifndef KSKIMMER_H
#define KSKIMMER_H
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
		KSkimmer(TTree* tree, TH1F* nEventHist_, OptionMap* globalOpt_) : 
			NtupleClass(tree), nEventHist(nEventHist_), globalOpt(globalOpt_), nentries(0), width1(0)
		{ 
			//must always have an option map
			if(!globalOpt) globalOpt = new OptionMap();
			
			//PDG mass values in GeV
			MuonMass = 0.1056583715;
			ElectronMass = 0.000510998928;
			TauMass = 1.77682;	
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
			//if(nentries>10000) nentries = 10000;
			
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Skimming " << jentry << "/" << nentries << endl;
				
				for(unsigned s = 0; s < theSelections.size(); s++){
					theSelections[s]->DoSelection();
				}
			}
			
			//create nEventProc if necessary
			if(!nEventHist){
				nEventHist = new TH1F("nEventProc","",1,0,1);
				nEventHist->SetBinContent(1,fChain->GetEntries());
			}
			
			//final steps
			for(unsigned s = 0; s < theSelections.size(); s++){
				theSelections[s]->PrintEfficiency(width1,nentries);
				theSelections[s]->Finalize(nEventHist);
			}
		}
		void AddSelection(KSelection<KSkimmer>* sel) { 
			theSelections.push_back(sel);
			if(sel->GetSelectorWidth()>width1) width1 = sel->GetSelectorWidth();
		}
		
		//member variables
		TH1F *nEventHist;
		vector<KSelection<KSkimmer>*> theSelections;
		OptionMap* globalOpt;
		Long64_t nentries;
		double  MuonMass, ElectronMass, TauMass;
		int width1;

};

#endif