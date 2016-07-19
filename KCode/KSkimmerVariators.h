#ifndef KSKIMMERVARIATORS_H
#define KSKIMMERVARIATORS_H

//custom headers
#include "KVariation.h"
#include "KSkimmer.h"
#include "KMath.h"

//ROOT headers
#include <TLorentzVector.h>
#include <TMath.h>

//STL headers
#include <string>
#include <vector>

using namespace std;

//base class for variators is in KVariation.h

//----------------------------------------------------
//variation of jet-related variables
//for: JECs, JERs, Zinv cleaning
class KJetVariator : public KVariator<KSkimmer> {
	public:
		enum vartypes { NoVar=0, JECup=1, JECdown=2, JERup=3, JERdown=4, clean=5 };
		//constructor
		KJetVariator() : KVariator<KSkimmer>() { }
		KJetVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_), enabled(false), vtype(NoVar),
			b_Jets(true),b_HTJetsMask(true),b_MHTJetsMask(true),b_Jets_ID(true),b_Jets_partonFlavor(true),b_Jets_hadronFlavor(true),b_Jets_bDiscriminatorCSV(true),b_Jets_bDiscriminatorMVA(true)
		{
			//check options
			string vname = "";
			localOpt->Get("vartype",vname);
			if(vname=="JECup") vtype = JECup;
			else if(vname=="JECdown") vtype = JECdown;
			else if(vname=="JERup") vtype = JERup;
			else if(vname=="JERdown") vtype = JERdown;
			else if(vname=="clean") vtype = clean;
		}
		//helper functions
		virtual void EnableBranches(string* branches, unsigned size){
			if(enabled) return;
			for(unsigned i = 0; i < size; ++i){
				looper->fChain->SetBranchStatus(branches[i].c_str(),1);
			}
			enabled = true;
		}
		virtual void CheckBranches(){
			if(vtype==JECup){
				string branchlist[] = {"JetsJECup","HTJetsMaskJECup","MHTJetsMaskJECup","ISRJetsMaskJECup","JetLeptonMaskJECup","JetsJECup_ID","JetsJECup_partonFlavor","JetsJECup_hadronFlavor","JetsJECup_bDiscriminatorCSV","JetsJECup_bDiscriminatorMVA","JetIDJECup","NJetsJECup","NJetsISRJECup","BTagsJECup","BTagsMVAJECup","HTJECup","MHTJECup","MHTPhiJECup","DeltaPhi1JECup","DeltaPhi2JECup","DeltaPhi3JECup","DeltaPhi4JECup"};
				unsigned branchsize = 19;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==JECdown){
				string branchlist[] = {"JetsJECdown","HTJetsMaskJECdown","MHTJetsMaskJECdown","ISRJetsMaskJECdown","JetLeptonMaskJECdown","JetsJECdown_ID","JetsJECdown_partonFlavor","JetsJECdown_hadronFlavor","JetsJECdown_bDiscriminatorCSV","JetsJECdown_bDiscriminatorMVA","JetIDJECdown","NJetsJECdown","NJetsISRJECdown","BTagsJECdown","BTagsMVAJECdown","HTJECdown","MHTJECdown","MHTPhiJECdown","DeltaPhi1JECdown","DeltaPhi2JECdown","DeltaPhi3JECdown","DeltaPhi4JECdown"};
				unsigned branchsize = 19;
				EnableBranches(branchlist,branchsize);
			}
			if(vtype==JERup){
				string branchlist[] = {"JetsJERup","HTJetsMaskJERup","MHTJetsMaskJERup","ISRJetsMaskJERup","JetLeptonMaskJERup","JetsJERup_ID","JetsJERup_partonFlavor","JetsJERup_hadronFlavor","JetsJERup_bDiscriminatorCSV","JetsJERup_bDiscriminatorMVA","JetIDJERup","NJetsJERup","NJetsISRJERup","BTagsJERup","BTagsMVAJERup","HTJERup","MHTJERup","MHTPhiJERup","DeltaPhi1JERup","DeltaPhi2JERup","DeltaPhi3JERup","DeltaPhi4JERup"};
				unsigned branchsize = 19;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==JERdown){
				string branchlist[] = {"JetsJERdown","HTJetsMaskJERdown","MHTJetsMaskJERdown","ISRJetsMaskJERdown","JetLeptonMaskJERdown","JetsJERdown_ID","JetsJERdown_partonFlavor","JetsJERdown_hadronFlavor","JetsJERdown_bDiscriminatorCSV","JetsJERdown_bDiscriminatorMVA","JetIDJERdown","NJetsJERdown","NJetsISRJERdown","BTagsJERdown","BTagsMVAJERdown","HTJERdown","MHTJERdown","MHTPhiJERdown","DeltaPhi1JERdown","DeltaPhi2JERdown","DeltaPhi3JERdown","DeltaPhi4JERdown"};
				unsigned branchsize = 19;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==clean){
				string branchlist[] = {"Jetsclean","HTJetsMaskclean","MHTJetsMaskclean","ISRJetsMaskclean","JetLeptonMaskclean","Jetsclean_ID","Jetsclean_partonFlavor","Jetsclean_hadronFlavor","Jetsclean_bDiscriminatorCSV","Jetsclean_bDiscriminatorMVA","JetIDclean","NJetsclean","NJetsISRclean","BTagsclean","BTagsMVAclean","HTclean","MHTclean","MHTPhiclean","METclean","METPhiclean","DeltaPhi1clean","DeltaPhi2clean","DeltaPhi3clean","DeltaPhi4clean","isoElectronTracksclean","isoMuonTracksclean","isoPionTracksclean"};
				unsigned branchsize = 21;
				EnableBranches(branchlist,branchsize);
			}
			
			
			//check pointer branches
			if(!looper->fChain->GetBranchStatus("Jets")) b_Jets = false;
			if(!looper->fChain->GetBranchStatus("HTJetsMask")) b_HTJetsMask = false;
			if(!looper->fChain->GetBranchStatus("MHTJetsMask")) b_MHTJetsMask = false;
			if(!looper->fChain->GetBranchStatus("ISRJetsMask")) b_ISRJetsMask = false;
			if(!looper->fChain->GetBranchStatus("JetLeptonMask")) b_JetLeptonMask = false;
			if(!looper->fChain->GetBranchStatus("Jets_ID")) b_Jets_ID = false;
			if(!looper->fChain->GetBranchStatus("Jets_partonFlavor")) b_Jets_partonFlavor = false;
			if(!looper->fChain->GetBranchStatus("Jets_hadronFlavor")) b_Jets_hadronFlavor = false;
			if(!looper->fChain->GetBranchStatus("Jets_bDiscriminatorCSV")) b_Jets_bDiscriminatorCSV = false;
			if(!looper->fChain->GetBranchStatus("Jets_bDiscriminatorMVA")) b_Jets_bDiscriminatorMVA = false;
		}
		//functions
		virtual void DoVariation() {
			//store original values
			if(b_Jets) Jets = *(looper->Jets);
			if(b_HTJetsMask) HTJetsMask = *(looper->HTJetsMask);
			if(b_MHTJetsMask) MHTJetsMask = *(looper->MHTJetsMask);
			if(b_ISRJetsMask) ISRJetsMask = *(looper->ISRJetsMask);
			if(b_JetLeptonMask) JetLeptonMask = *(looper->JetLeptonMask);
			if(b_Jets_ID) Jets_ID = *(looper->Jets_ID);
			if(b_Jets_partonFlavor) Jets_partonFlavor = *(looper->Jets_partonFlavor);
			if(b_Jets_hadronFlavor) Jets_hadronFlavor = *(looper->Jets_hadronFlavor);
			if(b_Jets_bDiscriminatorCSV) Jets_bDiscriminatorCSV = *(looper->Jets_bDiscriminatorCSV);
			if(b_Jets_bDiscriminatorMVA) Jets_bDiscriminatorMVA = *(looper->Jets_bDiscriminatorMVA);
			JetID = looper->JetID;
			NJets = looper->NJets;
			NJetsISR = looper->NJetsISR;
			BTags = looper->BTags;
			BTagsMVA = looper->BTagsMVA;
			isoElectronTracks = looper->isoElectronTracks;
			isoMuonTracks = looper->isoMuonTracks;
			isoPionTracks = looper->isoPionTracks;
			HT = looper->HT;
			MET = looper->MET;
			METPhi = looper->METPhi;
			MHT = looper->MHT;
			MHTPhi = looper->MHTPhi;
			DeltaPhi1 = looper->DeltaPhi1;
			DeltaPhi2 = looper->DeltaPhi2;
			DeltaPhi3 = looper->DeltaPhi3;
			DeltaPhi4 = looper->DeltaPhi4;
			
			//set to clean vars
			if(vtype==JECup){
				if(b_Jets) *(looper->Jets) = *(looper->JetsJECup);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskJECup);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECup);
				if(b_ISRJetsMask) *(looper->ISRJetsMask) = *(looper->ISRJetsMaskJECup);
				if(b_JetLeptonMask) *(looper->JetLeptonMask) = *(looper->JetLeptonMaskJECup);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJECup_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJECup_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJECup_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECup_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->JetsJECup_bDiscriminatorMVA);
				looper->JetID = looper->JetIDJECup;
				looper->NJets = looper->NJetsJECup;
				looper->NJetsISR = looper->NJetsISRJECup;
				looper->BTags = looper->BTagsJECup;
				looper->BTagsMVA = looper->BTagsMVAJECup;
				looper->HT = looper->HTJECup;
				looper->MHT = looper->MHTJECup;
				looper->MHTPhi = looper->MHTPhiJECup;
				looper->DeltaPhi1 = looper->DeltaPhi1JECup;
				looper->DeltaPhi2 = looper->DeltaPhi2JECup;
				looper->DeltaPhi3 = looper->DeltaPhi3JECup;
				looper->DeltaPhi4 = looper->DeltaPhi4JECup;
			}
			else if(vtype==JECdown){
				if(b_Jets) *(looper->Jets) = *(looper->JetsJECdown);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskJECdown);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECdown);
				if(b_ISRJetsMask) *(looper->ISRJetsMask) = *(looper->ISRJetsMaskJECdown);
				if(b_JetLeptonMask) *(looper->JetLeptonMask) = *(looper->JetLeptonMaskJECdown);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJECdown_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJECdown_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJECdown_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECdown_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->JetsJECdown_bDiscriminatorMVA);
				looper->JetID = looper->JetIDJECdown;
				looper->NJets = looper->NJetsJECdown;
				looper->NJetsISR = looper->NJetsISRJECdown;
				looper->BTags = looper->BTagsJECdown;
				looper->BTagsMVA = looper->BTagsMVAJECdown;
				looper->HT = looper->HTJECdown;
				looper->MHT = looper->MHTJECdown;
				looper->MHTPhi = looper->MHTPhiJECdown;
				looper->DeltaPhi1 = looper->DeltaPhi1JECdown;
				looper->DeltaPhi2 = looper->DeltaPhi2JECdown;
				looper->DeltaPhi3 = looper->DeltaPhi3JECdown;
				looper->DeltaPhi4 = looper->DeltaPhi4JECdown;
			}
			else if(vtype==JERup){
				if(b_Jets) *(looper->Jets) = *(looper->JetsJERup);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskJERup);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskJERup);
				if(b_ISRJetsMask) *(looper->ISRJetsMask) = *(looper->ISRJetsMaskJERup);
				if(b_JetLeptonMask) *(looper->JetLeptonMask) = *(looper->JetLeptonMaskJERup);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJERup_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJERup_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJERup_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJERup_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->JetsJERup_bDiscriminatorMVA);
				looper->JetID = looper->JetIDJERup;
				looper->NJets = looper->NJetsJERup;
				looper->NJetsISR = looper->NJetsISRJERup;
				looper->BTags = looper->BTagsJERup;
				looper->BTagsMVA = looper->BTagsMVAJERup;
				looper->HT = looper->HTJERup;
				looper->MHT = looper->MHTJERup;
				looper->MHTPhi = looper->MHTPhiJERup;
				looper->DeltaPhi1 = looper->DeltaPhi1JERup;
				looper->DeltaPhi2 = looper->DeltaPhi2JERup;
				looper->DeltaPhi3 = looper->DeltaPhi3JERup;
				looper->DeltaPhi4 = looper->DeltaPhi4JERup;
			}
			else if(vtype==JERdown){
				if(b_Jets) *(looper->Jets) = *(looper->JetsJERdown);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskJERdown);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskJERdown);
				if(b_ISRJetsMask) *(looper->ISRJetsMask) = *(looper->ISRJetsMaskJERdown);
				if(b_JetLeptonMask) *(looper->JetLeptonMask) = *(looper->JetLeptonMaskJERdown);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJERdown_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJERdown_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJERdown_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJERdown_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->JetsJERdown_bDiscriminatorMVA);
				looper->JetID = looper->JetIDJERdown;
				looper->NJets = looper->NJetsJERdown;
				looper->NJetsISR = looper->NJetsISRJERdown;
				looper->BTags = looper->BTagsJERdown;
				looper->BTagsMVA = looper->BTagsMVAJERdown;
				looper->HT = looper->HTJERdown;
				looper->MHT = looper->MHTJERdown;
				looper->MHTPhi = looper->MHTPhiJERdown;
				looper->DeltaPhi1 = looper->DeltaPhi1JERdown;
				looper->DeltaPhi2 = looper->DeltaPhi2JERdown;
				looper->DeltaPhi3 = looper->DeltaPhi3JERdown;
				looper->DeltaPhi4 = looper->DeltaPhi4JERdown;
			}
			else if(vtype==clean){
				if(b_Jets) *(looper->Jets) = *(looper->Jetsclean);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskclean);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskclean);
				if(b_ISRJetsMask) *(looper->ISRJetsMask) = *(looper->ISRJetsMaskclean);
				if(b_JetLeptonMask) *(looper->JetLeptonMask) = *(looper->JetLeptonMaskclean);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->Jetsclean_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->Jetsclean_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->Jetsclean_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->Jetsclean_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->Jetsclean_bDiscriminatorMVA);
				looper->JetID = looper->JetIDclean;
				looper->NJets = looper->NJetsclean;
				looper->NJetsISR = looper->NJetsISRclean;
				looper->BTags = looper->BTagsclean;
				looper->BTagsMVA = looper->BTagsMVAclean;
				looper->isoElectronTracks = looper->isoElectronTracksclean;
				looper->isoMuonTracks = looper->isoMuonTracksclean;
				looper->isoPionTracks = looper->isoPionTracksclean;
				looper->HT = looper->HTclean;
				looper->MHT = looper->MHTclean;
				looper->MHTPhi = looper->MHTPhiclean;
				looper->MET = looper->METclean;
				looper->METPhi = looper->METPhiclean;
				looper->DeltaPhi1 = looper->DeltaPhi1clean;
				looper->DeltaPhi2 = looper->DeltaPhi2clean;
				looper->DeltaPhi3 = looper->DeltaPhi3clean;
				looper->DeltaPhi4 = looper->DeltaPhi4clean;
			}
		}
		virtual void UndoVariation() {
			//restore original values
			if(b_Jets) *(looper->Jets) = Jets;
			if(b_HTJetsMask) *(looper->HTJetsMask) = HTJetsMask;
			if(b_MHTJetsMask) *(looper->MHTJetsMask) = MHTJetsMask;
			if(b_ISRJetsMask) *(looper->ISRJetsMask) = ISRJetsMask;
			if(b_JetLeptonMask) *(looper->JetLeptonMask) = JetLeptonMask;
			if(b_Jets_ID) *(looper->Jets_ID) = Jets_ID;
			if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = Jets_partonFlavor;
			if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = Jets_hadronFlavor;
			if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = Jets_bDiscriminatorCSV;
			if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = Jets_bDiscriminatorMVA;
			looper->JetID = JetID;
			looper->NJets = NJets;
			looper->NJetsISR = NJetsISR;
			looper->BTags = BTags;
			looper->BTagsMVA = BTagsMVA;
			looper->isoElectronTracks = isoElectronTracks;
			looper->isoMuonTracks = isoMuonTracks;
			looper->isoPionTracks = isoPionTracks;
			looper->HT = HT;
			looper->MET = MET;
			looper->METPhi = METPhi;
			looper->MHT = MHT;
			looper->MHTPhi = MHTPhi;
			looper->DeltaPhi1 = DeltaPhi1;
			looper->DeltaPhi2 = DeltaPhi2;
			looper->DeltaPhi3 = DeltaPhi3;
			looper->DeltaPhi4 = DeltaPhi4;
		}
		
		//member variables
		bool enabled;
		bool b_Jets,b_HTJetsMask,b_MHTJetsMask,b_ISRJetsMask,b_JetLeptonMask,b_Jets_ID,b_Jets_partonFlavor,b_Jets_hadronFlavor,b_Jets_bDiscriminatorCSV,b_Jets_bDiscriminatorMVA;
		vartypes vtype;
		vector<TLorentzVector> Jets;
		vector<bool> HTJetsMask;
		vector<bool> MHTJetsMask;
		vector<bool> ISRJetsMask;
		vector<bool> JetLeptonMask;
		vector<bool> Jets_ID;
		vector<int> Jets_hadronFlavor;
		vector<int> Jets_partonFlavor;
		vector<double> Jets_bDiscriminatorCSV;
		vector<double> Jets_bDiscriminatorMVA;
		Bool_t          JetID;
		Int_t           NJets;
		Int_t           NJetsISR;
		Int_t           BTags;
		Int_t           BTagsMVA;
		Int_t           isoElectronTracks;
		Int_t           isoMuonTracks;
		Int_t           isoPionTracks;
		Double_t        HT;
		Double_t        MET;
		Double_t        METPhi;
		Double_t        MHT;
		Double_t        MHTPhi;
		Double_t        DeltaPhi1;
		Double_t        DeltaPhi2;
		Double_t        DeltaPhi3;
		Double_t        DeltaPhi4;
};

class KGenMHTVariator : public KVariator<KSkimmer> {
	public:
		//constructor
		KGenMHTVariator() : KVariator<KSkimmer>() { }
		KGenMHTVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_) { }
		
		virtual void DoVariation() {
			//store original values
			MHT = looper->MHT;
			HT = looper->HT;
			
			//set to gen vars
			looper->MHT = looper->GenMHT;
			looper->HT = looper->GenHT;
			
		}
		virtual void UndoVariation(){
			//restore original values
			looper->MHT = MHT;
			looper->HT = HT;
		}
		
		//member variables
		double MHT;
		double HT;
};

namespace KParser {
	template <>
	KVariator<KSkimmer>* processVariator<KSkimmer>(KNamed* tmp){
		KVariator<KSkimmer>* vtmp = 0;
		string vname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known variators
		if(vname=="Jet") vtmp = new KJetVariator(vname,omap);
		if(vname=="GenMHT") vtmp = new KGenMHTVariator(vname,omap);
		else {} //skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif