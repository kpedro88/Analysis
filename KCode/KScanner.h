#ifndef KSCANNER_H
#define KSCANNER_H

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
#include <iomanip>
#include <sstream>
#include <map>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <exception>
#include <algorithm>

using namespace std;

class KScanner : public KLooper {
	public :
		//constructor
		KScanner(KBase* MyBase_) :
			KLooper(MyBase_->GetLocalOpt(),MyBase_->GetGlobalOpt()), MyBase(MyBase_), globalOpt(MyBase_->GetGlobalOpt()), nentries(0), outpre(""), outsuff("")
		{
			//get options
			globalOpt->Get("outpre",outpre);
			globalOpt->Get("outsuff",outsuff);
			//wait to check these
			splitT1ttbb = false;
			splitSVJ = false;
		}
		//destructor
		virtual ~KScanner() {}
		//functions
		virtual void Loop(){
			if (fChain == 0) {
				throw runtime_error("fChain is null!");
			}

			//loop over ntuple tree
			nentries = fChain->GetEntries();
			int maxevents = 0;
			if(globalOpt->Get("maxevents",maxevents) && maxevents < nentries) nentries = maxevents;

			splitT1ttbb = globalOpt->Get("splitT1ttbb",false);
			splitSVJ = globalOpt->Get("splitSVJ",false);
			
			vector<KSelection*> allSels;
			vector<string> namesT1ttbb = {"T1bbtt","T1tbtb","T1tbbb","T1tbtt"};
			if(splitT1ttbb){
				allSels.reserve(namesT1ttbb.size());
				for(auto& name : namesT1ttbb){
					string otmp = MyBase->GetName();
					otmp.replace(0,6,name);
					KSelection* seltmp = new KSelection(otmp,globalOpt);
					seltmp->ScanTree(fChain);
					allSels.push_back(seltmp);
				}
			}
			
			KSelection* currSel = NULL;
			vector<double> currPoint;
			unsigned blocknum = 0;
			MyBase->GetLocalOpt()->Get("block",blocknum);
			Long64_t nbytes = 0, nb = 0;
			for (Long64_t jentry=0; jentry<nentries;jentry++) {
				Long64_t ientry = LoadTree(jentry);
				if (ientry < 0) break;
				nb = fChain->GetEntry(jentry);   nbytes += nb;
				if(jentry % 10000 == 0) cout << "Scanning " << jentry << "/" << nentries << endl;
				
				//special mode to split T1ttbb topologies
				if(splitT1ttbb){
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
					
					if(ntop==2 && nchargino==0) allSels[0]->FillTree(); //T1bbtt (g->bb, g->tt)
					else if(ntop==2 && nchargino==2) allSels[1]->FillTree(); //T1tbtb (g->tbchi+, g->tbchi+)
					else if(ntop==1 && nchargino==1) allSels[2]->FillTree(); //T1tbbb (g->tbchi+, g->bb)
					else if(ntop==3 && nchargino==1) allSels[3]->FillTree(); //T1tbtt (g->tbchi+, g->tt)
					
					continue;
				}
				
				//check this event's mass point
				//use old variables for SUSY because not all ntuples may have SignalParameters vector filled yet
				vector<double> thePoint = splitSVJ ? *SignalParameters : vector<double>({SusyMotherMass,SusyLSPMass});
					
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
						partMap.emplace(thePoint,partnum+1);
					}
					
					//make the name
					string oname = makeName(thePoint,blocknum,partnum)+outsuff;
					
					//make the selection and tree, fill it
					currSel = new KSelection(oname,globalOpt);
					currSel->ScanTree(fChain);
					currSel->FillTree();
				}
			}

			if(splitT1ttbb){
				for(auto seltmp : allSels){
					seltmp->Finalize();
				}
				return;
			}
			
			//write last tree to file
			if(currSel) currSel->Finalize();
			delete currSel; currSel = NULL;
			
			//final steps
			//loop over map
			vector<string> headers = splitSVJ ? vector<string>({"mZprime [GeV]","mDark [GeV]","rinv","alpha"}) : vector<string>({"Mother mass [GeV]","LSP mass [GeV]"});
			//get total length and concatenate header
			unsigned header_length = 2*headers.size(); //spaces
			string header;
			for_each(headers.begin(),headers.end(),[&](const string& s){ header_length += s.size(); header += s + "  "; });
			cout << string(header_length,'-') << endl;
			cout << header << endl;
			for(const auto& mp : partMap){
				//print all the mass points in this file
				for(unsigned p = 0; p < mp.first.size(); ++p){
					cout << right << setw(headers[p].size()) << mp.first[p] << "  ";
				}
				cout << endl;
			}
			
			//hadd parts now
			if(globalOpt->Get("hadd",false)){
				for(auto& mp : partMap){
					stringstream ssys;
					//just mv if only one part
					if(mp.second==1) ssys << "mv " << makeName(mp.first,blocknum,0)+outsuff << ".root " << makeName(mp.first,blocknum)+outsuff << ".root";
					else ssys << "batch/haddHelper.sh " << makeName(mp.first,blocknum) << " " << mp.second - 1 << " " << outsuff;
					cout << ssys.str() << endl;
					system(ssys.str().c_str());
				}
			}
		}
		string makeName(const vector<double>& params, unsigned block){
			const map<int,string> alpha_vals{
				{-2,"peak"},
				{-1,"high"},
				{-3,"low"},
			};
			stringstream oss;
			if(splitSVJ) oss << outpre << "_" << "mZprime-" << params[0] << "_mDark-" << params[1] << "_rinv-" << params[2] << "_alpha-" << alpha_vals.at(int(params[3])) << "_block" << block;
			else oss << outpre << "_" << "mMother-" << params[0] << "_mLSP-" << params[1] << "_block" << block;
			return oss.str();
		}
		string makeName(const vector<double>& params, unsigned block, unsigned part){
			stringstream oss;
			oss << makeName(params,block) << "_part" << part;
			return oss.str();
		}
		
		//member variables
		KBase* MyBase;
		map<vector<double>,unsigned> partMap;
		OptionMap* globalOpt;
		Long64_t nentries;
		string outpre, outsuff;
		bool splitT1ttbb, splitSVJ;
};

//-------------------------------------------
//extension of base class for scanner
class KBaseScan : public KBase {
	public:
		//constructors
		KBaseScan() : KBase() {}
		KBaseScan(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : KBase(name_, localOpt_, globalOpt_) {
			SetLooper();
		}
		virtual KLooper* MakeLooper() { return new KScanner(this); }
};
REGISTER_SET(KBaseScan,base,scan);

//avoid circular dependency
double KHisto::GetWeight(){
	return 1.;
}
double KHisto::GetWeightPerJet(unsigned index){
	return 1.;
}
//dummy classes
class KMCWeightSelector : public KSelector {};
class KSVJFilterSelector : public KSelector {};
class KLepFracFilterSelector : public KSelector {};

#endif
