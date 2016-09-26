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
#include <cstdlib>
#include <cmath>

using namespace std;

void NtupleClass::Loop() {}

class KScanner : public NtupleClass {
	public :
		typedef pair<double,double> massPoint;
	
		//constructor
		KScanner(KBase* MyBase_) : 
			NtupleClass(MyBase_->GetTree()), MyBase(MyBase_), globalOpt(MyBase->GetGlobalOpt()), nentries(0), outpre(""), outsuff(""),
			sT1bbtt("T1bbtt"), sT1tbtb("T1tbtb"), sT1tbbb("T1tbbb"), sT1tbtt("T1tbtt")
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
				double MassOffset = 0.0;
				if(T1ttbb){
					//topologies defined by ntop, nchargino
					int ntop = 0;
					int nchargino = 0;
					for(unsigned g = 0; g < GenParticles_PdgId->size(); ++g){
						if(abs(GenParticles_PdgId->at(g))==6){ //top
							++ntop;
						}
						else if(abs(GenParticles_PdgId->at(g))==1000024){ //chargino
							++nchargino;
						}
					}
					
					MassOffset = getMassOffset(ntop, nchargino);
					if(MassOffset==0.0) continue; //discard other topologies (T1bbbb, T1tttt);
				}
				
				//check this event's mass point
				massPoint thePoint = make_pair(SusyMotherMass+MassOffset,SusyLSPMass);
					
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
					auto mp = partMap.find(thePoint);
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
					string oname = makeName(thePoint.first,thePoint.second,blocknum,partnum)+outsuff;
					
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
			for(auto& mp : partMap){
				//print all the mass points in this file
				cout << right << setw(17) << mp.first.first;
				cout << "  ";
				cout << right << setw(14) << mp.first.second;
				cout << endl;
			}
			
			//hadd parts now
			if(globalOpt->Get("hadd",false)){
				for(auto& mp : partMap){
					stringstream ssys;
					//just mv if only one part
					if(mp.second==1) ssys << "mv " << makeName(mp.first.first,mp.first.second,blocknum,0)+outsuff << ".root " << makeName(mp.first.first,mp.first.second,blocknum)+outsuff << ".root";
					else ssys << "batch/haddHelper.sh " << makeName(mp.first.first,mp.first.second,blocknum) << " " << mp.second - 1 << " " << outsuff;
					cout << ssys.str() << endl;
					system(ssys.str().c_str());
				}
			}
		}
		string makeName(double mass1, double mass2, unsigned block){
			double mass1i, mass1f;
			mass1f = modf(mass1,&mass1i);
			stringstream oss;
			oss << getTopologyName(mass1f) << "_" << "mMother-" << mass1i << "_mLSP-" << mass2 << "_block" << block;
			return oss.str();
		}
		string makeName(double mass1, double mass2, unsigned block, unsigned part){
			stringstream oss;
			oss << makeName(mass1,mass2,block) << "_part" << part;
			return oss.str();
		}
		double getMassOffset(int ntop, int nchargino){
			if(ntop==2 && nchargino==0) return 0.1; //T1bbtt (g->bb, g->tt)
			else if(ntop==2 && nchargino==2) return 0.2; //T1tbtb (g->tbchi+, g->tbchi+)
			else if(ntop==1 && nchargino==1) return 0.3; //T1tbbb (g->tbchi+, g->bb)
			else if(ntop==3 && nchargino==1) return 0.4; //T1tbtt (g->tbchi+, g->tt)
			else return 0.0; //default
		}
		const string& getTopologyName(double MassOffset){
			const double eps = 0.0001;
			if(abs(MassOffset-0.1)<eps) return sT1bbtt; //T1bbtt (g->bb, g->tt)
			else if(abs(MassOffset-0.2)<eps) return sT1tbtb; //T1tbtb (g->tbchi+, g->tbchi+)
			else if(abs(MassOffset-0.3)<eps) return sT1tbbb; //T1tbbb (g->tbchi+, g->bb)
			else if(abs(MassOffset-0.4)<eps) return sT1tbtt; //T1tbtt (g->tbchi+, g->tt)
			else return outpre; //default
		}
		
		//member variables
		KBase* MyBase;
		map<massPoint,unsigned> partMap;
		OptionMap* globalOpt;
		Long64_t nentries;
		string outpre, outsuff;
		bool T1ttbb;
		const string sT1bbtt, sT1tbtb, sT1tbbb, sT1tbtt;
};

//fake implementations of unneeded KBase classes
void KBase::Build() {}
void KBaseData::Build() {}
void KBaseMC::Build() {}

#endif
#endif
