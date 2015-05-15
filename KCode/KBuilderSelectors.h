#ifndef KBUILDERSELECTORS_H
#define KBUILDERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KBuilder.h"
#include "KMath.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>
#include <map>

using namespace std;

//base class for Selectors is in KSelection.h

//---------------------------------------------------------------
//checks for double-counted events
class KDoubleCountSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KDoubleCountSelector() : KSelector<KBuilder>() { }
		KDoubleCountSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			pair<int,int> tmp(looper->RunNum,looper->EvtNum);
			map<pair<int,int>,int>::iterator cmit = countmap.find(tmp);
			if(cmit!=countmap.end()) return false; //fails the event if it finds a double count
			else {
				countmap[tmp] = 1;
				return true;
			}
		}
		
		//member variables
		map<pair<int,int>,int> countmap;
};

//---------------------------------------------------------------
//little class to store value & weight pairs for filling histos
class KValue {
	public:
		//constructor
		KValue() : values(0), weights(0) {}
		//accessors
		void Fill(double v, double w=1){
			values.push_back(v);
			weights.push_back(w);
		}
		double & GetValue(int iv) { return values[iv]; }
		double & GetWeight(int iw) { return weights[iw]; }
		int GetSize() { return values.size(); }
		
	protected:
		//member variables
		vector<double> values;
		vector<double> weights;
	
};

//----------------------------------------------------
//final selector to fill histograms
//(accounts for dependence on other selectors)
class KHistoSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KHistoSelector() : KSelector<KBuilder>() { }
		KHistoSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_) { canfail = false; }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			double w = looper->Weight();
			
			for(int h = 0; h < looper->htmp.size(); h++){
				unsigned vsize = looper->vars[h].size();
				vector<KValue> values(vsize);				
			
				for(int i = 0; i < vsize; i++){
					string vname = looper->vars[h][i];
					//list of cases for histo calculation and filling
					if(vname=="RA2bin"){//plot yield vs. bin of RA2 search
						values[i].Fill(looper->RA2bin,w);
					}
					else if(vname=="njets"){//jet multiplicity
						values[i].Fill(looper->NJets,w);
					}
					else if(vname=="nbjets"){//b-jet multiplicity
						values[i].Fill(looper->BTags,w);
					}
					else if(vname=="ht"){//sum of jet pt
						values[i].Fill(looper->HT,w);
					}
					else if(vname=="mht"){//missing hadronic energy
						values[i].Fill(looper->MHT,w);
					}
					else if(vname=="mindeltaphiN"){//min normalized deltaphi between jets and MET
						values[i].Fill(looper->minDeltaPhiN,w);
					}
					else if(vname=="nleptons"){//# leptons (mu or ele)
						values[i].Fill(looper->Leptons,w);
					}
					else if(vname=="nelectrons"){//# electrons
						values[i].Fill(looper->Electrons->size(),w);
					}
					else if(vname=="nmuons"){//# muons
						values[i].Fill(looper->Muons->size(),w);
					}
					else if(vname=="nisotrack"){//# iso tracks
						values[i].Fill(looper->isoElectronTracks+looper->isoMuonTracks+looper->isoPionTracks,w);
					}
					else if(vname=="nvertex"){//# good vertices
						values[i].Fill(looper->NVtx,w);
					}
					else { //if it's a histogram with no known variable or calculation, do nothing
					}
				}
				
				//now fill the histogram
				TH1* htmp = looper->htmp[h];
				if(vsize==1){
					for(int ix = 0; ix < values[0].GetSize(); ix++){
						htmp->Fill(values[0].GetValue(ix), values[0].GetWeight(ix));
					}
				}
				else if(vsize==2){
					//need to cast in order to use Fill(x,y,w)
					//these three cases allow for various x vs. y comparisons: same # entries per event, or 1 vs. N per event
					if(values[0].GetSize()==values[1].GetSize()) {
						for(int i = 0; i < values[0].GetSize(); i++){
							if(htmp->GetDimension()==1)
								static_cast<TProfile*>(htmp)->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
							else if(htmp->GetDimension()==2)
								static_cast<TH2*>(htmp)->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
						}
					}
					else if(values[0].GetSize()==1){
						for(int iy = 0; iy < values[1].GetSize(); iy++){
							if(htmp->GetDimension()==1)
								static_cast<TProfile*>(htmp)->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
							else if(htmp->GetDimension()==2)
								static_cast<TH2*>(htmp)->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
						}
					}
					else if(values[1].GetSize()==1){
						for(int ix = 0; ix < values[0].GetSize(); ix++){
							if(htmp->GetDimension()==1)
								static_cast<TProfile*>(htmp)->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
							else if(htmp->GetDimension()==2)
								static_cast<TH2*>(htmp)->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
						}
					}
				}
				else { //no support for other # of vars
				}
			}
			
			return true;
		}

};

//-------------------------------------------------------------
//addition to KParser to create selectors
namespace KParser {
	template <>
	KSelector<KBuilder>* processSelector<KBuilder>(KNamed* tmp){
		KSelector<KBuilder>* srtmp = 0;
		string sname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known selectors
		if(sname=="Histo") srtmp = new KHistoSelector(sname,omap);
		if(sname=="DoubleCount") srtmp = new KDoubleCountSelector(sname,omap);
		else {} //skip unknown selectors
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif