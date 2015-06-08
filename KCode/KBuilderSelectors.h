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
//class to store and apply RA2 binning
class KRA2BinSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KRA2BinSelector() : KSelector<KBuilder>() { }
		KRA2BinSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_), RA2Exclusive(true) { }
		virtual void SetSelection(KSelection<KBuilder>* sel_) {
			sel = sel_;

			//check if initial reading of input has already been done
			if(sel->GetGlobalOpt()->Get("prepared_RA2bin",false)){
				//if the initial reading failed, abort
				if(sel->GetGlobalOpt()->Get("failed_RA2bin",false)){
					depfailed = true;
					return;
				}
				
				//otherwise grab assembled member vars
				sel->GetGlobalOpt()->Get("IDtoBinNumber",IDtoBinNumber);
				sel->GetGlobalOpt()->Get("RA2VarNames",RA2VarNames);
				sel->GetGlobalOpt()->Get("RA2VarMin",RA2VarMin);
				sel->GetGlobalOpt()->Get("RA2VarMax",RA2VarMax);
			}
			else { //assemble member vars from user input
				sel->GetGlobalOpt()->Get("RA2VarNames",RA2VarNames);
				
				vector<vector<unsigned> > all_bins;
				for(unsigned q = 0; q < RA2VarNames.size(); ++q){
					stringstream pre;
					pre << "RA2Var" << q;
					
					vector<float> min, max;
					sel->GetGlobalOpt()->Get(pre.str()+"Min",min);
					sel->GetGlobalOpt()->Get(pre.str()+"Max",max);
					if(min.size() != max.size()){
						cout << "Input error: vector length mismatches in " << pre.str() << " min and max specification. RA2 binning will not be computed." << endl;
						depfailed = true;
						sel->GetGlobalOpt()->Set<bool>("prepared_RA2bin",true);
						sel->GetGlobalOpt()->Set<bool>("failed_RA2bin",true);
						return;
					}
					
					vector<unsigned> bins;
					sel->GetGlobalOpt()->Get(pre.str()+"Bins",bins);
					if(q>0 && bins.size()!=all_bins[0].size()){
						cout << "Input error: vector length mismatches in " << pre.str() << " bins specification. RA2 binning will not be computed." << endl;
						depfailed = true;
						sel->GetGlobalOpt()->Set<bool>("prepared_RA2bin",true);
						sel->GetGlobalOpt()->Set<bool>("failed_RA2bin",true);
						return;	
					}
					
					//store member vars
					RA2VarMin.push_back(min);
					RA2VarMax.push_back(max);
					all_bins.push_back(bins);
				}
				
				//create map of RA2 bin IDs to bin numbers
				//and associated bin labels (in case they are requested)
				vector<string> labels;
				labels.reserve(all_bins[0].size());
				for(unsigned b = 0; b < all_bins[0].size(); ++b){
					vector<unsigned> bin_id;
					bin_id.reserve(all_bins.size());
					stringstream label;
					for(unsigned q = 0; q < all_bins.size(); ++q){
						bin_id.push_back(all_bins[q][b]);
						label << RA2VarNames[q] << all_bins[q][b]; //the specific bin label: currently just the bin number
						if(q<all_bins.size()-1) label << "_";
					}
					IDtoBinNumber[bin_id] = b+1; //bin numbers start at 1
					labels.push_back(label.str());
				}
				
				//store assembled member vars with global options
				sel->GetGlobalOpt()->Set<map<vector<unsigned>,unsigned> >("IDtoBinNumber",IDtoBinNumber);
				sel->GetGlobalOpt()->Set<vector<vector<float> > >("RA2VarMin",RA2VarMin);
				sel->GetGlobalOpt()->Set<vector<vector<float> > >("RA2VarMax",RA2VarMax);
				sel->GetGlobalOpt()->Set<vector<string> >("RA2bin_labels",labels);
			}
			
			//check exclusive binning option
			RA2Exclusive = sel->GetGlobalOpt()->Get("RA2Exclusive",true);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(depfailed) return false;
			
			RA2bins = GetBinNumbers();
			
			return RA2bins.size()!=0;
		}
		
		//functions
		vector<unsigned> GetBinNumbers() {
			vector<vector<unsigned> > bins;
			bins.reserve(RA2VarNames.size());
			for(int q = 0; q < RA2VarNames.size(); ++q){
				bins.push_back(GetBins(q));
				
				//skip loop if no bin was found for a value
				if(bins.back().size()==0) return vector<unsigned>();
			}
			
			//set up for variable # of for loops
			vector<unsigned> indices(RA2VarNames.size(),0);
			vector<unsigned> bin_num(RA2VarNames.size(),0);
			vector<unsigned> found_bins;
			FindBin(indices,bins,0,bin_num,found_bins);
			return found_bins;
		}
		//recursive function to implement variable # of for loops
		//ref: http://stackoverflow.com/questions/9555864/variable-nested-for-loops
		void FindBin(vector<unsigned>& indices, vector<vector<unsigned> >& bins, unsigned pos, vector<unsigned>& bin_num, vector<unsigned>& found_bins){
			for(indices[pos] = 0; indices[pos] < bins[pos].size(); indices[pos]++){
				bin_num[pos] = bins[pos][indices[pos]];
				if(pos == indices.size()-1){
					map<vector<unsigned>, unsigned>::iterator it = IDtoBinNumber.find(bin_num);
					//exit loop as soon as an existing bin is found
					if(it != IDtoBinNumber.end()){
						found_bins.push_back(it->second);
					}
				}
				else {
					FindBin(indices,bins,pos+1,bin_num,found_bins);
				}
			}
		}
		
		vector<unsigned> GetBins(unsigned qty){
			//assume all values are floats
			float val = 0;
			if(RA2VarNames[qty]=="NJets") val = looper->NJets;
			else if(RA2VarNames[qty]=="BTags") val = looper->BTags;
			else if(RA2VarNames[qty]=="MHT") val = looper->MHT;
			else if(RA2VarNames[qty]=="HT") val = looper->HT;
			else if(RA2VarNames[qty]=="ak1p2Jets_sumJetMass") val = looper->ak1p2Jets_sumJetMass;
			else {}			
			
			vector<unsigned> bins;
			for(unsigned n = 0; n < RA2VarMin[qty].size(); ++n){
				if(val > RA2VarMin[qty][n] && val <= RA2VarMax[qty][n]) bins.push_back(n);
			}
			return bins;
		}
		
	public:
		//member variables
		bool RA2Exclusive;
		vector<unsigned> RA2bins;
		map<vector<unsigned>, unsigned> IDtoBinNumber;
		vector<string> RA2VarNames;
		vector<vector<float> > RA2VarMin, RA2VarMax;
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
						if(RA2Bin->RA2Exclusive) values[i].Fill(RA2Bin->RA2bins[0],w);
						else {
							for(int b = 0; b < RA2Bin->RA2bins.size(); b++){
								values[i].Fill(RA2Bin->RA2bins[b],w);
							}
						}
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