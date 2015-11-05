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
//variation of jet energy scale/corrections
class KJetUncVariator : public KVariator<KSkimmer> {
	public:
		//constructor
		KJetUncVariator() : KVariator<KSkimmer>() { }
		KJetUncVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_) {
			//check options
			up = localOpt->Get("up",true);
		}
		//functions
		virtual void DoVariation() {
			//initialize
			Jets = new vector<TLorentzVector>();
			HTJetsMask = new vector<bool>();
			MHTJetsMask = new vector<bool>();
			Jets_partonFlavor = new vector<int>();
			Jets_hadronFlavor = new vector<int>();
			Jets_bDiscriminatorCSV = new vector<double>();
			
			//store original values
			*(Jets) = *(looper->Jets);
			*(HTJetsMask) = *(looper->HTJetsMask);
			*(MHTJetsMask) = *(looper->MHTJetsMask);
			*(Jets_partonFlavor) = *(looper->Jets_partonFlavor);
			*(Jets_hadronFlavor) = *(looper->Jets_hadronFlavor);
			*(Jets_bDiscriminatorCSV) = *(looper->Jets_bDiscriminatorCSV);
			NJets = looper->NJets;
			BTags = looper->BTags;
			HT = looper->HT;
			MHT = looper->MHT;
			MHT_Phi = looper->MHT_Phi;
			JetID = looper->JetID;
			DeltaPhi1 = looper->DeltaPhi1;
			DeltaPhi2 = looper->DeltaPhi2;
			DeltaPhi3 = looper->DeltaPhi3;
			DeltaPhi4 = looper->DeltaPhi4;
			
			//set to clean vars
			if(up){
				*(looper->Jets) = *(looper->JetsJECup);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskJECup);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECup);
				*(looper->Jets_partonFlavor) = *(looper->JetsJECup_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->JetsJECup_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECup_bDiscriminatorCSV);
				looper->NJets = looper->NJetsJECup;
				looper->BTags = looper->BTagsJECup;
				looper->HT = looper->HTJECup;
				looper->MHT = looper->MHTJECup;
				looper->MHT_Phi = looper->MHT_PhiJECup;
				looper->JetID = looper->JetIDJECup;
				looper->DeltaPhi1 = looper->DeltaPhi1JECup;
				looper->DeltaPhi2 = looper->DeltaPhi2JECup;
				looper->DeltaPhi3 = looper->DeltaPhi3JECup;
				looper->DeltaPhi4 = looper->DeltaPhi4JECup;
			}
			else{
				*(looper->Jets) = *(looper->JetsJECdown);
				*(looper->HTJetsMask) = *(looper->HTJetsMaskJECdown);
				*(looper->MHTJetsMask) = *(looper->MHTJetsMaskJECdown);
				*(looper->Jets_partonFlavor) = *(looper->JetsJECdown_partonFlavor);
				*(looper->Jets_hadronFlavor) = *(looper->JetsJECdown_hadronFlavor);
				*(looper->Jets_bDiscriminatorCSV) = *(looper->JetsJECdown_bDiscriminatorCSV);
				looper->NJets = looper->NJetsJECdown;
				looper->BTags = looper->BTagsJECdown;
				looper->HT = looper->HTJECdown;
				looper->MHT = looper->MHTJECdown;
				looper->MHT_Phi = looper->MHT_PhiJECdown;
				looper->JetID = looper->JetIDJECdown;
				looper->DeltaPhi1 = looper->DeltaPhi1JECdown;
				looper->DeltaPhi2 = looper->DeltaPhi2JECdown;
				looper->DeltaPhi3 = looper->DeltaPhi3JECdown;
				looper->DeltaPhi4 = looper->DeltaPhi4JECdown;
			}
		}
		virtual void UndoVariation() {
			//restore original values
			*(looper->Jets) = *(Jets);
			*(looper->HTJetsMask) = *(HTJetsMask);
			*(looper->MHTJetsMask) = *(MHTJetsMask);
			*(looper->Jets_partonFlavor) = *(Jets_partonFlavor);
			*(looper->Jets_hadronFlavor) = *(Jets_hadronFlavor);
			*(looper->Jets_bDiscriminatorCSV) = *(Jets_bDiscriminatorCSV);
			looper->NJets = NJets;
			looper->BTags = BTags;
			looper->HT = HT;
			looper->MHT = MHT;
			looper->MHT_Phi = MHT_Phi;
			looper->JetID = JetID;
			looper->DeltaPhi1 = DeltaPhi1;
			looper->DeltaPhi2 = DeltaPhi2;
			looper->DeltaPhi3 = DeltaPhi3;
			looper->DeltaPhi4 = DeltaPhi4;
		}
		
		//member variables
		bool up;
		vector<TLorentzVector> *Jets;
		vector<bool> *HTJetsMask;
		vector<bool> *MHTJetsMask;
		vector<int> *Jets_hadronFlavor;
		vector<int> *Jets_partonFlavor;
		vector<double> *Jets_bDiscriminatorCSV;
		Int_t           NJets;
		Int_t           BTags;
		Float_t         HT;
		Float_t         MHT;
		Float_t         MHT_Phi;
		Bool_t          JetID;
		Float_t         DeltaPhi1;
		Float_t         DeltaPhi2;
		Float_t         DeltaPhi3;
		Float_t         DeltaPhi4;
};

//----------------------------------------------------
//set hadronic variables to NoPhotons version
class KCleanVariator : public KVariator<KSkimmer> {
	public:
		//constructor
		KCleanVariator() : KVariator<KSkimmer>() { }
		KCleanVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_) {}
		//functions
		virtual void DoVariation() {
			//store original values
			NJets = looper->NJets;
			BTags = looper->BTags;
			HT = looper->HT;
			MHT = looper->MHT;
			MHT_Phi = looper->MHT_Phi;
			METPt = looper->METPt;
			METPhi = looper->METPhi;
			DeltaPhi1 = looper->DeltaPhi1;
			DeltaPhi2 = looper->DeltaPhi2;
			DeltaPhi3 = looper->DeltaPhi3;
			DeltaPhi4 = looper->DeltaPhi4;
			
			//set to clean vars
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
		virtual void UndoVariation() {
			//restore original values
			looper->NJets = NJets;
			looper->BTags = BTags;
			looper->HT = HT;
			looper->MHT = MHT;
			looper->MHT_Phi = MHT_Phi;
			looper->METPt = METPt;
			looper->METPhi = METPhi;
			looper->DeltaPhi1 = DeltaPhi1;
			looper->DeltaPhi2 = DeltaPhi2;
			looper->DeltaPhi3 = DeltaPhi3;
			looper->DeltaPhi4 = DeltaPhi4;
		}
		
		//member variables
		Int_t           NJets;
		Int_t           BTags;
		Float_t         HT;
		Float_t         MHT;
		Float_t         MHT_Phi;
		Float_t         METPt;
		Float_t         METPhi;
		Float_t         DeltaPhi1;
		Float_t         DeltaPhi2;
		Float_t         DeltaPhi3;
		Float_t         DeltaPhi4;
};

//----------------------------------------------------------
//manually calculate NoPhotons version of hadronic variables
//(for photon ID studies)
class KManualCleanVariator : public KVariator<KSkimmer> {
	public:
		//constructor
		KManualCleanVariator() : KVariator<KSkimmer>() { }
		KManualCleanVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_), JetConeSize(0.4)
		{
			localOpt->Get("JetConeSize",JetConeSize);
		}
		//functions
		virtual void DoVariation() {
			//initialize
			bestPhoton = new vector<TLorentzVector>();
			
			//store original values
			*(bestPhoton) = *(looper->bestPhoton);
			NumPhotons = looper->NumPhotons;
			NJets = looper->NJets;
			BTags = looper->BTags;
			HT = looper->HT;
			MHT = looper->MHT;
			MHT_Phi = looper->MHT_Phi;
			METPt = looper->METPt;
			METPhi = looper->METPhi;
			DeltaPhi1 = looper->DeltaPhi1;
			DeltaPhi2 = looper->DeltaPhi2;
			DeltaPhi3 = looper->DeltaPhi3;
			DeltaPhi4 = looper->DeltaPhi4;
			
			//modified photon ID - do not apply sigma_ieta_ieta_, charged hadron isolation cuts
			double bestPhotonPt = 0;
			int bestPhotonIndex = -1;
			int newNumPhotons = 0;
			for(unsigned p = 0; p < looper->photonCands->size(); ++p){
				bool isBarrelPhoton = fabs(looper->photonCands->at(p).Eta()) < 1.4442;
				bool isEndcapPhoton = fabs(looper->photonCands->at(p).Eta()) > 1.566 && fabs(looper->photonCands->at(p).Eta()) < 2.5;
				bool passAcc = isBarrelPhoton || isEndcapPhoton;
				//no sigma_ieta_ieta cuts in ID
				bool passID =  (isBarrelPhoton && looper->photon_hadTowOverEM->at(p) < 0.028 && looper->photon_passElectronVeto->at(p))
							|| (isEndcapPhoton && looper->photon_hadTowOverEM->at(p) < 0.093 && looper->photon_passElectronVeto->at(p));
				//no charged iso cuts in iso
				bool passIso = (isBarrelPhoton && looper->photon_pfNeutralIsoRhoCorr->at(p) < (7.23 + TMath::Exp(0.0028*(looper->photonCands->at(p).Pt()+0.5408))) && looper->photon_pfGammaIsoRhoCorr->at(p) < (2.11 + 0.0014*looper->photonCands->at(p).Pt()))
							|| (isEndcapPhoton && looper->photon_pfNeutralIsoRhoCorr->at(p) < (8.89 + 0.01725*looper->photonCands->at(p).Pt()) && looper->photon_pfGammaIsoRhoCorr->at(p) < (3.09 + 0.0091*looper->photonCands->at(p).Pt()));
				bool goodPhoton = passAcc && passID && passIso && looper->photonCands->at(p).Pt() > 100.0;
				if(goodPhoton){
					++newNumPhotons;
					if(looper->photonCands->at(p).Pt() > bestPhotonPt){
						bestPhotonPt = looper->photonCands->at(p).Pt();
						bestPhotonIndex = p;
					}
				}
			}
			
			//reassign best photon if found
			if(bestPhotonIndex!=-1){
				vector<TLorentzVector>* bestPhotonClean = new vector<TLorentzVector>();
				bestPhotonClean->push_back(looper->photonCands->at(bestPhotonIndex));
				*(looper->bestPhoton) = *(bestPhotonClean);
			}
			looper->NumPhotons = newNumPhotons;
			
			//recalculate jet-based quantities, cleaning out new bestPhoton
			vector<TLorentzVector> htjetsClean;
			TLorentzVector mhtLorentzClean; mhtLorentzClean.SetPtEtaPhiE(0,0,0,0);
			double HTClean = 0.0;
			int NJetsClean = 0;
			int BTagsClean = 0;
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				//MHTJets cuts
				if(!looper->MHTJetsMask->at(j)) continue;
				
				//object cleaning
				bool overlap = false;
				for(unsigned m = 0; m < looper->Muons->size(); ++m){
					if(KMath::DeltaR(looper->Muons->at(m).Phi(),looper->Muons->at(m).Eta(),looper->Jets->at(j).Phi(),looper->Jets->at(j).Eta())<JetConeSize) { overlap = true; break; }
				}
				if(overlap) continue;
				for(unsigned e = 0; e < looper->Electrons->size(); ++e){
					if(KMath::DeltaR(looper->Electrons->at(e).Phi(),looper->Electrons->at(e).Eta(),looper->Jets->at(j).Phi(),looper->Jets->at(j).Eta())<JetConeSize) { overlap = true; break; }
				}
				if(overlap) continue;
				for(unsigned g = 0; g < looper->bestPhoton->size(); ++g){
					if(KMath::DeltaR(looper->bestPhoton->at(g).Phi(),looper->bestPhoton->at(g).Eta(),looper->Jets->at(j).Phi(),looper->Jets->at(j).Eta())<JetConeSize) { overlap = true; break; }
				}
				if(overlap) continue;
				
				//jet has passed all cuts
				mhtLorentzClean -= looper->Jets->at(j);
				
				//HTJets cuts
				if(looper->HTJetsMask->at(j)) {
					htjetsClean.push_back(looper->Jets->at(j));
					HTClean += looper->Jets->at(j).Pt();
					++NJetsClean;
					if(looper->Jets_bDiscriminatorCSV->at(j) > 0.890) ++BTagsClean;
				}
			}
			
			//set to clean vars
			looper->NJets = NJetsClean;
			looper->BTags = BTagsClean;
			looper->HT = HTClean;
			looper->MHT = mhtLorentzClean.Pt();
			looper->MHT_Phi = mhtLorentzClean.Phi();

			double MHTPhiClean = mhtLorentzClean.Phi();
			//test MHT clean calc
			//cout << "delta MHT clean = " << mhtLorentzClean.Pt() - looper->MHTclean << endl;
			
			//reset vars
			looper->DeltaPhi1 = looper->DeltaPhi2 = looper->DeltaPhi3 = looper->DeltaPhi4 = 0;
			for(unsigned j = 0; j < min((unsigned)4,(unsigned)htjetsClean.size()); ++j){
				//recalc delta phi
				if(j==0) looper->DeltaPhi1 = abs(KMath::DeltaPhi(htjetsClean.at(j).Phi(),MHTPhiClean));
				else if(j==1) looper->DeltaPhi2 = abs(KMath::DeltaPhi(htjetsClean.at(j).Phi(),MHTPhiClean));
				else if(j==2) looper->DeltaPhi3 = abs(KMath::DeltaPhi(htjetsClean.at(j).Phi(),MHTPhiClean));
				else if(j==3) looper->DeltaPhi4 = abs(KMath::DeltaPhi(htjetsClean.at(j).Phi(),MHTPhiClean));
			}
			
			//too lazy to recalculate these
			looper->METPt = looper->METPtclean;
			looper->METPhi = looper->METPhiclean;
		}
		virtual void UndoVariation() {
			//restore original values
			*(looper->bestPhoton) = *(bestPhoton);
			looper->NumPhotons = NumPhotons;
			looper->NJets = NJets;
			looper->BTags = BTags;
			looper->HT = HT;
			looper->MHT = MHT;
			looper->MHT_Phi = MHT_Phi;
			looper->METPt = METPt;
			looper->METPhi = METPhi;
			looper->DeltaPhi1 = DeltaPhi1;
			looper->DeltaPhi2 = DeltaPhi2;
			looper->DeltaPhi3 = DeltaPhi3;
			looper->DeltaPhi4 = DeltaPhi4;
		}
		
		//member variables
		double JetConeSize;
		vector<TLorentzVector> *bestPhoton;
		Int_t           NumPhotons;
		Int_t           NJets;
		Int_t           BTags;
		Float_t         HT;
		Float_t         MHT;
		Float_t         MHT_Phi;
		Float_t         METPt;
		Float_t         METPhi;
		Float_t         DeltaPhi1;
		Float_t         DeltaPhi2;
		Float_t         DeltaPhi3;
		Float_t         DeltaPhi4;
};

namespace KParser {
	template <>
	KVariator<KSkimmer>* processVariator<KSkimmer>(KNamed* tmp){
		KVariator<KSkimmer>* vtmp = 0;
		string vname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known variators
		if(vname=="Clean") vtmp = new KCleanVariator(vname,omap);
		else if(vname=="ManualClean") vtmp = new KManualCleanVariator(vname,omap);
		else if(vname=="JetUnc") vtmp = new KJetUncVariator(vname,omap);
		else {} //skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif