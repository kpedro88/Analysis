#ifndef KSKIMMERVARIATORS_H
#define KSKIMMERVARIATORS_H

//custom headers
#include "KVariation.h"
#include "KSkimmer.h"

//ROOT headers
#include <TLorentzVector.h>

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
		KCleanVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_), 
		cleanDeltaPhi(false), minPtMHT(30.), maxEtaMHT(5.), minPtHT(30.), maxEtaHT(2.4), muonR(0.4), electronR(0.4), photonR(0.4)
		{ 
			//check for options
			cleanDeltaPhi = localOpt->Get("cleanDeltaPhi",false);
			if(cleanDeltaPhi){
				localOpt->Get("minPtMHT",minPtMHT);
				localOpt->Get("maxEtaMHT",maxEtaMHT);
				localOpt->Get("minPtHT",minPtHT);
				localOpt->Get("maxEtaHT",maxEtaHT);
				localOpt->Get("muonR",muonR);
				localOpt->Get("electronR",electronR);
				localOpt->Get("photonR",photonR);
			}
		}
		//functions
		virtual void DoVariation() {			
			//store original values
			NJets = looper->NJets;
			BTags = looper->BTags;
			HT = looper->HT;
			MHT = looper->MHT;
			minDeltaPhiN = looper->minDeltaPhiN;
			METPt = looper->METPt;
			
			//set to clean vars
			looper->NJets = looper->NJetsclean;
			looper->BTags = looper->BTagsclean;
			looper->HT = looper->HTclean;
			looper->MHT = looper->MHTclean;
			looper->minDeltaPhiN = looper->minDeltaPhiNclean;
			looper->METPt = looper->METPtclean;
			
			//keep uncleaned MHT
			looper->MHTclean = MHT;
			
			if(cleanDeltaPhi){
				DeltaPhi1 = looper->DeltaPhi1;
				DeltaPhi2 = looper->DeltaPhi2;
				DeltaPhi3 = looper->DeltaPhi3;
				
				//we have to clean DeltaPhi by hand...
				//first recalculate cleaned MHT to get MHTPhi variable
				vector<TLorentzVector> htjets;
				TLorentzVector mhtLorentz; mhtLorentz.SetPtEtaPhiE(0,0,0,0);
				for(unsigned j = 0; j < looper->ak4Jets->size(); ++j){
					//MHTJets cuts
					if(looper->ak4Jets->at(j).Pt()<=minPtMHT) continue;
					if(looper->ak4Jets->at(j).Eta()>=maxEtaMHT) continue;
					
					//object cleaning
					bool overlap = false;
					for(unsigned m = 0; m < looper->Muons->size(); ++m){
						if(KMath::DeltaR(looper->Muons->at(m).Phi(),looper->Muons->at(m).Eta(),looper->ak4Jets->at(j).Phi(),looper->ak4Jets->at(j).Eta())<muonR) { overlap = true; break; }
					}
					if(overlap) continue;
					for(unsigned e = 0; e < looper->Electrons->size(); ++e){
						if(KMath::DeltaR(looper->Electrons->at(e).Phi(),looper->Electrons->at(e).Eta(),looper->ak4Jets->at(j).Phi(),looper->ak4Jets->at(j).Eta())<electronR) { overlap = true; break; }
					}
					if(overlap) continue;
					for(unsigned g = 0; g < looper->bestPhoton->size(); ++g){
						if(KMath::DeltaR(looper->bestPhoton->at(g).Phi(),looper->bestPhoton->at(g).Eta(),looper->ak4Jets->at(j).Phi(),looper->ak4Jets->at(j).Eta())<photonR) { overlap = true; break; }
					}
					if(overlap) continue;
					
					//jet has passed all cuts
					mhtLorentz -= looper->ak4Jets->at(j);
					
					//HTJets cuts for actual deltaphi calc
					if(looper->ak4Jets->at(j).Pt()>minPtHT && looper->ak4Jets->at(j).Eta()<maxEtaHT) htjets.push_back(looper->ak4Jets->at(j));
				}
			
				double MHTPhiclean = mhtLorentz.Phi();
				//test MHT clean calc
				//cout << "delta MHT clean = " << mhtLorentz.Pt() - looper->MHTclean << endl;
				
				//reset vars
				looper->DeltaPhi1 = looper->DeltaPhi2 = looper->DeltaPhi3 = 0;
				for(unsigned j = 0; j < min((unsigned)3,(unsigned)htjets.size()); ++j){
					//recalc delta phi
					if(j==0) looper->DeltaPhi1 = abs(KMath::DeltaPhi(htjets.at(j).Phi(),MHTPhiclean));
					else if(j==1) looper->DeltaPhi2 = abs(KMath::DeltaPhi(htjets.at(j).Phi(),MHTPhiclean));
					else if(j==2) looper->DeltaPhi3 = abs(KMath::DeltaPhi(htjets.at(j).Phi(),MHTPhiclean));
				}
			}
		}
		virtual void UndoVariation() {
			//restore original values
			looper->NJets = NJets;
			looper->BTags = BTags;
			looper->HT = HT;
			looper->MHT = MHT;
			looper->minDeltaPhiN = minDeltaPhiN;
			looper->METPt = METPt;
			if(cleanDeltaPhi){
				looper->DeltaPhi1 = DeltaPhi1;
				looper->DeltaPhi2 = DeltaPhi2;
				looper->DeltaPhi3 = DeltaPhi3;
			}
		}
		
		//member variables
		bool cleanDeltaPhi;
		double minPtMHT, maxEtaMHT, minPtHT, maxEtaHT, muonR, electronR, photonR;
		Int_t           NJets;
		Int_t           BTags;
		Float_t         HT;
		Float_t         MHT;
		Float_t         minDeltaPhiN;
		Float_t         METPt;
		Float_t         DeltaPhi1;
		Float_t         DeltaPhi2;
		Float_t         DeltaPhi3;
};

namespace KParser {
	template <>
	KVariator<KSkimmer>* processVariator<KSkimmer>(KNamed* tmp){
		KVariator<KSkimmer>* vtmp = 0;
		string vname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known variators
		if(vname=="Clean") vtmp = new KCleanVariator(vname,omap);
		//else if(vname=="JES") vtmp = new KJetESVariator(vname,omap);
		//else if(vname=="JER") vtmp = new KJetERVariator(vname,omap);
		else {} //skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif