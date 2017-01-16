#ifndef KSKIMMERSELECTORS_H
#define KSKIMMERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KCommonSelectors.h"
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

//base class for Selectors is in KSelection.h

//----------------------------------------------------
//selects negative-weight events (used for KSkimmer)
class KNegativeWeightSelector : public KSelector {
	public:
		//constructor
		KNegativeWeightSelector() : KSelector() { }
		KNegativeWeightSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->Weight<0;
		}
		
		//member variables
};
REGISTER_SELECTOR(NegativeWeight);

//------------------------------------------------------
//selects events based on run number (for blinding data)
class KBlindSelector : public KSelector {
	public:
		//constructor
		KBlindSelector() : KSelector() { }
		KBlindSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), firstUnblindRun(0), lastUnblindRun(257599) { 
			//check for option
			localOpt->Get("firstUnblindRun",firstUnblindRun);
			localOpt->Get("lastUnblindRun",lastUnblindRun);
		}
		virtual void CheckBase(){
			//disable this for non-data
			if(!base->IsData()) dummy = true;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return (firstUnblindRun==-1 or looper->RunNum >= firstUnblindRun) and (lastUnblindRun==-1 or looper->RunNum <= lastUnblindRun);
		}
		
		//member variables
		int firstUnblindRun, lastUnblindRun;
};
REGISTER_SELECTOR(Blind);

//----------------------------------------------------
//selects events based on number of jets
class KNJetSelector : public KSelector {
	public:
		//constructor
		KNJetSelector() : KSelector() { }
		KNJetSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(4) { 
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
REGISTER_SELECTOR(NJet);

//----------------------------------------------------
//low MHT selection for single photon test
class KLowMHTSelector : public KSelector {
	public:
		//constructor
		KLowMHTSelector() : KSelector() { }
		KLowMHTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), MHTmax(200) { 
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
REGISTER_SELECTOR(LowMHT);

//-------------------------------------------------------------
//vetos events with muons
class KMuonVetoSelector : public KSelector {
	public:
		//constructor
		KMuonVetoSelector() : KSelector() { }
		KMuonVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->Muons->size()==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(MuonVeto);

//-------------------------------------------------------------
//vetos events with electrons
class KElectronVetoSelector : public KSelector {
	public:
		//constructor
		KElectronVetoSelector() : KSelector() { }
		KElectronVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->Electrons->size()==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(ElectronVeto);

//------------------------------------------------------
//single muon selector
class KMuonSelector : public KSelector {
	public:
		//constructor
		KMuonSelector() : KSelector() { }
		KMuonSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
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
REGISTER_SELECTOR(Muon);

//------------------------------------------------------
//single electron selector
class KElectronSelector : public KSelector {
	public:
		//constructor
		KElectronSelector() : KSelector() { }
		KElectronSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
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
REGISTER_SELECTOR(Electron);

//------------------------------------------------------
//single photon selector
class KPhotonSelector : public KSelector {
	public:
		//constructor
		KPhotonSelector() : KSelector() { }
		KPhotonSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
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
REGISTER_SELECTOR(Photon);

//------------------------------------------------------
//dimuon selector
class KDiMuonSelector : public KSelector {
	public:
		//constructor
		KDiMuonSelector() : KSelector() { }
		KDiMuonSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//todo: add mass cut?
			return looper->Muons->size() == 2 && looper->Muons_charge->at(0) != looper->Muons_charge->at(1);
		}
		
		//member variables
};
REGISTER_SELECTOR(DiMuon);

//------------------------------------------------------
//dielectron selector
class KDiElectronSelector : public KSelector {
	public:
		//constructor
		KDiElectronSelector() : KSelector() { }
		KDiElectronSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//todo: add mass cut?
			return looper->Electrons->size() == 2 && looper->Electrons_charge->at(0) != looper->Electrons_charge->at(1);
		}
		
		//member variables
};
REGISTER_SELECTOR(DiElectron);

//------------------------------------------------------
//muon selector for trigger study
class KMuonTriggerSelector : public KSelector {
	public:
		//constructor
		KMuonTriggerSelector() : KSelector() { }
		KMuonTriggerSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
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
REGISTER_SELECTOR(MuonTrigger);

//------------------------------------------------------
//electron selector for trigger study
class KElectronTriggerSelector : public KSelector {
	public:
		//constructor
		KElectronTriggerSelector() : KSelector() { }
		KElectronTriggerSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
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
REGISTER_SELECTOR(ElectronTrigger);

//----------------------------------------------------
//selects events based on minDeltaPhi value
class KMinDeltaPhiSelector : public KSelector {
	public:
		//constructor
		KMinDeltaPhiSelector() : KSelector() { }
		KMinDeltaPhiSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), minDeltaPhi(0.4), invert(false) { 
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
REGISTER_SELECTOR(MinDeltaPhi);

//----------------------------------------------------
//selects events based on DeltaPhi values
class KDeltaPhiSelector : public KSelector {
	public:
		//constructor
		KDeltaPhiSelector() : KSelector() { }
		KDeltaPhiSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), DeltaPhi(4,0), invert(false) { 
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
REGISTER_SELECTOR(DeltaPhi);

//----------------------------------------------------
//selects events based on single DeltaPhi value
class KDeltaPhiJSelector : public KSelector {
	public:
		//constructor
		KDeltaPhiJSelector() : KSelector() { }
		KDeltaPhiJSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), DeltaPhi(0.5), jet(1) { 
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
//allow multiple instances
REGISTER_SELECTOR2(DeltaPhiJ,DeltaPhiJ1);
REGISTER_SELECTOR2(DeltaPhiJ,DeltaPhiJ2);
REGISTER_SELECTOR2(DeltaPhiJ,DeltaPhiJ3);
REGISTER_SELECTOR2(DeltaPhiJ,DeltaPhiJ4);

//-------------------------------------------------------------
//vetos events with isolated electron tracks
class KIsoElectronTrackVetoSelector : public KSelector {
	public:
		//constructor
		KIsoElectronTrackVetoSelector() : KSelector() { }
		KIsoElectronTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoElectronTracks==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(IsoElectronTrackVeto);

//-------------------------------------------------------------
//vetos events with isolated muon tracks
class KIsoMuonTrackVetoSelector : public KSelector {
	public:
		//constructor
		KIsoMuonTrackVetoSelector() : KSelector() { }
		KIsoMuonTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoMuonTracks==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(IsoMuonTrackVeto);

//-------------------------------------------------------------
//vetos events with isolated charged hadron tracks
class KIsoPionTrackVetoSelector : public KSelector {
	public:
		//constructor
		KIsoPionTrackVetoSelector() : KSelector() { }
		KIsoPionTrackVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->isoPionTracks==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(IsoPionTrackVeto);

//-------------------------------------------------------------
//vetos events with bad jets (using PFJetID loose WP)
class KEventCleaningSelector : public KSelector {
	public:
		//constructor
		KEventCleaningSelector() : KSelector() { }
		KEventCleaningSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
			//check for option
			doJetID = localOpt->Get("JetID",false);
			doMETRatio = localOpt->Get("METRatio",false);
			doMuonJet = localOpt->Get("MuonJet",false);
			doFakeJet = localOpt->Get("FakeJet",false);
		}
		virtual void CheckBase(){
			//check if fastsim
			bool fastsim = base->GetLocalOpt()->Get("fastsim",false);
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
REGISTER_SELECTOR(EventCleaning);

//-------------------------------------------------------------
//selects based on nvtx
class KNVtxSelector : public KSelector {
	public:
		//constructor
		KNVtxSelector() : KSelector() { }
		KNVtxSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), invert(false), num(20) {
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
REGISTER_SELECTOR(NVtx);

//----------------------------------------------------
//selects events based on number of b-jets
class KNBJetSelector : public KSelector {
	public:
		//constructor
		KNBJetSelector() : KSelector() { }
		KNBJetSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), nbjet_min(-1), nbjet_max(-1) { 
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
REGISTER_SELECTOR(NBJet);

//----------------------------------------------------
//selects events based on genHT bin
class KGenHTBinSelector : public KSelector {
	public:
		//constructor
		KGenHTBinSelector() : KSelector() { }
		KGenHTBinSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
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
REGISTER_SELECTOR(GenHTBin);

//----------------------------------------------------
//calculate b-tagging & mistagging efficiencies
class KBTagEfficiencySelector : public KSelector {
	public:
		//constructor
		KBTagEfficiencySelector() : KSelector() { }
		KBTagEfficiencySelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
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
REGISTER_SELECTOR(BTagEfficiency);

//-----------------------------------------------------------------
//stores the NJetISR distribution
class KNJetsISRSelector : public KSelector {
	public:
		//constructor
		KNJetsISRSelector() : KSelector() { }
		KNJetsISRSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), h_njetsisr(NULL) {
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
REGISTER_SELECTOR(NJetsISR);

//-----------------------------------------------------------------
//stores info for normalizing PDF and r/f scale uncertainties
class KPDFNormSelector : public KSelector {
	public:
		//constructor
		KPDFNormSelector() : KSelector() { }
		KPDFNormSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), h_norm(NULL) {
			canfail = false;
			//initialize histogram
			TH1::AddDirectory(kFALSE);
			//bins:
			//1: nominal, 2: PDF up, 3: PDF down, 4: scale up, 5: scale down
			h_norm = new TH1F("PDFNorm","",5,0.5,5.5);
		}
		virtual void CheckBase(){
			//disable this for data
			if(base->IsData()) {
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
REGISTER_SELECTOR(PDFNorm);

//-------------------------------------------------------------
//look at only a specific range of events
class KEventRangeSelector : public KSelector {
	public:
		//constructor
		KEventRangeSelector() : KSelector() { }
		KEventRangeSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), start(0), finish(0) { 
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
REGISTER_SELECTOR(EventRange);

//---------------------------------------------------------------
//updates CSC filter
class KCSCFilterSelector : public KSelector {
	public:
		//constructor
		KCSCFilterSelector() : KSelector() { }
		KCSCFilterSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), inputfile("") {
			canfail = false;
		}
		virtual void CheckBase(){
			//disable this for non-data
			if(!base->IsData()) dummy = true;
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
REGISTER_SELECTOR(CSCFilter);

//-------------------------------------------------------------
//special version of selector class for object sync
class KSyncSelector : public KSelector {
	public:
		//constructor
		KSyncSelector() : KSelector() { }
		KSyncSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), depname(""), prevSel(NULL), obj_counter(0), debug(false) { 
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
REGISTER_SELECTOR(JetEtaRegion);

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
REGISTER_SELECTOR(NeutralHadronFraction);

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
REGISTER_SELECTOR(PhotonFraction);

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
REGISTER_SELECTOR(NumConstit);

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
REGISTER_SELECTOR(ChargedHadronFraction);

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
REGISTER_SELECTOR(ChargedMult);

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
REGISTER_SELECTOR(ChargedEmFraction);

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
REGISTER_SELECTOR(PhotonFractionHF);

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
REGISTER_SELECTOR(NeutralMult);

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
REGISTER_SELECTOR(PhotonAll);

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
REGISTER_SELECTOR(PhotonEta);

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
REGISTER_SELECTOR(PhotonPt);

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
REGISTER_SELECTOR(PhotonID);

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
REGISTER_SELECTOR(PhotonCHIso);

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
REGISTER_SELECTOR(PhotonNHIso);

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
REGISTER_SELECTOR(PhotonPHIso);

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
REGISTER_SELECTOR(IsoElectronTrack);

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
REGISTER_SELECTOR(IsoMuonTrack);

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
REGISTER_SELECTOR(IsoPionTrack);

#endif
