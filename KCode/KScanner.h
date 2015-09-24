#ifndef KSCANNER_H
#define KSCANNER_H

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
#include <iomanip>
#include <sstream>
#include <map>
#include <utility>

using namespace std;

void NtupleClass::Loop() {}

class KScanner : public NtupleClass {
	public :
		typedef pair<double,double> massPoint;
		typedef map<massPoint,KSelection<KScanner>*>::iterator MPit;
	
		//constructor
		KScanner(TTree* tree, OptionMap* globalOpt_) : 
			NtupleClass(tree), globalOpt(globalOpt_ ? globalOpt_ : new OptionMap()), nentries(0), outpre(""), outsuff("")
		{
			//get options
			globalOpt->Get("outpre",outpre);
			globalOpt->Get("outsuff",outsuff);
		}
		//destructor
		virtual ~KScanner() {}
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
			
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Scanning " << jentry << "/" << nentries << endl;
				
				//check map for this event's mass point
				massPoint thePoint = make_pair(SusyMotherMass,SusyLSPMass);
				MPit mp = selectionMap.find(thePoint);
				
				//if known, fill the corresponding tree
				if(mp != selectionMap.end()){
					mp->second->FillTree();
				}
				//if unknown, make a new selection/output file for it
				else {
					//make the name
					stringstream oss;
					oss << outpre << "mMother-" << thePoint.first << "_mLSP-" << thePoint.second << outsuff;
					string oname = oss.str();
					
					//make the selection and tree, fill it
					KSelection<KScanner>* stmp = new KSelection<KScanner>(oname,globalOpt);
					stmp->ScanTree(fChain);
					stmp->FillTree();
					
					//add to map
					selectionMap.insert(make_pair(thePoint,stmp));
				}
			}

			//final steps
			//loop over map
			cout << string(17+2+14,'-') << endl;
			cout << "Mother mass [GeV]" << "  " << "LSP mass [GeV]" << endl;
			for(MPit mp = selectionMap.begin(); mp != selectionMap.end(); ++mp){
				//print all the mass points in this file
				cout << right << setw(17) << mp->first.first;
				cout << "  ";
				cout << right << setw(14) << mp->first.second;
				cout << endl;
				
				mp->second->Finalize();
			}
		}
		
		//member variables
		map<massPoint,KSelection<KScanner>*> selectionMap;
		OptionMap* globalOpt;
		Long64_t nentries;
		string outpre, outsuff;

};

//fake implementations of unneeded KBase classes
void KBase::Build() {}
void KBaseData::Build() {}
void KBaseMC::Build() {}

#endif
#endif