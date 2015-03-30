#ifndef KSELECTORS_H
#define KSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KSkimmer.h"
#include "KMath.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

//base class for Selectors is in KSelection.h

//----------------------------------------------------
//selects events based on HLT line
class KHLTSelector : public KSelector {
	public:
		//constructor
		KHLTSelector() : KSelector() { }
		KHLTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), HLTLine("") { 
			//get selected line from options
			localOpt->Get("HLTLine",HLTLine);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//skip if no line provided
			if(HLTLine.size()==0) return true;
			
			//loop over trigger names
			bool goodTrigger = false;
			for(unsigned h = 0; h < sk->HLTInsideDatasetTriggerNames->size(); h++){
				//check:
				//1) if the line matches the current inside dataset line
				//2) if the decision was true (the line fired)
				//3) if the line was not prescaled
				if(sk->HLTInsideDatasetTriggerNames->at(h).compare(0,HLTLine.size(),HLTLine)==0 && sk->HLTInsideDatasetTriggerDecisions->at(h) && sk->HLTInsideDatasetTriggerPrescales->at(h)==1) {
					goodTrigger = true;
					break;
				}
			}
			//skip event if finished searching and no HLT lines found
			return goodTrigger;
		}
		
		//member variables
		string HLTLine;
};

//----------------------------------------------------
//stores event info
class KEventInfoSelector : public KSelector {
	public:
		//constructor
		KEventInfoSelector() : KSelector() { }
		KEventInfoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { canfail = false; }
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("run",&run,"run/I");
			tree->Branch("event",&event,"event/I");
			tree->Branch("ls",&ls,"ls/I");
			tree->Branch("trueNInteraction",&trueNInteraction,"trueNInteraction/D");
			//default values for variables
			run = ls = event = 0;
			trueNInteraction = 0.0;
		}
		
		//set variable values for tree
		virtual bool Cut() {
			run = sk->run;
			ls = sk->ls;
			event = sk->event;
			trueNInteraction = sk->trueNInteraction;
			
			return true;
		}
		
		//member variables
		int run, ls, event;
		double trueNInteraction;
};

//------------------------------------------------------
//base class for leptons with ID functions & observables
class KLeptonBaseSelector : public KSelector {
	public:
		//constructor
		KLeptonBaseSelector() : KSelector() { }
		KLeptonBaseSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//ID functions
		
		//relative PF isolation
		double MuonIso(int m){
			return (sk->MuonPFIsoR04ChargedHadron->at(m) + max(sk->MuonPFIsoR04Photon->at(m) + sk->MuonPFIsoR04NeutralHadron->at(m) - 0.5*sk->MuonPFIsoR04PU->at(m),0.))/sk->MuonPt->at(m);
		}		
		
		//tight muons
		//default: pt = 30
		bool MuonIDTight(int m, double pt=30){
			//boolean to check requirements
			bool goodMuon = (sk->MuonPt->at(m)>pt) //pT cut in GeV
			&& (fabs(sk->MuonEta->at(m))<2.1) //eta cut to match trigger IsoMu24_eta2p1 (2012A)
			&& (sk->MuonIsPF->at(m)==1) //require PF muon
			&& (sk->MuonPassID->at(m)==1) //require global, chi2 < 10, chamber hits
			//&& (sk->MuonIsGlobal->at(m)==1) //require global
			//&& (sk->MuonGlobalChi2->at(m)<10) //require global fit chi2/ndof < 10
			//&& (sk->MuonSegmentMatches->at(m)-sk->MuonStationMatches->at(m)>=1) //require chamber hits (segment - station)
			&& (sk->MuonStationMatches->at(m)>=1) //require station hits
			&& (fabs(sk->MuonBestTrackVtxDistXY->at(m))<0.2) //require impact parameter < 0.2
			&& (fabs(sk->MuonBestTrackVtxDistZ->at(m))<0.5) //require longitudinal distance of track < 5 mm = .5 cm
			&& (sk->MuonPixelHits->at(m)>0) //require pixel hits
			&& (sk->MuonTrackLayersWithMeasurement->at(m)>5) //require 5+ tracker layers with hits
			&& (MuonIso(m)<0.12); //require tight (<0.12 GeV) relative PF isolation

			return goodMuon;
		}

		//loose muons
		//default: pt = 20
		bool MuonIDLoose(int m, double pt=20){
			//boolean to check requirements
			bool looseMuon = (sk->MuonPt->at(m)>pt) //pT cut in GeV
			&& (fabs(sk->MuonEta->at(m))<2.4) //eta cut to ensure good ID
			&& (sk->MuonIsPF->at(m)==1) //require PF muon
			&& (sk->MuonIsGlobal->at(m)==1 || sk->MuonIsTracker->at(m)==1) //require global OR tracker
			&& (MuonIso(m)<0.3); //require loose (<0.3) relative PF isolation

			return looseMuon;
		}

		//medium electron ID
		Bool_t ElectronIDMedium(int e, double pt=30){
			bool goodEle = (sk->ElectronPt->at(e)>pt) //pT cut in GeV
			&& (fabs(sk->ElectronEta->at(e))<1.4442||(fabs(sk->ElectronEta->at(e))>1.56&&fabs(sk->ElectronEta->at(e))<2.5)) //ECAL eta requirements
			&& (sk->ElectronPassEGammaIDMedium->at(e)==1023); //EGammaID, medium working point (TestWP rather than PassWP)
			
			return goodEle;
		}

		//loose electron ID
		Bool_t ElectronIDLoose(int e, double pt=20){
			bool goodEle = (sk->ElectronPt->at(e)>pt) //pT cut in GeV
			&& (fabs(sk->ElectronEta->at(e))<1.4442||(fabs(sk->ElectronEta->at(e))>1.56&&fabs(sk->ElectronEta->at(e))<2.5)) //ECAL eta requirements
			&& (sk->ElectronPassEGammaIDLoose->at(e)==1023); //EGammaID, loose working point
			
			return goodEle;
		}
		
		//member variables
		int theGoodLepton;
		double Energy, Eta, Phi, Pt;
		int Charge, VtxIndex, Multiplicity;
		TLorentzVector LorentzVector;
};

//----------------------------------------------------
//selects events based on muons
class KMuonSelector : public KLeptonBaseSelector {
	public:
		//constructor
		KMuonSelector() : KLeptonBaseSelector() { }
		KMuonSelector(string name_, OptionMap* localOpt_) : KLeptonBaseSelector(name_,localOpt_) {
			//check options
			LooseNotTight = localOpt->Get("LooseNotTight",false);
		}
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("MuonEnergy",&Energy,"Energy/D");
			tree->Branch("MuonEta",&Eta,"Eta/D");
			tree->Branch("MuonPhi",&Phi,"Phi/D");
			tree->Branch("MuonPt",&Pt,"Pt/D");
			tree->Branch("MuonPFRelIsoTight",&PFRelIsoTight,"PFRelIsoTight/D");
			tree->Branch("MuonMultiplicity",&Multiplicity,"Multiplicity/I");
			tree->Branch("MuonCharge",&Charge,"Charge/I");
			tree->Branch("MuonVtxIndex",&VtxIndex,"VtxIndex/I");
			//default values for variables
			Energy = Eta = Phi = Pt = PFRelIsoTight = 0;
			Charge = Multiplicity = VtxIndex = 0;
		}
		
		//used for non-dummy selectors
		virtual bool Cut() { 
			theGoodLeptons.clear(); theGoodLepton = -1;
			for(unsigned m = 0; m < sk->MuonPt->size(); m++){
				//boolean to check requirements
				bool goodMuon = MuonIDTight(m,30);
				if(LooseNotTight) goodMuon = !goodMuon && MuonIDLoose(m,30);
				
				//if it passed, store it
				if(goodMuon) theGoodLeptons.push_back(m);
			}
			//skip event if there is not at least one good muon
			if(theGoodLeptons.size()<1) return false;
			
			//store leading muon (lowest index) and observables
			theGoodLepton = theGoodLeptons[0];
			Energy = sk->MuonEnergy->at(theGoodLepton);
			Eta = sk->MuonEta->at(theGoodLepton);
			Phi = sk->MuonPhi->at(theGoodLepton);
			Pt = sk->MuonPt->at(theGoodLepton);
			PFRelIsoTight = MuonIso(theGoodLepton);
			Charge = sk->MuonCharge->at(theGoodLepton);
			Multiplicity = theGoodLeptons.size();
			VtxIndex = sk->MuonBestTrackVtxIndex->at(theGoodLepton);
			LorentzVector.SetPtEtaPhiM(Pt,Eta,Phi,sk->MuonMass);
			
			return true;
		}
		
		//member variables
		bool LooseNotTight;
		vector<int> theGoodLeptons;
		double PFRelIsoTight;
};

//----------------------------------------------------
//selects events based on second OS muon
class KSecondMuonSelector : public KLeptonBaseSelector {
	public:
		//constructor
		KSecondMuonSelector() : KLeptonBaseSelector() { }
		KSecondMuonSelector(string name_, OptionMap* localOpt_) : KLeptonBaseSelector(name_,localOpt_) { }
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("SecondMuonEnergy",&Energy,"Energy/D");
			tree->Branch("SecondMuonEta",&Eta,"Eta/D");
			tree->Branch("SecondMuonPhi",&Phi,"Phi/D");
			tree->Branch("SecondMuonPt",&Pt,"Pt/D");
			tree->Branch("SecondMuonPFRelIsoTight",&PFRelIsoTight,"PFRelIsoTight/D");
			tree->Branch("SecondMuonMultiplicity",&Multiplicity,"Multiplicity/I");
			tree->Branch("SecondMuonCharge",&Charge,"Charge/I");
			tree->Branch("MassMuonMuon",&Mass,"Mass/D");
			//default values for variables
			Energy = Eta = Phi = Pt = PFRelIsoTight = Mass = 0;
			Charge = Multiplicity = 0;
		}
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstMuon = sel->Get<KLeptonBaseSelector*>("Muon");
			if(!FirstMuon){
				cout << "Input error: dependency Muon failed in SecondMuon!" << endl;
				depfailed = true;
			}
			else if(FirstMuon->Dummy()){
				cout << "Input error: dependency Muon is dummy in SecondMuon!" << endl;
				depfailed = true;
			}
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//the first muon selector must be present for this to work
			if(depfailed) return false;
			
			theGoodLeptons.clear(); theGoodLepton = -1;
			for(unsigned m = 0; m < sk->MuonPt->size(); m++){
				//boolean to check requirements
				bool goodMuon = MuonIDLoose(m,25);
				
				//comparison with leading muon
				goodMuon &= (m != FirstMuon->theGoodLepton) //different muon
				&& (sk->MuonCharge->at(m) != FirstMuon->Charge) //opposite charge
				&& (sk->MuonBestTrackVtxIndex->at(m) == FirstMuon->VtxIndex); //same vertex
				
				//if it passed, store it
				if(goodMuon) theGoodLeptons.push_back(m);
			}
			//skip event if there is not at least one good muon
			if(theGoodLeptons.size()<1) return false;
			
			//store second muon (lowest index) and observables
			theGoodLepton = theGoodLeptons[0];
			Energy = sk->MuonEnergy->at(theGoodLepton);
			Eta = sk->MuonEta->at(theGoodLepton);
			Phi = sk->MuonPhi->at(theGoodLepton);
			Pt = sk->MuonPt->at(theGoodLepton);
			PFRelIsoTight = MuonIso(theGoodLepton);
			Charge = sk->MuonCharge->at(theGoodLepton);
			Multiplicity = theGoodLeptons.size();
			
			//store invariant mass of lepton system
			LorentzVector.SetPtEtaPhiM(Pt,Eta,Phi,sk->MuonMass);
			TLorentzVector v_mumu;
			v_mumu = FirstMuon->LorentzVector + LorentzVector; //sum up
			Mass = v_mumu.M();
			
			return true;
		}

		//member variables
		KLeptonBaseSelector* FirstMuon;
		vector<int> theGoodLeptons;
		double PFRelIsoTight, Mass;
};

//----------------------------------------------------
//vetoes events with second muon
class KSecondMuonVetoSelector : public KLeptonBaseSelector {
	public:
		//constructor
		KSecondMuonVetoSelector() : KLeptonBaseSelector() { }
		KSecondMuonVetoSelector(string name_, OptionMap* localOpt_) : KLeptonBaseSelector(name_,localOpt_) {
			//check options
			OppositeSign = localOpt->Get("OppositeSign",false);
		}
		
		//accessors
		//this selector doesn't add anything to tree
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstMuon = sel->Get<KLeptonBaseSelector*>("Muon");
			if(!FirstMuon){
				cout << "Input error: dependency Muon failed in SecondMuonVeto!" << endl;
				depfailed = true;
			}
			else if(FirstMuon->Dummy()){
				cout << "Input error: dependency Muon is dummy in SecondMuonVeto!" << endl;
				depfailed = true;
			}
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//the first muon selector must be present for this to work
			if(depfailed) return false;
			
			for(unsigned m = 0; m < sk->MuonPt->size(); m++){
				//boolean to check requirements
				bool looseMuon = MuonIDLoose(m,20);
				if(OppositeSign) looseMuon &= (FirstMuon->Charge != sk->MuonCharge->at(m));
				
				//skip event if there are multiple muons
				if(looseMuon) return false;
			}
			return true;
		}
		
		//member variables
		KLeptonBaseSelector* FirstMuon;
		bool OppositeSign;
};

//-------------------------------------------------------------
//muon veto (opposite-sign to avoid overlap w/ emu channel)
class KMuonVetoSelector : public KLeptonBaseSelector {
	public:
		//constructor
		KMuonVetoSelector() : KLeptonBaseSelector() { }
		KMuonVetoSelector(string name_, OptionMap* localOpt_) : KLeptonBaseSelector(name_,localOpt_) {
			//check options
			OppositeSign = localOpt->Get("OppositeSign",false);
			LooseID = localOpt->Get("LooseID",false);
		}
		
		//accessors
		//this selector doesn't add anything to tree
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstElectron = sel->Get<KLeptonBaseSelector*>("Electron");
			if(!FirstElectron){
				cout << "Input error: dependency Electron failed in MuonVeto!" << endl;
				depfailed = true;
			}
			else if(FirstElectron->Dummy()){
				cout << "Input error: dependency Electron is dummy in MuonVeto!" << endl;
				depfailed = true;
			}
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//the first electron selector must be present for this to work
			if(depfailed) return false;
			
			for(unsigned m = 0; m < sk->MuonPt->size(); m++){
				//boolean to check requirements
				bool goodMuon;
				if(LooseID) goodMuon = MuonIDLoose(m,20);
				else goodMuon = MuonIDTight(m,30);

				if(OppositeSign) goodMuon &= (FirstElectron->Charge != sk->MuonCharge->at(m));
				
				//skip event if there is a good muon
				if(goodMuon) return false;
			}
			return true;
		}
		
		//member variables
		KLeptonBaseSelector* FirstElectron;
		bool OppositeSign;
		bool LooseID;
};

//----------------------------------------------------
//selects events based on electrons
class KElectronSelector : public KLeptonBaseSelector {
	public:
		//constructor
		KElectronSelector() : KLeptonBaseSelector() { }
		KElectronSelector(string name_, OptionMap* localOpt_) : KLeptonBaseSelector(name_,localOpt_) { }
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("ElectronEnergy",&Energy,"Energy/D");
			tree->Branch("ElectronEta",&Eta,"Eta/D");
			tree->Branch("ElectronPhi",&Phi,"Phi/D");
			tree->Branch("ElectronPt",&Pt,"Pt/D");
			tree->Branch("ElectronMultiplicity",&Multiplicity,"Multiplicity/I");
			tree->Branch("ElectronCharge",&Charge,"Charge/I");
			tree->Branch("ElectronVtxIndex",&VtxIndex,"VtxIndex/I");
			//default values for variables
			Energy = Eta = Phi = Pt = 0;
			Charge = Multiplicity = VtxIndex = 0;
		}
		
		//used for non-dummy selectors
		virtual bool Cut() { 
			theGoodLeptons.clear(); theGoodLepton = -1;
			for(unsigned e = 0; e < sk->ElectronPt->size(); e++){
				//boolean to check requirements
				bool goodElectron = ElectronIDMedium(e,30);
				
				//if it passed, store it
				if(goodElectron) theGoodLeptons.push_back(e);
			}
			//skip event if there is not at least one good muon
			if(theGoodLeptons.size()<1) return false;
			
			//store leading electron (lowest index) and observables
			theGoodLepton = theGoodLeptons[0];
			Energy = sk->ElectronEnergy->at(theGoodLepton);
			Eta = sk->ElectronEta->at(theGoodLepton);
			Phi = sk->ElectronPhi->at(theGoodLepton);
			Pt = sk->ElectronPt->at(theGoodLepton);
			Charge = sk->ElectronCharge->at(theGoodLepton);
			Multiplicity = theGoodLeptons.size();
			VtxIndex = sk->ElectronVtxIndex->at(theGoodLepton);
			LorentzVector.SetPtEtaPhiM(Pt,Eta,Phi,sk->ElectronMass);
			
			return true;
		}
		
		//member variables
		vector<int> theGoodLeptons;
};

//----------------------------------------------------
//selects events based on second OS electron
class KSecondElectronSelector : public KLeptonBaseSelector {
	public:
		//constructor
		KSecondElectronSelector() : KLeptonBaseSelector() { }
		KSecondElectronSelector(string name_, OptionMap* localOpt_) : KLeptonBaseSelector(name_,localOpt_) { }
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("SecondElectronEnergy",&Energy,"Energy/D");
			tree->Branch("SecondElectronEta",&Eta,"Eta/D");
			tree->Branch("SecondElectronPhi",&Phi,"Phi/D");
			tree->Branch("SecondElectronPt",&Pt,"Pt/D");
			tree->Branch("SecondElectronMultiplicity",&Multiplicity,"Multiplicity/I");
			tree->Branch("SecondElectronCharge",&Charge,"Charge/I");
			tree->Branch("MassElectronElectron",&Mass,"Mass/D");
			//default values for variables
			Energy = Eta = Phi = Pt = Mass = 0;
			Charge = Multiplicity = 0;
		}
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstElectron = sel->Get<KLeptonBaseSelector*>("Electron");
			if(!FirstElectron){
				cout << "Input error: dependency Electron failed in SecondElectron!" << endl;
				depfailed = true;
			}
			else if(FirstElectron->Dummy()){
				cout << "Input error: dependency Electron is dummy in SecondElectron!" << endl;
				depfailed = true;
			}
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//the first electron selector must be present for this to work
			if(depfailed) return false;
			
			theGoodLeptons.clear(); theGoodLepton = -1;
			for(unsigned e = 0; e < sk->ElectronPt->size(); e++){
				//boolean to check requirements
				bool goodElectron = ElectronIDLoose(e,20);
				
				//comparison with leading electron
				goodElectron &= (e != FirstElectron->theGoodLepton) //different muon
				&& (sk->ElectronCharge->at(e) != FirstElectron->Charge) //opposite charge
				&& (sk->ElectronVtxIndex->at(e) == FirstElectron->VtxIndex); //same vertex
				
				//if it passed, store it
				if(goodElectron) theGoodLeptons.push_back(e);
			}
			//skip event if there is not at least one good muon
			if(theGoodLeptons.size()<1) return false;
			
			//store second electron (lowest index) and observables
			theGoodLepton = theGoodLeptons[0];
			Energy = sk->ElectronEnergy->at(theGoodLepton);
			Eta = sk->ElectronEta->at(theGoodLepton);
			Phi = sk->ElectronPhi->at(theGoodLepton);
			Pt = sk->ElectronPt->at(theGoodLepton);
			Charge = sk->ElectronCharge->at(theGoodLepton);
			Multiplicity = theGoodLeptons.size();
			
			//store invariant mass of lepton system
			LorentzVector.SetPtEtaPhiM(Pt,Eta,Phi,sk->ElectronMass);
			TLorentzVector v_elel;
			v_elel = FirstElectron->LorentzVector + LorentzVector; //sum up
			Mass = v_elel.M();
			
			return true;
		}
		
		//member variables
		KLeptonBaseSelector* FirstElectron;
		vector<int> theGoodLeptons;
		double Mass;
};

//----------------------------------------------------
//vetoes events with second electron
class KSecondElectronVetoSelector : public KLeptonBaseSelector {
	public:
		//constructor
		KSecondElectronVetoSelector() : KLeptonBaseSelector() { }
		KSecondElectronVetoSelector(string name_, OptionMap* localOpt_) : KLeptonBaseSelector(name_,localOpt_) {
			//check options
			OppositeSign = localOpt->Get("OppositeSign",false);
		}
		
		//accessors
		//this selector doesn't add anything to tree
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstElectron = sel->Get<KLeptonBaseSelector*>("Electron");
			if(!FirstElectron){
				cout << "Input error: dependency Electron failed in SecondElectronVeto!" << endl;
				depfailed = true;
			}
			else if(FirstElectron->Dummy()){
				cout << "Input error: dependency Electron is dummy in SecondElectronVeto!" << endl;
				depfailed = true;
			}
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//the first electron selector must be present for this to work
			if(depfailed) return false;
			
			for(unsigned e = 0; e < sk->ElectronPt->size(); e++){
				//boolean to check requirements
				bool looseElectron = ElectronIDLoose(e,20);
				if(OppositeSign) looseElectron &= (FirstElectron->Charge != sk->ElectronCharge->at(e));
				
				//skip event if there are multiple muons
				if(looseElectron) return false;
			}
			return true;
		}
		
		//member variables
		KLeptonBaseSelector* FirstElectron;
		bool OppositeSign;
};

//-------------------------------------------------------------
//electron veto (opposite-sign to avoid overlap w/ emu channel)
class KElectronVetoSelector : public KLeptonBaseSelector {
	public:
		//constructor
		KElectronVetoSelector() : KLeptonBaseSelector() { }
		KElectronVetoSelector(string name_, OptionMap* localOpt_) : KLeptonBaseSelector(name_,localOpt_) {
			//check options
			OppositeSign = localOpt->Get("OppositeSign",false);
			LooseID = localOpt->Get("LooseID",false);
		}
		
		//accessors
		//this selector doesn't add anything to tree
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstMuon = sel->Get<KLeptonBaseSelector*>("Muon");
			if(!FirstMuon){
				cout << "Input error: dependency Muon failed in ElectronVeto!" << endl;
				depfailed = true;
			}
			else if(FirstMuon->Dummy()){
				cout << "Input error: dependency Muon is dummy in ElectronVeto!" << endl;
				depfailed = true;
			}
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//the first muon selector must be present for this to work
			if(depfailed) return false;
			
			for(unsigned e = 0; e < sk->ElectronPt->size(); e++){
				//boolean to check requirements
				bool goodElectron;
				if(LooseID) goodElectron = ElectronIDLoose(e,20);
				else goodElectron = ElectronIDMedium(e,30);
				
				if(OppositeSign) goodElectron &= (FirstMuon->Charge != sk->ElectronCharge->at(e));
				
				//skip event if there is a good electron
				if(goodElectron) return false;
			}
			return true;
		}
		
		//member variables
		KLeptonBaseSelector* FirstMuon;
		bool OppositeSign;
		bool LooseID;
};

//--------------------------------------------------------
//selects events based on vertex quality of leading lepton
class KVertexSelector : public KSelector {
	public:
		//constructor
		KVertexSelector() : KSelector() { }
		KVertexSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("nGoodVertices",&nGoodVertices,"nGoodVertices/I");
			//default values for variables
			nGoodVertices = 0;
		}
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstLepton = sel->Get<KLeptonBaseSelector*>("Muon");
			if(!FirstLepton) FirstLepton = sel->Get<KLeptonBaseSelector*>("Electron");
			if(!FirstLepton){
				cout << "Input error: dependency Lepton failed in Vertex!" << endl;
				depfailed = true;
			}
			else if(FirstLepton->Dummy()){
				cout << "Input error: dependency Lepton is dummy in Vertex!" << endl;
				depfailed = true;
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() { 
			//some first lepton selector must be present for this to work
			if(depfailed) return false;
		
			unsigned assocVtxIndex = FirstLepton->VtxIndex;
			bool goodVtx = false;
			nGoodVertices = 0;
			for(unsigned ivtx = 0; ivtx != sk->VertexZ->size(); ++ivtx) {
				//double chi2 = sk->VertexChi2->at(ivtx);
				double ndf  = sk->VertexNDF->at(ivtx);
				double z    = sk->VertexZ->at(ivtx);
				double rho  = sk->VertexRho->at(ivtx);
				
				if (!(sk->VertexIsFake->at(ivtx)) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2){
					++nGoodVertices;
					if(ivtx==assocVtxIndex) goodVtx = true;
				}
			}
			//skip event if there are no good vertices
			if(nGoodVertices==0) return false;
			//or if the lepton vertex is not good
			else if(!goodVtx) return false;
			else return true;
		}
		
		//member variables
		KLeptonBaseSelector* FirstLepton;
		int nGoodVertices;
};

//----------------------------------------------------
//selects events based on hadronic tau
class KTauSelector : public KSelector {
	public:
		//constructor
		KTauSelector() : KSelector() { }
		KTauSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
			//check options
			IsoOption = 1;
			localOpt->Get("IsoOption",IsoOption);
			ChargeSign = -1;
			localOpt->Get("ChargeSign",ChargeSign);
			IsoVeto = localOpt->Get("IsoVeto",false);
		}
		
		//accessors
		virtual void SetTree(TTree* tree_) {
			tree = tree_;
			//set tree branches here
			tree->Branch("HPSTauEt","std::vector<double>",&Et);
			tree->Branch("HPSTauEta","std::vector<double>",&Eta);
			tree->Branch("HPSTauPhi","std::vector<double>",&Phi);
			tree->Branch("HPSTauPt","std::vector<double>",&Pt);
			tree->Branch("HPSTauMultiplicity",&Multiplicity,"Multiplicity/I");
			tree->Branch("HPSTauCharge","std::vector<int>",&Charge);
			tree->Branch("HPSTauDecayMode","std::vector<int>",&DecayMode);
			tree->Branch("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits","std::vector<bool>",&byLooseCombinedIsolationDeltaBetaCorr3Hits);
			if(!depfailed){
				if(FirstLepton->GetName()=="Muon") {
					tree->Branch("MassMuonTau",&MassLepTau,"MassLepTau/D");
					tree->Branch("PtMuonTau",&PtLepTau,"PtLepTau/D");
				}
				else {
					tree->Branch("MassElectronTau",&MassLepTau,"MassLepTau/D");
					tree->Branch("PtElectronTau",&PtLepTau,"PtLepTau/D");
				}
			}
			//default values for variables
			Et = Eta = Phi = Pt = NULL;
			Charge = DecayMode = NULL;
			byLooseCombinedIsolationDeltaBetaCorr3Hits = NULL;
			Multiplicity = 0;
			MassLepTau = PtLepTau = 0.0;
		}
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstLeptonIsMuon = true;
			FirstLepton = sel->Get<KLeptonBaseSelector*>("Muon");
			if(!FirstLepton) { FirstLepton = sel->Get<KLeptonBaseSelector*>("Electron"); FirstLeptonIsMuon = false; }
			
			if(!FirstLepton){
				cout << "Input error: dependency Lepton failed in Tau!" << endl;
				depfailed = true;
			}
			else if(FirstLepton->Dummy()){
				cout << "Input error: dependency Lepton is dummy in Tau!" << endl;
				depfailed = true;
			}
			
			SecondLepton = sel->Get<KLeptonBaseSelector*>("SecondMuon");
			if(!SecondLepton) SecondLepton = sel->Get<KLeptonBaseSelector*>("SecondElectron");
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//some first lepton selector must be present for this to work
			if(depfailed) return false;
			
			theGoodTaus.clear();
			for(unsigned t = 0; t < sk->HPSTauPt->size(); t++){
				double dR = KMath::DeltaR(FirstLepton->Phi,FirstLepton->Eta,sk->HPSTauPhi->at(t),sk->HPSTauEta->at(t));
				double dR2 = (SecondLepton && !SecondLepton->Dummy()) ? KMath::DeltaR(SecondLepton->Phi,SecondLepton->Eta,sk->HPSTauPhi->at(t),sk->HPSTauEta->at(t)) : 100;
				
				//boolean to check requirements
				bool goodTau = (sk->HPSTauPt->at(t)>30) //pT cut in GeV
				&& (fabs(sk->HPSTauEta->at(t))<2.3) //require eta < 2.3
				//iso_option -1 -> no isolation required, iso_option 1 -> isolation, iso_option 0 -> anti-isolation
				&& ((IsoOption==-1 || sk->HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)==IsoOption)) //require loose isolation: 3Hits -> lower fake rate
				&& (sk->HPSTauagainstElectronLooseMVA3->at(t)==1) //require loose electron-tau discrimination: MVA3 -> lower fake rate
				//require tight (version 2) muon-tau discrimination for muons, loose (version 2) for electrons
				&& ( (FirstLeptonIsMuon && sk->HPSTauagainstMuonTight2->at(t)==1) || (!FirstLeptonIsMuon && sk->HPSTauagainstMuonLoose2->at(t)==1) )
				//comparison of leading lepton and tau
				&& (dR > 0.5) //require dR > 0.5 with leading lepton
				&& (dR2 > 0.5) //require dR > 0.5 with second lepton (if selected)
				&& (sk->HPSTauVtxIndex->at(t)==FirstLepton->VtxIndex) //require same vertex
				&& (ChargeSign==0 || FirstLepton->Charge*sk->HPSTauCharge->at(t)==ChargeSign); //charge sign requirement (no req if chargesign==0)
				
				//if it passed, store it
				if(goodTau){
					//first check iso veto (for anti-iso region)
					if(IsoVeto && sk->HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)) return false;
					theGoodTaus.push_back(t);
				}
			}
			//skip event if there is not at least one good tau
			if(theGoodTaus.size()<1) return false;
			
			//reset variables
			delete Et; Et = new vector<double>();
			delete Eta; Eta = new vector<double>();
			delete Phi; Phi = new vector<double>();
			delete Pt; Pt = new vector<double>();
			delete byLooseCombinedIsolationDeltaBetaCorr3Hits; byLooseCombinedIsolationDeltaBetaCorr3Hits = new vector<bool>();
			delete Charge; Charge = new vector<int>();
			delete DecayMode; DecayMode = new vector<int>();
			
			//store tau observables
			for(int t = 0; t < theGoodTaus.size(); t++){
				Et->push_back(sk->HPSTauEt->at(theGoodTaus[t]));
				Eta->push_back(sk->HPSTauEta->at(theGoodTaus[t]));
				Phi->push_back(sk->HPSTauPhi->at(theGoodTaus[t]));
				Pt->push_back(sk->HPSTauPt->at(theGoodTaus[t]));
				Charge->push_back(sk->HPSTauCharge->at(theGoodTaus[t]));
				DecayMode->push_back(sk->HPSTauDecayMode->at(theGoodTaus[t]));
				byLooseCombinedIsolationDeltaBetaCorr3Hits->push_back(sk->HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(theGoodTaus[t]));
			}
			Multiplicity = theGoodTaus.size();
			
			//store invariant mass & pT of (highest pT) lepton and tau
			LorentzVector.SetPtEtaPhiM(Pt->at(0),Eta->at(0),Phi->at(0),sk->TauMass);
			TLorentzVector v_tot;
			v_tot = FirstLepton->LorentzVector + LorentzVector; //sum up
			MassLepTau = v_tot.M();
			PtLepTau = v_tot.Pt();
			
			return true;
		}

		//member variables
		int IsoOption, ChargeSign;
		bool IsoVeto, FirstLeptonIsMuon;
		KLeptonBaseSelector *FirstLepton, *SecondLepton;
		vector<int> theGoodTaus;
		vector<double> *Et, *Eta, *Phi, *Pt;
		vector<int> *Charge, *DecayMode;
		vector<bool> *byLooseCombinedIsolationDeltaBetaCorr3Hits;
		int Multiplicity;
		double MassLepTau, PtLepTau;
		TLorentzVector LorentzVector;
};

//----------------------------------------------------
//gen-level checks for hadronic tau
class KGenTauSelector : public KSelector {
	public:
		//constructor
		KGenTauSelector() : KSelector() { }
		KGenTauSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { canfail = false; }
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("HPSTauFakeJet","std::vector<bool>",&FakeJet);
			tree->Branch("HPSTauFakeLep","std::vector<bool>",&FakeLep);
			tree->Branch("HPSTauReal","std::vector<bool>",&Real);
			tree->Branch("HPSTauFakeBJet","std::vector<bool>",&FakeBJet);
			//default values for variables
			FakeJet = FakeLep = Real = FakeBJet = NULL;
		}
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			TheTau = sel->Get<KTauSelector*>("Tau");
			if(!TheTau){
				cout << "Input error: dependency Tau failed in GenTau!" << endl;
				depfailed = true;
			}
			else if(TheTau->Dummy()){
				cout << "Input error: dependency Tau is dummy in GenTau!" << endl;
				depfailed = true;
			}
		}
		virtual void SetSkimmer(KSkimmer* sk_) {
			sk = sk_;
			//get mother particle ID from global options
			if(!sk->globalOpt->Get("mother",mother)) mother = -1;
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//the tau selector must be present for this to work
			if(depfailed) return false;
			
			//reset variables
			delete FakeJet; FakeJet = new vector<bool>(); FakeJet->resize(TheTau->theGoodTaus.size());
			delete FakeLep; FakeLep = new vector<bool>(); FakeLep->resize(TheTau->theGoodTaus.size());
			delete Real; Real = new vector<bool>(); Real->resize(TheTau->theGoodTaus.size());
			delete FakeBJet; FakeBJet = new vector<bool>(); FakeBJet->resize(TheTau->theGoodTaus.size());
			for(unsigned t = 0; t < TheTau->theGoodTaus.size(); t++){
				//first, assume jet faking tau
				Real->at(t) = FakeLep->at(t) = FakeBJet->at(t) = false;
				FakeJet->at(t) = true;
				
				//assume all real taus for data
				if(mother==-1) { Real->at(t) = true; FakeLep->at(t) = FakeJet->at(t) = FakeBJet->at(t) = false; } 
				else {
					vector<int> goodGens(8,-1); //indices 0..7 correspond to: 15, -15, 13, -13, 11, -11, 5, -5
				
					//loop over genparticles to find good leptons
					for(unsigned g = 0; g < sk->GenParticlePdgId->size(); g++){
						//check mother PDGID = specified mother ID, w/ safety check for -1 (no mother)
						bool goodGen = (sk->GenParticleMotherIndex->at(g)>=0 && fabs(sk->GenParticlePdgId->at(sk->GenParticleMotherIndex->at(g)))==mother);
						if(goodGen){
							if(sk->GenParticlePdgId->at(g)==15) goodGens[0] = g;
							else if(sk->GenParticlePdgId->at(g)==-15) goodGens[1] = g;
							else if(sk->GenParticlePdgId->at(g)==13) goodGens[2] = g;
							else if(sk->GenParticlePdgId->at(g)==-13) goodGens[3] = g;
							else if(sk->GenParticlePdgId->at(g)==11) goodGens[4] = g;
							else if(sk->GenParticlePdgId->at(g)==-11) goodGens[5] = g;
						}
						//check mother PDGID = top, w/ safety check for -1 (no mother)
						goodGen = (sk->GenParticleMotherIndex->at(g)>=0 && fabs(sk->GenParticlePdgId->at(sk->GenParticleMotherIndex->at(g)))==6);
						if(goodGen){
							if(sk->GenParticlePdgId->at(g)==5) goodGens[6] = g;
							else if(sk->GenParticlePdgId->at(g)==-5) goodGens[7] = g;
						}
					}
					
					//loop over good gen particles to compare w/ reco tau
					for(unsigned g = 0; g < goodGens.size(); g++){
						if(goodGens[g]==-1) continue;
						double dR = KMath::DeltaR(sk->GenParticlePhi->at(goodGens[g]),sk->GenParticleEta->at(goodGens[g]),TheTau->Phi->at(t),TheTau->Eta->at(t));
						if(dR < 0.1){
							//check for ele or mu PDGID
							if(fabs(sk->GenParticlePdgId->at(goodGens[g]))==11 || fabs(sk->GenParticlePdgId->at(goodGens[g]))==13) { 
								FakeLep->at(t) = true; FakeJet->at(t) = Real->at(t) = FakeBJet->at(t) = false;
							}
							//check for b quark PDGID
							else if(fabs(sk->GenParticlePdgId->at(goodGens[g]))==5){
								FakeJet->at(t) = FakeBJet->at(t) = true; FakeLep->at(t) = Real->at(t) = false;
							}
							//check for tau PDGID
							else if(fabs(sk->GenParticlePdgId->at(goodGens[g]))==15){
								Real->at(t) = true; FakeLep->at(t) = FakeJet->at(t) = FakeBJet->at(t) = false;
							} 
							//if none of these conditions are met, default assumption of jet faking tau will be kept
							break;
						}
					}
				}
			}
			
			return true;
		}
		
		//member variables
		KTauSelector *TheTau;
		int mother;
		vector<bool> *FakeJet, *FakeLep, *Real, *FakeBJet;
};

//----------------------------------------------------
//gen-level checks for charginos from RPV stops
class KGenCharginoSelector : public KSelector {
	public:
		//constructor
		KGenCharginoSelector() : KSelector() { }
		KGenCharginoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { canfail = false; }
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("CharginoMultiplicity",&Multiplicity,"Multiplicity/I");
			//default values for variables
			Multiplicity = 0;
		}
		virtual void SetSkimmer(KSkimmer* sk_) {
			sk = sk_;
			//get mother particle ID from global options
			if(!sk->globalOpt->Get("mother",mother)) mother = -1;
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//check for 2 charginos from RPV stops
			Multiplicity = 0;
			if(mother==1000006){
				for(unsigned g = 0; g < sk->GenParticlePdgId->size(); g++){
					if(fabs(sk->GenParticlePdgId->at(g))==1000024 && sk->GenParticleMotherIndex->at(g)>=0 && fabs(sk->GenParticlePdgId->at(sk->GenParticleMotherIndex->at(g)))==mother) ++Multiplicity;
				}
			}
			
			return true;
		}
		
		//member variables
		int mother;
		int Multiplicity;
};

//----------------------------------------------------
//stores MET info
class KMETSelector : public KSelector {
	public:
		//constructor
		KMETSelector() : KSelector() { }
		KMETSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { canfail = false; }
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("PFMET",&PFMET,"PFMET/D");
			tree->Branch("PFMETPatType1",&PFMETPatType1,"PFMETPatType1/D");
			tree->Branch("PFMETPhi",&PFMETPhi,"PFMETPhi/D");
			tree->Branch("PFMETPhiPatType1",&PFMETPhiPatType1,"PFMETPhiPatType1/D");
			//default values for variables
			PFMET = PFMETPatType1 = PFMETPhi = PFMETPhiPatType1 = 0;
		}
		
		//set variable values for tree
		virtual bool Cut() {
			PFMET = sk->PFMET->at(0);
			PFMETPatType1 = sk->PFMETPatType1->at(0);
			PFMETPhi = sk->PFMETPhi->at(0);
			PFMETPhiPatType1 = sk->PFMETPhiPatType1->at(0);
			
			return true;
		}
		
		//member variables
		double PFMET, PFMETPatType1, PFMETPhi, PFMETPhiPatType1;
		int run, ls, event;
		double trueNInteraction;
};

//----------------------------------------------------
//selects events based on jets
class KJetSelector : public KSelector {
	public:
		//constructor
		KJetSelector() : KSelector() { }
		KJetSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) {
			canfail = false;
			//check options
			nTau = 1;
			localOpt->Get("nTau",nTau);
		}
		
		//accessors
		virtual void SetTree(TTree* tree_) { 
			tree = tree_;
			//set tree branches here
			tree->Branch("PFJetEnergy","std::vector<double>",&Energy);
			tree->Branch("PFJetPt","std::vector<double>",&Pt);
			tree->Branch("PFJetEta","std::vector<double>",&Eta);
			tree->Branch("PFJetPhi","std::vector<double>",&Phi);
			tree->Branch("PFJetCSVBTag","std::vector<double>",&CSVBTag);
			tree->Branch("PFJetPartonFlavour","std::vector<double>",&PartonFlavour);
			tree->Branch("PFJetVtxIndex","std::vector<int>",&VtxIndex);
			tree->Branch("PFJetMultiplicity",&Multiplicity,"Multiplicity/I");
			tree->Branch("PFMainJetPt",MainPt,"MainPt[2]/D");
			tree->Branch("PFMainJetEnergy",MainEnergy,"MainEnergy[2]/D");
			tree->Branch("PFMainJetEta",MainEta,"MainEta[2]/D");
			tree->Branch("PFMainJetPhi",MainPhi,"MainPhi[2]/D");
			tree->Branch("PFMainJetCSVBTag",MainCSVBTag,"MainCSVBTag[2]/D");
			tree->Branch("PFMainJetPartonFlavour",MainPartonFlavour,"MainPartonFlavour[2]/D");
			tree->Branch("IndexTauJet",&IndexTauJet,"IndexTauJet/I");
			tree->Branch("MassTauJet",&MassTauJet,"MassTauJet/D");
			tree->Branch("PtTauJet",&PtTauJet,"PtTauJet/D");
			if(!depfailed){
				if(FirstLepton->GetName()=="Muon") {
					tree->Branch("IndexMuonJet",&IndexLepJet,"IndexLepJet/I");
					tree->Branch("MassMuonJet",&MassLepJet,"MassLepJet/D");
					tree->Branch("PtMuonJet",&PtLepJet,"PtLepJet/D");
				}
				else {
					tree->Branch("IndexElectronJet",&IndexLepJet,"IndexLepJet/I");
					tree->Branch("MassElectronJet",&MassLepJet,"MassLepJet/D");
					tree->Branch("PtElectronJet",&PtLepJet,"PtLepJet/D");
				}
			}
			//default values for variables
			Energy = Eta = Phi = Pt = CSVBTag = PartonFlavour = NULL;
			VtxIndex = NULL;
			Multiplicity = IndexTauJet = IndexLepJet = 0;
			MassTauJet = PtTauJet = MassLepJet = PtLepJet = 0.0;
			MainPt[0] = MainEnergy[0] = MainPhi[0] = MainEta[0] = MainCSVBTag[0] = MainPartonFlavour[0] = 0; //jet
			MainPt[1] = MainEnergy[1] = MainPhi[1] = MainEta[1] = MainCSVBTag[1] = MainPartonFlavour[1] = 0; //bjet
		}
		virtual void SetSelection(KSelection* sel_) {
			sel = sel_;
			//set dependencies here
			FirstLepton = sel->Get<KLeptonBaseSelector*>("Muon");
			if(!FirstLepton) FirstLepton = sel->Get<KLeptonBaseSelector*>("Electron");
			
			if(!FirstLepton){
				cout << "Input error: dependency Lepton failed in Jet!" << endl;
				depfailed = true;
			}
			else if(FirstLepton->Dummy()){
				cout << "Input error: dependency Lepton is dummy in Jet!" << endl;
				depfailed = true;
			}
			
			SecondLepton = sel->Get<KLeptonBaseSelector*>("SecondMuon");
			if(!SecondLepton) SecondLepton = sel->Get<KLeptonBaseSelector*>("SecondElectron");
			
			TheTau = sel->Get<KTauSelector*>("Tau");
			
			if(!TheTau){
				cout << "Input error: dependency Tau failed in Jet!" << endl;
				depfailed = true;
			}
			else if(TheTau->Dummy()){
				cout << "Input error: dependency Tau is dummy in Jet!" << endl;
				depfailed = true;
			}
		} 
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//first lepton and tau selectors must be present for this to work
			if(depfailed) return false;
			
			theGoodJets.clear();
			theMainJet = -1;
			theMainBJet = -1;
			for(unsigned j = 0; j < sk->PFJetPt->size(); j++){
				double dRl = KMath::DeltaR(FirstLepton->Phi,FirstLepton->Eta,sk->PFJetPhi->at(j),sk->PFJetEta->at(j));
				double dRl2 = (SecondLepton && !SecondLepton->Dummy()) ? KMath::DeltaR(SecondLepton->Phi,SecondLepton->Eta,sk->PFJetPhi->at(j),sk->PFJetEta->at(j)) : 100;
				
				bool goodJet = (sk->PFJetPt->at(j)>30) //pT cut
				&& (fabs(sk->PFJetEta->at(j))<2.4) //eta cut
				&& sk->PFJetPassLooseID->at(j) //loose ID
				&& (dRl > 0.5) //separation from lepton
				&& (dRl2 > 0.5); //separation from second lepton (if selected)
				
				//separation from leading good tau or all good taus
				int nTau_ = nTau;
				if(nTau==-1) nTau_ = TheTau->theGoodTaus.size();
				for(int t = 0; t < nTau_; t++){
					double dRt = KMath::DeltaR(TheTau->Phi->at(t),TheTau->Eta->at(t),sk->PFJetPhi->at(j),sk->PFJetEta->at(j));
					goodJet &= (dRt > 0.5); 
				}
				
				//if it passed, store it
				if(goodJet){
					theGoodJets.push_back(j); //jets vector *does* contain the main 2 jets
				
					if(theMainBJet==-1 && sk->PFJetCombinedSecondaryVertexBTag->at(j)>0.244) theMainBJet = j;
					else if(theMainJet==-1) theMainJet = j;
				}
			}
			
			//reset variables
			delete Energy; Energy = new vector<double>();
			delete Eta; Eta = new vector<double>();
			delete Phi; Phi = new vector<double>();
			delete Pt; Pt = new vector<double>();
			delete CSVBTag; CSVBTag = new vector<double>();
			delete PartonFlavour; PartonFlavour = new vector<double>();
			delete VtxIndex; VtxIndex = new vector<int>();
			MainPt[0] = MainEnergy[0] = MainPhi[0] = MainEta[0] = MainCSVBTag[0] = MainPartonFlavour[0] = 0; //jet
			MainPt[1] = MainEnergy[1] = MainPhi[1] = MainEta[1] = MainCSVBTag[1] = MainPartonFlavour[1] = 0; //bjet
			IndexTauJet = IndexLepJet = 0;
			MassTauJet = PtTauJet = MassLepJet = PtLepJet = 0.0;
			
			//store tau observables
			for(int j = 0; j < theGoodJets.size(); j++){
				Energy->push_back(sk->PFJetEnergy->at(theGoodJets[j]));
				Eta->push_back(sk->PFJetEta->at(theGoodJets[j]));
				Phi->push_back(sk->PFJetPhi->at(theGoodJets[j]));
				Pt->push_back(sk->PFJetPt->at(theGoodJets[j]));
				CSVBTag->push_back(sk->PFJetCombinedSecondaryVertexBTag->at(theGoodJets[j]));
				PartonFlavour->push_back(sk->PFJetPartonFlavour->at(theGoodJets[j]));
				VtxIndex->push_back(sk->PFJetBestVertexTrackAssociationIndex->at(theGoodJets[j]));
			}
			Multiplicity = theGoodJets.size();
			
			//store main jet & bjet observables
			//bjet is always index 1, other jet is always index 0
			if(theMainJet!=-1 && theMainBJet!=-1){
				int theJets[2] = {theMainJet,theMainBJet};
				
				TLorentzVector v_jet[2];
				TLorentzVector v_lepjet[2];
				TLorentzVector v_taujet[2];
				for(int j = 0; j < 2; j++){
					MainPt[j] = sk->PFJetPt->at(theJets[j]);
					MainEnergy[j] = sk->PFJetEnergy->at(theJets[j]);
					MainPhi[j] = sk->PFJetPhi->at(theJets[j]);
					MainEta[j] = sk->PFJetEta->at(theJets[j]);
					MainCSVBTag[j] = sk->PFJetCombinedSecondaryVertexBTag->at(theJets[j]);
					MainPartonFlavour[j] = sk->PFJetPartonFlavour->at(theJets[j]);
					
					//make jet vector and lepton+jet combination vectors
					v_jet[j].SetPtEtaPhiE(MainPt[j],MainEta[j],MainPhi[j],MainEnergy[j]);
					v_lepjet[j] = FirstLepton->LorentzVector + v_jet[j];
					v_taujet[j] = TheTau->LorentzVector + v_jet[j];
				}

				//minimize delta(M_tj - M_mj) (pairing of leptons and jets)
				double pair_t0m1 = fabs(v_taujet[0].M()-v_lepjet[1].M());
				double pair_t1m0 = fabs(v_taujet[1].M()-v_lepjet[0].M());
				if(pair_t0m1 < pair_t1m0){
					IndexTauJet = 0;
					IndexLepJet = 1;
				}
				else{
					IndexTauJet = 1;
					IndexLepJet = 0;
				}
				MassTauJet = v_taujet[IndexTauJet].M();
				PtTauJet = v_taujet[IndexTauJet].Pt();
				MassLepJet = v_lepjet[IndexLepJet].M();
				PtLepJet = v_lepjet[IndexLepJet].Pt();
			}
			
			return true;
		}
		
		//member variables
		int nTau;
		KLeptonBaseSelector *FirstLepton, *SecondLepton;
		KTauSelector *TheTau;
		vector<int> theGoodJets;
		vector<double> *Energy, *Eta, *Phi, *Pt, *CSVBTag, *PartonFlavour;
		vector<int> *VtxIndex;
		int theMainJet, theMainBJet;
		double MainPt[2], MainEnergy[2], MainEta[2], MainPhi[2], MainCSVBTag[2], MainPartonFlavour[2];
		int Multiplicity, IndexTauJet, IndexLepJet;
		double MassTauJet, PtTauJet, MassLepJet, PtLepJet;
};

//-------------------------------------------------------------
//addition to KParser to create selectors
namespace KParser {
	KSelector* processSelector(KNamed* tmp){
		KSelector* srtmp = 0;
		string sname = tmp->first;
		OptionMap* omap = tmp->second;
		
		//check for all known selectors
		if(sname=="EventInfo") srtmp = new KEventInfoSelector(sname,omap);
		else if(sname=="HLT") srtmp = new KHLTSelector(sname,omap);
		else if(sname=="Muon") srtmp = new KMuonSelector(sname,omap);
		else if(sname=="SecondMuon") srtmp = new KSecondMuonSelector(sname,omap);
		else if(sname=="SecondMuonVeto") srtmp = new KSecondMuonVetoSelector(sname,omap);
		else if(sname=="MuonVeto") srtmp = new KMuonVetoSelector(sname,omap);
		else if(sname=="Electron") srtmp = new KElectronSelector(sname,omap);
		else if(sname=="SecondElectron") srtmp = new KSecondElectronSelector(sname,omap);
		else if(sname=="SecondElectronVeto") srtmp = new KSecondElectronVetoSelector(sname,omap);
		else if(sname=="ElectronVeto") srtmp = new KElectronVetoSelector(sname,omap);
		else if(sname=="Vertex") srtmp = new KVertexSelector(sname,omap);
		else if(sname=="Tau") srtmp = new KTauSelector(sname,omap);
		else if(sname=="GenTau") srtmp = new KGenTauSelector(sname,omap);
		else if(sname=="GenChargino") srtmp = new KGenCharginoSelector(sname,omap);
		else if(sname=="Jet") srtmp = new KJetSelector(sname,omap);
		else if(sname=="MET") srtmp = new KMETSelector(sname,omap);
		else {} //skip unknown selectors
		
		if(!srtmp) cout << "Input error: unknown selector " << sname << ". This selector will be skipped." << endl;
		
		return srtmp;
	}
}

#endif