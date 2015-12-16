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
		typedef map<massPoint,unsigned>::iterator MPit;
	
		//constructor
		KScanner(KBase* MyBase_) : 
			NtupleClass(MyBase_->GetTree()), MyBase(MyBase_), globalOpt(MyBase->GetGlobalOpt()), nentries(0), outpre(""), outsuff("")
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
			
			KSelection<KScanner>* currSel = NULL;
			massPoint currPoint = make_pair(0.0,0.0);
			unsigned blocknum = 0;
			MyBase->GetLocalOpt()->Get("block",blocknum);
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Scanning " << jentry << "/" << nentries << endl;
				
				//check this event's mass point
				massPoint thePoint = make_pair(SusyMotherMass,SusyLSPMass);
				
				//if current, fill the corresponding tree
				if(currPoint == thePoint){
					currSel->FillTree();
				}
				//if not, make a new selection/output file for it, with a new part number
				else{
					//reset vars
					currPoint = thePoint;
					//write existing tree to file
					if(currSel) currSel->Finalize();
					delete currSel; currSel = NULL;
					
					//get part number - check if this mass point has been seen before
					MPit mp = selectionMap.find(thePoint);
					unsigned partnum = 0;
					if(mp != selectionMap.end()) {
						partnum = mp->second;
						//increment map
						mp->second += 1;
					}
					else {
						selectionMap.insert(make_pair(thePoint,partnum+1));
					}
					
					//make the name
					stringstream oss;
					oss << outpre << "_" << "mMother-" << thePoint.first << "_mLSP-" << thePoint.second << "_block" << blocknum << "_part" << partnum << outsuff;
					string oname = oss.str();
					
					//make the selection and tree, fill it
					currSel = new KSelection<KScanner>(oname,globalOpt);
					currSel->ScanTree(fChain);
					currSel->FillTree();
				}
			}

			//write last tree to file
			if(currSel) currSel->Finalize();
			delete currSel; currSel = NULL;
			
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
			}
		}
		
		//member variables
		KBase* MyBase;
		map<massPoint,unsigned> selectionMap;
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