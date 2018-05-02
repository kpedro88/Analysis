#ifndef KCOMMONSELECTORS_H
#define KCOMMONSELECTORS_H

//custom headers
#include "KSelection.h"
#include "../corrections/EventShapeVariables.c"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>

//STL headers
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//----------------------------------------------------
//selects events based on HLT line
class KHLTSelector : public KSelector {
	public:
		//constructor
		KHLTSelector() : KSelector() { }
		KHLTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
			//get selected line from options
			localOpt->Get("HLTLines",HLTLines);
			debug = localOpt->Get("debug",false);
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
					if(debug){
						cout << HLTLines[h] << " " << *lb << endl;
					}
					if(lb != looper->TriggerNames->end() && lb->find(HLTLines[h]) != std::string::npos){
						HLTIndices.push_back(distance(looper->TriggerNames->begin(),lb));
					}
				}
				if(debug){
					for(unsigned h = 0; h < HLTIndices.size(); h++){
						cout << HLTLines[h] << ": " << HLTIndices[h] << endl;
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
		bool debug;
};
REGISTER_SELECTOR(HLT);

//----------------------------------------------------
class KDijetSelector : public KSelector {
	public:
		//constructor
		KDijetSelector() : KSelector() { }
		KDijetSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(2), pt(-1), eta(2.4) {
			//check for options
			localOpt->Get("njet",njet);
			id = localOpt->Get("id",true);
			simple = localOpt->Get("simple",true);
			localOpt->Get("pt",pt);
			localOpt->Get("eta",eta);
		}
		virtual void CheckBranches(){
			if(simple) {
				//avoid loading jet branch just to check size==2
				looper->fChain->SetBranchStatus("MT_AK8",1);
			}
			else{
				looper->fChain->SetBranchStatus("JetsAK8",1);
				if(id) looper->fChain->SetBranchStatus("JetsAK8_ID",1);
			}
		}

		//this selector doesn't add anything to tree

		//used for non-dummy selectors
		virtual bool Cut() {
			if(simple) return looper->MT_AK8 > 0;
			if(looper->JetsAK8->size()<njet) return false;
			//per-jet cuts, if enabled
			for(unsigned j = 0; j < njet; ++j){
				if(id and !looper->JetsAK8_ID->at(j)) return false;
				if(pt>0 and looper->JetsAK8->at(j).Pt()<=pt) return false;
				if(eta>0 and abs(looper->JetsAK8->at(j).Eta())>=eta) return false;
			}
			return true;
		}

		//member variables
		int njet;
		bool simple, id;
		double pt, eta;
};
REGISTER_SELECTOR(Dijet);

//----------------------------------------------------
//selects events based on HT value
class KHTSelector : public KSelector {
	public:
		//constructor
		KHTSelector() : KSelector() { }
		KHTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(500) { 
			//check for option
			localOpt->Get("min",min);
			doGen = localOpt->Get("gen",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("HT",1);
			if(doGen) looper->fChain->SetBranchStatus("GenHT",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(doGen) return looper->GenHT > min;
			else return looper->HT > min;
		}
		
		//member variables
		double min;
		bool doGen;
};
REGISTER_SELECTOR(HT);

//----------------------------------------------------
//selects events based on MHT value
class KMHTSelector : public KSelector {
	public:
		//constructor
		KMHTSelector() : KSelector() { }
		KMHTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(200) { 
			//check for option
			localOpt->Get("min",min);
			doGen = localOpt->Get("gen",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MHT",1);
			if(doGen) looper->fChain->SetBranchStatus("GenMHT",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(doGen) return looper->GenMHT > min;
			else return looper->MHT > min;
		}
		
		//member variables
		double min;
		bool doGen;
};
REGISTER_SELECTOR(MHT);

//----------------------------------------------------
class KDijetAK4Selector : public KSelector {
	public:
		//constructor
		KDijetAK4Selector() : KSelector() { }
		KDijetAK4Selector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(2), id(true), pt(-1), eta(2.4) {
			//check for options
			localOpt->Get("njet",njet);
			id = localOpt->Get("id",id);
			localOpt->Get("pt",pt);
			localOpt->Get("eta",eta);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("Jets_ID",1);
		}

		//this selector doesn't add anything to tree

		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->Jets->size()<njet) return false;
			//per-jet cuts, if enabled
			for(unsigned j = 0; j < njet; ++j){
				if(id and !looper->Jets_ID->at(j)) return false;
				if(pt>0 and looper->Jets->at(j).Pt()<=pt) return false;
				if(eta>0 and abs(looper->Jets->at(j).Eta())>=eta) return false;
			}
			return true;
		}

		//member variables
		int njet;
		bool id;
		double pt, eta;
};
REGISTER_SELECTOR(DijetAK4);

//----------------------------------------------------
//selects events based on MET value
class KMETSelector : public KSelector {
	public:
		//constructor
		KMETSelector() : KSelector() { }
		KMETSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(100) { 
			//check for option
			localOpt->Get("min",min);
			invert = localOpt->Get("invert",false);
			doGen = localOpt->Get("gen",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MET",1);
			if(doGen) looper->fChain->SetBranchStatus("GenMET",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(doGen) return ( invert ? looper->GenMET <= min : looper->GenMET > min );
			else return ( invert ? looper->MET <= min : looper->MET > min );
		}
		
		//member variables
		double min;
		bool invert, doGen;
};
REGISTER_SELECTOR(MET);

//-------------------------------------------------------------------
//selects events based on AK8 MT(jj,MET) value (possibly in a window)
class KMTAK8Selector : public KSelector {
	public:
		//constructor
		KMTAK8Selector() : KSelector() { }
		KMTAK8Selector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(1000), max(-1) { 
			//check for option
			localOpt->Get("min",min);
			localOpt->Get("max",max);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MT_AK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return ( (min<0 or looper->MT_AK8>min) and (max<0 or looper->MT_AK8<max) );
		}
		
		//member variables
		double min, max;
};
REGISTER_SELECTOR(MTAK8);

//-------------------------------------------------------------------
//selects events based on AK8 Mjj value (possibly in a window)
class KMJJAK8Selector : public KSelector {
	public:
		//constructor
		KMJJAK8Selector() : KSelector() { }
		KMJJAK8Selector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(1000), max(-1) { 
			//check for option
			localOpt->Get("min",min);
			localOpt->Get("max",max);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MJJ_AK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return ( (min<0 or looper->MJJ_AK8>min) and (max<0 or looper->MJJ_AK8<max) );
		}
		
		//member variables
		double min, max;
};
REGISTER_SELECTOR(MJJAK8);

//-------------------------------------------------------------
//vetos events with leptons
class KLeptonVetoSelector : public KSelector {
	public:
		//constructor
		KLeptonVetoSelector() : KSelector() { }
		KLeptonVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("NMuons",1);
			looper->fChain->SetBranchStatus("NElectrons",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->NMuons==0 and looper->NElectrons==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(LeptonVeto);


//----------------------------------------------------
//selects events based on MET/MT value
class KMETMTRatioSelector : public KSelector {
	public:
		//constructor
		KMETMTRatioSelector() : KSelector() { }
		KMETMTRatioSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(0.15) { 
			//check for option
			localOpt->Get("min",min);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MET",1);
			looper->fChain->SetBranchStatus("MT_AK8",1);
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->JetsAK8->size()<2) return false;
			double metMTratio = looper->MT_AK8 > 0 ? looper->MET/looper->MT_AK8 : 0.0;
			return metMTratio > min;
		}
		
		//member variables
		double min;
};
REGISTER_SELECTOR(METMTRatio);

//----------------------------------------------------
//selects events based on delta eta(j1,j2)
class KDeltaEtaSelector : public KSelector {
	public:
		//constructor
		KDeltaEtaSelector() : KSelector() { }
		KDeltaEtaSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), detamax(1.1) { 
			//check for option
			localOpt->Get("detamax",detamax);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return abs(looper->JetsAK8->at(0).Eta()-looper->JetsAK8->at(1).Eta()) < detamax;
		}
		
		//member variables
		double detamax;
};
REGISTER_SELECTOR(DeltaEta);

//----------------------------------------------------
//selects events based on METsig value
class KMETSignificanceSelector : public KSelector {
	public:
		//constructor
		KMETSignificanceSelector() : KSelector() { }
		KMETSignificanceSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(20) { 
			//check for option
			localOpt->Get("min",min);
			invert = localOpt->Get("invert",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("METSignificance",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return ( invert ? looper->METSignificance <= min : looper->METSignificance > min );
		}
		
		//member variables
		double min;
		bool invert;
};
REGISTER_SELECTOR(METSignificance);

//----------------------------------------------------
//selects events based on dphi_min_AK8 value
class KDeltaPhiMinAK8Selector : public KSelector {
	public:
		//constructor
		KDeltaPhiMinAK8Selector() : KSelector() { }
		KDeltaPhiMinAK8Selector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), max(1.0) { 
			//check for option
			localOpt->Get("max",max);
			invert = localOpt->Get("invert",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("DeltaPhiMin_AK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return ( invert ? looper->DeltaPhiMin_AK8 >= max : looper->DeltaPhiMin_AK8 < max );
		}
		
		//member variables
		double max;
		bool invert;
};
REGISTER_SELECTOR(DeltaPhiMinAK8);

//----------------------------------------------------
//computes event shape variables (from jets)
class KEventShapeSelector : public KSelector {
	public:
		enum JetSource { NoSource = 0, AK4 = 1, AK8 = 2 };
		//constructor
		KEventShapeSelector() : KSelector() { }
		KEventShapeSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
			canfail = false;
			//check for option
			string s_source;
			localOpt->Get("source",s_source);
			if(s_source=="AK4") source = AK4;
			else if(s_source=="AK8") source = AK8;
			else source = NoSource;
		}
		virtual void CheckBranches(){
			if(source==AK4) looper->fChain->SetBranchStatus("Jets",1);
			else if(source==AK8) looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree (yet)
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//clear output vars
			sphericity = aplanarity = esvC = esvD = -1;
			lambda1 = lambda2 = lambda3 = -1;
			fwm1 = fwm2 = fwm3 = fwm4 = fwm5 = fwm6 = -1;

			if(source==NoSource) return true;

			vector<TLorentzVector>* Jets;
			if(source==AK4) Jets = looper->Jets;
			else if(source==AK8) Jets = looper->JetsAK8;

			//find z boost of system, get CM frame
			TLorentzVector v_all;
			for(const auto& Jet: *Jets){
				v_all += Jet;
			}
			TVector3 v_boost(0.0, 0.0, -1.0*v_all.Pz()/v_all.E());

			//make boosted collection in proper format
			vector<math::RThetaPhiVector> v_cm;
			for(auto Jet: *Jets){//copy loop object in order to boost
				Jet.Boost(v_boost);
				v_cm.emplace_back(Jet.P(),Jet.Theta(),Jet.Phi());
			}

			//compute and store values
			boost = v_boost.Z();
			EventShapeVariables esv(v_cm);
			sphericity = esv.sphericity();
			aplanarity = esv.aplanarity();
			esvC = esv.C();
			esvD = esv.D();
			const auto& eigenvals  = esv.getEigenValues();
			lambda1 = eigenvals[0];
			lambda2 = eigenvals[1];
			lambda3 = eigenvals[2];
			esv.setFWmax(7);
			double fwm0 = esv.getFWmoment(0);
			if(fwm0==0.) fwm0 = 1; //avoid nan
			fwm1 = esv.getFWmoment(1)/fwm0;
			fwm2 = esv.getFWmoment(2)/fwm0;
			fwm3 = esv.getFWmoment(3)/fwm0;
			fwm4 = esv.getFWmoment(4)/fwm0;
			fwm5 = esv.getFWmoment(5)/fwm0;
			fwm6 = esv.getFWmoment(6)/fwm0;

			return true;
		}
		
		//member variables
		JetSource source;
		double boost, sphericity, aplanarity, esvC, esvD, lambda1, lambda2, lambda3, fwm1, fwm2, fwm3, fwm4, fwm5, fwm6;
};
REGISTER_SELECTOR(EventShape);

//forward declaration
class KMCWeightSelector;
//---------------------------------------------------------------
//class to store and apply RA2 binning
class KRA2BinSelector : public KSelector {
	public:
		//constructor
		KRA2BinSelector() : KSelector() { }
		KRA2BinSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), RA2Exclusive(true), DoBTagSF(false), debug(0), MCWeight(0) { 
			//assemble member vars from user input
			localOpt->Get("RA2VarNames",RA2VarNames);
			
			for(unsigned q = 0; q < RA2VarNames.size(); ++q){
				stringstream pre;
				pre << "RA2Var" << q;
				
				vector<float> min, max;
				localOpt->Get(pre.str()+"Min",min);
				localOpt->Get(pre.str()+"Max",max);
				if(min.size() != max.size()){
					cout << "Input error: vector length mismatches in " << pre.str() << " min and max specification. RA2 binning will not be computed." << endl;
					depfailed = true;
					return;
				}
				
				vector<unsigned> bins;
				localOpt->Get(pre.str()+"Bins",bins);
				if(q>0 && bins.size()!=all_bins[0].size()){
					cout << "Input error: vector length mismatches in " << pre.str() << " bins specification. RA2 binning will not be computed." << endl;
					depfailed = true;
					return;	
				}
				
				//store member vars
				RA2VarMin.push_back(min);
				RA2VarMax.push_back(max);
				all_bins.push_back(bins);
			}
			
			//create map of RA2 bin IDs to bin numbers
			for(unsigned b = 0; b < all_bins[0].size(); ++b){
				vector<unsigned> bin_id;
				bin_id.reserve(all_bins.size());
				for(unsigned q = 0; q < all_bins.size(); ++q){
					bin_id.push_back(all_bins[q][b]);
				}
				IDtoBinNumber[bin_id] = b+1; //bin numbers start at 1
			}
			
			//check other options
			RA2Exclusive = localOpt->Get("RA2Exclusive",true);
			localOpt->Get("RA2debug",debug);
			if(forceadd and not depfailed) canfail = false;
		}
		void CheckLabels(){
			//check if (shared) bin labels have already been created
			if(sel->GetGlobalOpt()->Has("RA2bin_labels")){
				sel->GetGlobalOpt()->Get("RA2bin_labels",labels);
			}
			else {
				//create map of bin labels (in case they are requested)
				labels.reserve(all_bins[0].size());
				string prefix = "";
				localOpt->Get("RA2prefix",prefix);
				bool suffix = localOpt->Get("RA2suffix",true);
				for(unsigned b = 0; b < all_bins[0].size(); ++b){
					stringstream label;
					if(prefix.size()>0) {
						label << prefix;
						if(suffix) label << "_";
					}
					for(unsigned q = 0; q < all_bins.size(); ++q){
						if(!suffix) continue;
						label << RA2VarNames[q] << all_bins[q][b]; //the specific bin label: currently just the bin number
						if(q<all_bins.size()-1) label << "_";
					}
					labels.push_back(label.str());
				}
				
				//store labels in global options
				sel->GetGlobalOpt()->Set<vector<string> >("RA2bin_labels",labels);
			}
		}
		//implemented in KBuilderSelectors or KSkimmerSelectors to avoid dependencies
		virtual void CheckDeps();
		virtual void CheckBase();
		virtual void CheckBranches(){
			for(unsigned q = 0; q < RA2VarNames.size(); ++q){
				looper->fChain->SetBranchStatus(RA2VarNames[q].c_str(),1);
			}
		}
		virtual void SetBranches(){
			if(!tree) return;

			//default values
			RA2binBranch = 0;
			tree->Branch("RA2bin",&RA2binBranch,"RA2binBranch/i");
			//only for mc
			if(base->IsMC()) tree->Branch("RA2bins","std::vector<unsigned>",&RA2binsBranch,32000,0);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(depfailed) return false;
			
			//mode to add tree branches
			if(forceadd){
				//reset vars
				RA2binBranch = 0;
				
				DoBTagSF = false;
				vector<vector<unsigned>> RA2binVectmp;
				vector<unsigned> RA2binstmp = GetBinNumbers(RA2binVectmp);
				if(RA2binstmp.size()>0) RA2binBranch = RA2binstmp[0];
				
				//check all separate btag bins for SFs
				if(base->IsMC()){
					DoBTagSF = true;
					RA2binsBranch = GetBinNumbers(RA2binVectmp);
				}
				
				//passthrough
				return true;
			}
			else {
				RA2bins = GetBinNumbers(RA2binVec);
			}
			
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
			else if(RA2VarNames[qty]=="GenMHT") val.push_back(looper->GenMHT);
			else if(RA2VarNames[qty]=="GenHT") val.push_back(looper->GenHT);
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
		int debug;
		vector<vector<unsigned> > all_bins;
		vector<unsigned> RA2bins;
		vector<vector<unsigned> > RA2binVec;
		map<vector<unsigned>, unsigned> IDtoBinNumber;
		vector<string> RA2VarNames;
		vector<vector<float> > RA2VarMin, RA2VarMax;
		vector<string> labels;
		KMCWeightSelector* MCWeight;
		unsigned RA2binBranch;
		vector<unsigned> RA2binsBranch;
};
REGISTER_SELECTOR(RA2Bin);

#endif
