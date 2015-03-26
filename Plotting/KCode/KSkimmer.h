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

//STL headers
#include <string>
#include <vector>

using namespace std;

void NtupleClass::Loop() {}

class KSkimmer : public NtupleClass {
	public :
		//constructor
		KSkimmer(TTree* tree, TH1* nEventHist_, vector<KSelection*>& theSelections_, OptionMap* globalOpt_) : 
			NtupleClass(tree), nEventHist(nEventHist_), theSelections(theSelections), globalOpt(globalOpt_), nentries(0)
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
			if (fChain == 0) return;

			//loop over ntuple tree
			nentries = fChain->GetEntriesFast();
			//if(nentries>10000) nentries = 10000;
			
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				
				for(unsigned s = 0; s < theSelections.size(); s++){
					theSelections[s]->DoSelection();
				}
			}
			
			//final steps
			for(unsigned s = 0; s < theSelections.size(); s++){
				theSelections[s]->PrintEfficiency();
				theSelections[s]->Finalize();
			}
			
		}
		
		//member variables
		TH1  *nEventHist;
		vector<KSelection*> theSelections;
		OptionMap* globalOpt;
		Long64_t nentries;
		double  MuonMass, ElectronMass, TauMass;

};

#endif