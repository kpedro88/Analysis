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
//class to build RA2 bin IDs
class RA2binID {
	public:
		//constructors
		RA2binID() : id(0) {}
		RA2binID(unsigned id_) : id(id_) {}
		RA2binID(unsigned NJetBin, unsigned NBJetBin, unsigned MHTBin, unsigned HTBin) : 
			id(NJetBin + 100*NBJetBin + 10000*MHTBin + 1000000*HTBin) {}
		//destructor
		~RA2binID() {}
		
		//accessors
		unsigned raw() { return id; }
		unsigned NJetBin() { return id%100; }
		unsigned NBJetBin() { return (id/100)%100; }
		unsigned MHTBin() { return (id/10000)%100; }
		unsigned HTBin() { return (id/1000000)%100; }
	
	private:
		//member variables
		unsigned id;
};

//---------------------------------------------------------------
//class to store and apply RA2 binning
class KRA2BinSelector : public KSelector<KBuilder> {
	public:
		typedef map<unsigned, unsigned>::iterator BNit;
	
		//constructor
		KRA2BinSelector() : KSelector<KBuilder>() { }
		KRA2BinSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_), initialized(false), RA2bin(0) { }
		void Initialize(){
			if(initialized) return;
			
			//get objects
			vector<unsigned> NJetBins, NBJetBins, MHTBins, HTBins;
			looper->globalOpt->Get("NJetBinMin",NJetMin); looper->globalOpt->Get("NJetBinMax",NJetMax);
			looper->globalOpt->Get("NBJetBinMin",NBJetMin); looper->globalOpt->Get("NBJetBinMax",NBJetMax);
			looper->globalOpt->Get("MHTBinMin",MHTMin); looper->globalOpt->Get("MHTBinMax",MHTMax);
			looper->globalOpt->Get("HTBinMin",HTMin); looper->globalOpt->Get("HTBinMax",HTMax);
			looper->globalOpt->Get("NJetBins",NJetBins); looper->globalOpt->Get("NBJetBins",NBJetBins);
			looper->globalOpt->Get("MHTBins",MHTBins); looper->globalOpt->Get("HTBins",HTBins);
			
			//safety checks
			bool minmax_lengths = NJetMin.size()==NJetMax.size() && NBJetMin.size()==NBJetMax.size() && MHTMin.size()==MHTMax.size() && HTMin.size()==HTMax.size();
			if(!minmax_lengths) {
				cout << "Input error: vector length mismatches in RA2 min and max specification. RA2 binning will not be computed." << endl;
				depfailed = true;
				return;
			}
			bool binning_lengths = NJetBins.size()==NBJetBins.size() && NJetBins.size()==MHTBins.size() && NJetBins.size()==HTBins.size();
			if(!binning_lengths) {
				cout << "Input error: vector length mismatches in RA2 bins specification. RA2 binning will not be computed." << endl;
				depfailed = true;
				return;
			}
			
			//create map of RA2 bin IDs to bin numbers (if not already created)
			if(!(looper->globalOpt->Get("IDtoBinNumber",IDtoBinNumber))){
				for(unsigned b = 0; b < NJetBins.size(); b++){
					IDtoBinNumber[RA2binID(NJetBins[b],NBJetBins[b],MHTBins[b],HTBins[b]).raw()] = b+1; //bin numbers start at 1
				}
				
				//store map with global options
				looper->globalOpt->Set<map<unsigned,unsigned> >("IDtoBinNumber",IDtoBinNumber);
			}
			initialized = true;
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(depfailed) return false;
			Initialize();
			
			RA2bin = GetBinNumber(looper->NJets,looper->BTags,looper->MHT,looper->HT);
			
			return RA2bin!=0;
		}
		
		//functions
		unsigned GetBinNumber(int& NJetVal, int& NBJetVal, float& MHTVal, float& HTVal) {
			vector<unsigned> NJetBins = GetNJetBins(NJetVal);
			vector<unsigned> NBJetBins = GetNBJetBins(NBJetVal);
			vector<unsigned> MHTBins = GetMHTBins(MHTVal);
			vector<unsigned> HTBins = GetHTBins(HTVal);
			
			//skip loop if no bin was found for a value
			if(NJetBins.size()==0 || NBJetBins.size()==0 || MHTBins.size()==0 || HTBins.size()==0) return 0;
			
			//find the correct combination of bin numbers that exists in the map
			for(unsigned nj = 0; nj < NJetBins.size(); nj++){
				for(unsigned nb = 0; nb < NBJetBins.size(); nb++){
					for(unsigned nm = 0; nm < MHTBins.size(); nm++){
						for(unsigned nh = 0; nh < HTBins.size(); nh++){
							unsigned raw = RA2binID(NJetBins[nm],NBJetBins[nb],MHTBins[nm],HTBins[nh]).raw();
							BNit it = IDtoBinNumber.find(raw);
							//exit loop as soon as an existing bin is found
							if(it != IDtoBinNumber.end()){
								return it->second;
							}
						}
					}
				}
			}
			
			//if no bins are were found, return default
			return 0;
		}
		vector<unsigned> GetNJetBins(int& NJetVal){
			vector<unsigned> bins;
			for(unsigned n = 0; n < NJetMin.size(); ++n){
				if(NJetVal >= NJetMin[n] && NJetVal <= NJetMax[n]) bins.push_back(n);
			}
			return bins;
		}
		vector<unsigned> GetNBJetBins(int& NBJetVal){
			vector<unsigned> bins;
			for(unsigned n = 0; n < NBJetMin.size(); ++n){
				if(NBJetVal >= NBJetMin[n] && NBJetVal <= NBJetMax[n]) bins.push_back(n);
			}
			return bins;
		}
		vector<unsigned> GetMHTBins(float& MHTVal){
			vector<unsigned> bins;
			for(unsigned n = 0; n < MHTMin.size(); ++n){
				if(MHTVal > MHTMin[n] && MHTVal <= MHTMax[n]) bins.push_back(n);
			}
			return bins;
		}
		vector<unsigned> GetHTBins(float& HTVal){
			vector<unsigned> bins;
			for(unsigned n = 0; n < HTMin.size(); ++n){
				if(HTVal > HTMin[n] && HTVal <= HTMax[n]) bins.push_back(n);
			}
			return bins;
		}
		
	public:
		//member variables
		bool initialized;
		int RA2bin;
		map<unsigned, unsigned> IDtoBinNumber;
		vector<int> NJetMin, NJetMax, NBJetMin, NBJetMax;
		vector<float> MHTMin, MHTMax, HTMin, HTMax;
};

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
		
		virtual void SetSelection(KSelection<KBuilder>* sel_) {
			sel = sel_;
			//set dependencies here
			RA2Bin = sel->Get<KRA2BinSelector*>("RA2Bin");
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			double w = looper->Weight();
			
			for(int h = 0; h < looper->htmp.size(); h++){
				unsigned vsize = looper->vars[h].size();
				vector<KValue> values(vsize);				
			
				for(int i = 0; i < vsize; i++){
					string vname = looper->vars[h][i];
					//list of cases for histo calculation and filling
					if(vname=="RA2bin" && RA2Bin){//plot yield vs. bin of RA2 search -> depends on RA2Bin selector
						values[i].Fill(RA2Bin->RA2bin,w);
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
		
		//member variables
		KRA2BinSelector* RA2Bin;
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
		if(sname=="RA2Bin") srtmp = new KRA2BinSelector(sname,omap);
		else {} //skip unknown selectors
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif