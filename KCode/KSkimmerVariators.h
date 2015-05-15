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
class KNoPhotonVariator : public KVariator<KSkimmer> {
	public:
		//constructor
		KNoPhotonVariator() : KVariator<KSkimmer>() { }
		KNoPhotonVariator(string name_, OptionMap* localOpt_) : KVariator<KSkimmer>(name_,localOpt_), ak4Jets(NULL) { }
		//functions
		virtual void DoVariation() {
			//initialize
			ak4Jets = new vector<TLorentzVector>();
			
			//store original values
			NJets = looper->NJets;
			HT = looper->HT;
			MHT = looper->MHT;
			DeltaPhi1 = looper->DeltaPhi1;
			DeltaPhi2 = looper->DeltaPhi2;
			DeltaPhi3 = looper->DeltaPhi3;
			minDeltaPhiN = looper->minDeltaPhiN;
			DeltaPhiN1 = looper->DeltaPhiN1;
			DeltaPhiN2 = looper->DeltaPhiN2;
			DeltaPhiN3 = looper->DeltaPhiN3;
			METPt = looper->METPt;
			METPhi = looper->METPhi;
			*ak4Jets = *(looper->ak4Jets);
			
			//set to noPhotons vars
			looper->NJets = looper->NJetsNoPhotons;
			looper->HT = looper->HTnoPhotons;
			looper->MHT = looper->MHTnoPhotons;
			looper->DeltaPhi1 = looper->DeltaPhi1noPhotons;
			looper->DeltaPhi2 = looper->DeltaPhi2noPhotons;
			looper->DeltaPhi3 = looper->DeltaPhi3noPhotons;
			looper->minDeltaPhiN = looper->minDeltaPhiNnoPhotons;
			looper->DeltaPhiN1 = looper->DeltaPhiN1noPhotons;
			looper->DeltaPhiN2 = looper->DeltaPhiN2noPhotons;
			looper->DeltaPhiN3 = looper->DeltaPhiN3noPhotons;
			looper->METPt = looper->METnoPhotonsPt;
			looper->METPhi = looper->METnoPhotonsPhi;
			*(looper->ak4Jets) = *(looper->ak4JetsNoPhotons);
		}
		virtual void UndoVariation() {
			//restore original values
			looper->NJets = NJets;
			looper->HT = HT;
			looper->MHT = MHT;
			looper->DeltaPhi1 = DeltaPhi1;
			looper->DeltaPhi2 = DeltaPhi2;
			looper->DeltaPhi3 = DeltaPhi3;
			looper->minDeltaPhiN = minDeltaPhiN;
			looper->DeltaPhiN1 = DeltaPhiN1;
			looper->DeltaPhiN2 = DeltaPhiN2;
			looper->DeltaPhiN3 = DeltaPhiN3;
			looper->METPt = METPt;
			looper->METPhi = METPhi;
			*(looper->ak4Jets) = *ak4Jets;
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
	template <>
	KVariator<KSkimmer>* processVariator<KSkimmer>(KNamed* tmp){
		KVariator<KSkimmer>* vtmp = 0;
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