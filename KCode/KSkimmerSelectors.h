#ifndef KSKIMMERSELECTORS_H
#define KSKIMMERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KSkimmer.h"
#include "KMath.h"
#include "KPlot.h"
#include "../corrections/EventListFilter.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TMath.h>

//STL headers
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

//implementation of NtupleClass-member-dependent KSkimmer function
int KSkimmer::GetEventSign() {
	if(Weight<0) return -1;
	else return 1;
}

//base class for Selectors is in KSelection.h

//----------------------------------------------------
//selects events based on HLT line
class KHLTSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KHLTSelector() : KSelector<KSkimmer>() { }
		KHLTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { 
			//get selected line from options
			localOpt->Get("HLTLines",HLTLines);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//skip if no line provided
			if(HLTLines.size()==0) return true;
			
			//initial loop over trigger names to find indices (position is consistent for all events)
			if(HLTIndices.empty()){
				for(unsigned h = 0; h < HLTLines.size(); h++){
					vector<string>::iterator lb = lower_bound(looper->TriggerNames->begin(),looper->TriggerNames->end(),HLTLines[h]);
					if(lb != looper->TriggerNames->end() && lb->find(HLTLines[h]) != std::string::npos){
						HLTIndices.push_back(distance(looper->TriggerNames->begin(),lb));
					}
				}
			}
			
			//loop over trigger names
			bool goodTrigger = false;
			for(unsigned h = 0; h < HLTIndices.size(); h++){
				unsigned index = HLTIndices[h];
				//check:
				//1) if the decision was true (the line fired)
				//2) if the line was not prescaled (currently ignored)
				if(looper->TriggerPass->at(index)==1) {
					goodTrigger = true;
					break;
				}
			}
			//skip event if finished searching and no HLT lines found
			return goodTrigger;
		}
		
		//member variables
		vector<string> HLTLines;
		vector<unsigned> HLTIndices;
};

//------------------------------------------------------
//selects events based on run number (for blinding data)
class KBlindSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KBlindSelector() : KSelector<KSkimmer>() { }
		KBlindSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), firstUnblindRun(0), lastUnblindRun(257599) { 
			//check for option
			localOpt->Get("firstUnblindRun",firstUnblindRun);
			localOpt->Get("lastUnblindRun",lastUnblindRun);
		}
		virtual void CheckLooper(){
			//check if data
			bool data = looper->MyBase->GetLocalOpt()->Get("data",false);
			//disable this for non-data
			if(!data) dummy = true;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return (firstUnblindRun==-1 or looper->RunNum >= firstUnblindRun) and (lastUnblindRun==-1 or looper->RunNum <= lastUnblindRun);
		}
		
		//member variables
		int firstUnblindRun, lastUnblindRun;
};

//----------------------------------------------------
//selects events based on number of jets
class KNJetSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KNJetSelector() : KSelector<KSkimmer>() { }
		KNJetSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), njet(4) { 
			//check for option
			localOpt->Get("njet",njet);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->NJets >= njet;
		}
		
		//member variables
		int njet;
};

//----------------------------------------------------
//selects events based on HT value
class KHTSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KHTSelector() : KSelector<KSkimmer>() { }
		KHTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), HTmin(500) { 
			//check for option
			localOpt->Get("HTmin",HTmin);
			doGen = localOpt->Get("gen",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(doGen) return looper->GenHT > HTmin;
			else return looper->HT > HTmin;
		}
		
		//member variables
		double HTmin;
		bool doGen;
};

//----------------------------------------------------
//selects events based on MHT value
class KMHTSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMHTSelector() : KSelector<KSkimmer>() { }
		KMHTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), MHTmin(200) { 
			//check for option
			localOpt->Get("MHTmin",MHTmin);
			doGen = localOpt->Get("gen",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(doGen) return looper->GenMHT > MHTmin;
			else return looper->MHT > MHTmin;
		}
		
		//member variables
		double MHTmin;
		bool doGen;
};

//----------------------------------------------------
//low MHT selection for single photon test
class KLowMHTSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KLowMHTSelector() : KSelector<KSkimmer>() { }
		KLowMHTSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), MHTmax(200) { 
			//check for option
			localOpt->Get("MHTmax",MHTmax);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->MHTclean < MHTmax;
		}
		
		//member variables
		double MHTmax;
};

//-------------------------------------------------------------
//vetos events with muons
class KMuonVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMuonVetoSelector() : KSelector<KSkimmer>() { }
		KMuonVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->Muons->size()==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with electrons
class KElectronVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KElectronVetoSelector() : KSelector<KSkimmer>() { }
		KElectronVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->Electrons->size()==0;
		}
		
		//member variables
};

//------------------------------------------------------
//single muon selector
class KMuonSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMuonSelector() : KSelector<KSkimmer>() { }
		KMuonSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			//check for option
			doMTcut = localOpt->Get("doMTcut",true);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->Muons->size()!=1) return false;
			
			if(doMTcut){
				double mT = KMath::TransverseMass(looper->MET,looper->METPhi,looper->Muons->at(0).Pt(),looper->Muons->at(0).Phi());
				return mT<100;
			}
			else return true;
		}
		
		//member variables
		bool doMTcut;
};

//------------------------------------------------------
//single electron selector
class KElectronSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KElectronSelector() : KSelector<KSkimmer>() { }
		KElectronSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			//check for option
			doMTcut = localOpt->Get("doMTcut",true);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->Electrons->size()!=1) return false;
			
			if(doMTcut){
				double mT = KMath::TransverseMass(looper->MET,looper->METPhi,looper->Electrons->at(0).Pt(),looper->Electrons->at(0).Phi());
				return mT<100;
			}
			else return true;
		}
		
		//member variables
		bool doMTcut;
};

//------------------------------------------------------
//single photon selector
class KPhotonSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KPhotonSelector() : KSelector<KSkimmer>() { }
		KPhotonSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			//check option
			loose = localOpt->Get("loose",true);
			veto = localOpt->Get("veto",false);
			trigger = localOpt->Get("trigger",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			int NumPhotons = 0;
			if(loose){
				NumPhotons = looper->Photons->size();
			}
			else{
				//tighten up ID
				for(unsigned p = 0; p < looper->Photons->size(); ++p){
					if(looper->Photons_fullID->at(p) and (!trigger || looper->Photons->at(p).Pt()>200)) ++NumPhotons;
				}
			}
			
			return NumPhotons==(veto? 0 : 1);
		}
		
		//member variables
		bool loose;
		bool veto;
		bool trigger;
};

//------------------------------------------------------
//dimuon selector
class KDiMuonSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KDiMuonSelector() : KSelector<KSkimmer>() { }
		KDiMuonSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//todo: add mass cut?
			return looper->Muons->size() == 2 && looper->Muons_charge->at(0) != looper->Muons_charge->at(1);
		}
		
		//member variables
};

//------------------------------------------------------
//dielectron selector
class KDiElectronSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KDiElectronSelector() : KSelector<KSkimmer>() { }
		KDiElectronSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//todo: add mass cut?
			return looper->Electrons->size() == 2 && looper->Electrons_charge->at(0) != looper->Electrons_charge->at(1);
		}
		
		//member variables
};

//------------------------------------------------------
//muon selector for trigger study
class KMuonTriggerSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMuonTriggerSelector() : KSelector<KSkimmer>() { }
		KMuonTriggerSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			//check for option
			localOpt->Get("pTmin",pTmin);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			int num = 0;
			for(unsigned m = 0; m < looper->Muons->size(); ++m){
				if(looper->Muons->at(m).Pt()>pTmin) ++num;
			}

			return num>0;
		}
		
		//member variables
		double pTmin;
};

//------------------------------------------------------
//electron selector for trigger study
class KElectronTriggerSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KElectronTriggerSelector() : KSelector<KSkimmer>() { }
		KElectronTriggerSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			//check for option
			localOpt->Get("pTmin",pTmin);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			int num = 0;
			for(unsigned e = 0; e < looper->Electrons->size(); ++e){
				if(looper->Electrons->at(e).Pt()>pTmin) ++num;
			}

			return num>0;
		}
		
		//member variables
		double pTmin;
};

//----------------------------------------------------
//selects events based on minDeltaPhi value
class KMinDeltaPhiSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KMinDeltaPhiSelector() : KSelector<KSkimmer>() { }
		KMinDeltaPhiSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), minDeltaPhi(0.4), invert(false) { 
			//check for option
			localOpt->Get("minDeltaPhi",minDeltaPhi);
			invert = localOpt->Get("invert",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//min delta phi needs to be defined
			double looperMinDeltaPhi = min(looper->DeltaPhi1,min(looper->DeltaPhi2,looper->DeltaPhi3));
			if(invert) return looperMinDeltaPhi < minDeltaPhi;
			else return looperMinDeltaPhi > minDeltaPhi;
		}
		
		//member variables
		double minDeltaPhi;
		bool invert;
};

//----------------------------------------------------
//selects events based on DeltaPhi values
class KDeltaPhiSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KDeltaPhiSelector() : KSelector<KSkimmer>() { }
		KDeltaPhiSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), DeltaPhi(4,0), invert(false) { 
			//check for option
			DeltaPhi[0] = 0.5; DeltaPhi[1] = 0.5; DeltaPhi[2] = 0.3; DeltaPhi[3] = 0.3;
			localOpt->Get("DeltaPhi",DeltaPhi);
			invert = localOpt->Get("invert",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(invert) return looper->DeltaPhi1 < DeltaPhi[0] || looper->DeltaPhi2 < DeltaPhi[1] || looper->DeltaPhi3 < DeltaPhi[2] || looper->DeltaPhi4 < DeltaPhi[3];
			else return looper->DeltaPhi1 > DeltaPhi[0] && looper->DeltaPhi2 > DeltaPhi[1] && looper->DeltaPhi3 > DeltaPhi[2] && looper->DeltaPhi4 > DeltaPhi[3];
		}
		
		//member variables
		vector<double> DeltaPhi;
		bool invert;
};

//----------------------------------------------------
//selects events based on single DeltaPhi value
class KDeltaPhiJSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KDeltaPhiJSelector() : KSelector<KSkimmer>() { }
		KDeltaPhiJSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), DeltaPhi(0.5), jet(1) { 
			//check for option
			localOpt->Get("DeltaPhi",DeltaPhi);
			localOpt->Get("jet",jet);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(jet==1) return looper->DeltaPhi1 > DeltaPhi;
			else if(jet==2) return looper->DeltaPhi2 > DeltaPhi;
			else if(jet==3) return looper->DeltaPhi3 > DeltaPhi;
			else if(jet==4) return looper->DeltaPhi4 > DeltaPhi;
			else return true;
		}
		
		//member variables
		double DeltaPhi;
		int jet;
};

//-------------------------------------------------------------
//vetos events with isolated electron tracks
class KIsoElectronTrackVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KIsoElectronTrackVetoSelector() : KSelector<KSkimmer>() { }
		KIsoElectronTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoElectronTracks==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with isolated muon tracks
class KIsoMuonTrackVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KIsoMuonTrackVetoSelector() : KSelector<KSkimmer>() { }
		KIsoMuonTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoMuonTracks==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with isolated charged hadron tracks
class KIsoPionTrackVetoSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KIsoPionTrackVetoSelector() : KSelector<KSkimmer>() { }
		KIsoPionTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoPionTracks==0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//vetos events with bad jets (using PFJetID loose WP)
class KEventCleaningSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KEventCleaningSelector() : KSelector<KSkimmer>() { }
		KEventCleaningSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { 
			//check for option
			doJetID = localOpt->Get("JetID",false);
			doMETRatio = localOpt->Get("METRatio",false);
			doMuonJet = localOpt->Get("MuonJet",false);
			doFakeJet = localOpt->Get("FakeJet",false);
		}
		virtual void CheckLooper(){
			//check if fastsim
			bool fastsim = looper->MyBase->GetLocalOpt()->Get("fastsim",false);
			//disable JetID for fastsim
			if(fastsim) doJetID = false;
			//disable FakeJet for non-fastsim
			if(!fastsim) doFakeJet = false;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool goodEvent = (!doJetID || looper->JetID) &&
							 (!doMETRatio || looper->PFCaloMETRatio < 5);
			if(doMuonJet){
				bool noMuonJet = true;
				for(unsigned j = 0; j < looper->Jets->size(); ++j){
					if(looper->Jets->at(j).Pt() > 200 && looper->Jets_muonEnergyFraction->at(j) > 0.5 && KMath::DeltaPhi(looper->Jets->at(j).Phi(),looper->METPhi) > (TMath::Pi() - 0.4)){
						noMuonJet = false;
						break;
					}
				}
				goodEvent &= noMuonJet;
			}
			if(doFakeJet){
				bool noFakeJet = true;
				//reject events with any jet pt>20, |eta|<2.5 NOT matched to a GenJet (w/in DeltaR<0.3) and chfrac < 0.1
				for(unsigned j = 0; j < looper->Jets->size(); ++j){
					if(looper->Jets->at(j).Pt() <= 20 || fabs(looper->Jets->at(j).Eta())>=2.5) continue;
					bool genMatched = false;
					for(unsigned g = 0; g < looper->GenJets->size(); ++g){
						if(looper->GenJets->at(g).DeltaR(looper->Jets->at(j)) < 0.3) {
							genMatched = true;
							break;
						}
					}
					if(!genMatched && looper->Jets_chargedHadronEnergyFraction->at(j) < 0.1){
						noFakeJet = false;
						break;
					}
				}
				goodEvent &= noFakeJet;
			}
			return goodEvent;
		}
		
		//member variables
		bool doJetID, doMETRatio, doMuonJet, doFakeJet;
};

//-------------------------------------------------------------
//selects based on nvtx
class KNVtxSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KNVtxSelector() : KSelector<KSkimmer>() { }
		KNVtxSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), invert(false), num(20) {
			invert = localOpt->Get("invert",false);
			localOpt->Get("num",num);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return invert ? looper->NVtx < num : looper->NVtx >= num;
		}
		
		//member variables
		bool invert;
		int num;
};

//----------------------------------------------------
//selects events based on number of b-jets
class KNBJetSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KNBJetSelector() : KSelector<KSkimmer>() { }
		KNBJetSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), nbjet_min(-1), nbjet_max(-1) { 
			//check for option
			localOpt->Get("nbjet_min",nbjet_min);
			localOpt->Get("nbjet_max",nbjet_max);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return ( (nbjet_min==-1 || looper->BTags >= nbjet_min) && (nbjet_max==-1 || looper->BTags <= nbjet_max) );
		}
		
		//member variables
		int nbjet_min, nbjet_max;
};

//----------------------------------------------------
//selects events based on genHT bin
class KGenHTBinSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KGenHTBinSelector() : KSelector<KSkimmer>() { }
		KGenHTBinSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) { 
			//check for option
			localOpt->Get("bins",genHTbins);
			count = vector<unsigned>(genHTbins.size()-1, 0);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			for(unsigned g = 0; g < genHTbins.size()-1; ++g){
				if(looper->madHT > genHTbins[g] && (looper->madHT < genHTbins[g+1] || genHTbins[g+1]==-1)) {
					count[g] += 1;
					break;
				}
			}
			return true;
		}
		
		virtual void Finalize(TFile* file){
			unsigned total = 0;
			for(unsigned c = 0; c < count.size(); ++c){
				total += count[c];
			}
			
			for(unsigned g = 0; g < genHTbins.size()-1; ++g){
				cout << genHTbins[g] << " - " << genHTbins[g+1] << " : " << double(count[g])/double(total)*100 << "%" << endl;
			}
		}
		
		//member variables
		vector<double> genHTbins;
		vector<unsigned> count;
};

//----------------------------------------------------
//calculate b-tagging & mistagging efficiencies
class KBTagEfficiencySelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KBTagEfficiencySelector() : KSelector<KSkimmer>() { }
		KBTagEfficiencySelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_) {
			canfail = false;
			//initialize histograms using KPlot2D::CreateHist() method
			TH1::AddDirectory(kFALSE);
			KPlot2D* ptmp = new KPlot2D("n_eff_b","",localOpt,NULL);
			ptmp->CreateHist();
			n_eff_b = (TH2F*)ptmp->GetHisto();
			delete ptmp;
			//reset z-title
			n_eff_b->GetZaxis()->SetTitle("");
			//clone histo
			n_eff_c = (TH2F*)n_eff_b->Clone("n_eff_c");
			n_eff_udsg = (TH2F*)n_eff_b->Clone("n_eff_udsg");
			d_eff_b = (TH2F*)n_eff_b->Clone("d_eff_b");
			d_eff_c = (TH2F*)n_eff_b->Clone("d_eff_c");
			d_eff_udsg = (TH2F*)n_eff_b->Clone("d_eff_udsg");
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//loop over jets
			for(unsigned ja = 0; ja < looper->Jets->size(); ++ja){
				//HT jet cuts
				if(!looper->Jets_HTMask->at(ja)) continue;
				
				//fill by flavor
				int flav = abs(looper->Jets_hadronFlavor->at(ja));
				double csv = looper->Jets_bDiscriminatorCSV->at(ja);
				double pt = looper->Jets->at(ja).Pt();
				//use abs(eta) for now
				double eta = fabs(looper->Jets->at(ja).Eta());
				if(flav==5){
					d_eff_b->Fill(pt,eta);
					if(csv > 0.800) n_eff_b->Fill(pt,eta);
				}
				else if(flav==4){
					d_eff_c->Fill(pt,eta);
					if(csv > 0.800) n_eff_c->Fill(pt,eta);
				}
				else if(flav<4 || flav==21){
					d_eff_udsg->Fill(pt,eta);
					if(csv > 0.800) n_eff_udsg->Fill(pt,eta);
				}
			}
			
			return true;
		}
		
		virtual void Finalize(TFile* file){
			if(localOpt->Get("saveAll",false)){
				//write to file
				file->cd();
				d_eff_b->Write();
				d_eff_c->Write();
				d_eff_udsg->Write();
				n_eff_b->Write();
				n_eff_c->Write();
				n_eff_udsg->Write();
			}
			else{		
				//write numer and denom to file (in case of hadding)
				file->cd();
				n_eff_b->Write();
				d_eff_b->Write();
				n_eff_c->Write();
				d_eff_c->Write();
				n_eff_udsg->Write();
				d_eff_udsg->Write();
			}
		}
		
		//member variables
		TH2F *n_eff_b, *n_eff_c, *n_eff_udsg;
		TH2F *d_eff_b, *d_eff_c, *d_eff_udsg;
};

//-----------------------------------------------------------------
//stores the NJetISR distribution
class KNJetsISRSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KNJetsISRSelector() : KSelector<KSkimmer>() { }
		KNJetsISRSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), h_njetsisr(NULL) {
			canfail = false;
			localOpt->Get("xbins",xbins);
			//initialize histograms using KPlot:CreateHist() method
			TH1::AddDirectory(kFALSE);
			KPlot* ptmp = new KPlot("NJetsISR",localOpt,NULL);
			ptmp->CreateHist();
			h_njetsisr = ptmp->GetHisto();
			delete ptmp;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//fill histo
			h_njetsisr->Fill(min(double(looper->NJetsISR),xbins[xbins.size()-2]));
			
			return true;
		}
		
		virtual void Finalize(TFile* file){
			//write to file
			file->cd();
			h_njetsisr->Write();
		}
		
		//member variables
		TH1 *h_njetsisr;
		vector<double> xbins;
};

//-----------------------------------------------------------------
//stores info for normalizing PDF and r/f scale uncertainties
class KPDFNormSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KPDFNormSelector() : KSelector<KSkimmer>() { }
		KPDFNormSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), h_norm(NULL) {
			canfail = false;
			//initialize histogram
			TH1::AddDirectory(kFALSE);
			//bins:
			//1: nominal, 2: PDF up, 3: PDF down, 4: scale up, 5: scale down
			h_norm = new TH1F("PDFNorm","",5,0.5,5.5);
		}
		virtual void CheckLooper(){
			//check if data
			bool data = looper->MyBase->GetLocalOpt()->Get("data",false);
			//disable this for data
			if(data) {
				dummy = true;
				delete h_norm;
			}
		}

		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//get weights
			
			//nominal
			h_norm->Fill(1);
			if(looper->PDFweights->size()>0){
				//PDF up
				h_norm->Fill(2,*(TMath::LocMax(looper->PDFweights->begin(),looper->PDFweights->end())));
				//PDF down
				h_norm->Fill(3,*(TMath::LocMin(looper->PDFweights->begin(),looper->PDFweights->end())));
			}
			
			if(looper->ScaleWeights->size()>0){
				vector<double> ScaleWeightsMod = *looper->ScaleWeights;
				//remove unwanted variations
				if(ScaleWeightsMod.size()>7) ScaleWeightsMod.erase(ScaleWeightsMod.begin()+7);
				if(ScaleWeightsMod.size()>5) ScaleWeightsMod.erase(ScaleWeightsMod.begin()+5);
				if(ScaleWeightsMod.size()>0) ScaleWeightsMod.erase(ScaleWeightsMod.begin());
				
				//scale up
				h_norm->Fill(4,*(TMath::LocMax(ScaleWeightsMod.begin(),ScaleWeightsMod.end())));
				//scale down
				h_norm->Fill(5,*(TMath::LocMin(ScaleWeightsMod.begin(),ScaleWeightsMod.end())));
			}
			
			return true;
		}
		
		virtual void Finalize(TFile* file){
			if(dummy) return;
			//write to file
			file->cd();
			h_norm->Write();
		}
		
		//member variables
		TH1F *h_norm;
};

//-------------------------------------------------------------
//look at only a specific range of events
class KEventRangeSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KEventRangeSelector() : KSelector<KSkimmer>() { }
		KEventRangeSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), start(0), finish(0) { 
			//check for option
			localOpt->Get("start",start);
			localOpt->Get("finish",finish);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(finish>=start && (looper->EvtNum < start || looper->EvtNum > finish)) return false;
			else return true;
		}
		
		//member variables
		unsigned start, finish;
};

//---------------------------------------------------------------
//updates CSC filter
class KCSCFilterSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KCSCFilterSelector() : KSelector<KSkimmer>() { }
		KCSCFilterSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), inputfile("") {
			canfail = false;
		}
		virtual void CheckLooper(){
			//check if data
			bool data = looper->MyBase->GetLocalOpt()->Get("data",false);
			//disable this for non-data
			if(!data) dummy = true;
			else {
				localOpt->Get("inputfile",inputfile);
				if(inputfile.size()>0){
					string gunzip_cmd = "gunzip -c "+inputfile+".gz > "+inputfile;
					system(gunzip_cmd.c_str());
					filter = EventListFilter(inputfile);
				}
			}
		}

		//used for non-dummy selectors
		virtual bool Cut() {
			if(filter.Initialized()) looper->CSCTightHaloFilter = filter.CheckEvent(looper->RunNum,looper->LumiBlockNum,looper->EvtNum);
			return true;
		}
		
		//member variables
		string inputfile;
		EventListFilter filter;
};

//-------------------------------------------------------------
//special version of selector class for object sync
class KSyncSelector : public KSelector<KSkimmer> {
	public:
		//constructor
		KSyncSelector() : KSelector<KSkimmer>() { }
		KSyncSelector(string name_, OptionMap* localOpt_) : KSelector<KSkimmer>(name_,localOpt_), depname(""), prevSel(NULL), obj_counter(0), debug(false) { 
			//check for option
			localOpt->Get("dep",depname);
		}
		
		//override accessors
		virtual void CheckDeps(){
			debug = sel->GetGlobalOpt()->Get("debug",false);
			//set dependencies here
			if(depname.size()>0){
				prevSel = sel->Get<KSyncSelector*>(depname);
				if(!prevSel){
					cout << "Input error: dependency " << depname << " failed in " << name << "!" << endl;
					depfailed = true;
				}
			}
		}
		virtual bool Select(){
			if(depfailed) return false;
			bool result = dummy || Cut();
			if(result) counter++;
			return result;
		}
		virtual void GetEfficiency(vector<KCutflowEntry>& cuts){
			if(dummy || !canfail) return;
			cuts.push_back(KCutflowEntry(name, counter, KMath::PoissonErrorUp(counter), obj_counter, KMath::PoissonErrorUp(obj_counter)));
		}
		
		//member variables
		vector<unsigned> goodObjects;
		string depname;
		KSyncSelector* prevSel;
		
	protected:
		//member variables
		int obj_counter;
		bool debug;
};

//---------------------------------------------------------------
//eta regions for PFJetID: 0 = 0.0 < |eta| < 2.4; 1 = 0.0 < |eta| < 3.0; 2 = 3.0 < |eta|
//all require pt > 30
class KJetEtaRegionSelector : public KSyncSelector {
	public:
		//constructor
		KJetEtaRegionSelector() : KSyncSelector() { }
		KJetEtaRegionSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), region(0) { 
			localOpt->Get("region",region);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//reset mask
			goodObjects.clear();
			//check eta for each jet
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(looper->Jets->at(j).Pt() <= 30) continue;
				
				if(region==-1) { goodObjects.push_back(j); ++obj_counter; }
				else if(region==0 && fabs(looper->Jets->at(j).Eta()) <= 2.4) { goodObjects.push_back(j); ++obj_counter; }
				else if(region==1 && fabs(looper->Jets->at(j).Eta()) > 2.4 && fabs(looper->Jets->at(j).Eta()) <= 3.0) { goodObjects.push_back(j); ++obj_counter; }
				else if(region==2 && fabs(looper->Jets->at(j).Eta()) > 3.0) { goodObjects.push_back(j); ++obj_counter; }
			}
			if(goodObjects.size()==0) { if(debug) { cout << "0 jets passed ID" << endl; } return false; }
			else return true;
		}
		
		//member variables
		int region;
};

//---------------------------------------------------------------
//neutral hadron fraction selector for jet ID check
class KNeutralHadronFractionSelector : public KSyncSelector {
	public:
		//constructor
		KNeutralHadronFractionSelector() : KSyncSelector() { }
		KNeutralHadronFractionSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned j = 0; j < prevSel->goodObjects.size(); ++j){
				unsigned jj = prevSel->goodObjects[j];
				bool goodJet = true;
				if(fabs(looper->Jets->at(jj).Eta())<=3.0) goodJet = looper->Jets_neutralHadronEnergyFraction->at(jj) < 0.99;
				if(goodJet) {
					goodObjects.push_back(jj);
					++obj_counter;
				}
			}
			if(goodObjects.size()<prevSel->goodObjects.size()) { if(debug) { cout << prevSel->goodObjects.size()-goodObjects.size() << " jets failed ID" << endl; } return false; }
			else return true;
		}
};

//---------------------------------------------------------------
//photon (neutral em) fraction selector for jet ID check
class KPhotonFractionSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonFractionSelector() : KSyncSelector() { }
		KPhotonFractionSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned j = 0; j < prevSel->goodObjects.size(); ++j){
				unsigned jj = prevSel->goodObjects[j];
				bool goodJet = true;
				if(fabs(looper->Jets->at(jj).Eta())<=3.0) goodJet = looper->Jets_photonEnergyFraction->at(jj) < 0.99;
				if(goodJet) {
					goodObjects.push_back(jj);
					++obj_counter;
				}
			}
			if(goodObjects.size()<prevSel->goodObjects.size()) { if(debug) { cout << prevSel->goodObjects.size()-goodObjects.size() << " jets failed ID" << endl; } return false; }
			else return true;
		}
};

//---------------------------------------------------------------
//num constit selector for jet ID check
class KNumConstitSelector : public KSyncSelector {
	public:
		//constructor
		KNumConstitSelector() : KSyncSelector() { }
		KNumConstitSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned j = 0; j < prevSel->goodObjects.size(); ++j){
				unsigned jj = prevSel->goodObjects[j];
				bool goodJet = true;
				if(fabs(looper->Jets->at(jj).Eta())<=3.0) goodJet = looper->Jets_chargedMultiplicity->at(jj)+looper->Jets_neutralMultiplicity->at(jj) > 1;
				if(goodJet) {
					goodObjects.push_back(jj);
					++obj_counter;
				}
			}
			if(goodObjects.size()<prevSel->goodObjects.size()) { if(debug) { cout << prevSel->goodObjects.size()-goodObjects.size() << " jets failed ID" << endl; } return false; }
			else return true;
		}
};

//---------------------------------------------------------------
//charged hadron fraction selector for jet ID check
class KChargedHadronFractionSelector : public KSyncSelector {
	public:
		//constructor
		KChargedHadronFractionSelector() : KSyncSelector() { }
		KChargedHadronFractionSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned j = 0; j < prevSel->goodObjects.size(); ++j){
				unsigned jj = prevSel->goodObjects[j];
				bool goodJet = true;
				if(fabs(looper->Jets->at(jj).Eta())<=2.4) goodJet = looper->Jets_chargedHadronEnergyFraction->at(jj) > 0.0;
				if(goodJet) {
					goodObjects.push_back(jj);
					++obj_counter;
				}
			}
			if(goodObjects.size()<prevSel->goodObjects.size()) { if(debug) { cout << prevSel->goodObjects.size()-goodObjects.size() << " jets failed ID" << endl; } return false; }
			else return true;
		}
};

//---------------------------------------------------------------
//charged mult selector for jet ID check
class KChargedMultSelector : public KSyncSelector {
	public:
		//constructor
		KChargedMultSelector() : KSyncSelector() { }
		KChargedMultSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned j = 0; j < prevSel->goodObjects.size(); ++j){
				unsigned jj = prevSel->goodObjects[j];
				bool goodJet = true;
				if(fabs(looper->Jets->at(jj).Eta())<=2.4) goodJet = looper->Jets_chargedMultiplicity->at(jj) > 0;
				if(goodJet) {
					goodObjects.push_back(jj);
					++obj_counter;
				}
			}
			if(goodObjects.size()<prevSel->goodObjects.size()) { if(debug) { cout << prevSel->goodObjects.size()-goodObjects.size() << " jets failed ID" << endl; } return false; }
			else return true;
		}
};

//---------------------------------------------------------------
//charged EM fraction selector for jet ID check
class KChargedEmFractionSelector : public KSyncSelector {
	public:
		//constructor
		KChargedEmFractionSelector() : KSyncSelector() { }
		KChargedEmFractionSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned j = 0; j < prevSel->goodObjects.size(); ++j){
				unsigned jj = prevSel->goodObjects[j];
				bool goodJet = true;
				if(fabs(looper->Jets->at(jj).Eta())<=2.4) goodJet = looper->Jets_chargedEmEnergyFraction->at(jj) < 0.99;
				if(goodJet) {
					goodObjects.push_back(jj);
					++obj_counter;
				}
			}
			if(goodObjects.size()<prevSel->goodObjects.size()) { if(debug) { cout << prevSel->goodObjects.size()-goodObjects.size() << " jets failed ID" << endl; } return false; }
			else return true;
		}
};

//---------------------------------------------------------------
//photon (neutral em) fraction selector for jet ID check - HF
class KPhotonFractionHFSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonFractionHFSelector() : KSyncSelector() { }
		KPhotonFractionHFSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned j = 0; j < prevSel->goodObjects.size(); ++j){
				unsigned jj = prevSel->goodObjects[j];
				bool goodJet = true;
				if(fabs(looper->Jets->at(jj).Eta())>3.0) goodJet = looper->Jets_photonEnergyFraction->at(jj) < 0.90;
				if(goodJet) {
					goodObjects.push_back(jj);
					++obj_counter;
				}
			}
			if(goodObjects.size()<prevSel->goodObjects.size()) { if(debug) { cout << prevSel->goodObjects.size()-goodObjects.size() << " jets failed ID" << endl; } return false; }
			else return true;
		}
};

//---------------------------------------------------------------
//neutral mult selector for jet ID check
class KNeutralMultSelector : public KSyncSelector {
	public:
		//constructor
		KNeutralMultSelector() : KSyncSelector() { }
		KNeutralMultSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { 
			localOpt->Get("cut",cut);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned j = 0; j < prevSel->goodObjects.size(); ++j){
				unsigned jj = prevSel->goodObjects[j];
				bool goodJet = true;
				if(fabs(looper->Jets->at(jj).Eta())>3.0) goodJet = looper->Jets_neutralMultiplicity->at(jj) > 10;
				if(goodJet) {
					goodObjects.push_back(jj);
					++obj_counter;
				}
			}
			if(goodObjects.size()<prevSel->goodObjects.size()) { if(debug) { cout << prevSel->goodObjects.size()-goodObjects.size() << " jets failed ID" << endl; } return false; }
			else return true;
		}
		
		//member variables
		int cut;
};

//-------------------------------------------------------------
//photon all selector for object sync
class KPhotonAllSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonAllSelector() : KSyncSelector() { }
		KPhotonAllSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < looper->Photons->size(); ++p){
				goodObjects.push_back(p);
				++obj_counter;
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
		
		//member variables
};

//-------------------------------------------------------------
//photon eta selector for object sync
class KPhotonEtaSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonEtaSelector() : KSyncSelector() { }
		KPhotonEtaSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }

		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < looper->Photons->size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = abs(looper->Photons->at(pp).Eta())<1.4442 || ((abs(looper->Photons->at(pp).Eta())>1.566 && abs(looper->Photons->at(pp).Eta())<2.5));
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
		
		//member variables
};

//-------------------------------------------------------------
//photon pt selector for object sync
class KPhotonPtSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonPtSelector() : KSyncSelector() { }
		KPhotonPtSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(debug) cout << "***** event " << looper->EvtNum << " *****" << endl;
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = looper->Photons->at(pp).Pt() > 100;
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
					if(debug) {
						cout << "Photon " << pp << ": pt " << looper->Photons->at(pp).Pt() << ", eta " << looper->Photons->at(pp).Eta() << endl;
						cout << "sieie " << looper->Photons_sigmaIetaIeta->at(pp) << ", hOverE " << looper->Photons_hadTowOverEM->at(pp) << ", hasPixelSeed " << looper->Photons_hasPixelSeed->at(pp) << endl;
						cout << "CH Iso " << looper->Photons_pfChargedIsoRhoCorr->at(pp) << ", NH Iso " << looper->Photons_pfNeutralIsoRhoCorr->at(pp) << ", PH Iso " << looper->Photons_pfGammaIsoRhoCorr->at(pp) << endl;
					}
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
};

//-------------------------------------------------------------
//photon ID selector for object sync
class KPhotonIDSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonIDSelector() : KSyncSelector() { }
		KPhotonIDSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_), vetoType(0) { 
			//check for option
			localOpt->Get("vetoType",vetoType);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				//common cut
				bool goodPhoton = true;
				if(vetoType==1) goodPhoton = !looper->Photons_hasPixelSeed->at(pp);
				else if(vetoType==2) goodPhoton = looper->Photons_passElectronVeto->at(pp);
				if(looper->Photons_isEB->at(pp)){ //barrel cuts
					goodPhoton &= looper->Photons_hadTowOverEM->at(pp) < 0.028 && looper->Photons_sigmaIetaIeta->at(pp) < 0.0107;
				}
				else { //endcap cuts
					goodPhoton &= looper->Photons_hadTowOverEM->at(pp) < 0.093 && looper->Photons_sigmaIetaIeta->at(pp) < 0.0272;
				}
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
					if(debug) {
						cout << "Photon " << pp << ": passed ID" << endl;
					}
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
		
		//member variables
		int vetoType;
};

//-------------------------------------------------------------
//photon CH iso selector for object sync
class KPhotonCHIsoSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonCHIsoSelector() : KSyncSelector() { }
		KPhotonCHIsoSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = false;
				if(looper->Photons_isEB->at(pp)){ //barrel cuts
					goodPhoton = looper->Photons_pfChargedIsoRhoCorr->at(pp) < 2.67;
				}
				else { //endcap cuts
					goodPhoton = looper->Photons_pfChargedIsoRhoCorr->at(pp) < 1.79;
				}
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
};

//-------------------------------------------------------------
//photon NH iso selector for object sync
class KPhotonNHIsoSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonNHIsoSelector() : KSyncSelector() { }
		KPhotonNHIsoSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = false;
				if(looper->Photons_isEB->at(pp)){ //barrel cuts
					goodPhoton = looper->Photons_pfNeutralIsoRhoCorr->at(pp) < 7.23 + exp(0.0028*looper->Photons->at(pp).Pt()+0.5408);
				}
				else { //endcap cuts
					goodPhoton = looper->Photons_pfNeutralIsoRhoCorr->at(pp) < 8.89 + 0.01725*looper->Photons->at(pp).Pt();
				}
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else return true;
		}
};

//-------------------------------------------------------------
//photon PH iso selector for object sync
class KPhotonPHIsoSelector : public KSyncSelector {
	public:
		//constructor
		KPhotonPHIsoSelector() : KSyncSelector() { }
		KPhotonPHIsoSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodObjects.clear();
			for(unsigned p = 0; p < prevSel->goodObjects.size(); ++p){
				unsigned pp = prevSel->goodObjects[p];
				bool goodPhoton = false;
				if(looper->Photons_isEB->at(pp)){ //barrel cuts
					goodPhoton = looper->Photons_pfGammaIsoRhoCorr->at(pp) < 2.11 + 0.0014*looper->Photons->at(pp).Pt();
				}
				else { //endcap cuts
					goodPhoton = looper->Photons_pfGammaIsoRhoCorr->at(pp) < 3.09 + 0.0091*looper->Photons->at(pp).Pt();
				}
				if(goodPhoton) {
					goodObjects.push_back(pp);
					++obj_counter;
					if(debug) {
						cout << "Photon " << pp << ": passed isolation" << endl;
					}
				}
			}
			if(goodObjects.size()==0) { if(debug) { cout << "found 0 photons" << endl; } return false; }
			else { if(debug){ cout << "found " << goodObjects.size() << " photons" << endl; } return true; }
		}
};

//-------------------------------------------------------------
//selects events with isolated electron tracks (for sync)
class KIsoElectronTrackSelector : public KSyncSelector {
	public:
		//constructor
		KIsoElectronTrackSelector() : KSyncSelector() { }
		KIsoElectronTrackSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			obj_counter += looper->isoElectronTracks;
			return looper->isoElectronTracks>0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//selects events with isolated muon tracks (for sync)
class KIsoMuonTrackSelector : public KSyncSelector {
	public:
		//constructor
		KIsoMuonTrackSelector() : KSyncSelector() { }
		KIsoMuonTrackSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			obj_counter += looper->isoMuonTracks;
			return looper->isoMuonTracks>0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//selects events with isolated charged hadron tracks (for sync)
class KIsoPionTrackSelector : public KSyncSelector {
	public:
		//constructor
		KIsoPionTrackSelector() : KSyncSelector() { }
		KIsoPionTrackSelector(string name_, OptionMap* localOpt_) : KSyncSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			obj_counter += looper->isoPionTracks;
			return looper->isoPionTracks>0;
		}
		
		//member variables
};

//-------------------------------------------------------------
//addition to KParser to create selectors
namespace KParser {
	template <>
	KSelector<KSkimmer>* processSelector<KSkimmer>(KNamed* tmp){
		KSelector<KSkimmer>* srtmp = 0;
		string sname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known selectors
		if(sname=="HLT") srtmp = new KHLTSelector(sname,omap);
		else if(sname=="Blind") srtmp = new KBlindSelector(sname,omap);
		else if(sname=="NJet") srtmp = new KNJetSelector(sname,omap);
		else if(sname=="HT") srtmp = new KHTSelector(sname,omap);
		else if(sname=="MHT") srtmp = new KMHTSelector(sname,omap);
		else if(sname=="LowMHT") srtmp = new KLowMHTSelector(sname,omap);
		else if(sname=="MuonVeto") srtmp = new KMuonVetoSelector(sname,omap);
		else if(sname=="ElectronVeto") srtmp = new KElectronVetoSelector(sname,omap);
		else if(sname=="Muon") srtmp = new KMuonSelector(sname,omap);
		else if(sname=="Electron") srtmp = new KElectronSelector(sname,omap);
		else if(sname=="Photon") srtmp = new KPhotonSelector(sname,omap);
		else if(sname=="DiMuon") srtmp = new KDiMuonSelector(sname,omap);
		else if(sname=="DiElectron") srtmp = new KDiElectronSelector(sname,omap);
		else if(sname=="MuonTrigger") srtmp = new KMuonTriggerSelector(sname,omap);
		else if(sname=="ElectronTrigger") srtmp = new KElectronTriggerSelector(sname,omap);
		else if(sname=="IsoElectronTrackVeto") srtmp = new KIsoElectronTrackVetoSelector(sname,omap);
		else if(sname=="IsoMuonTrackVeto") srtmp = new KIsoMuonTrackVetoSelector(sname,omap);
		else if(sname=="IsoPionTrackVeto") srtmp = new KIsoPionTrackVetoSelector(sname,omap);
		else if(sname=="MinDeltaPhi") srtmp = new KMinDeltaPhiSelector(sname,omap);
		else if(sname=="DeltaPhi") srtmp = new KDeltaPhiSelector(sname,omap);
		else if(sname.find("DeltaPhiJ")!=string::npos) srtmp = new KDeltaPhiJSelector(sname,omap); //allow multiple instances
		else if(sname=="EventCleaning") srtmp = new KEventCleaningSelector(sname,omap);
		else if(sname=="NVtx") srtmp = new KNVtxSelector(sname,omap);
		else if(sname=="NBJet") srtmp = new KNBJetSelector(sname,omap);
		else if(sname=="GenHTBin") srtmp = new KGenHTBinSelector(sname,omap);
		else if(sname=="BTagEfficiency") srtmp = new KBTagEfficiencySelector(sname,omap);
		else if(sname=="NJetsISR") srtmp = new KNJetsISRSelector(sname,omap);
		else if(sname=="PDFNorm") srtmp = new KPDFNormSelector(sname,omap);
		else if(sname=="EventRange") srtmp = new KEventRangeSelector(sname,omap);
		else if(sname=="CSCFilter") srtmp = new KCSCFilterSelector(sname,omap);
		else if(sname=="JetEtaRegion") srtmp = new KJetEtaRegionSelector(sname,omap);
		else if(sname=="NeutralHadronFraction") srtmp = new KNeutralHadronFractionSelector(sname,omap);
		else if(sname=="PhotonFraction") srtmp = new KPhotonFractionSelector(sname,omap);
		else if(sname=="PhotonFractionHF") srtmp = new KPhotonFractionHFSelector(sname,omap);
		else if(sname=="NumConstit") srtmp = new KNumConstitSelector(sname,omap);
		else if(sname=="ChargedHadronFraction") srtmp = new KChargedHadronFractionSelector(sname,omap);
		else if(sname=="ChargedMult") srtmp = new KChargedMultSelector(sname,omap);
		else if(sname=="ChargedEmFraction") srtmp = new KChargedEmFractionSelector(sname,omap);
		else if(sname=="NeutralMult") srtmp = new KNeutralMultSelector(sname,omap);
		else if(sname=="PhotonAll") srtmp = new KPhotonAllSelector(sname,omap);
		else if(sname=="PhotonEta") srtmp = new KPhotonEtaSelector(sname,omap);
		else if(sname=="PhotonPt") srtmp = new KPhotonPtSelector(sname,omap);
		else if(sname=="PhotonID") srtmp = new KPhotonIDSelector(sname,omap);
		else if(sname=="PhotonCHIso") srtmp = new KPhotonCHIsoSelector(sname,omap);
		else if(sname=="PhotonNHIso") srtmp = new KPhotonNHIsoSelector(sname,omap);
		else if(sname=="PhotonPHIso") srtmp = new KPhotonPHIsoSelector(sname,omap);
		else if(sname=="IsoElectronTrack") srtmp = new KIsoElectronTrackSelector(sname,omap);
		else if(sname=="IsoMuonTrack") srtmp = new KIsoMuonTrackSelector(sname,omap);
		else if(sname=="IsoPionTrack") srtmp = new KIsoPionTrackSelector(sname,omap);
		else {} //skip unknown selectors
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif