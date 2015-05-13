#ifndef KVARIATORS_H
#define KVARIATORS_H

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
class KJetESVariator : public KVariator {
	public:
		//constructor
		KJetESVariator() : KVariator() { }
		KJetESVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_) { 
			//check options
			up = localOpt->Get("up",true);
			//set default values
			PFJetPt = PFJetEnergy = 0;
		}
		//functions
		virtual void DoVariation() {
			delete PFJetPt; PFJetPt = new vector<double>();
			PFJetPt->reserve(sk->PFJetPt->size());
			delete PFJetEnergy; PFJetEnergy = new vector<double>();
			PFJetEnergy->reserve(sk->PFJetEnergy->size());
			
			for(unsigned j = 0; j < sk->PFJetPt->size(); j++){
				//store original values
				PFJetPt->push_back(sk->PFJetPt->at(j));
				PFJetEnergy->push_back(sk->PFJetEnergy->at(j));
				
				//Scale jet pT by JEC uncertainty
				double unc = sk->PFJetJECUnc->at(j);
				TLorentzVector vj;
				vj.SetPtEtaPhiE(sk->PFJetPt->at(j),sk->PFJetEta->at(j),sk->PFJetPhi->at(j),sk->PFJetEnergy->at(j));
				//smear 4-vector (nonzero jet mass)
				if(up) vj *= (1+unc);
				else vj *= (1-unc);
				sk->PFJetPt->at(j) = vj.Pt();
				sk->PFJetEnergy->at(j) = vj.E();
			}
		}
		virtual void UndoVariation() {
			//restore original values
			for(unsigned j = 0; j < sk->PFJetPt->size(); j++){
				sk->PFJetPt->at(j) = PFJetPt->at(j);
				sk->PFJetEnergy->at(j) = PFJetEnergy->at(j);
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
			PFJetPt->reserve(sk->PFJetPt->size());
			delete PFJetEnergy; PFJetEnergy = new vector<double>();
			PFJetEnergy->reserve(sk->PFJetEnergy->size());
			
			for(unsigned j = 0; j < sk->PFJetPt->size(); j++){
				//store original values
				PFJetPt->push_back(sk->PFJetPt->at(j));
				PFJetEnergy->push_back(sk->PFJetEnergy->at(j));
				
				//JER smearing
				double min_dR = 1000;
				bool matchedGenJetFound = false;
				int index_gen = -1;

				for (unsigned g = 0; g < sk->GenJetPt->size(); g++){
					double dR = KMath::DeltaR(sk->PFJetPhi->at(j),sk->PFJetEta->at(j),sk->GenJetPhi->at(g),sk->GenJetEta->at(g));
					if(dR < min_dR) {
						min_dR = dR;
						index_gen = g;
					}
				}
				if (min_dR < 0.4) matchedGenJetFound = true;
				
				if(matchedGenJetFound){
					TLorentzVector vj, vg;
					vj.SetPtEtaPhiE(sk->PFJetPt->at(j),sk->PFJetEta->at(j),sk->PFJetPhi->at(j),sk->PFJetEnergy->at(j));
					vg.SetPtEtaPhiE(sk->GenJetPt->at(index_gen),sk->GenJetEta->at(index_gen),sk->GenJetPhi->at(index_gen),sk->GenJetEnergy->at(index_gen));
					
					vj = SmearJER(vj,vg);
					sk->PFJetPt->at(j) = vj.Pt();
					sk->PFJetEnergy->at(j) = vj.E();
				}
			}
		}
		virtual void UndoVariation() {
			//restore original values
			for(unsigned j = 0; j < sk->PFJetPt->size(); j++){
				sk->PFJetPt->at(j) = PFJetPt->at(j);
				sk->PFJetEnergy->at(j) = PFJetEnergy->at(j);
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
class KNoPhotonVariator : public KVariator {
	public:
		//constructor
		KNoPhotonVariator() : KVariator() { }
		KNoPhotonVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_), ak4Jets(NULL) { }
		//functions
		virtual void DoVariation() {
			//initialize
			ak4Jets = new vector<TLorentzVector>();
			
			//store original values
			NJets = sk->NJets;
			HT = sk->HT;
			MHT = sk->MHT;
			DeltaPhi1 = sk->DeltaPhi1;
			DeltaPhi2 = sk->DeltaPhi2;
			DeltaPhi3 = sk->DeltaPhi3;
			minDeltaPhiN = sk->minDeltaPhiN;
			DeltaPhiN1 = sk->DeltaPhiN1;
			DeltaPhiN2 = sk->DeltaPhiN2;
			DeltaPhiN3 = sk->DeltaPhiN3;
			METPt = sk->METPt;
			METPhi = sk->METPhi;
			*ak4Jets = *(sk->ak4Jets);
			
			//set to noPhotons vars
			sk->NJets = sk->NJetsNoPhotons;
			sk->HT = sk->HTnoPhotons;
			sk->MHT = sk->MHTnoPhotons;
			sk->DeltaPhi1 = sk->DeltaPhi1noPhotons;
			sk->DeltaPhi2 = sk->DeltaPhi2noPhotons;
			sk->DeltaPhi3 = sk->DeltaPhi3noPhotons;
			sk->minDeltaPhiN = sk->minDeltaPhiNnoPhotons;
			sk->DeltaPhiN1 = sk->DeltaPhiN1noPhotons;
			sk->DeltaPhiN2 = sk->DeltaPhiN2noPhotons;
			sk->DeltaPhiN3 = sk->DeltaPhiN3noPhotons;
			sk->METPt = sk->METnoPhotonsPt;
			sk->METPhi = sk->METnoPhotonsPhi;
			*(sk->ak4Jets) = *(sk->ak4JetsNoPhotons);
		}
		virtual void UndoVariation() {
			//restore original values
			sk->NJets = NJets;
			sk->HT = HT;
			sk->MHT = MHT;
			sk->DeltaPhi1 = DeltaPhi1;
			sk->DeltaPhi2 = DeltaPhi2;
			sk->DeltaPhi3 = DeltaPhi3;
			sk->minDeltaPhiN = minDeltaPhiN;
			sk->DeltaPhiN1 = DeltaPhiN1;
			sk->DeltaPhiN2 = DeltaPhiN2;
			sk->DeltaPhiN3 = DeltaPhiN3;
			sk->METPt = METPt;
			sk->METPhi = METPhi;
			*(sk->ak4Jets) = *ak4Jets;
		}
		
		//member variables
		Int_t           NJets;
		Float_t         HT;
		Float_t         MHT;
		Float_t         DeltaPhi1;
		Float_t         DeltaPhi2;
		Float_t         DeltaPhi3;
		Float_t         minDeltaPhiN;
		Float_t         DeltaPhiN1;
		Float_t         DeltaPhiN2;
		Float_t         DeltaPhiN3;
		Float_t         METPt;
		Float_t         METPhi;
		vector<TLorentzVector> *ak4Jets;
};

namespace KParser {
	KVariator* processVariator(KNamed* tmp){
		KVariator* vtmp = 0;
		string vname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known variators
		if(vname=="NoPhoton") vtmp = new KNoPhotonVariator(vname,omap);
		//else if(vname=="JES") vtmp = new KJetESVariator(vname,omap);
		//else if(vname=="JER") vtmp = new KJetERVariator(vname,omap);
		else {} //skip unknown variators

		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif