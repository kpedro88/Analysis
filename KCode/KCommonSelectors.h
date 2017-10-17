#ifndef KCOMMONSELECTORS_H
#define KCOMMONSELECTORS_H

//custom headers
#include "KSelection.h"

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
//selects events based on MET value
class KMETSelector : public KSelector {
	public:
		//constructor
		KMETSelector() : KSelector() { }
		KMETSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), METmin(100) { 
			//check for option
			localOpt->Get("METmin",METmin);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MET",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->MET > METmin;
		}
		
		//member variables
		double METmin;
};
REGISTER_SELECTOR(MET);

//----------------------------------------------------
//selects events based on MET/MT ratio
class KMETMTRatioSelector : public KSelector {
	public:
		//constructor
		KMETMTRatioSelector() : KSelector() { }
		KMETMTRatioSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), cut(0.15) { 
			//check for option
			localOpt->Get("cut",cut);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MET",1);
			looper->fChain->SetBranchStatus("MT",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->MET/looper->MT > cut;
		}
		
		//member variables
		double cut;
};
REGISTER_SELECTOR(METMTRatio);

//----------------------------------------------------
//selects events based on leading jet pT range
class KPTRangeSelector : public KSelector {
	public:
		//constructor
		KPTRangeSelector() : KSelector() { }
		KPTRangeSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), ptmin(1000), ptmax(1200) { 
			//check for option
			localOpt->Get("ptmin",ptmin);
			localOpt->Get("ptmax",ptmax);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("GenJetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->GenJetsAK8->empty()) return false;
			double leadjetpt = looper->GenJetsAK8->at(0).Pt();
			return ptmin < leadjetpt and leadjetpt <= ptmax;
		}
		
		//member variables
		double ptmin, ptmax;
};
REGISTER_SELECTOR(PTRange);

#endif
