#ifndef KCOMMONSELECTORS_H
#define KCOMMONSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KBDTVar.h"
#include "../corrections/EventShapeVariables.c"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

//STL headers
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//----------------------------------------------------
//selects events based on a BDT
class KBDTSelector : public KSelector {
	public:
		//constructor
		KBDTSelector() : KSelector() { }
		KBDTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), wp(-1) {
			//get BDT weights
			localOpt->Get("weights",weights);
			//get BDT type
			localOpt->Get("type",type);
			//get BDT range
			positive = localOpt->Get("positive",true);
			//get working point
			localOpt->Get("wp",wp);
			//check for tagging mode
			tag = localOpt->Get("tag",false);
			if(tag) canfail = false;
			//get variables used
			vector<string> variable_names;
			localOpt->Get("variables",variable_names);
			for(const auto& v: variable_names){
				variables.push_back(KBDTVarFactory::GetFactory().construct(v,v));
			}

			//load TMVA library
			TMVA::Tools::Instance();
			//input for reader
			reader = new TMVA::Reader("Silent");
			for(auto& v : variables){
				v->SetVariable(reader);
			}
			//setup reader
			reader->BookMVA(type.c_str(),weights.c_str());
		}
		virtual void CheckBranches(){
			for(auto& v : variables){
				v->CheckBranches();
			}
		}
		virtual void CheckBase(){
			//propagate to vars
			for(auto& v : variables){
				v->SetBase(base);
			}
		}
		virtual void SetBranches(){
			if(!tree) return;

			tree->Branch("JetsAK8_SVJtag","std::vector<double>",&JetsAK8_bdt,32000,0);
		}

		//this selector doesn't add anything to tree

		//used for non-dummy selectors
		virtual bool Cut() {
			JetsAK8_bdt.clear();
			unsigned njets = min(looper->JetsAK8->size(),3ul);
			bool good = true;
			for(unsigned j = 0; j < looper->JetsAK8->size(); ++j){
				//load variables for this jet
				for(auto& v : variables){
					v->Fill(j);
				}
				double bdt_val = reader->EvaluateMVA(type.c_str());
				//convert range from [-1,1] to [0,1]: (x-xmin)/(xmax-xmin)
				if(positive) bdt_val = (bdt_val + 1)*0.5;
				JetsAK8_bdt.push_back(bdt_val);
				if(JetsAK8_bdt[j]<wp) good &= false;
			}
			return (tag or good);
		}

		//member variables
		string weights, type;
		double wp;
		bool tag, positive;
		TMVA::Reader* reader;
		//input variables
		vector<KBDTVar*> variables;
		//bdt output
		vector<double> JetsAK8_bdt;
};
REGISTER_SELECTOR(BDT);

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
			simple = localOpt->Get("simple",false);
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
//selects events based on dijet sum pt value
class KDijetPTSelector : public KSelector {
	public:
		//constructor
		KDijetPTSelector() : KSelector() { }
		KDijetPTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(500) { 
			//check for option
			localOpt->Get("min",min);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->JetsAK8->size()<2) return false;
			return (looper->JetsAK8->at(0).Pt()+looper->JetsAK8->at(1).Pt()) > min;
		}
		
		//member variables
		double min;
};
REGISTER_SELECTOR(DijetPT);

//----------------------------------------------------
//selects events based on lead jet pt value
class KLeadJetPTSelector : public KSelector {
	public:
		//constructor
		KLeadJetPTSelector() : KSelector() { }
		KLeadJetPTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(500) { 
			//check for option
			localOpt->Get("min",min);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return (looper->JetsAK8->at(0).Pt()) > min;
		}
		
		//member variables
		double min;
};
REGISTER_SELECTOR(LeadJetPT);

//----------------------------------------------------
//selects events based on MHT value
class KMHTSelector : public KSelector {
	public:
		//constructor
		KMHTSelector() : KSelector() { }
		KMHTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(200), max(-1) { 
			//check for options
			localOpt->Get("min",min);
			localOpt->Get("max",max);
			doGen = localOpt->Get("gen",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MHT",1);
			if(doGen) looper->fChain->SetBranchStatus("GenMHT",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			double mht = doGen ? looper->GenMHT : looper->MHT;
			return ( (min<0 or mht>min) and (max<0 or mht<max) );
		}
		
		//member variables
		double min, max;
		bool doGen;
};
REGISTER_SELECTOR(MHT);

//----------------------------------------------------
//selects events based on a diagonal cut along the HT/MHT plane
class KMHTHTRatioSelector : public KSelector {
	public:
		//constructor
		KMHTHTRatioSelector() : KSelector() { }
		KMHTHTRatioSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), debug(0) { 
			//check for option
			localOpt->Get("debug",debug);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("HT",1);
			looper->fChain->SetBranchStatus("MHT",1);
		}

		//this selector doesn't add anything to tree

		//used for non-dummy selectors
		virtual bool Cut() {
			ratio = looper->MHT/looper->HT;
			if (debug) cout << "HT=" << looper->HT << " MHT=" << looper->MHT << " Ratio=" << ratio << endl;
			return ratio <= 1.0;
		}

		//member variables;
		bool debug;
		double ratio;
};
REGISTER_SELECTOR(MHTHTRatio);

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

//-------------------------------------------------------------------
//selects events based on EvtNum (possibly in a window)
class KEvtNumSelector : public KSelector {
	public:
		//constructor
		KEvtNumSelector() : KSelector() { }
		KEvtNumSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(0), max(0) { 
			//check for option
			hasmin = localOpt->Get("min",min);
			hasmax = localOpt->Get("max",max);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MT_AK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return ( (!hasmin or looper->EvtNum>min) and (!hasmax or looper->EvtNum<max) );
		}
		
		//member variables
		bool hasmin, hasmax;
		unsigned min, max;
};
REGISTER_SELECTOR(EvtNum);

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
//selects events based on HT/MT value
class KHTMTRatioSelector : public KSelector {
	public:
		//constructor
		KHTMTRatioSelector() : KSelector() { }
		KHTMTRatioSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), max(0.7) { 
			//check for option
			localOpt->Get("max",max);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("HT",1);
			looper->fChain->SetBranchStatus("MT_AK8",1);
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->JetsAK8->size()<2) return false;
			double htMTratio = looper->MT_AK8 > 0 ? looper->HT/looper->MT_AK8 : 0.0;
			return htMTratio < max;
		}
		
		//member variables
		double max;
};
REGISTER_SELECTOR(HTMTRatio);

//----------------------------------------------------
//selects events based on PT/MT value
class KPTMTRatioSelector : public KSelector {
	public:
		//constructor
		KPTMTRatioSelector() : KSelector() { }
		KPTMTRatioSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), max(0.7) { 
			//check for option
			localOpt->Get("max",max);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MT_AK8",1);
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->JetsAK8->size()<2) return false;
			double ptMTratio = looper->MT_AK8 > 0 ? (looper->JetsAK8->at(0).Pt()+looper->JetsAK8->at(1).Pt())/looper->MT_AK8 : 0.0;
			return ptMTratio < max;
		}
		
		//member variables
		double max;
};
REGISTER_SELECTOR(PTMTRatio);

//----------------------------------------------------
//selects events based on PT1/MT value
class KPT1MTRatioSelector : public KSelector {
	public:
		//constructor
		KPT1MTRatioSelector() : KSelector() { }
		KPT1MTRatioSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), max(0.7) { 
			//check for option
			localOpt->Get("max",max);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MT_AK8",1);
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			double ptMTratio = looper->MT_AK8 > 0 ? looper->JetsAK8->at(0).Pt()/looper->MT_AK8 : 0.0;
			return ptMTratio < max;
		}
		
		//member variables
		double max;
};
REGISTER_SELECTOR(PT1MTRatio);

//----------------------------------------------------
//selects events based on delta eta(j1,j2)
class KDeltaEtaSelector : public KSelector {
	public:
		//constructor
		KDeltaEtaSelector() : KSelector() { }
		KDeltaEtaSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), max(1.1) { 
			//check for option
			localOpt->Get("max",max);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return abs(looper->JetsAK8->at(0).Eta()-looper->JetsAK8->at(1).Eta()) < max;
		}
		
		//member variables
		double max;
};
REGISTER_SELECTOR(DeltaEta);

//----------------------------------------------------
//selects events based on num b or c hadrons in j_i
class KNumBCHadronSelector : public KSelector {
	public:
		//constructor
		KNumBCHadronSelector() : KSelector() { }
		KNumBCHadronSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), index(0), cut(-1) { 
			//check for option
			localOpt->Get("index",index);
			localOpt->Get("cut",cut);
			use_c = localOpt->Get("c",false);
			invert = localOpt->Get("invert",false);
		}
		virtual void CheckBranches(){
			if(use_c) looper->fChain->SetBranchStatus("JetsAK8_NumChadrons",1);
			else looper->fChain->SetBranchStatus("JetsAK8_NumBhadrons",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			auto NumHadrons = use_c ? looper->JetsAK8_NumChadrons : looper-> JetsAK8_NumBhadrons;
			if(NumHadrons->size()<index+1) return true;
			auto num = NumHadrons->at(index);
			if(invert) return num <= cut;
			else return num > cut;
		}
		
		//member variables
		unsigned index;
		int cut;
		bool use_c, invert;
};
REGISTER_SELECTOR(NumBCHadron);

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
//selects events based on dphi(jj,MET) value
class KDeltaPhiJJMETSelector : public KSelector {
	public:
		//constructor
		KDeltaPhiJJMETSelector() : KSelector() { }
		KDeltaPhiJJMETSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), min(2.0) { 
			//check for option
			localOpt->Get("min",min);
			invert = localOpt->Get("invert",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("METPhi",1);
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
            TLorentzVector vjj;
			double dphi = 0.;
            if(looper->JetsAK8->size()>1){
                for(unsigned j = 0; j < 2; ++j){
                    vjj += looper->JetsAK8->at(j);
                }
                dphi = abs(KMath::DeltaPhi(vjj.Phi(),looper->METPhi));
            }
			return ( invert ? dphi <= min : dphi > min );
		}
		
		//member variables
		double min;
		bool invert;
};
REGISTER_SELECTOR(DeltaPhiJJMET);

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
		KRA2BinSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), RA2Exclusive(true), DoBTagSF(false), bqty(-1), debug(0), MCWeight(0) { 
			//assemble member vars from user input
			localOpt->Get("RA2VarNames",RA2VarNames);
			
			for(unsigned q = 0; q < RA2VarNames.size(); ++q){
				if(RA2VarNames[q]=="BTags" or RA2VarNames[q]=="BTagsDeepCSV") bqty = q;

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
			vector<vector<unsigned>> found_bin_nums;
			FindBin(indices,bins,0,bin_num,found_bins,found_bin_nums);

			if(DoBTagSF and bqty>=0){
				//search for other possible btag bins
				vector<unsigned> extra_bins;
				vector<vector<unsigned>> extra_bin_nums;
				for(unsigned n = 0; n < found_bins.size(); ++n){
					unsigned borig = found_bin_nums[n][bqty];
					for(unsigned b = 0; b < RA2VarMax[bqty].size(); ++b){
						//avoid duplicates
						if(b==borig) continue;
						auto new_bin_num = found_bin_nums[n];
						new_bin_num[bqty] = b;
						auto it = IDtoBinNumber.find(new_bin_num);
						if(it != IDtoBinNumber.end()){
							extra_bins.push_back(it->second);
							extra_bin_nums.push_back(new_bin_num);
						}
					}
				}
				found_bins.insert(found_bins.end(),extra_bins.begin(),extra_bins.end());
				found_bin_nums.insert(found_bin_nums.end(),extra_bin_nums.begin(),extra_bin_nums.end());
			}

			bin_vec = found_bin_nums;
			return found_bins;
		}
		//recursive function to implement variable # of for loops
		//ref: http://stackoverflow.com/questions/9555864/variable-nested-for-loops
		void FindBin(vector<unsigned>& indices, vector<vector<unsigned> >& bins, unsigned pos, vector<unsigned>& bin_num, vector<unsigned>& found_bins, vector<vector<unsigned> >& found_bin_nums){
			for(indices[pos] = 0; indices[pos] < bins[pos].size(); indices[pos]++){
				bin_num[pos] = bins[pos][indices[pos]];
				if(pos == indices.size()-1){
					auto it = IDtoBinNumber.find(bin_num);
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
			else if(RA2VarNames[qty]=="BTags") val.push_back(looper->BTags);
			else if(RA2VarNames[qty]=="BTagsDeepCSV") val.push_back(looper->BTagsDeepCSV);
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

		float GetBinMax(string qty_name, const vector<unsigned>& bin_num){
			for(unsigned q = 0; q < RA2VarNames.size(); ++q){
				if(RA2VarNames[q]==qty_name) return RA2VarMax[q][bin_num[q]];
			}
			return -1;
		}
		
		float GetBinMin(string qty_name, const vector<unsigned>& bin_num){
			for(unsigned q = 0; q < RA2VarNames.size(); ++q){
				if(RA2VarNames[q]==qty_name) return RA2VarMin[q][bin_num[q]];
			}
			return -1;
		}
		
		string GetBtagBinName(){
			if(bqty>=0) return RA2VarNames[bqty];
			else return "";
		}

	public:
		//member variables
		bool RA2Exclusive, DoBTagSF;
		int bqty;
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

//-------------------------------------------------------------
//vetos events where reco muons don't match HLT muons
class KMuonHLTSelector : public KSelector {
	public:
		//constructor
		KMuonHLTSelector() : KSelector() { }
		KMuonHLTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), minDR(0.2) {
			localOpt->Get("minDR",minDR);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("HLTMuonObjects",1);
			looper->fChain->SetBranchStatus("Muons",1);
			looper->fChain->SetBranchStatus("Muons_mediumID",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			const auto& HLTMuons = *looper->HLTMuonObjects;
			if(HLTMuons.size()==0) return false;

			const auto& Muons = *looper->Muons;
			const auto& Muons_mediumID = *looper->Muons_mediumID;
			for(unsigned m = 0; m < Muons.size(); ++m){
				if(Muons_mediumID[m]){
					return HLTMuons[0].DeltaR(Muons[m]) < minDR;
				}
			}
			return false;
		}
		
		//member variables
		double minDR;
};
REGISTER_SELECTOR(MuonHLT);

//-------------------------------------------------------------
//vetos events with muons
class KMuonVetoSelector : public KSelector {
	public:
		//constructor
		KMuonVetoSelector() : KSelector() { }
		KMuonVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("NMuons",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->NMuons==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(MuonVeto);

//-------------------------------------------------------------
//vetos events with electrons
class KElectronVetoSelector : public KSelector {
	public:
		//constructor
		KElectronVetoSelector() : KSelector() { }
		KElectronVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("NElectrons",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->NElectrons==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(ElectronVeto);

//---------------------------------------------------------------
//applies MET filters
class KMETFilterSelector : public KSelector {
	public:
		//constructor
		KMETFilterSelector() : KSelector() { }
		KMETFilterSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
			localOpt->Get("filterfiles",filterfiles);
			for(unsigned f = 0; f < filterfiles.size(); ++f){
				filters.push_back(new EventListFilter(filterfiles[f]));
			}
			onlydata = localOpt->Get("onlydata",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("NVtx",1);
			looper->fChain->SetBranchStatus("eeBadScFilter",1);
			looper->fChain->SetBranchStatus("HBHENoiseFilter",1);
			looper->fChain->SetBranchStatus("HBHEIsoNoiseFilter",1);
			looper->fChain->SetBranchStatus("EcalDeadCellTriggerPrimitiveFilter",1);
			looper->fChain->SetBranchStatus("globalSuperTightHalo2016Filter",1);
			looper->fChain->SetBranchStatus("BadChargedCandidateFilter",1);
			looper->fChain->SetBranchStatus("BadPFMuonFilter",1);
			if(filters.size()>0 && filters[0]->Initialized()){
				looper->fChain->SetBranchStatus("RunNum",1);
				looper->fChain->SetBranchStatus("LumiBlockNum",1);
				looper->fChain->SetBranchStatus("EvtNum",1);				
			}
		}
		virtual void CheckBase(){
			//check fastsim stuff
			if(base->GetLocalOpt()->Get("fastsim",false)){
				//disable this for fastsim
				dummy = true;
			}
			if(onlydata and !base->IsData()){
				//disable this for non-data if desired
				dummy = true;
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool TightHaloFilter = looper->globalSuperTightHalo2016Filter==1;
			bool HBHENoiseFilter = looper->HBHENoiseFilter==1;
			bool HBHEIsoNoiseFilter = looper->HBHEIsoNoiseFilter==1;
			bool EcalDeadCellTriggerPrimitiveFilter = looper->EcalDeadCellTriggerPrimitiveFilter==1;
			bool eeBadScFilter = looper->eeBadScFilter==1;
			bool BadChargedCandidateFilter = looper->BadChargedCandidateFilter;
			bool BadPFMuonFilter = looper->BadPFMuonFilter;
			bool VtxFilter = looper->NVtx > 0;
			bool otherFilters = true;
			for(unsigned f = 0; f < filters.size(); ++f){
				otherFilters &= filters[f]->CheckEvent(looper->RunNum,looper->LumiBlockNum,looper->EvtNum);
			}
			return VtxFilter && eeBadScFilter && HBHENoiseFilter && HBHEIsoNoiseFilter && TightHaloFilter 
				&& EcalDeadCellTriggerPrimitiveFilter && BadChargedCandidateFilter && BadPFMuonFilter && otherFilters;
		}
		
		//member variables
		bool onlydata;
		vector<string> filterfiles;
		vector<EventListFilter*> filters;
};
REGISTER_SELECTOR(METFilter);

//---------------------------------------------------------------
//applies extra filters
class KExtraFilterSelector : public KSelector {
	public:
		//constructor
		KExtraFilterSelector() : KSelector() { }
		KExtraFilterSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), fastsim(false), data(false) {
			//check option
			onlydata = localOpt->Get("onlydata",false);
			ecalnoise = localOpt->Get("ecalnoise",false);
			hem = localOpt->Get("hem",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("METRatioFilter",1);
			looper->fChain->SetBranchStatus("MuonJetFilter",1);
			if(ecalnoise) looper->fChain->SetBranchStatus("EcalNoiseJetFilter",1);
			looper->fChain->SetBranchStatus("HTRatioDPhiTightFilter",1);
			looper->fChain->SetBranchStatus("LowNeutralJetFilter",1);
			looper->fChain->SetBranchStatus("FakeJetFilter",1);
			if(hem) looper->fChain->SetBranchStatus("HEMDPhiVetoFilter",1);
		}
		virtual void CheckBase(){
			//check fastsim stuff
			fastsim = base->GetLocalOpt()->Get("fastsim",false);
			data = base->IsData();
			if(onlydata and !data){
				//disable this for non-data if desired
				dummy = true;
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool METRatioFilter = looper->METRatioFilter;
			bool MuonJetFilter = looper->MuonJetFilter;
			bool EcalNoiseJetFilter = (!ecalnoise) or looper->EcalNoiseJetFilter;
			bool HTRatioDPhiTightFilter = looper->HTRatioDPhiTightFilter;
			bool LowNeutralJetFilter = looper->LowNeutralJetFilter;
			bool FakeJetFilter = (!fastsim) or looper->FakeJetFilter;
			bool HEMDPhiVetoFilter = (!hem) or looper->HEMDPhiVetoFilter;

			return METRatioFilter and MuonJetFilter and EcalNoiseJetFilter and HTRatioDPhiTightFilter and LowNeutralJetFilter and FakeJetFilter and HEMDPhiVetoFilter;
		}
		
		//member variables
		bool onlydata, ecalnoise, hem;
		bool fastsim, data;
};
REGISTER_SELECTOR(ExtraFilter);

//------------------------------------------------------
//selects events based on run number (for blinding data)
class KBlindSelector : public KSelector {
	public:
		//constructor
		KBlindSelector() : KSelector() { }
		KBlindSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), firstUnblindRun(-1), lastUnblindRun(-1) { 
			//check for option
			localOpt->Get("firstUnblindRun",firstUnblindRun);
			localOpt->Get("lastUnblindRun",lastUnblindRun);
			localOpt->Get("intervalUnblindRuns",intervalUnblindRuns);
			if(intervalUnblindRuns.size()%2!=0){
				cout << "Input error: intervalUnblindRuns must have an even number of entries. This input will be ignored." << endl;
				intervalUnblindRuns.clear();
			}
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("RunNum",1);
		}
		virtual void CheckBase(){
			//disable this for non-data
			if(!base->IsData()) dummy = true;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(intervalUnblindRuns.size()>0){
				//check each run pair
				for(unsigned r = 0; r < intervalUnblindRuns.size(); r+=2){
					if(looper->RunNum >= intervalUnblindRuns[r] and looper->RunNum <= intervalUnblindRuns[r+1]) return true;
				}
				return false;
			}
			return (firstUnblindRun==-1 or looper->RunNum >= firstUnblindRun) and (lastUnblindRun==-1 or looper->RunNum <= lastUnblindRun);
		}
		
		//member variables
		int firstUnblindRun, lastUnblindRun;
		vector<int> intervalUnblindRuns;
};
REGISTER_SELECTOR(Blind);

//---------------------------------------------------------------
//compares ecal noise filters
class KEcalNoiseComparisonSelector : public KSelector {
	public:
		//constructor
		KEcalNoiseComparisonSelector() : KSelector() { }
		KEcalNoiseComparisonSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
			//check option
			localOpt->Get("filterfile",filterfile);
			filter = new EventListFilter(filterfile);
			counters = {0,0,0};
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("EcalNoiseJetFilter",1);
			looper->fChain->SetBranchStatus("RunNum",1);
			looper->fChain->SetBranchStatus("LumiBlockNum",1);
			looper->fChain->SetBranchStatus("EvtNum",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool fails1 = !looper->EcalNoiseJetFilter;
			bool fails2 = !filter->CheckEvent(looper->RunNum,looper->LumiBlockNum,looper->EvtNum);
			bool fails_both = fails1 and fails2;
			bool fails_only_1 = fails1 and not fails2;
			bool fails_only_2 = fails2 and not fails1;

			counters[0] += fails_only_1;
			counters[1] += fails_only_2;
			counters[2] += fails_both;
			
			return true;
		}

		virtual void Finalize(TFile* file){
			cout << "Fail      EcalNoiseJetFilter: " << counters[0] << "\n";
			cout << "Fail ecalBadCalib event list: " << counters[1] << "\n";
			cout << "Fail                    both: " << counters[2] << "\n";
		}
		
		//member variables
		string filterfile;
		EventListFilter* filter;
		array<int,3> counters;
};
REGISTER_SELECTOR(EcalNoiseComparison);

//-------------------------------------------------------------
//base class for filters (w/ tag mode to add branches)
class KFilterSelector : public KSelector {
	public:
		//constructor
		KFilterSelector() : KSelector() { }
		KFilterSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), result(false), disable(false) {
			//check for option
			tag = localOpt->Get("tag",false);
			if(tag) {
				canfail = false;
				forceadd = true;
			}
		}
		virtual void SetBranches(){
			if(!tree) return;
			if(tag) tree->Branch((branchname+"Filter").c_str(),&result,(branchname+"Filter/O").c_str());
		}
		virtual void GetResult() { }
		virtual bool Cut(){
			if(disable) result = true;
			else GetResult();

			return (tag or result);
		}

		//members
		bool tag;
		bool result;
		bool disable;
		string branchname;
};

//-------------------------------------------------------------
//vetos events with bad jets (using PFJetID loose WP)
class KJetIDSelector : public KFilterSelector {
	public:
		//constructor
		KJetIDSelector() : KFilterSelector() { }
		KJetIDSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_) {
			branchname = "JetID";
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("JetID",1);
		}
		virtual void CheckBase(){
			//disable JetID for fastsim
			if(base->GetLocalOpt()->Get("fastsim",false)) {
				disable = true;
			}
		}
		virtual void GetResult() {
			result = looper->JetID;
		}
};
REGISTER_SELECTOR(JetID);

//-------------------------------------------------------------
//vetos events with bad PFMET/CaloMET ratio
class KMETRatioSelector : public KFilterSelector {
	public:
		//constructor
		KMETRatioSelector() : KFilterSelector() { }
		KMETRatioSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_) {
			branchname = "METRatio";
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("PFCaloMETRatio",1);
			looper->fChain->SetBranchStatus("MET",1);
			looper->fChain->SetBranchStatus("CaloMET",1);
		}
		virtual void GetResult() {
			double ratio = looper->PFCaloMETRatio;
			//negative value denotes buggy AK8 jets
			if(ratio<0) ratio = looper->MET/looper->CaloMET;
			result = (ratio < 5.0);
		}
};
REGISTER_SELECTOR(METRatio);

//-------------------------------------------------------------
//vetos events with bad muon jets
class KMuonJetSelector : public KFilterSelector {
	public:
		//constructor
		KMuonJetSelector() : KFilterSelector() { }
		KMuonJetSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_) {
			branchname = "MuonJet";
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("Jets_muonEnergyFraction",1);
			looper->fChain->SetBranchStatus("METPhi",1);
		}
		virtual void GetResult() {
			bool noMuonJet = true;
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(looper->Jets->at(j).Pt() > 200 && looper->Jets_muonEnergyFraction->at(j) > 0.5 && abs(KMath::DeltaPhi(looper->Jets->at(j).Phi(),looper->METPhi)) > (TMath::Pi() - 0.4)){
					noMuonJet = false;
					break;
				}
			}
			result = noMuonJet;
		}
};
REGISTER_SELECTOR(MuonJet);

//-------------------------------------------------------------
//vetos events with fake jets (fastsim only)
class KFakeJetSelector : public KFilterSelector {
	public:
		//constructor
		KFakeJetSelector() : KFilterSelector() { }
		KFakeJetSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_) {
			branchname = "FakeJet";
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("GenJets",1);
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("Jets_chargedHadronEnergyFraction",1);
		}
		virtual void CheckBase(){
			//only enable for fastsim
			if(!base->GetLocalOpt()->Get("fastsim",false)) {
				disable = true;
			}
		}
		virtual void GetResult() {
			bool noFakeJet = true;
			//reject events with any jet pt>20, |eta|<2.5 NOT matched to a GenJet (w/in DeltaR<0.3) and chfrac < 0.1
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(looper->Jets->at(j).Pt() <= 20 || abs(looper->Jets->at(j).Eta())>=2.5) continue;
				bool genMatched = false;
				for(unsigned g = 0; g < looper->GenJets->size(); ++g){
					if(looper->GenJets->at(g).DeltaR(looper->Jets->at(j)) < 0.3) {
						genMatched = true;
						break;
					}
				}
				if(!genMatched && looper->Jets_chargedHadronEnergyFraction->at(j) < 0.1){
					noFakeJet = false;
					break;
				}
			}
			result = noFakeJet;
		}
};
REGISTER_SELECTOR(FakeJet);

//-------------------------------------------------------------
//vetos events with ecal noise jet
class KEcalNoiseJetSelector : public KFilterSelector {
	public:
		//constructor
		KEcalNoiseJetSelector() : KFilterSelector() { }
		KEcalNoiseJetSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_) {
			branchname = "EcalNoiseJet";
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("MHTPhi",1);
		}
		virtual void GetResult() {
			int counter = 0;
			bool goodJet[2] = {true,true};
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(counter>=2) break;
				const auto& Jet = looper->Jets->at(j);
				//MHT mask doesn't work for 2017 because of EE "fix", so check manually
				if(Jet.Pt()>30 and abs(Jet.Eta())<5.0 and abs(Jet.Eta())>2.4){
					double dphi = abs(KMath::DeltaPhi(Jet.Phi(),looper->MHTPhi));
					if(Jet.Pt()>250 and (dphi > 2.6 or dphi < 0.1)) goodJet[counter] = false;
					++counter;
				}
			}
			result = (goodJet[0] and goodJet[1]);
		}
};
REGISTER_SELECTOR(EcalNoiseJet);

//-------------------------------------------------------------
//vetos events with leading jet having low neutral em
class KLowNeutralJetSelector : public KFilterSelector {
	public:
		//constructor
		KLowNeutralJetSelector() : KFilterSelector() { }
		KLowNeutralJetSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_) {
			tight = localOpt->Get("tight",false);
			if(tight) branchname = "LowNeutralJetTight";
			else branchname = "LowNeutralJet";
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("DeltaPhi1",1);
			looper->fChain->SetBranchStatus("Jets_neutralEmEnergyFraction",1);
		}
		virtual void GetResult() {
			if(looper->Jets_neutralEmEnergyFraction->empty()) {
				result = true;
				return;
			}
			result = !(looper->Jets_neutralEmEnergyFraction->at(0)<(tight ? 0.05 : 0.03) and abs(looper->DeltaPhi1)>(TMath::Pi()-0.4));
		}

		//member variables
		bool tight;
};
REGISTER_SELECTOR(LowNeutralJet);

//----------------------------------------------------
//selects events based on a cut along the HT/DeltaPhi plane
class KHTRatioSelector : public KFilterSelector {
	public:
		//constructor
		KHTRatioSelector() : KFilterSelector() { }
		KHTRatioSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_) { 
			//check for option
            doHTDPhiCut = localOpt->Get("HTDPhi",false);
            tight = localOpt->Get("tight",false);
			if(doHTDPhiCut) {
				if(tight) branchname = "HTRatioDPhiTight";
				else branchname = "HTRatioDPhi";
			}
			else {
				if(tight) branchname = "HTRatioTight";
				else branchname = "HTRatio";
			}
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("HT",1);
			looper->fChain->SetBranchStatus("HT5",1);
			looper->fChain->SetBranchStatus("DeltaPhi1",1);
		}
		virtual void GetResult() {
            //line slope from: https://indico.cern.ch/event/769759/contributions/3198262/attachments/1744238/2823253/HT5_noisyForwardJets.pdf
            //x1 = 1.5 y1 = 0.95
            //x2 = 3.5 y2 = 3.0
            //slope = (3-0.95)/(3.5-1.5) = 1.025
            //y = mx + b == > 0.95 = 1.025 *1.5 + b == > b = -0.5875
			double htratio = looper->HT5/looper->HT;
			//keep any event with ht5/ht < 1.2
            if(doHTDPhiCut) result = (htratio < 1.2 ? true : (looper->DeltaPhi1 >= (tight ? 5.3*htratio - 4.78 : 1.025*htratio - 0.5875) ) );
            else result = (htratio <= (tight ? 1.3 : 2.0));
		}

		//member variables
		bool doHTDPhiCut, tight;
};
REGISTER_SELECTOR(HTRatio);

//-------------------------------------------------------------
//vetos events with activity in HEM region
class KHEMVetoSelector : public KFilterSelector {
	public:
		//constructor
		KHEMVetoSelector() : KFilterSelector() { }
		KHEMVetoSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_), minpt(30) {
			branchname = "HEMVeto";
			localOpt->Get("minpt",minpt);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Electrons",1);
			looper->fChain->SetBranchStatus("Electrons_passIso",1);
			looper->fChain->SetBranchStatus("Jets",1);
		}
		bool InHEMRegion(const TLorentzVector& tlv, double ptcut){
			return tlv.Pt()>ptcut and -3 < tlv.Eta() and tlv.Eta() < -1.4 and -1.57 < tlv.Phi() and tlv.Phi() < -0.87;
		}
		virtual void GetResult() {
			bool activity = false;
			//check electrons - only isolated ones
			for(unsigned e = 0; e < looper->Electrons->size(); ++e){
				if(!looper->Electrons_passIso->at(e)) continue;
				const auto& Electron = looper->Electrons->at(e);
				if(!activity and InHEMRegion(Electron,0.)) activity = true;
				if(activity) break;
			}
			//check jets
			for(const auto& Jet : *looper->Jets){
				if(!activity and InHEMRegion(Jet,minpt)) activity = true;
				if(activity) break;
			}
			result = !activity;
		}

		//member variables
		double minpt;
};
REGISTER_SELECTOR(HEMVeto);

//-------------------------------------------------------------
//vetos events with activity in wider HEM region (w/ dphi req.)
class KHEMDPhiVetoSelector : public KFilterSelector {
	public:
		//constructor
		KHEMDPhiVetoSelector() : KFilterSelector() { }
		KHEMDPhiVetoSelector(string name_, OptionMap* localOpt_) : KFilterSelector(name_,localOpt_), minpt(30) {
			branchname = "HEMDPhiVeto";
			localOpt->Get("minpt",minpt);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Electrons",1);
			looper->fChain->SetBranchStatus("Electrons_passIso",1);
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("MHTPhi",1);
		}
		bool InHEMRegion(const TLorentzVector& tlv, double ptcut){
			return tlv.Pt()>ptcut and -3 < tlv.Eta() and tlv.Eta() < -1.4 and -1.57 < tlv.Phi() and tlv.Phi() < -0.87;
		}
		bool InWideHEMRegion(const TLorentzVector& tlv, double ptcut, double MHTPhi){
			return tlv.Pt()>ptcut and -3.2 < tlv.Eta() and tlv.Eta() < -1.2 and -1.77 < tlv.Phi() and tlv.Phi() < -0.67 and KMath::DeltaPhi(tlv.Phi(),MHTPhi)<0.5;
		}
		virtual void GetResult() {
			bool activity = false;
			//check electrons - only isolated ones
			for(unsigned e = 0; e < looper->Electrons->size(); ++e){
				if(!looper->Electrons_passIso->at(e)) continue;
				const auto& Electron = looper->Electrons->at(e);
				if(!activity and InHEMRegion(Electron,0.)) activity = true;
				if(activity) break;
			}
			//check jets
			for(const auto& Jet : *looper->Jets){
				if(!activity and InWideHEMRegion(Jet,minpt,looper->MHTPhi)) activity = true;
				if(activity) break;
			}
			result = !activity;
		}

		//member variables
		double minpt;
};
REGISTER_SELECTOR(HEMDPhiVeto);

//---------------------------------------------------------
//selects events based on DeltaPhi values (jets w/ |eta|<5)
class KDeltaPhiExtendedSelector : public KSelector {
	public:
		//constructor
		KDeltaPhiExtendedSelector() : KSelector() { }
		KDeltaPhiExtendedSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), DeltaPhi(4,0), invert(false) { 
			//check for option
			DeltaPhi[0] = 0.3; DeltaPhi[1] = 0.3; DeltaPhi[2] = 0.3; DeltaPhi[3] = 0.3;
			localOpt->Get("DeltaPhi",DeltaPhi);
			invert = localOpt->Get("invert",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("MHTPhi",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			vector<double> dphi(4,10.0);
			int counter = 0;
			for(const auto& Jet : *looper->Jets){
				if(Jet.Pt()>30 and abs(Jet.Eta())<5.0){
					dphi[counter] = abs(KMath::DeltaPhi(Jet.Phi(),looper->MHTPhi));
					++counter;
				}
				if(counter>=4) break;
			}
			if(invert) return dphi[0] < DeltaPhi[0] || dphi[1] < DeltaPhi[1] || dphi[2] < DeltaPhi[2] || dphi[3] < DeltaPhi[3];
			else return dphi[0] > DeltaPhi[0] && dphi[1] > DeltaPhi[1] && dphi[2] > DeltaPhi[2] && dphi[3] > DeltaPhi[3];
		}
		
		//member variables
		vector<double> DeltaPhi;
		bool invert;
};
REGISTER_SELECTOR(DeltaPhiExtended);

//---------------------------------------------------------------
//associate ak4 jets w/ ak8 jets
class KJetMatchSelector : public KSelector {
	public:
		//constructor
		KJetMatchSelector() : KSelector() { }
		KJetMatchSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(2), mindr(0.8) {
			canfail = false;
			//check options
			localOpt->Get("njet",njet);
			localOpt->Get("mindr",mindr);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("JetsAK8",1);
			looper->fChain->SetBranchStatus("Jets",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			unsigned njet_ = min(njet,(unsigned)looper->JetsAK8->size());
			//clear
			JetIndices.clear(); JetIndices.resize(njet_);

			//compare each AK4 jet to all AK8 jets, take the closest (if < mindr)
			const auto& Jets = *looper->Jets;
			const auto& JetsAK8 = *looper->JetsAK8;
			for(unsigned j = 0; j < Jets.size(); ++j){
				double min = 1e10;
				unsigned min_ind = njet_+1;
				for(unsigned jj = 0; jj < njet_; ++jj){
					double dr = Jets[j].DeltaR(JetsAK8[jj]);
					if(dr < min) {
						min = dr;
						min_ind = jj;
					}
				}
				if(min_ind < njet_ and min < mindr) JetIndices[min_ind].push_back(j);
			}

			return true;
		}
		
		//member variables
		unsigned njet;
		double mindr;
		vector<vector<unsigned>> JetIndices;
};
REGISTER_SELECTOR(JetMatch);

#endif
