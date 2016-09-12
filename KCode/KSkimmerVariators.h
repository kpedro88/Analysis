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
			b_Jets(true),b_Jets_HTMask(true),b_Jets_MHTMask(true),b_Jets_ID(true),b_Jets_partonFlavor(true),b_Jets_hadronFlavor(true),b_Jets_bDiscriminatorCSV(true),b_Jets_bDiscriminatorMVA(true),b_Jets_muonEnergyFraction(true)
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
				string branchlist[] = {"JetsJECup","JetsJECup_HTMask","JetsJECup_MHTMask","JetsJECup_ISRMask","JetsJECup_LeptonMask","JetsJECup_ID","JetsJECup_partonFlavor","JetsJECup_hadronFlavor","JetsJECup_bDiscriminatorCSV","JetsJECup_bDiscriminatorMVA","JetsJECup_muonEnergyFraction","JetIDJECup","NJetsJECup","NJetsISRJECup","BTagsJECup","BTagsMVAJECup","HTJECup","MHTJECup","MHTPhiJECup","DeltaPhi1JECup","DeltaPhi2JECup","DeltaPhi3JECup","DeltaPhi4JECup"};
				unsigned branchsize = 20;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==JECdown){
				string branchlist[] = {"JetsJECdown","JetsJECdown_HTMask","JetsJECdown_MHTMask","JetsJECdown_ISRMask","JetsJECdown_LeptonMask","JetsJECdown_ID","JetsJECdown_partonFlavor","JetsJECdown_hadronFlavor","JetsJECdown_bDiscriminatorCSV","JetsJECdown_bDiscriminatorMVA","JetsJECdown_muonEnergyFraction","JetIDJECdown","NJetsJECdown","NJetsISRJECdown","BTagsJECdown","BTagsMVAJECdown","HTJECdown","MHTJECdown","MHTPhiJECdown","DeltaPhi1JECdown","DeltaPhi2JECdown","DeltaPhi3JECdown","DeltaPhi4JECdown"};
				unsigned branchsize = 20;
				EnableBranches(branchlist,branchsize);
			}
			if(vtype==JERup){
				string branchlist[] = {"JetsJERup","JetsJERup_HTMask","JetsJERup_MHTMask","JetsJERup_ISRMask","JetsJERup_LeptonMask","JetsJERup_ID","JetsJERup_partonFlavor","JetsJERup_hadronFlavor","JetsJERup_bDiscriminatorCSV","JetsJERup_bDiscriminatorMVA","JetsJERup_muonEnergyFraction","JetIDJERup","NJetsJERup","NJetsISRJERup","BTagsJERup","BTagsMVAJERup","HTJERup","MHTJERup","MHTPhiJERup","DeltaPhi1JERup","DeltaPhi2JERup","DeltaPhi3JERup","DeltaPhi4JERup"};
				unsigned branchsize = 20;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==JERdown){
				string branchlist[] = {"JetsJERdown","JetsJERdown_HTMask","JetsJERdown_MHTMask","JetsJERdown_ISRMask","JetsJERdown_LeptonMask","JetsJERdown_ID","JetsJERdown_partonFlavor","JetsJERdown_hadronFlavor","JetsJERdown_bDiscriminatorCSV","JetsJERdown_bDiscriminatorMVA","JetsJERdown_muonEnergyFraction","JetIDJERdown","NJetsJERdown","NJetsISRJERdown","BTagsJERdown","BTagsMVAJERdown","HTJERdown","MHTJERdown","MHTPhiJERdown","DeltaPhi1JERdown","DeltaPhi2JERdown","DeltaPhi3JERdown","DeltaPhi4JERdown"};
				unsigned branchsize = 20;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==clean){
				string branchlist[] = {"Jetsclean","Jetsclean_HTMask","Jetsclean_MHTMask","Jetsclean_ISRMask","Jetsclean_ID","Jetsclean_partonFlavor","Jetsclean_hadronFlavor","Jetsclean_bDiscriminatorCSV","Jetsclean_bDiscriminatorMVA","Jetsclean_muonEnergyFraction","JetIDclean","NJetsclean","NJetsISRclean","BTagsclean","BTagsMVAclean","HTclean","MHTclean","MHTPhiclean","METclean","METPhiclean","DeltaPhi1clean","DeltaPhi2clean","DeltaPhi3clean","DeltaPhi4clean","isoElectronTracksclean","isoMuonTracksclean","isoPionTracksclean"};
				unsigned branchsize = 22;
				EnableBranches(branchlist,branchsize);
			}
			
			
			//check pointer branches
			if(!looper->fChain->GetBranchStatus("Jets")) b_Jets = false;
			if(!looper->fChain->GetBranchStatus("Jets_HTMask")) b_Jets_HTMask = false;
			if(!looper->fChain->GetBranchStatus("Jets_MHTMask")) b_Jets_MHTMask = false;
			if(!looper->fChain->GetBranchStatus("Jets_ISRMask")) b_Jets_ISRMask = false;
			if(!looper->fChain->GetBranchStatus("Jets_LeptonMask")) b_Jets_LeptonMask = false;
			if(!looper->fChain->GetBranchStatus("Jets_ID")) b_Jets_ID = false;
			if(!looper->fChain->GetBranchStatus("Jets_partonFlavor")) b_Jets_partonFlavor = false;
			if(!looper->fChain->GetBranchStatus("Jets_hadronFlavor")) b_Jets_hadronFlavor = false;
			if(!looper->fChain->GetBranchStatus("Jets_bDiscriminatorCSV")) b_Jets_bDiscriminatorCSV = false;
			if(!looper->fChain->GetBranchStatus("Jets_bDiscriminatorMVA")) b_Jets_bDiscriminatorMVA = false;
			if(!looper->fChain->GetBranchStatus("Jets_muonEnergyFraction")) b_Jets_muonEnergyFraction = false;
		}
		//functions
		virtual void DoVariation() {
			//store original values
			if(b_Jets) Jets = *(looper->Jets);
			if(b_Jets_HTMask) Jets_HTMask = *(looper->Jets_HTMask);
			if(b_Jets_MHTMask) Jets_MHTMask = *(looper->Jets_MHTMask);
			if(b_Jets_ISRMask) Jets_ISRMask = *(looper->Jets_ISRMask);
			if(b_Jets_LeptonMask) Jets_LeptonMask = *(looper->Jets_LeptonMask);
			if(b_Jets_ID) Jets_ID = *(looper->Jets_ID);
			if(b_Jets_partonFlavor) Jets_partonFlavor = *(looper->Jets_partonFlavor);
			if(b_Jets_hadronFlavor) Jets_hadronFlavor = *(looper->Jets_hadronFlavor);
			if(b_Jets_bDiscriminatorCSV) Jets_bDiscriminatorCSV = *(looper->Jets_bDiscriminatorCSV);
			if(b_Jets_bDiscriminatorMVA) Jets_bDiscriminatorMVA = *(looper->Jets_bDiscriminatorMVA);
			if(b_Jets_muonEnergyFraction) Jets_muonEnergyFraction = *(looper->Jets_muonEnergyFraction);
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
				if(b_Jets_HTMask) *(looper->Jets_HTMask) = *(looper->JetsJECup_HTMask);
				if(b_Jets_MHTMask) *(looper->Jets_MHTMask) = *(looper->JetsJECup_MHTMask);
				if(b_Jets_ISRMask) *(looper->Jets_ISRMask) = *(looper->JetsJECup_ISRMask);
				if(b_Jets_LeptonMask) *(looper->Jets_LeptonMask) = *(looper->JetsJECup_LeptonMask);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJECup_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJECup_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJECup_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECup_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->JetsJECup_bDiscriminatorMVA);
				if(b_Jets_muonEnergyFraction) *(looper->Jets_muonEnergyFraction) = *(looper->JetsJECup_muonEnergyFraction);
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
				if(b_Jets_HTMask) *(looper->Jets_HTMask) = *(looper->JetsJECdown_HTMask);
				if(b_Jets_MHTMask) *(looper->Jets_MHTMask) = *(looper->JetsJECdown_MHTMask);
				if(b_Jets_ISRMask) *(looper->Jets_ISRMask) = *(looper->JetsJECdown_ISRMask);
				if(b_Jets_LeptonMask) *(looper->Jets_LeptonMask) = *(looper->JetsJECdown_LeptonMask);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJECdown_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJECdown_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJECdown_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECdown_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->JetsJECdown_bDiscriminatorMVA);
				if(b_Jets_muonEnergyFraction) *(looper->Jets_muonEnergyFraction) = *(looper->JetsJECdown_muonEnergyFraction);
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
				if(b_Jets_HTMask) *(looper->Jets_HTMask) = *(looper->JetsJERup_HTMask);
				if(b_Jets_MHTMask) *(looper->Jets_MHTMask) = *(looper->JetsJERup_MHTMask);
				if(b_Jets_ISRMask) *(looper->Jets_ISRMask) = *(looper->JetsJERup_ISRMask);
				if(b_Jets_LeptonMask) *(looper->Jets_LeptonMask) = *(looper->JetsJERup_LeptonMask);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJERup_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJERup_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJERup_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJERup_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->JetsJERup_bDiscriminatorMVA);
				if(b_Jets_muonEnergyFraction) *(looper->Jets_muonEnergyFraction) = *(looper->JetsJERup_muonEnergyFraction);
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
				if(b_Jets_HTMask) *(looper->Jets_HTMask) = *(looper->JetsJERdown_HTMask);
				if(b_Jets_MHTMask) *(looper->Jets_MHTMask) = *(looper->JetsJERdown_MHTMask);
				if(b_Jets_ISRMask) *(looper->Jets_ISRMask) = *(looper->JetsJERdown_ISRMask);
				if(b_Jets_LeptonMask) *(looper->Jets_LeptonMask) = *(looper->JetsJERdown_LeptonMask);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJERdown_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJERdown_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJERdown_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJERdown_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->JetsJERdown_bDiscriminatorMVA);
				if(b_Jets_muonEnergyFraction) *(looper->Jets_muonEnergyFraction) = *(looper->JetsJERdown_muonEnergyFraction);
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
				if(b_Jets_HTMask) *(looper->Jets_HTMask) = *(looper->Jetsclean_HTMask);
				if(b_Jets_MHTMask) *(looper->Jets_MHTMask) = *(looper->Jetsclean_MHTMask);
				if(b_Jets_ISRMask) *(looper->Jets_ISRMask) = *(looper->Jetsclean_ISRMask);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->Jetsclean_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->Jetsclean_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->Jetsclean_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->Jetsclean_bDiscriminatorCSV);
				if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = *(looper->Jetsclean_bDiscriminatorMVA);
				if(b_Jets_muonEnergyFraction) *(looper->Jets_muonEnergyFraction) = *(looper->Jetsclean_muonEnergyFraction);
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
			if(b_Jets_HTMask) *(looper->Jets_HTMask) = Jets_HTMask;
			if(b_Jets_MHTMask) *(looper->Jets_MHTMask) = Jets_MHTMask;
			if(b_Jets_ISRMask) *(looper->Jets_ISRMask) = Jets_ISRMask;
			if(b_Jets_LeptonMask) *(looper->Jets_LeptonMask) = Jets_LeptonMask;
			if(b_Jets_ID) *(looper->Jets_ID) = Jets_ID;
			if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = Jets_partonFlavor;
			if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = Jets_hadronFlavor;
			if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = Jets_bDiscriminatorCSV;
			if(b_Jets_bDiscriminatorMVA) *(looper->Jets_bDiscriminatorMVA) = Jets_bDiscriminatorMVA;
			if(b_Jets_muonEnergyFraction) *(looper->Jets_muonEnergyFraction) = Jets_muonEnergyFraction;
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
		bool b_Jets,b_Jets_HTMask,b_Jets_MHTMask,b_Jets_ISRMask,b_Jets_LeptonMask,b_Jets_ID,b_Jets_partonFlavor,b_Jets_hadronFlavor,b_Jets_bDiscriminatorCSV,b_Jets_bDiscriminatorMVA,b_Jets_muonEnergyFraction;
		vartypes vtype;
		vector<TLorentzVector> Jets;
		vector<bool> Jets_HTMask;
		vector<bool> Jets_MHTMask;
		vector<bool> Jets_ISRMask;
		vector<bool> Jets_LeptonMask;
		vector<bool> Jets_ID;
		vector<int> Jets_hadronFlavor;
		vector<int> Jets_partonFlavor;
		vector<double> Jets_bDiscriminatorCSV;
		vector<double> Jets_bDiscriminatorMVA;
		vector<double> Jets_muonEnergyFraction;
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


class KJetLeptonVariator : public KVariator<KSkimmer> {
	public:
		//constructor
		KJetLeptonVariator() : KVariator<KSkimmer>() { }
		KJetLeptonVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_) { }
		
		virtual void DoVariation() {
			//store original values
			Jets = *(looper->Jets);
			Jets_HTMask = *(looper->Jets_HTMask);
			Jets_MHTMask = *(looper->Jets_MHTMask);
			Jets_ID = *(looper->Jets_ID);
			Jets_muonEnergyFraction = *(looper->Jets_muonEnergyFraction);
			Jets_bDiscriminatorCSV = *(looper->Jets_bDiscriminatorCSV);
			JetID = looper->JetID;
			NJets = looper->NJets;
			BTags = looper->BTags;
			MHT = looper->MHT;
			MHTPhi = looper->MHTPhi;
			HT = looper->HT;
			DeltaPhi1 = looper->DeltaPhi1;
			DeltaPhi2 = looper->DeltaPhi2;
			DeltaPhi3 = looper->DeltaPhi3;
			DeltaPhi4 = looper->DeltaPhi4;

			vector<TLorentzVector> Jets_;
			vector<bool> Jets_HTMask_;
			vector<bool> Jets_MHTMask_;
			vector<bool> Jets_ID_;
			vector<double> Jets_muonEnergyFraction_;
			vector<double> Jets_bDiscriminatorCSV_;
			bool JetID_ = true;
			int NJets_ = 0;
			int BTags_ = 0;
			double MHT_ = 0;
			double MHTPhi_ = 0;
			double HT_ = 0;
			double DeltaPhi1_ = 0;
			double DeltaPhi2_ = 0;
			double DeltaPhi3_ = 0;
			double DeltaPhi4_ = 0;
			TLorentzVector MHTvec; MHTvec.SetPtEtaPhiE(0,0,0,0);
			
			vector<TLorentzVector>* handles[] = {looper->Muons,looper->Electrons};
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(looper->Jets->at(j).Pt()<=30) continue; //skip all low pT jets
				bool skip = false;
				for(unsigned h = 0; h < 2; ++h){
					for(unsigned ih = 0; ih < handles[h]->size(); ++ih){
						if( abs(looper->Jets->at(j).Pt() - handles[h]->at(ih).Pt())/handles[h]->at(ih).Pt() < 1 && looper->Jets->at(j).DeltaR(handles[h]->at(ih)) < 0.4) {
							skip = true;
							break;
						}
					}
					if(skip) break;
				}
				if(!skip){
					Jets_.push_back(looper->Jets->at(j));
					Jets_HTMask_.push_back(looper->Jets_HTMask->at(j));
					Jets_MHTMask_.push_back(looper->Jets_MHTMask->at(j));
					Jets_ID_.push_back(looper->Jets_ID->at(j));
					Jets_muonEnergyFraction_.push_back(looper->Jets_muonEnergyFraction->at(j));
					Jets_bDiscriminatorCSV_.push_back(looper->Jets_bDiscriminatorCSV->at(j));
					if(looper->Jets->at(j).Pt() > 30) JetID &= looper->Jets_ID->at(j);
					if(looper->Jets_HTMask->at(j)){
						HT_ += looper->Jets->at(j).Pt();
						++NJets_;
						if(looper->Jets_bDiscriminatorCSV->at(j)>0.800) ++BTags_;
					}
					if(looper->Jets_MHTMask->at(j)){
						MHTvec -= looper->Jets->at(j);
					}
				}
			}
			
			MHT_ = MHTvec.Pt();
			MHTPhi_ = MHTvec.Phi();
			
			for(unsigned j = 0; j < min((unsigned)4,(unsigned)Jets_.size()); ++j){
				//recalc delta phi
				if(j==0) DeltaPhi1_ = abs(KMath::DeltaPhi(Jets_.at(j).Phi(),MHTPhi_));
				else if(j==1) DeltaPhi2_ = abs(KMath::DeltaPhi(Jets_.at(j).Phi(),MHTPhi_));
				else if(j==2) DeltaPhi3_ = abs(KMath::DeltaPhi(Jets_.at(j).Phi(),MHTPhi_));
				else if(j==3) DeltaPhi4_ = abs(KMath::DeltaPhi(Jets_.at(j).Phi(),MHTPhi_));
			}
			
			//set to new vars
			*(looper->Jets) = Jets_;
			*(looper->Jets_HTMask) = Jets_HTMask_;
			*(looper->Jets_MHTMask) = Jets_MHTMask_;
			*(looper->Jets_ID) = Jets_ID_;
			*(looper->Jets_bDiscriminatorCSV) = Jets_bDiscriminatorCSV_;
			*(looper->Jets_muonEnergyFraction) = Jets_muonEnergyFraction_;
			looper->JetID = JetID_;
			looper->NJets = NJets_;
			looper->BTags = BTags_;
			looper->MHT = MHT_;
			looper->MHTPhi = MHTPhi_;
			looper->HT = HT_;
			looper->DeltaPhi1 = DeltaPhi1_;
			looper->DeltaPhi2 = DeltaPhi2_;
			looper->DeltaPhi3 = DeltaPhi3_;
			looper->DeltaPhi4 = DeltaPhi4_;
			
		}
		virtual void UndoVariation(){
			//restore original values
			*(looper->Jets) = Jets;
			*(looper->Jets_HTMask) = Jets_HTMask;
			*(looper->Jets_MHTMask) = Jets_MHTMask;
			*(looper->Jets_ID) = Jets_ID;
			*(looper->Jets_bDiscriminatorCSV) = Jets_bDiscriminatorCSV;
			*(looper->Jets_muonEnergyFraction) = Jets_muonEnergyFraction;
			looper->JetID = JetID;
			looper->NJets = NJets;
			looper->BTags = BTags;
			looper->MHT = MHT;
			looper->MHTPhi = MHTPhi;
			looper->HT = HT;
			looper->DeltaPhi1 = DeltaPhi1;
			looper->DeltaPhi2 = DeltaPhi2;
			looper->DeltaPhi3 = DeltaPhi3;
			looper->DeltaPhi4 = DeltaPhi4;
		}
		
		//member variables
		vector<TLorentzVector> Jets;
		vector<bool> Jets_HTMask;
		vector<bool> Jets_MHTMask;
		vector<bool> Jets_ID;
		vector<double> Jets_bDiscriminatorCSV;
		vector<double> Jets_muonEnergyFraction;
		Bool_t          JetID;
		Int_t           NJets;
		Int_t           BTags;
		Double_t        MHT;
		Double_t        MHTPhi;
		Double_t        HT;
		Double_t        DeltaPhi1;
		Double_t        DeltaPhi2;
		Double_t        DeltaPhi3;
		Double_t        DeltaPhi4;
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
		if(vname=="JetLepton") vtmp = new KJetLeptonVariator(vname,omap);
		else {} //skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif