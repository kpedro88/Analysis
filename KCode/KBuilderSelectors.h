#ifndef KBUILDERSELECTORS_H
#define KBUILDERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KBuilder.h"
#include "KMath.h"
#include "../btag/BTagCorrector.h"
#include "../corrections/EventListFilter.h"

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

//----------------------------------------------------
//selects events based on HLT line
class KHLTSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KHLTSelector() : KSelector<KBuilder>() { }
		KHLTSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_) { 
			//get selected line from options
			localOpt->Get("HLTLines",HLTLines);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("TriggerNames",1);
			looper->fChain->SetBranchStatus("TriggerPass",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//skip if no line provided
			if(HLTLines.size()==0) return true;
			
			//initial loop over trigger names to find indices (position is consistent for all events)
			if(HLTIndices.empty()){
				for(unsigned h = 0; h < HLTLines.size(); h++){
					vector<string>::iterator lb = lower_bound(looper->TriggerNames->begin(),looper->TriggerNames->end(),HLTLines[h]);
					if(lb != looper->TriggerNames->end() && lb->find(HLTLines[h]) != std::string::npos){
						HLTIndices.push_back(distance(looper->TriggerNames->begin(),lb));
					}
				}
			}
			
			//loop over trigger names
			bool goodTrigger = false;
			for(unsigned h = 0; h < HLTIndices.size(); h++){
				unsigned index = HLTIndices[h];
				//check:
				//1) if the decision was true (the line fired)
				//2) if the line was not prescaled (currently ignored)
				if(looper->TriggerPass->at(index)==1) {
					goodTrigger = true;
					break;
				}
			}
			//skip event if finished searching and no HLT lines found
			return goodTrigger;
		}
		
		//member variables
		vector<string> HLTLines;
		vector<unsigned> HLTIndices;
};

//----------------------------------------------------
//selects events based on HT value
class KHTSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KHTSelector() : KSelector<KBuilder>() { }
		KHTSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_), HTmin(500) { 
			//check for option
			localOpt->Get("HTmin",HTmin);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("HT",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->HT > HTmin;
		}
		
		//member variables
		double HTmin;
};

//----------------------------------------------------
//selects events based on MHT value
class KMHTSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KMHTSelector() : KSelector<KBuilder>() { }
		KMHTSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_), MHTmin(200), debug(false), minPtMHT(30.), maxEtaMHT(5.) { 
			//check for option
			localOpt->Get("MHTmin",MHTmin);
			debug = localOpt->Get("debug",false);
			localOpt->Get("minPtMHT",minPtMHT);
			localOpt->Get("maxEtaMHT",maxEtaMHT);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MHT",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->MHT > MHTmin;
		}
		
		//member variables
		double MHTmin;
		bool debug;
		double minPtMHT, maxEtaMHT;
};

//---------------------------------------------------------------
//class to store and apply RA2 binning
class KRA2BinSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KRA2BinSelector() : KSelector<KBuilder>() { }
		KRA2BinSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_), RA2Exclusive(true), DoBTagSF(false) { }
		virtual void CheckDeps(){
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
			
			//check other options
			RA2Exclusive = sel->GetGlobalOpt()->Get("RA2Exclusive",true);
			DoBTagSF = sel->GetGlobalOpt()->Get("btagcorr",false);
		}
		virtual void CheckBranches(){
			for(unsigned q = 0; q < RA2VarNames.size(); ++q){
				looper->fChain->SetBranchStatus(RA2VarNames[q].c_str(),1);
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(depfailed) return false;
			
			RA2bins = GetBinNumbers(RA2binVec);
			
			return RA2bins.size()!=0;
		}
		
		//functions
		vector<unsigned> GetBinNumbers(vector<vector<unsigned> >& bin_vec) {
			vector<vector<unsigned> > bins;
			bins.reserve(RA2VarNames.size());
			for(unsigned q = 0; q < RA2VarNames.size(); ++q){
				bins.push_back(GetBins(q));
				
				//skip loop if no bin was found for a value
				if(bins.back().size()==0) return vector<unsigned>();
			}
			
			//set up for variable # of for loops
			vector<unsigned> indices(RA2VarNames.size(),0);
			vector<unsigned> bin_num(RA2VarNames.size(),0);
			vector<unsigned> found_bins;
			vector<vector<unsigned> > found_bin_nums;
			FindBin(indices,bins,0,bin_num,found_bins,found_bin_nums);
			bin_vec = found_bin_nums;
			return found_bins;
		}
		//recursive function to implement variable # of for loops
		//ref: http://stackoverflow.com/questions/9555864/variable-nested-for-loops
		void FindBin(vector<unsigned>& indices, vector<vector<unsigned> >& bins, unsigned pos, vector<unsigned>& bin_num, vector<unsigned>& found_bins, vector<vector<unsigned> >& found_bin_nums){
			for(indices[pos] = 0; indices[pos] < bins[pos].size(); indices[pos]++){
				bin_num[pos] = bins[pos][indices[pos]];
				if(pos == indices.size()-1){
					map<vector<unsigned>, unsigned>::iterator it = IDtoBinNumber.find(bin_num);
					if(it != IDtoBinNumber.end()){
						found_bins.push_back(it->second);
						found_bin_nums.push_back(bin_num);
					}
				}
				else {
					FindBin(indices,bins,pos+1,bin_num,found_bins,found_bin_nums);
				}
			}
		}
		
		vector<unsigned> GetBins(unsigned qty){
			//assume all values are floats
			vector<float> val;
			if(RA2VarNames[qty]=="NJets") val.push_back(looper->NJets);
			else if(RA2VarNames[qty]=="BTags") {
				if(DoBTagSF){
					//put this event into all btag bins
					for(unsigned b = 0; b < RA2VarMax[qty].size(); ++b){
						val.push_back(RA2VarMax[qty][b]);
					}
				}
				else val.push_back(looper->BTags);
			}
			else if(RA2VarNames[qty]=="MHT") val.push_back(looper->MHT);
			else if(RA2VarNames[qty]=="HT") val.push_back(looper->HT);
			//else if(RA2VarNames[qty]=="ak1p2Jets_sumJetMass") val.push_back(looper->ak1p2Jets_sumJetMass);
			else {}			
			
			vector<unsigned> bins;
			for(unsigned v = 0; v < val.size(); ++v){
				for(unsigned n = 0; n < RA2VarMin[qty].size(); ++n){
					if(val[v] > RA2VarMin[qty][n] && val[v] <= RA2VarMax[qty][n]) bins.push_back(n);
				}
			}
			return bins;
		}
		
		unsigned GetBin(string qty_name, const vector<unsigned>& bin_num){
			for(unsigned q = 0; q < RA2VarNames.size(); ++q){
				if(RA2VarNames[q]==qty_name) return bin_num[q];
			}
			return -1;
		}
		
	public:
		//member variables
		bool RA2Exclusive, DoBTagSF;
		vector<unsigned> RA2bins;
		vector<vector<unsigned> > RA2binVec;
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
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("RunNum",1);
			looper->fChain->SetBranchStatus("EvtNum",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			pair<unsigned,unsigned long long> tmp(looper->RunNum,looper->EvtNum);
			map<pair<unsigned,unsigned long long>,int>::iterator cmit = countmap.find(tmp);
			if(cmit!=countmap.end()) return false; //fails the event if it finds a double count
			else {
				countmap[tmp] = 1;
				return true;
			}
		}
		
		//member variables
		map<pair<unsigned,unsigned long long>,int> countmap;
};

//---------------------------------------------------------------
//applies photon ID to candidates
class KPhotonIDSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KPhotonIDSelector() : KSelector<KBuilder>() { }
		KPhotonIDSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_) { 
			canfail = false;
			//check for option
			best = localOpt->Get("best",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("photonCands",1);
			looper->fChain->SetBranchStatus("photon_hadTowOverEM",1);
			looper->fChain->SetBranchStatus("photon_hasPixelSeed",1);
			looper->fChain->SetBranchStatus("photon_isEB",1);
			looper->fChain->SetBranchStatus("photon_pfChargedIsoRhoCorr",1);
			looper->fChain->SetBranchStatus("photon_pfGammaIsoRhoCorr",1);
			looper->fChain->SetBranchStatus("photon_pfNeutralIsoRhoCorr",1);
			looper->fChain->SetBranchStatus("bestPhoton",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodPhotons.clear();
			for(unsigned p = 0; p < looper->photonCands->size(); ++p){
				bool goodPhoton = (abs(looper->photonCands->at(p).Eta())<1.4442 || ((abs(looper->photonCands->at(p).Eta())>1.566 && abs(looper->photonCands->at(p).Eta())<2.5)))
									&& looper->photonCands->at(p).Pt()>100.
									&& ((looper->photon_hadTowOverEM->at(p)<0.028 && !looper->photon_hasPixelSeed->at(p) && looper->photon_isEB->at(p)) 
										|| (looper->photon_hadTowOverEM->at(p)<0.093 && !looper->photon_hasPixelSeed->at(p) && !looper->photon_isEB->at(p)))
									&& looper->photon_pfChargedIsoRhoCorr->at(p)<(2.67*looper->photon_isEB->at(p) + 1.79*!looper->photon_isEB->at(p))
									&& looper->photon_pfGammaIsoRhoCorr->at(p)<((2.11+0.0014*looper->photonCands->at(p).Pt())*looper->photon_isEB->at(p)
																					+ (3.09+0.0091*looper->photonCands->at(p).Pt())*!looper->photon_isEB->at(p))
									&& looper->photon_pfNeutralIsoRhoCorr->at(p)<((7.23+exp(0.0028*looper->photonCands->at(p).Pt()+0.5408))*looper->photon_isEB->at(p)
																					+ (8.89+0.01725*looper->photonCands->at(p).Pt())*!looper->photon_isEB->at(p));
				if(best) goodPhoton &= looper->bestPhoton->size()>0 && KMath::DeltaR(looper->bestPhoton->at(0).Phi(),looper->bestPhoton->at(0).Eta(),looper->photonCands->at(p).Phi(),looper->photonCands->at(p).Eta()) < 0.15;
				if(goodPhoton) goodPhotons.push_back(p);
			}
			return true;
		}
		
		//member variables
		bool best;
		vector<unsigned> goodPhotons;
};

//---------------------------------------------------------------
//applies MET filters
class KMETFilterSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KMETFilterSelector() : KSelector<KBuilder>() { }
		KMETFilterSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_), fastsim(false), cscfile("") { 
			localOpt->Get("cscfile",cscfile);
			if(cscfile.size()>0){
				cscfilter = EventListFilter(cscfile);
			}
			localOpt->Get("filterfiles",filterfiles);
			for(unsigned f = 0; f < filterfiles.size(); ++f){
				filters.push_back(new EventListFilter(filterfiles[f]));
			}
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("NVtx",1);
			looper->fChain->SetBranchStatus("eeBadScFilter",1);
			looper->fChain->SetBranchStatus("eeBadSc4Filter",1);
			looper->fChain->SetBranchStatus("HBHENoiseFilter",1);
			looper->fChain->SetBranchStatus("HBHEIsoNoiseFilter",1);
			looper->fChain->SetBranchStatus("CSCTightHaloFilter",1);
			if(cscfilter.Initialized() || (filters.size()>0 && filters[0]->Initialized())){
				looper->fChain->SetBranchStatus("RunNum",1);
				looper->fChain->SetBranchStatus("LumiBlockNum",1);
				looper->fChain->SetBranchStatus("EvtNum",1);				
			}
		}
		virtual void CheckLooper(){
			//check fastsim stuff
			fastsim = looper->localOpt->Get("fastsim",false);
			if(fastsim){
				//disable this for fastsim
				dummy = true;
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool CSCTightHaloFilter = looper->CSCTightHaloFilter;
			if(cscfilter.Initialized()) CSCTightHaloFilter = cscfilter.CheckEvent(looper->RunNum,looper->LumiBlockNum,looper->EvtNum);
			bool otherFilters = true;
			for(unsigned f = 0; f < filters.size(); ++f){
				otherFilters &= filters[f]->CheckEvent(looper->RunNum,looper->LumiBlockNum,looper->EvtNum);
			}
			return looper->NVtx > 0 && looper->eeBadScFilter==1 && looper->eeBadSc4Filter && looper->HBHENoiseFilter && looper->HBHEIsoNoiseFilter && CSCTightHaloFilter && otherFilters;
		}
		
		//member variables
		bool fastsim;
		string cscfile;
		EventListFilter cscfilter;
		vector<string> filterfiles;
		vector<EventListFilter*> filters;
};

//---------------------------------------------------------------
//eta regions for PFJetID: 0 = 0.0 < |eta| < 2.4; 1 = 0.0 < |eta| < 3.0; 2 = 3.0 < |eta|
//all require pt > 30
class KJetEtaRegionSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KJetEtaRegionSelector() : KSelector<KBuilder>() { }
		KJetEtaRegionSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_), region(0) { 
			localOpt->Get("region",region);
			canfail = false;
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Jets",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//reset mask
			mask.clear(); mask.resize(looper->Jets->size(),false);
			//check eta for each jet
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(looper->Jets->at(j).Pt() <= 30) continue;
				
				if(region==0 && fabs(looper->Jets->at(j).Eta()) <= 2.4) mask[j] = true;
				else if(region==1 && fabs(looper->Jets->at(j).Eta()) > 2.4 && fabs(looper->Jets->at(j).Eta()) <= 3.0) mask[j] = true;
				else if(region==2 && fabs(looper->Jets->at(j).Eta()) > 3.0) mask[j] = true;
			}
			return true;
		}
		
		//member variables
		int region;
		vector<bool> mask;
};

//---------------------------------------------------------------
//recalculate NJets and BTags in a hemisphere around MHT_Phi
class KHemisphereSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KHemisphereSelector() : KSelector<KBuilder>() { }
		KHemisphereSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_) { 
			canfail = false;
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MHT_Phi",1);
			looper->fChain->SetBranchStatus("HTJetsMask",1);
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("Jets_bDiscriminatorCSV",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//reset output vars
			NJets = 0; BTags = 0;
			NJetsOpp = 0; BTagsOpp = 0;
			//check dphi for each jet
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				//HT jet cuts
				if(!looper->HTJetsMask->at(j)) continue;
				
				double dphi = KMath::DeltaPhi(looper->MHT_Phi,looper->Jets->at(j).Phi());
				if(fabs(dphi)<=TMath::Pi()/2){
					++NJets;
					if(looper->Jets_bDiscriminatorCSV->at(j) > 0.890) ++BTags;
				}
				else {
					++NJetsOpp;
					if(looper->Jets_bDiscriminatorCSV->at(j) > 0.890) ++BTagsOpp;
				}
			}
			return true;
		}
		
		//member variables
		int NJets, BTags;
		int NJetsOpp, BTagsOpp;
};

//---------------------------------------------------------------
//calculates btag scale factors
class KBTagSFSelector : public KSelector<KBuilder> {
	public:
		//constructor
		KBTagSFSelector() : KSelector<KBuilder>() { }
		KBTagSFSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_)
		{ 
			canfail = false;
			
			//check for option
			debug = localOpt->Get("debug",false); btagcorr.SetDebug(debug);
			
			//initialize btag corrector calibrations
			btagcorr.SetCalib("btag/CSVv2_mod.csv");
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("HTJetsMask",1);
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("Jets_hadronFlavor",1);
			if(debug) looper->fChain->SetBranchStatus("BTags",1);
		}
		virtual void CheckLooper(){
			//check for option
			int btagSFunc = 0; looper->globalOpt->Get("btagSFunc",btagSFunc); btagcorr.SetBtagSFunc(btagSFunc);
			int ctagSFunc = 0; looper->globalOpt->Get("ctagSFunc",ctagSFunc); btagcorr.SetCtagSFunc(ctagSFunc);
			int mistagSFunc = 0; looper->globalOpt->Get("mistagSFunc",mistagSFunc); btagcorr.SetMistagSFunc(mistagSFunc);

			//get efficiency histograms
			btagcorr.SetEffs(looper->MyBase->GetFile());
			
			if(!btagcorr.h_eff_b || !btagcorr.h_eff_c || !btagcorr.h_eff_udsg){
				cout << "Input error: b-tag efficiency histograms missing!" << endl;
				depfailed = true;
			}
			
			//check fastsim stuff
			bool fastsim = looper->localOpt->Get("fastsim",false); btagcorr.SetFastSim(fastsim);
			if(fastsim){
				//initialize btag corrector fastsim calibrations
				//todo: check the sample name and choose the appropriate CFs (once available)
				btagcorr.SetCalibFastSim("btag/CSV_13TEV_Combined_20_11_2015.csv");
				
				//check for option
				int btagCFunc = 0; looper->globalOpt->Get("btagCFunc",btagCFunc); btagcorr.SetBtagCFunc(btagCFunc);
				int ctagCFunc = 0; looper->globalOpt->Get("ctagCFunc",ctagCFunc); btagcorr.SetCtagCFunc(ctagCFunc);
				int mistagCFunc = 0; looper->globalOpt->Get("mistagCFunc",mistagCFunc);  btagcorr.SetMistagCFunc(mistagCFunc);
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//get probabilities
			prob = btagcorr.GetCorrections(looper->Jets,looper->Jets_hadronFlavor,looper->HTJetsMask);
			if(debug) cout << "BTags = " << looper->BTags << endl;
			return true;
		}
		
		//member variables
		bool debug;
		BTagCorrector btagcorr;
		vector<double> prob;
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
		KHistoSelector(string name_, OptionMap* localOpt_) : KSelector<KBuilder>(name_,localOpt_), RA2Bin(NULL), PhotonID(NULL), BTagSF(NULL), JetEtaRegion(NULL), Hemisphere(NULL) { 
			canfail = false;
		}
		
		virtual void CheckDeps(){
			//set dependencies here
			RA2Bin = sel->Get<KRA2BinSelector*>("RA2Bin");
			PhotonID = sel->Get<KPhotonIDSelector*>("PhotonID");
			bool DoBTagSF = sel->GetGlobalOpt()->Get("btagcorr",false);
			
			if(DoBTagSF) BTagSF = sel->Get<KBTagSFSelector*>("BTagSF");
			JetEtaRegion = sel->Get<KJetEtaRegionSelector*>("JetEtaRegion");
			Hemisphere = sel->Get<KHemisphereSelector*>("Hemisphere");
		}
		virtual void CheckLooper(){
			looper->localOpt->Get("mother",mother);
			deltaM = 0; looper->localOpt->Get("deltaM",deltaM);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			double w = looper->GetWeight();
			
			for(unsigned h = 0; h < looper->htmp.size(); h++){
				unsigned vsize = looper->vars[h].size();
				vector<KValue> values(vsize);				
			
				for(unsigned i = 0; i < vsize; i++){
					string vname = looper->vars[h][i];
					//list of cases for histo calculation and filling
					if(vname=="RA2bin" && RA2Bin){ //plot yield vs. bin of RA2 search -> depends on RA2Bin selector
						if(RA2Bin->RA2Exclusive) values[i].Fill(RA2Bin->RA2bins[0],w);
						else {
							for(unsigned b = 0; b < RA2Bin->RA2bins.size(); b++){
								double wb = w;
								//weight by btag scale factor probability if available
								if(BTagSF) {
									int nb = RA2Bin->GetBin("BTags",RA2Bin->RA2binVec[b]);
									if(nb>=0 && ((unsigned)nb)<BTagSF->prob.size()) wb *= BTagSF->prob[nb];
									else wb = 0; //btag sf failed
								}
								values[i].Fill(RA2Bin->RA2bins[b],wb);
							}
						}
					}
					else if(vname=="njets"){//jet multiplicity
						values[i].Fill(looper->NJets,w);
					}
					else if(vname=="nbjets"){//b-jet multiplicity
						values[i].Fill(looper->BTags,w);
					}
					else if(vname=="njetshemi"){//jet multiplicity
						if(Hemisphere) values[i].Fill(Hemisphere->NJets,w);
					}
					else if(vname=="nbjetshemi"){//b-jet multiplicity
						if(Hemisphere) values[i].Fill(Hemisphere->BTags,w);
					}
					else if(vname=="njetsopphemi"){//jet multiplicity
						if(Hemisphere) values[i].Fill(Hemisphere->NJetsOpp,w);
					}
					else if(vname=="nbjetsopphemi"){//b-jet multiplicity
						if(Hemisphere) values[i].Fill(Hemisphere->BTagsOpp,w);
					}
					else if(vname=="ht"){//sum of jet pt
						values[i].Fill(looper->HT,w);
					}
					else if(vname=="mht"){//missing hadronic energy
						values[i].Fill(looper->MHT,w);
					}
					else if(vname=="met"){//missing energy
						values[i].Fill(looper->METPt,w);
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
					else if(vname=="numint"){//# interactions
						values[i].Fill(looper->NumInteractions,w);
					}
					else if(vname=="genht"){//gen HT
						values[i].Fill(looper->genHT,w);
					}
					else if(vname=="leadjetpt"){//pT of leading jet
						if(looper->Jets->size()>0){
							values[i].Fill(looper->Jets->at(0).Pt(),w);
						}
					}
					else if(vname=="deltaphi1"){//deltaphi of leading jet
						values[i].Fill(looper->DeltaPhi1,w);
					}
					else if(vname=="deltaM"){//difference between mMother and mLSP
						values[i].Fill(deltaM,w);
					}
					else if(vname=="recoil"){//pT of mother particle system recoiling against ISR jets
						//loop over genparticles
						TLorentzVector vgen;
						vgen.SetPtEtaPhiE(0,0,0,0);
						for(unsigned g = 0; g < looper->genParticles_PDGid->size(); ++g){
							if(binary_search(mother.begin(),mother.end(),abs(looper->genParticles_PDGid->at(g)))){
								vgen += looper->genParticles->at(g);
							}
						}
						values[i].Fill(vgen.Pt(),w);
					}
					else if(vname=="deltaphirecoil"){//delta phi of mother particle system with MHT
						//loop over genparticles
						TLorentzVector vgen;
						vgen.SetPtEtaPhiE(0,0,0,0);
						for(unsigned g = 0; g < looper->genParticles_PDGid->size(); ++g){
							if(binary_search(mother.begin(),mother.end(),abs(looper->genParticles_PDGid->at(g)))){
								vgen += looper->genParticles->at(g);
							}
						}
						values[i].Fill(KMath::DeltaPhi(vgen.Phi(),looper->MHT_Phi),w);
					}
					else if(vname=="sigmaietaieta"){//sigma ieta ieta variable for all photon candidates
						if(PhotonID){
							for(unsigned p = 0; p < PhotonID->goodPhotons.size(); ++p){
								values[i].Fill(looper->photon_sigmaIetaIeta->at(PhotonID->goodPhotons[p]),w);
							}
						}
						else { //if no ID applied, just plot everything
							for(unsigned p = 0; p < looper->photon_sigmaIetaIeta->size(); ++p){
								values[i].Fill(looper->photon_sigmaIetaIeta->at(p),w);
							}
						}
					}
					else if(vname=="bestsigmaietaieta"){//sigma ieta ieta variable for best photon
						if(looper->bestPhoton->size()==0) continue;
						for(unsigned p = 0; p < looper->photonCands->size(); ++p){
							if(looper->bestPhoton->at(0)==looper->photonCands->at(p) && looper->photon_isEB->at(p)){
								values[i].Fill(looper->photon_sigmaIetaIeta->at(p),w);
								break;
							}
						}
					}
					else if(vname=="pdfrms"){
						values[i].Fill(TMath::RMS(looper->PDFweights->begin(),looper->PDFweights->end()),w);
					}
					else if(vname=="pdfweight"){
						for(unsigned w = 0; w < looper->PDFweights->size(); ++w){
							values[i].Fill(looper->PDFweights->at(w),w);
						}
					}
					//jet ID quantities... (w/ optional eta region specification)
					else if(vname=="neufrac"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_neutralHadronEnergyFraction->at(j),w);
						}
					}
					else if(vname=="phofrac"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_photonEnergyFraction->at(j),w);
						}
					}
					else if(vname=="chgfrac"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_chargedHadronEnergyFraction->at(j),w);
						}
					}
					else if(vname=="chgemfrac"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_chargedEmEnergyFraction->at(j),w);
						}
					}
					else if(vname=="chgmulti"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_chargedMultiplicity->at(j),w);
						}
					}
					else if(vname=="neumulti"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_neutralMultiplicity->at(j),w);
						}
					}
					else if(vname=="nconstit"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_chargedMultiplicity->at(j)+looper->Jets_neutralMultiplicity->at(j),w);
						}
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
		KPhotonIDSelector* PhotonID;
		KBTagSFSelector* BTagSF;
		KJetEtaRegionSelector* JetEtaRegion;
		KHemisphereSelector* Hemisphere;
		vector<int> mother;
		double deltaM;
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
		else if(sname=="DoubleCount") srtmp = new KDoubleCountSelector(sname,omap);
		else if(sname=="RA2Bin") srtmp = new KRA2BinSelector(sname,omap);
		else if(sname=="PhotonID") srtmp = new KPhotonIDSelector(sname,omap);
		else if(sname=="BTagSF") srtmp = new KBTagSFSelector(sname,omap);
		else if(sname=="METFilter") srtmp = new KMETFilterSelector(sname,omap);
		else if(sname=="HLT") srtmp = new KHLTSelector(sname,omap);
		else if(sname=="HT") srtmp = new KHTSelector(sname,omap);
		else if(sname=="MHT") srtmp = new KMHTSelector(sname,omap);
		else if(sname=="JetEtaRegion") srtmp = new KJetEtaRegionSelector(sname,omap);
		else if(sname=="Hemisphere") srtmp = new KHemisphereSelector(sname,omap);
		else {} //skip unknown selectors
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif