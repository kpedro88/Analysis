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

/*
//----------------------------------------------------
//variation of jet energy scale
class KJetESVariator : public KVariator<KSkimmer> {
	public:
		//constructor
		KJetESVariator() : KVariator<KSkimmer>() { }
		KJetESVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_) { 
			//check options
			up = localOpt->Get("up",true);
			//set default values
			PFJetPt = PFJetEnergy = 0;
		}
		//functions
		virtual void DoVariation() {
			delete PFJetPt; PFJetPt = new vector<double>();
			PFJetPt->reserve(looper->PFJetPt->size());
			delete PFJetEnergy; PFJetEnergy = new vector<double>();
			PFJetEnergy->reserve(looper->PFJetEnergy->size());
			
			for(unsigned j = 0; j < looper->PFJetPt->size(); j++){
				//store original values
				PFJetPt->push_back(looper->PFJetPt->at(j));
				PFJetEnergy->push_back(looper->PFJetEnergy->at(j));
				
				//Scale jet pT by JEC uncertainty
				double unc = looper->PFJetJECUnc->at(j);
				TLorentzVector vj;
				vj.SetPtEtaPhiE(looper->PFJetPt->at(j),looper->PFJetEta->at(j),looper->PFJetPhi->at(j),looper->PFJetEnergy->at(j));
				//smear 4-vector (nonzero jet mass)
				if(up) vj *= (1+unc);
				else vj *= (1-unc);
				looper->PFJetPt->at(j) = vj.Pt();
				looper->PFJetEnergy->at(j) = vj.E();
			}
		}
		virtual void UndoVariation() {
			//restore original values
			for(unsigned j = 0; j < looper->PFJetPt->size(); j++){
				looper->PFJetPt->at(j) = PFJetPt->at(j);
				looper->PFJetEnergy->at(j) = PFJetEnergy->at(j);
			}
		}
		
		//member variables
		bool up;
		vector<double> *PFJetPt, *PFJetEnergy;
};

//----------------------------------------------------
//variation of jet energy resolution
class KJetERVariator : public KVariator {
	public:
		//constructor
		KJetERVariator() : KVariator() { }
		KJetERVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_) { 
			//check options
			up = localOpt->Get("up",true);
			//set default values
			PFJetPt = PFJetEnergy = 0;
		}
		//functions
		virtual void DoVariation() {
			delete PFJetPt; PFJetPt = new vector<double>();
			PFJetPt->reserve(looper->PFJetPt->size());
			delete PFJetEnergy; PFJetEnergy = new vector<double>();
			PFJetEnergy->reserve(looper->PFJetEnergy->size());
			
			for(unsigned j = 0; j < looper->PFJetPt->size(); j++){
				//store original values
				PFJetPt->push_back(looper->PFJetPt->at(j));
				PFJetEnergy->push_back(looper->PFJetEnergy->at(j));
				
				//JER smearing
				double min_dR = 1000;
				bool matchedGenJetFound = false;
				int index_gen = -1;

				for (unsigned g = 0; g < looper->GenJetPt->size(); g++){
					double dR = KMath::DeltaR(looper->PFJetPhi->at(j),looper->PFJetEta->at(j),looper->GenJetPhi->at(g),looper->GenJetEta->at(g));
					if(dR < min_dR) {
						min_dR = dR;
						index_gen = g;
					}
				}
				if (min_dR < 0.4) matchedGenJetFound = true;
				
				if(matchedGenJetFound){
					TLorentzVector vj, vg;
					vj.SetPtEtaPhiE(looper->PFJetPt->at(j),looper->PFJetEta->at(j),looper->PFJetPhi->at(j),looper->PFJetEnergy->at(j));
					vg.SetPtEtaPhiE(looper->GenJetPt->at(index_gen),looper->GenJetEta->at(index_gen),looper->GenJetPhi->at(index_gen),looper->GenJetEnergy->at(index_gen));
					
					vj = SmearJER(vj,vg);
					looper->PFJetPt->at(j) = vj.Pt();
					looper->PFJetEnergy->at(j) = vj.E();
				}
			}
		}
		virtual void UndoVariation() {
			//restore original values
			for(unsigned j = 0; j < looper->PFJetPt->size(); j++){
				looper->PFJetPt->at(j) = PFJetPt->at(j);
				looper->PFJetEnergy->at(j) = PFJetEnergy->at(j);
			}
		}
		//helper for JER smearing
		TLorentzVector SmearJER(TLorentzVector vj, TLorentzVector vg){
			double c = 1.;
			double abseta = TMath::Abs(vj.Eta());
			if (abseta <= 0.5) c = 1.052;
			else if (abseta > 0.5 && abseta <= 1.1) c = 1.057;
			else if (abseta > 1.1 && abseta <= 1.7) c = 1.096;
			else if (abseta > 1.7 && abseta <= 2.3) c = 1.134;
			else c = 1.288;
			
			double f = 0;
			if(up) f = max(vg.E() + c*(vj.E()-vg.E()),0.)/vj.E();
			else f = max(vg.E() + (1.-(c-1.))*(vj.E()-vg.E()),0.)/vj.E();
			
			return f*vj;
		}
		
		//member variables
		bool up;
		vector<double> *PFJetPt, *PFJetEnergy;
};
*/

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
		//else if(vname=="JES") vtmp = new KJetESVariator(vname,omap);
		//else if(vname=="JER") vtmp = new KJetERVariator(vname,omap);
		else {} //skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif