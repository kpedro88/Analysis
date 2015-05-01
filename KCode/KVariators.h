#ifndef KVARIATORS_H
#define KVARIATORS_H

//custom headers
#include "KVariation.h"
#include "KSkimmer.h"

//ROOT headers

//STL headers
#include <string>
#include <vector>

using namespace std;

//base class for variators is in KVariation.h

/*
//----------------------------------------------------
//variation of tau energy scale
class KTauESVariator : public KVariator {
	public:
		//constructor
		KTauESVariator() : KVariator() { }
		KTauESVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_) { 
			//check options
			up = localOpt->Get("up",true);
			//set default values
			HPSTauPt = 0;
		}
		//functions
		virtual void DoVariation() {
			delete HPSTauPt; HPSTauPt = new vector<double>();
			HPSTauPt->reserve(sk->HPSTauPt->size());
			double unc = 0.03;
			
			for(unsigned t = 0; t < sk->HPSTauPt->size(); t++){
				//store original values
				HPSTauPt->push_back(sk->HPSTauPt->at(t));
				
				//Scale tau pT by TauES uncertainty
				TLorentzVector v_tau;
				v_tau.SetPtEtaPhiM(sk->HPSTauPt->at(t),sk->HPSTauEta->at(t),sk->HPSTauPhi->at(t),sk->TauMass);
			
				if(up) v_tau *= (1+unc);
				else v_tau *= (1-unc);
				
				sk->HPSTauPt->at(t) = v_tau.Pt();
			}
		}
		virtual void UndoVariation() {
			//restore original values
			for(unsigned t = 0; t < sk->HPSTauPt->size(); t++){
				sk->HPSTauPt->at(t) = HPSTauPt->at(t);
			}
		}
		
		//member variables
		bool up;
		vector<double> *HPSTauPt;
};

//----------------------------------------------------
//variation of tau energy resolution
class KTauERVariator : public KVariator {
	public:
		//constructor
		KTauERVariator() : KVariator() { }
		KTauERVariator(string name_, OptionMap* localOpt_) : KVariator(name_,localOpt_) { 
			//check options
			up = localOpt->Get("up",true);
			//set default values
			HPSTauPt = 0;
		}
		//functions
		virtual void DoVariation() {			
			delete HPSTauPt; HPSTauPt = new vector<double>();
			HPSTauPt->reserve(sk->HPSTauPt->size());
			double unc = 0.1;
			
			for(unsigned t = 0; t < sk->HPSTauPt->size(); t++){
				//store original values
				HPSTauPt->push_back(sk->HPSTauPt->at(t));
				
				//Tau ER smearing
				double min_dR = 1000;
				bool matchedGenJetFound = false;
				int index_gen = -1;

				for (unsigned g = 0; g < sk->GenJetPt->size(); g++){
					double dR = KMath::DeltaR(sk->HPSTauPhi->at(t),sk->HPSTauEta->at(t),sk->GenJetPhi->at(g),sk->GenJetEta->at(g));
					if(dR < min_dR) {
						min_dR = dR;
						index_gen = g;
					}
				}
				if (min_dR < 0.05) matchedGenJetFound = true;

				if (matchedGenJetFound) {
					TLorentzVector v_tau, vg;
					v_tau.SetPtEtaPhiM(sk->HPSTauPt->at(t),sk->HPSTauEta->at(t),sk->HPSTauPhi->at(t),sk->TauMass);
					vg.SetPtEtaPhiE(sk->GenJetPt->at(index_gen),sk->GenJetEta->at(index_gen),sk->GenJetPhi->at(index_gen),sk->GenJetEnergy->at(index_gen));
				
					double f = 0;
					if(up) f = max(vg.E() + (1+unc)*(v_tau.E()-vg.E()),0.)/v_tau.E();
					else f = max(vg.E() + (1-unc)*(v_tau.E()-vg.E()),0.)/v_tau.E();
					
					v_tau *= f;
					sk->HPSTauPt->at(t) = v_tau.Pt();
				}
			}
		}
		virtual void UndoVariation() {
			//restore original values
			for(unsigned t = 0; t < sk->HPSTauPt->size(); t++){
				sk->HPSTauPt->at(t) = HPSTauPt->at(t);
			}
		}
		
		//member variables
		bool up;
		vector<double> *HPSTauPt;
};

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

namespace KParser {
	KVariator* processVariator(KNamed* tmp){
		KVariator* vtmp = 0;
		string vname = tmp->first;
		OptionMap* omap = tmp->second;
		/*
		//check for all known variators
		if(vname=="TauES") vtmp = new KTauESVariator(vname,omap);
		else if(vname=="TauER") vtmp = new KTauERVariator(vname,omap);
		else if(vname=="JES") vtmp = new KJetESVariator(vname,omap);
		else if(vname=="JER") vtmp = new KJetERVariator(vname,omap);
		else {} //skip unknown variators
		*/
		if(!vtmp) cout << "Input error: unknown variator " << vname << ". This variator will be skipped." << endl;

		return vtmp;
	}
}

#endif