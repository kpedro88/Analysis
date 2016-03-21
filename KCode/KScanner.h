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
		typedef map<massPoint,KSelection<KScanner>*>::iterator MPSit;
	
		//constructor
		KScanner(KBase* MyBase_) : 
			NtupleClass(MyBase_->GetTree()), MyBase(MyBase_), globalOpt(MyBase->GetGlobalOpt()), nentries(0), outpre(""), outsuff("")
		{
			//get options
			globalOpt->Get("outpre",outpre);
			globalOpt->Get("outsuff",outsuff);
			T1ttbb = outpre=="T1ttbb";
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
				
				//special checks for T1ttbb - separate the mixed topologies
				if(T1ttbb){
					//topologies defined by ntop, nchargino
					int ntop = 0;
					int nchargino = 0;
					for(unsigned g = 0; g < genParticles_PDGid->size(); ++g){
						if(abs(genParticles_PDGid->at(g))==24){ //top - count W instead (for offshell tops)
							++ntop;
						}
						else if(abs(genParticles_PDGid->at(g))==1000024){ //chargino
							++nchargino;
						}
					}
					
					double MassOffset = 0.0;
					if(ntop==2 && nchargino==0){ //T1bbtt (g->bb, g->tt)
						MassOffset = 0.1;
						outpre = "T1bbtt";
					}
					else if(ntop==2 && nchargino==2){ //T1tbtb (g->tbchi+, g->tbchi+)
						MassOffset = 0.2;
						outpre = "T1tbtb";
					}
					else if(ntop==1 && nchargino==1){ //T1tbbb (g->tbchi+, g->bb)
						MassOffset = 0.3;
						outpre = "T1tbbb";
					}
					else if(ntop==3 && nchargino==1){ //T1tbtt (g->tbchi+, g->tt)
						MassOffset = 0.4;
						outpre = "T1tbtt";
					}
					else continue; //discard other topologies (T1bbbb, T1tttt)
					
					//check this event's mass point
					massPoint thePoint = make_pair(SusyMotherMass+MassOffset,SusyLSPMass);
					MPSit mp = selectionMap.find(thePoint);
					
					//if known, fill the corresponding tree
					if(mp != selectionMap.end()){
						mp->second->FillTree();
					}
					//if unknown, make a new selection/output file for it
					else {
						//make the name
						unsigned partnum = 0;
						stringstream oss;
						oss << outpre << "_" << "mMother-" << thePoint.first-MassOffset << "_mLSP-" << thePoint.second << "_block" << blocknum << "_part" << partnum << outsuff;
						string oname = oss.str();
						
						//make the selection and tree, fill it
						KSelection<KScanner>* stmp = new KSelection<KScanner>(oname,globalOpt);
						stmp->ScanTree(fChain);
						stmp->FillTree();
						
						//add to map
						selectionMap.insert(make_pair(thePoint,stmp));
					}
				}
				
				else {
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
						MPit mp = partMap.find(thePoint);
						unsigned partnum = 0;
						if(mp != partMap.end()) {
							partnum = mp->second;
							//increment map
							mp->second += 1;
						}
						else {
							partMap.insert(make_pair(thePoint,partnum+1));
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
			}

			if(T1ttbb){
				//final steps
				//loop over map
				cout << string(17+2+14,'-') << endl;
				cout << "Mother mass [GeV]" << "  " << "LSP mass [GeV]" << endl;
				for(MPSit mp = selectionMap.begin(); mp != selectionMap.end(); ++mp){
					//print all the mass points in this file
					cout << right << setw(17) << mp->first.first;
					cout << "  ";
					cout << right << setw(14) << mp->first.second;
					cout << endl;
					
					mp->second->Finalize();
				}
			}
			else {
				//write last tree to file
				if(currSel) currSel->Finalize();
				delete currSel; currSel = NULL;
				
				//final steps
				//loop over map
				cout << string(17+2+14,'-') << endl;
				cout << "Mother mass [GeV]" << "  " << "LSP mass [GeV]" << endl;
				for(MPit mp = partMap.begin(); mp != partMap.end(); ++mp){
					//print all the mass points in this file
					cout << right << setw(17) << mp->first.first;
					cout << "  ";
					cout << right << setw(14) << mp->first.second;
					cout << endl;
				}
			}
		}
		
		//member variables
		KBase* MyBase;
		map<massPoint,unsigned> partMap;
		map<massPoint,KSelection<KScanner>*> selectionMap;
		OptionMap* globalOpt;
		Long64_t nentries;
		string outpre, outsuff;
		bool T1ttbb;

};

//fake implementations of unneeded KBase classes
void KBase::Build() {}
void KBaseData::Build() {}
void KBaseMC::Build() {}

#endif
#endif