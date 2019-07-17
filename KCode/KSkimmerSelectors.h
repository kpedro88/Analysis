#ifndef KSKIMMERSELECTORS_H
#define KSKIMMERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KCommonSelectors.h"
#include "KMath.h"
#include "KPlot.h"
#include "KHisto.h"
#include "KBDTVar.h"
#include "../corrections/EventListFilter.h"
#include "../corrections/Flattener.h"
#include "../corrections/L1ECALPrefiringWeightCorrector.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TFitResult.h>
#include <TMatrixD.h>
#include <TVectorD.h>

//STL headers
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <valarray>
#include <utility>

using namespace std;

//base class for Selectors is in KSelection.h

//avoid unwanted dependency
void KRA2BinSelector::CheckDeps(){
}
void KRA2BinSelector::CheckBase(){
	//check fastsim
	if(base->GetLocalOpt()->Get("fastsim",false) and tightfast){
		//veto fastsim events not in any signal bin
		dotightfast = true;
		canfail = true;
	}
}
double KHisto::GetWeight(){
	return 1.;
}
double KHisto::GetWeightPerJet(unsigned index){
	return 1.;
}
//dummy classes
class KMCWeightSelector : public KSelector {};
class KSVJFilterSelector : public KSelector {};
class KLepFracFilterSelector : public KSelector {};

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

//------------------------------------------------------
//single muon selector
class KMuonSelector : public KSelector {
	public:
		//constructor
		KMuonSelector() : KSelector() { }
		KMuonSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
			//check for option
			doMTcut = localOpt->Get("doMTcut",true);
			loose = localOpt->Get("loose",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(loose){
				//loosen iso criteria (loose ID is ntuple baseline)
				unsigned nmuons = 0;
				for(auto m : *looper->Muons_MiniIso){
					if(m < 0.4) ++nmuons;
				}
				return nmuons==1;
			}

			if(looper->NMuons!=1) return false;

			if(doMTcut){
				//find the good muon
				const auto& Muons_passIsoMediumID = KMath::vector_and(*looper->Muons_passIso,*looper->Muons_mediumID);
				unsigned m_index = distance(Muons_passIsoMediumID.begin(),find(Muons_passIsoMediumID.begin(),Muons_passIsoMediumID.end(),true));
			
				double mT = KMath::TransverseMass(looper->MET,looper->METPhi,looper->Muons->at(m_index).Pt(),looper->Muons->at(m_index).Phi());
				return mT<100;
			}
			else return true;
		}
		
		//member variables
		bool doMTcut, loose;
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
			if(looper->NElectrons!=1) return false;
			
			if(doMTcut){
				//find the good electron
				unsigned e_index = distance(looper->Electrons_passIso->begin(),find(looper->Electrons_passIso->begin(),looper->Electrons_passIso->end(),true));
			
				double mT = KMath::TransverseMass(looper->MET,looper->METPhi,looper->Electrons->at(e_index).Pt(),looper->Electrons->at(e_index).Phi());
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
			for(unsigned p = 0; p < looper->Photons->size(); ++p){
				if(trigger){ if(looper->Photons_fullID->at(p) and looper->Photons->at(p).Pt()>200) ++NumPhotons; }
				else if(loose){ if(looper->Photons_hasPixelSeed->at(p)==0. and looper->Photons->at(p).Pt()>100) ++NumPhotons; }
				else{ if(looper->Photons_hasPixelSeed->at(p)==0. and looper->Photons_fullID->at(p) and looper->Photons->at(p).Pt()>100) ++NumPhotons; }
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
			if(looper->NMuons!=2) return false;
			//find the good muons
			const auto& Muons_passIsoMediumID = KMath::vector_and(*looper->Muons_passIso,*looper->Muons_mediumID);
			unsigned m_index = distance(Muons_passIsoMediumID.begin(),find(Muons_passIsoMediumID.begin(),Muons_passIsoMediumID.end(),true));
			unsigned m_index2 = distance(Muons_passIsoMediumID.begin(),find(Muons_passIsoMediumID.begin()+m_index+1,Muons_passIsoMediumID.end(),true));
			return looper->Muons_charge->at(m_index) != looper->Muons_charge->at(m_index2);
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
			if(looper->NElectrons!=2) return false;
			//find the good electrons
			unsigned e_index = distance(looper->Electrons_passIso->begin(),find(looper->Electrons_passIso->begin(),looper->Electrons_passIso->end(),true));
			unsigned e_index2 = distance(looper->Electrons_passIso->begin(),find(looper->Electrons_passIso->begin()+e_index+1,looper->Electrons_passIso->end(),true));
			return looper->Electrons_charge->at(e_index) != looper->Electrons_charge->at(e_index2);
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
				if(looper->Muons_mediumID->at(m) and looper->Muons_passIso->at(m) and looper->Muons->at(m).Pt()>pTmin) ++num;
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
				if(looper->Electrons_passIso->at(e) and looper->Electrons->at(e).Pt()>pTmin) ++num;
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
REGISTER_SELECTOR(DeltaPhiJ);

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
			return ( (nbjet_min==-1 || looper->BTagsDeepCSV >= nbjet_min) && (nbjet_max==-1 || looper->BTagsDeepCSV <= nbjet_max) );
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
			set_wp = localOpt->Get("wp",wp);
			if(!set_wp) localOpt->Get("wps",wps);
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
		virtual void CheckBase(){
			//disable this for data
			if(base->IsData()) {
				dummy = true;
			}
			//year-dependent WP
			if(!set_wp){
				if(base->GetName().find("MC2016")!=std::string::npos) wp = wps[0];
				else if(base->GetName().find("MC2017")!=std::string::npos) wp = wps[1];
				else if(base->GetName().find("MC2018")!=std::string::npos) wp = wps[2];
				else wp = 0.;
			}
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
				double csv = looper->Jets_bJetTagDeepCSVBvsAll->at(ja);
				double pt = looper->Jets->at(ja).Pt();
				//use abs(eta) for now
				double eta = fabs(looper->Jets->at(ja).Eta());
				if(flav==5){
					d_eff_b->Fill(pt,eta);
					if(csv > wp) n_eff_b->Fill(pt,eta);
				}
				else if(flav==4){
					d_eff_c->Fill(pt,eta);
					if(csv > wp) n_eff_c->Fill(pt,eta);
				}
				else if(flav<4 || flav==21){
					d_eff_udsg->Fill(pt,eta);
					if(csv > wp) n_eff_udsg->Fill(pt,eta);
				}
			}
			
			return true;
		}
		
		virtual void Finalize(TFile* file){
			if(!dummy){
				//write numer and denom to file (in case of hadding)
				file->cd();
				d_eff_b->Write();
				d_eff_c->Write();
				d_eff_udsg->Write();
				n_eff_b->Write();
				n_eff_c->Write();
				n_eff_udsg->Write();
			}
		}
		
		//member variables
		bool set_wp;
		double wp;
		vector<double> wps;
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
			double weight = looper->Weight<0 ? -1.0 : 1.0;
			h_njetsisr->Fill(min(double(looper->NJetsISR),xbins[xbins.size()-2]),weight);
			
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
		KPDFNormSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), h_norm(NULL), h_all(NULL) {
			canfail = false;
			//initialize histogram
			TH1::AddDirectory(kFALSE);
			//bins:
			//1: nominal, 2: PDF up, 3: PDF down, 4: scale up, 5: scale down
			h_norm = new TH1F("PDFNorm","",5,0.5,5.5);
			//1: nominal, 2-n: PDF weight i
			h_all = new TH1F("PDFAllNorm","",103,0.5,103.5);
		}
		virtual void CheckBase(){
			//disable this for data
			if(base->IsData()) {
				dummy = true;
				delete h_norm;
				delete h_all;
			}
		}

		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//get weights
			double weight = looper->Weight<0 ? -1.0 : 1.0;
			
			//nominal
			h_norm->Fill(1,weight);
			h_all->Fill(1,weight);
			if(looper->PDFweights->size()>0){
				double mean = TMath::Mean(looper->PDFweights->begin(),looper->PDFweights->end());
				double rms = TMath::RMS(looper->PDFweights->begin(),looper->PDFweights->end());
				//PDF up
				h_norm->Fill(2,(mean+rms)*weight);
				//PDF down
				h_norm->Fill(3,(mean-rms)*weight);
				//fill the histo for all weights
				for(unsigned p = 0; p < looper->PDFweights->size(); ++p){
					h_all->Fill(p+2,looper->PDFweights->at(p));
				}
			}
			
			if(looper->ScaleWeights->size()>0){
				vector<double> ScaleWeightsMod = *looper->ScaleWeights;
				//remove unwanted variations
				if(ScaleWeightsMod.size()>7) ScaleWeightsMod.erase(ScaleWeightsMod.begin()+7);
				if(ScaleWeightsMod.size()>5) ScaleWeightsMod.erase(ScaleWeightsMod.begin()+5);
				if(ScaleWeightsMod.size()>0) ScaleWeightsMod.erase(ScaleWeightsMod.begin());
				
				//scale up
				h_norm->Fill(4,*(TMath::LocMax(ScaleWeightsMod.begin(),ScaleWeightsMod.end()))*weight);
				//scale down
				h_norm->Fill(5,*(TMath::LocMin(ScaleWeightsMod.begin(),ScaleWeightsMod.end()))*weight);
			}
			
			return true;
		}
		
		virtual void Finalize(TFile* file){
			if(dummy) return;
			//write to file
			file->cd();
			h_norm->Write();
			h_all->Write();
		}
		
		//member variables
		TH1F *h_norm;
		TH1F *h_all;
};
REGISTER_SELECTOR(PDFNorm);

//-----------------------------------------------------------------
//stores info for pileup acceptance uncertainty
class KPileupAccSelector : public KSelector {
	public:
		//constructor
		KPileupAccSelector() : KSelector() { }
		KPileupAccSelector(string name_, OptionMap* localOpt_) : 
			KSelector(name_,localOpt_), debug(false), h_nvtxLo(NULL), h_nvtxHi(NULL), cut(20), depname(""), PileupAccBefore(NULL), RA2Bin(NULL)
		{
			canfail = false;
			//check options
			debug = localOpt->Get("debug",false);
			localOpt->Get("cut",cut);
			localOpt->Get("depname",depname);
			//initialize histograms using KPlot:CreateHist() method
			TH1::AddDirectory(kFALSE);
			KPlot* ptmpLo = new KPlot("NVtxLo",localOpt,NULL);
			ptmpLo->CreateHist();
			h_nvtxLo = ptmpLo->GetHisto();
			delete ptmpLo;
			KPlot* ptmpHi = new KPlot("NVtxHi",localOpt,NULL);
			ptmpHi->CreateHist();
			h_nvtxHi = ptmpHi->GetHisto();
			delete ptmpHi;
		}
		virtual void CheckDeps(){
			//set dependencies here - check for "before" (if this is "after")
			if(depname.size()>0) {
				PileupAccBefore = sel->Get<KPileupAccSelector*>(depname);
				RA2Bin = sel->Get<KRA2BinSelector*>("RA2Bin");
				if(!PileupAccBefore or !RA2Bin) {
					depfailed = true;
					canfail = true;
				}
			}
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(depfailed) return false;
			
			bool passed_RA2Bin = true;
			if(RA2Bin and RA2Bin->RA2binBranch==0) passed_RA2Bin = false;
			
			//exclude outside-signal-region events from "after"
			if(passed_RA2Bin){
				if(looper->NVtx<cut) h_nvtxLo->Fill(looper->NVtx);
				else h_nvtxHi->Fill(looper->NVtx);
			}

			return true;
		}
		
		virtual void Finalize(TFile* file){
			//"before" doesn't finalize
			if(depname.size()==0) return;
			//but "after" does
			
			//get pre-selection histos
			TH1 *h_nvtxLoBefore = PileupAccBefore->h_nvtxLo;
			TH1 *h_nvtxHiBefore = PileupAccBefore->h_nvtxHi;
			
			//get pass and fail
			double pL = h_nvtxLo->GetEntries();
			double fL = h_nvtxLoBefore->GetEntries() - pL;
			double pH = h_nvtxHi->GetEntries();
			double fH = h_nvtxHiBefore->GetEntries() - pH;
			if(debug) cout << "pL = " << pL << ", fL = " << fL << ", pH = " << pH << ", fH = " << fH << endl;
			
			//fit a graph of centroid(NVtx) vs. relative acceptance
			double x[] = {0,0}, xed[] = {0,0}, xeu[] = {0,0};
			double y[] = {0,0}, yed[] = {0,0}, yeu[] = {0,0};
			
			//first point from low
			x[0] = h_nvtxLoBefore->GetMean();
			xed[0] = h_nvtxLoBefore->GetMeanError();
			xeu[0] = h_nvtxLoBefore->GetMeanError();
			y[0] = effAB(pL,fL,pH,fH);
			pair<double,double> yeL = sigma_effAB(pL,fL,pH,fH);
			yed[0] = yeL.first;
			yeu[0] = yeL.second;
			
			//second point from high, exploiting symmetry of effAB functions
			x[1] = h_nvtxHiBefore->GetMean();
			xed[1] = h_nvtxHiBefore->GetMeanError();
			xeu[1] = h_nvtxHiBefore->GetMeanError();
			y[1] = effAB(pH,fH,pL,fL);
			pair<double,double> yeH = sigma_effAB(pH,fH,pL,fL);
			yed[1] = yeH.first;
			yeu[1] = yeH.second;
			
			//make graph and fit function
			TGraphAsymmErrors *g_acc = new TGraphAsymmErrors(2,x,y,xed,xeu,yed,yeu);
			g_acc->SetName("pileupAccPts");
			if(debug) g_acc->Print();
			double xmin = 0; localOpt->Get("xmin",xmin);
			double xmax = 0; localOpt->Get("xmax",xmax);
			TF1 *g_fit = new TF1("pileupAccFit","pol1",xmin,xmax);
			string fitopt = "NRQS";
			if(debug) fitopt = "NRS";
			TFitResultPtr fptr = g_acc->Fit(g_fit,fitopt.c_str());
			
			//make second graph to store confidence band
			int nbinsx = 0;
			localOpt->Get("xnum",nbinsx);
			TGraphErrors *g_conf = new TGraphErrors(nbinsx);
			for(int i = 0; i < nbinsx; ++i){
				g_conf->SetPoint(i,i,g_fit->Eval(i));
			}
			g_conf->SetName("pileupAccBand");
			g_conf->SetTitle("");
			g_conf->GetXaxis()->SetTitle("number of vertices");
			g_conf->GetYaxis()->SetTitle("relative acceptance");
			
			//get confidence band (1sigma = 0.68)
			//stored in error bars of graph
			fptr->GetConfidenceIntervals(g_conf->GetN(), 1, 1, g_conf->GetX(), g_conf->GetEY(), 0.68, false);
			
			//write to file (only g_conf strictly necessary, others kept for crosschecks)
			file->cd();
			g_acc->Write();
			g_fit->Write();
			g_conf->Write();
		}
		
		//dumb error propagation functions
		double effAB(double pA, double fA, double pB, double fB){
			double numer = pA*(pA+fA+pB+fB);
			double denom = (pA+fA)*(pA+pB);
			return numer/denom;
		}
		double DeffDpA(double pA, double fA, double pB, double fB){
			double numer = fA*pB*(fA+2*pA+pB)+fB*(fA*pB-pow(pA,2));
			double denom = pow(fA+pA,2)*pow(pA+pB,2);
			return numer/denom;
		}
		double DeffDfA(double pA, double fA, double pB, double fB){
			double numer = -pA*(fB+pB);
			double denom = pow(fA+pA,2)*(pA+pB);
			return numer/denom;
		}
		double DeffDpB(double pA, double fA, double pB, double fB){
			double numer = -pA*(fA+fB);
			double denom = (fA+pA)*pow(pA+pB,2);
			return numer/denom;
		}
		double DeffDfB(double pA, double fA, double pB, double fB){
			double numer = pA;
			double denom = (fA+pA)*(pA+pB);
			return numer/denom;
		}
		pair<double,double> sigma_effAB(double pA, double fA, double pB, double fB){
			valarray<double> derivs = {
				pow(DeffDpA(pA,fA,pB,fB),2),
				pow(DeffDfA(pA,fA,pB,fB),2),
				pow(DeffDpB(pA,fA,pB,fB),2),
				pow(DeffDfB(pA,fA,pB,fB),2)
			};
			valarray<double> errsDown = {
				pow(KMath::PoissonErrorLow(pA),2),
				pow(KMath::PoissonErrorLow(fA),2),
				pow(KMath::PoissonErrorLow(pB),2),
				pow(KMath::PoissonErrorLow(fB),2)			
			};
			valarray<double> errsUp = {
				pow(KMath::PoissonErrorUp(pA),2),
				pow(KMath::PoissonErrorUp(fA),2),
				pow(KMath::PoissonErrorUp(pB),2),
				pow(KMath::PoissonErrorUp(fB),2)			
			};
			auto result = make_pair(sqrt((derivs*errsDown).sum()),sqrt((derivs*errsUp).sum()));
			if(debug){
				cout << scientific;
				cout << "derivs = "; KParser::printarr(derivs,cout,", "); cout << endl;
				cout << "errsDown = "; KParser::printarr(errsDown,cout,", "); cout << endl;
				cout << "errsUp = "; KParser::printarr(errsUp,cout,", "); cout << endl;
				cout << fixed;
			}
			return result;
		}
		
		//member variables
		bool debug;
		TH1 *h_nvtxLo, *h_nvtxHi;
		int cut;
		string depname;
		KPileupAccSelector* PileupAccBefore;
		KRA2BinSelector* RA2Bin;
};
REGISTER_SELECTOR(PileupAcc);

//-------------------------------------------------------------
//add L1 prefiring weights to skims
class KL1PrefiringWeightSelector : public KSelector {
	public:
		//constructor
		KL1PrefiringWeightSelector() : KSelector() { }
		KL1PrefiringWeightSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), systUnc(0.2) {
			forceadd = true;
			canfail = false;
			//check for option
			useEMpt = localOpt->Get("useEMpt",true);
			localOpt->Get("systUnc",systUnc);
			localOpt->Get("fname",fname);
		}
		virtual void CheckBase(){
			//disable this for data
			if(base->IsData()) dummy = true;
			else {
				string basename = base->GetName();
				string dataEra;
				if(basename.find("2016")!=string::npos) dataEra = "2016BtoH";
				else if(basename.find("2017")!=string::npos) dataEra = "2017BtoF";
				corrector.setup(fname,dataEra,useEMpt,systUnc);
			}
		}
		virtual void SetBranches(){
			if(!tree or dummy) return;
			tree->Branch("NonPrefiringProb",&prob,"prob/D");
			tree->Branch("NonPrefiringProbUp",&probup,"probup/D");
			tree->Branch("NonPrefiringProbDn",&probdn,"probdn/D");
		}
		virtual bool Cut(){
			prob = probup = probdn = 1.0;
			const auto& probs = corrector.getWeights(*looper->Photons, *looper->Jets, *looper->Jets_neutralEmEnergyFraction, *looper->Jets_chargedEmEnergyFraction);
			prob = probs[0];
			probup = probs[1];
			probdn = probs[2];
			return true;
		}

		//members
		bool useEMpt;
		double systUnc;
		string fname;
		L1ECALPrefiringWeightCorrector corrector;
		bool setup;
		double prob, probup, probdn;
};
REGISTER_SELECTOR(L1PrefiringWeight);

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
					throw runtime_error("dependency "+depname+" failed in "+name+"!");
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

//----------------------------------------------------
//updates pileup weight branch
class KPileupWeightSelector : public KSelector {
	public:
		//constructor
		KPileupWeightSelector() : KSelector() { }
		KPileupWeightSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
			canfail = false;
			localOpt->Get("year",year);
		}
		virtual void CheckBase(){
			//check whether this should be enabled (only for MC of given year)
			if(base->IsData() or base->GetName().find(year)==string::npos){
				dummy = true;
			}

			if(dummy) return;
			else cout << "Enabled PileupWeight update for " << year << endl;

			//update existing PU correction using data ratio
			//only necessary because of 2017 wrong PU MC
			puupdcorr = localOpt->Get("puupdcorr",false);
			puupdhist = NULL;
			puupdhistUp = NULL;
			puupdhistDown = NULL;
			if(puupdcorr){
				string puname1; localOpt->Get("puname1",puname1);
				string puname2; localOpt->Get("puname2",puname2);
				HistoMap* hmtmp = puhistMap().Get(puname1+puname2);
				if(puname1.empty() or puname2.empty()){
					throw runtime_error("expected pileup weight file not specified!");
				}
				else if(hmtmp){
					puupdhist = hmtmp->Get("puupdhist");
					puupdhistUp = hmtmp->Get("puupdhistUp");
					puupdhistDown = hmtmp->Get("puupdhistDown");
				}
				else {
					TFile* pufile2 = KOpen(puname2);
					hmtmp = new HistoMap();
					puupdhist = KGet<TH1>(pufile2,"data_pu_central"); puupdhist->SetDirectory(0);
					puupdhistUp = KGet<TH1>(pufile2,"data_pu_up"); puupdhistUp->SetDirectory(0);
					puupdhistDown = KGet<TH1>(pufile2,"data_pu_down"); puupdhistDown->SetDirectory(0);
					pufile2->Close();

					TFile* pufile1 = KOpen(puname1);
					//correct puWeight branch (data1/mc) by data2/data1 to get data2/mc
					puupdhist->Divide(KGet<TH1>(pufile1,"data_pu_central"));
					puupdhistUp->Divide(KGet<TH1>(pufile1,"data_pu_up"));
					puupdhistDown->Divide(KGet<TH1>(pufile1,"data_pu_down"));
					pufile1->Close();

					hmtmp->Add("puupdhist",puupdhist);
					hmtmp->Add("puupdhistUp",puupdhistUp);
					hmtmp->Add("puupdhistDown",puupdhistDown);
				}
			}
		}
		//static members - kept in functions for safety
		static HistoMapMap& puhistMap(){
			static HistoMapMap puhistMap_;
			return puhistMap_;
		}
		double GetBinContentBounded(TH1* hist, double val){
			return hist->GetBinContent(hist->GetXaxis()->FindBin(min(val,hist->GetBinLowEdge(hist->GetNbinsX()+1))));
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(puupdcorr){
				looper->puSysUp = (puupdhistUp ? GetBinContentBounded(puupdhistUp,looper->TrueNumInteractions) : 1.0) * looper->puSysUp;
				looper->puSysDown = (puupdhistDown ? GetBinContentBounded(puupdhistDown,looper->TrueNumInteractions) : 1.0) * looper->puSysDown;
				looper->puWeight = (puupdhist ? GetBinContentBounded(puupdhist,looper->TrueNumInteractions) : 1.0) * looper->puWeight;
			}
			return true;
		}
		
		//member variables
		string year;
		bool puupdcorr;
		TH1 *puupdhist, *puupdhistUp, *puupdhistDown;
};
REGISTER_SELECTOR(PileupWeight);

//-------------------------------------------------------------
//generate per-jet training ntuples for tagging
class KJetAK8TrainingSelector : public KSelector {
	public:
		//constructor
		KJetAK8TrainingSelector() : KSelector() {}
		KJetAK8TrainingSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
			//check for option
			vector<string> branchnames;
			localOpt->Get("branches",branchnames);
			for(const auto& b: branchnames){
				branches.push_back(KBDTVarFactory::GetFactory().construct(b,b));
			}
			//flat options
			localOpt->Get("flatname",flatname);
			localOpt->Get("flatnumers",flatnumers);
			localOpt->Get("flatbranches",flatbranches);
			flatten = !flatname.empty() and !flatnumers.empty() and !flatbranches.empty() and flatnumers.size()==flatbranches.size();
		}
		virtual void CheckBranches(){
			for(auto&b : branches){
				b->CheckBranches();
			}
		}

		virtual void CheckBase(){
			//propagate to vars
			for(auto& b : branches){
				b->SetSelection(sel);
				b->SetBase(base);
			}
			//get pt flattening weights
			if(!flatten) return;
			string qty = "bothjetAK8pt";
			string flatsuff;
			if(!base->GetLocalOpt()->Get("flatsuff",flatsuff)) flatsuff = base->GetName();
			TFile* flatfile = KOpen(flatname);
			string flatdist = qty + "_" + flatsuff;
			if(flatten){
				TH1* flathist = KGet<TH1>(flatfile,flatdist);
				flathist->SetDirectory(0);
				for(const auto& numer : flatnumers){
					string flatnumerdist = qty + "_" + numer;
					TH1* flatnumerhist = KGet<TH1>(flatfile,flatnumerdist);
					if(flatnumerhist) flatnumerhist->SetDirectory(0);
					flatteners.emplace_back();
					flatteners.back().SetDist(flathist,flatnumerhist);
				}
				b_flatweights = vector<double>(flatbranches.size(),0);
			}
			flatfile->Close();
			//get signal parameters
			b_mZprime = 0.; base->GetLocalOpt()->Get("mZprime",b_mZprime);
			b_mDark = 0.; base->GetLocalOpt()->Get("mDark",b_mDark);
			b_rinv = 0.; base->GetLocalOpt()->Get("rinv",b_rinv);
			b_alpha = 0.; base->GetLocalOpt()->Get("alpha",b_alpha);
		}

		virtual void SetBranches(){
			if(!tree) return;
			
			for(auto& b : branches){
				b->SetBranch(tree);
			}
			
			//extra branches
			tree->Branch("mZprime",&b_mZprime,"mZprime/D");
			tree->Branch("mDark",&b_mDark,"mDark/D");
			tree->Branch("rinv",&b_rinv,"rinv/D");
			tree->Branch("alpha",&b_alpha,"alpha/D");
			if(flatten){
				for(unsigned b = 0; b < flatbranches.size(); ++b){
					tree->Branch(("flatweight"+flatbranches[b]).c_str(),&b_flatweights[b],("flatweight"+flatbranches[b]+"/D").c_str());
				}
			}
		}

		//used for non-dummy selectors
		virtual bool Cut(){
			for(unsigned j = 0; j < min(looper->JetsAK8->size(),3ul); ++j){
				//temporary protection for crazy values
				double pt = looper->JetsAK8->at(j).Pt();
				if(pt<100) continue;
				for(unsigned b = 0; b < branches.size(); ++b){
					branches[b]->Fill(j);
				}
				if(flatten) {
					for(unsigned b = 0; b < b_flatweights.size(); ++b){
						b_flatweights[b] = flatteners[b].GetWeight(pt);
					}
				}
				//fill tree per jet
				tree->Fill();
			}
			//prevents selection from trying to fill tree per event
			return false;
		}

		//member variables
		vector<KBDTVar*> branches;
		bool flatten;
		vector<string> flatnumers, flatbranches;
		string flatname;
		vector<Flattener> flatteners;
		//extra branches
		double b_mZprime, b_mDark, b_rinv, b_alpha;
		vector<double> b_flatweights;
};
REGISTER_SELECTOR(JetAK8Training);

#endif
