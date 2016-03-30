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
//for: JECs, various Zinv cleanings
class KJetVariator : public KVariator<KSkimmer> {
	public:
		enum vartypes { NoVar=0, JECup=1, JECdown=2, clean=3 };
		//constructor
		KJetVariator() : KVariator<KSkimmer>() { }
		KJetVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_), enabled(false), vtype(NoVar),
			b_Jets(true),b_HTJetsMask(true),b_MHTJetsMask(true),b_Jets_ID(true),b_Jets_partonFlavor(true),b_Jets_hadronFlavor(true),b_Jets_bDiscriminatorCSV(true)
		{
			//check options
			string vname = "";
			localOpt->Get("vartype",vname);
			if(vname=="JECup") vtype = JECup;
			else if(vname=="JECdown") vtype = JECdown;
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
				string branchlist[] = {"JetsJECup","HTJetsMaskJECup","MHTJetsMaskJECup","JetsJECup_ID","JetsJECup_partonFlavor","JetsJECup_hadronFlavor","JetsJECup_bDiscriminatorCSV","JetIDJECup","NJetsJECup","BTagsJECup","HTJECup","MHTJECup","MHT_PhiJECup","DeltaPhi1JECup","DeltaPhi2JECup","DeltaPhi3JECup","DeltaPhi4JECup"};
				unsigned branchsize = 17;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==JECdown){
				string branchlist[] = {"JetsJECdown","HTJetsMaskJECdown","MHTJetsMaskJECdown","JetsJECdown_ID","JetsJECdown_partonFlavor","JetsJECdown_hadronFlavor","JetsJECdown_bDiscriminatorCSV","JetIDJECdown","NJetsJECdown","BTagsJECdown","HTJECdown","MHTJECdown","MHT_PhiJECdown","DeltaPhi1JECdown","DeltaPhi2JECdown","DeltaPhi3JECdown","DeltaPhi4JECdown"};
				unsigned branchsize = 17;
				EnableBranches(branchlist,branchsize);
			}
			else if(vtype==clean){
				string branchlist[] = {"Jetsclean","HTJetsMaskclean","MHTJetsMaskclean","Jetsclean_ID","Jetsclean_partonFlavor","Jetsclean_hadronFlavor","Jetsclean_bDiscriminatorCSV","JetIDclean","NJetsclean","BTagsclean","HTclean","MHTclean","MHT_Phiclean","METPtclean","METPhiclean","DeltaPhi1clean","DeltaPhi2clean","DeltaPhi3clean","DeltaPhi4clean"};
				unsigned branchsize = 19;
				EnableBranches(branchlist,branchsize);
			}
			
			//check pointer branches
			if(!looper->fChain->GetBranchStatus("Jets")) b_Jets = false;
			if(!looper->fChain->GetBranchStatus("HTJetsMask")) b_HTJetsMask = false;
			if(!looper->fChain->GetBranchStatus("MHTJetsMask")) b_MHTJetsMask = false;
			if(!looper->fChain->GetBranchStatus("Jets_ID")) b_Jets_ID = false;
			if(!looper->fChain->GetBranchStatus("Jets_partonFlavor")) b_Jets_partonFlavor = false;
			if(!looper->fChain->GetBranchStatus("Jets_hadronFlavor")) b_Jets_hadronFlavor = false;
			if(!looper->fChain->GetBranchStatus("Jets_bDiscriminatorCSV")) b_Jets_bDiscriminatorCSV = false;
		}
		//functions
		virtual void DoVariation() {
			//store original values
			if(b_Jets) Jets = *(looper->Jets);
			if(b_HTJetsMask) HTJetsMask = *(looper->HTJetsMask);
			if(b_MHTJetsMask) MHTJetsMask = *(looper->MHTJetsMask);
			if(b_Jets_ID) Jets_ID = *(looper->Jets_ID);
			if(b_Jets_partonFlavor) Jets_partonFlavor = *(looper->Jets_partonFlavor);
			if(b_Jets_hadronFlavor) Jets_hadronFlavor = *(looper->Jets_hadronFlavor);
			if(b_Jets_bDiscriminatorCSV)Jets_bDiscriminatorCSV = *(looper->Jets_bDiscriminatorCSV);
			JetID = looper->JetID;
			NJets = looper->NJets;
			BTags = looper->BTags;
			isoElectronTracks = looper->isoElectronTracks;
			isoMuonTracks = looper->isoMuonTracks;
			isoPionTracks = looper->isoPionTracks;
			HT = looper->HT;
			METPt = looper->METPt;
			METPhi = looper->METPhi;
			MHT = looper->MHT;
			MHT_Phi = looper->MHT_Phi;
			DeltaPhi1 = looper->DeltaPhi1;
			DeltaPhi2 = looper->DeltaPhi2;
			DeltaPhi3 = looper->DeltaPhi3;
			DeltaPhi4 = looper->DeltaPhi4;
			
			//set to clean vars
			if(vtype==JECup){
				if(b_Jets) *(looper->Jets) = *(looper->JetsJECup);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskJECup);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECup);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJECup_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJECup_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJECup_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECup_bDiscriminatorCSV);
				looper->JetID = looper->JetIDJECup;
				looper->NJets = looper->NJetsJECup;
				looper->BTags = looper->BTagsJECup;
				looper->HT = looper->HTJECup;
				looper->MHT = looper->MHTJECup;
				looper->MHT_Phi = looper->MHT_PhiJECup;
				looper->DeltaPhi1 = looper->DeltaPhi1JECup;
				looper->DeltaPhi2 = looper->DeltaPhi2JECup;
				looper->DeltaPhi3 = looper->DeltaPhi3JECup;
				looper->DeltaPhi4 = looper->DeltaPhi4JECup;
			}
			else if(vtype==JECdown){
				if(b_Jets) *(looper->Jets) = *(looper->JetsJECdown);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskJECdown);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECdown);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->JetsJECdown_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->JetsJECdown_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->JetsJECdown_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECdown_bDiscriminatorCSV);
				looper->JetID = looper->JetIDJECdown;
				looper->NJets = looper->NJetsJECdown;
				looper->BTags = looper->BTagsJECdown;
				looper->HT = looper->HTJECdown;
				looper->MHT = looper->MHTJECdown;
				looper->MHT_Phi = looper->MHT_PhiJECdown;
				looper->DeltaPhi1 = looper->DeltaPhi1JECdown;
				looper->DeltaPhi2 = looper->DeltaPhi2JECdown;
				looper->DeltaPhi3 = looper->DeltaPhi3JECdown;
				looper->DeltaPhi4 = looper->DeltaPhi4JECdown;
			}
			else if(vtype==clean){
				if(b_Jets) *(looper->Jets) = *(looper->Jetsclean);
				if(b_HTJetsMask) *(looper->HTJetsMask) = *(looper->HTJetsMaskclean);
				if(b_MHTJetsMask) *(looper->MHTJetsMask) = *(looper->MHTJetsMaskclean);
				if(b_Jets_ID) *(looper->Jets_ID) = *(looper->Jetsclean_ID);
				if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = *(looper->Jetsclean_partonFlavor);
				if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = *(looper->Jetsclean_hadronFlavor);
				if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = *(looper->Jetsclean_bDiscriminatorCSV);
				looper->JetID = looper->JetIDclean;
				looper->NJets = looper->NJetsclean;
				looper->BTags = looper->BTagsclean;
				looper->HT = looper->HTclean;
				looper->MHT = looper->MHTclean;
				looper->MHT_Phi = looper->MHT_Phiclean;
				looper->METPt = looper->METPtclean;
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
			if(b_Jets_ID) *(looper->Jets_ID) = Jets_ID;
			if(b_Jets_partonFlavor) *(looper->Jets_partonFlavor) = Jets_partonFlavor;
			if(b_Jets_hadronFlavor) *(looper->Jets_hadronFlavor) = Jets_hadronFlavor;
			if(b_Jets_bDiscriminatorCSV) *(looper->Jets_bDiscriminatorCSV) = Jets_bDiscriminatorCSV;
			looper->JetID = JetID;
			looper->NJets = NJets;
			looper->BTags = BTags;
			looper->isoElectronTracks = isoElectronTracks;
			looper->isoMuonTracks = isoMuonTracks;
			looper->isoPionTracks = isoPionTracks;
			looper->HT = HT;
			looper->METPt = METPt;
			looper->METPhi = METPhi;
			looper->MHT = MHT;
			looper->MHT_Phi = MHT_Phi;
			looper->DeltaPhi1 = DeltaPhi1;
			looper->DeltaPhi2 = DeltaPhi2;
			looper->DeltaPhi3 = DeltaPhi3;
			looper->DeltaPhi4 = DeltaPhi4;
		}
		
		//member variables
		bool enabled;
		bool b_Jets,b_HTJetsMask,b_MHTJetsMask,b_Jets_ID,b_Jets_partonFlavor,b_Jets_hadronFlavor,b_Jets_bDiscriminatorCSV;
		vartypes vtype;
		vector<TLorentzVector> Jets;
		vector<bool> HTJetsMask;
		vector<bool> MHTJetsMask;
		vector<bool> Jets_ID;
		vector<int> Jets_hadronFlavor;
		vector<int> Jets_partonFlavor;
		vector<double> Jets_bDiscriminatorCSV;
		Bool_t          JetID;
		Int_t           NJets;
		Int_t           BTags;
		Int_t           isoElectronTracks;
		Int_t           isoMuonTracks;
		Int_t           isoPionTracks;
		Double_t        HT;
		Double_t        METPt;
		Double_t        METPhi;
		Double_t        MHT;
		Double_t        MHT_Phi;
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
		else {} //skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif