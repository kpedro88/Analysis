#define LQClass_cxx
#include "LQClass.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <unistd.h> //for sleep

using namespace std;

#define nCtr 6

//----------------
//helper functions
double LQClass::phi(double x, double y) {
	double phi_ = atan2(y, x);
	return (phi_>=0) ?  phi_ : phi_ + 2*TMath::Pi();
}
double LQClass::DeltaPhi(double phi1, double phi2) {
	double phi1_= phi( cos(phi1), sin(phi1) );
	double phi2_= phi( cos(phi2), sin(phi2) );
	double dphi_= phi1_-phi2_;
	if( dphi_> TMath::Pi() ) dphi_-=2*TMath::Pi();
	if( dphi_<-TMath::Pi() ) dphi_+=2*TMath::Pi();

	return dphi_;
}
double LQClass::DeltaR(double phi1, double eta1, double phi2, double eta2){
	double dphi = DeltaPhi(phi1,phi2);
	double deta = eta2 - eta1;
	double dR2 = dphi*dphi + deta*deta;
	return sqrt(dR2);
}
//generalization for processing a line
void LQClass::process(string line, char delim, vector<string>& fields){
	stringstream ss(line);
	string field;
	while(getline(ss,field,delim)){
		fields.push_back(field);
	}
}

//------------------------
//systematic uncertainties

//--------------------------------------------
//clear and fill vectors of original pt values
void LQClass::StoreOrigVars(){
	PFJetPtOrig.clear();
	PFJetEnergyOrig.clear();
	HPSTauPtOrig.clear();
	
	PFJetPtOrig.reserve(PFJetPt->size());
	PFJetEnergyOrig.reserve(PFJetEnergy->size());
	for(unsigned j = 0; j < PFJetPt->size(); j++){
		PFJetPtOrig.push_back(PFJetPt->at(j));
		PFJetEnergyOrig.push_back(PFJetEnergy->at(j));
	}
	
	HPSTauPtOrig.reserve(HPSTauPt->size());
	for(unsigned t = 0; t < HPSTauPt->size(); t++){
		HPSTauPtOrig.push_back(HPSTauPt->at(t));
	}
}

//-------------------------------------------
//restore original pt values to standard vars
void LQClass::RestoreOrigVars(){
	for(unsigned j = 0; j < PFJetPt->size(); j++){
		PFJetPt->at(j) = PFJetPtOrig[j];
		PFJetEnergy->at(j) = PFJetEnergyOrig[j];
	}

	for(unsigned t = 0; t < HPSTauPt->size(); t++){
		HPSTauPt->at(t) = HPSTauPtOrig[t];
	}
}

//---------------------------------
//Scale tau pT by TauES uncertainty
void LQClass::TauES(bool up){
	double unc = 0.03;
	for(unsigned t = 0; t < HPSTauPt->size(); t++){
		TLorentzVector v_tau;
		v_tau.SetPtEtaPhiM(HPSTauPt->at(t),HPSTauEta->at(t),HPSTauPhi->at(t),s_TauMass);
	
		if(up) v_tau *= (1+unc);
		else v_tau *= (1-unc);
		
		HPSTauPt->at(t) = v_tau.Pt();
	}
}

//---------------
//Tau ER smearing
void LQClass::TauER(bool up){
	double unc = 0.1;

	for(unsigned t = 0; t < HPSTauPt->size(); t++){
		double min_dR = 1000;
		bool matchedGenJetFound = false;
		int index_gen = -1;

		for (unsigned g = 0; g < GenJetPt->size(); g++){
			double dR = DeltaR(HPSTauPhi->at(t),HPSTauEta->at(t),GenJetPhi->at(g),GenJetEta->at(g));
			if(dR < min_dR) {
				min_dR = dR;
				index_gen = g;
			}
		}
		if (min_dR < 0.05) matchedGenJetFound = true;

		if (matchedGenJetFound) {
			TLorentzVector v_tau, vg;
			v_tau.SetPtEtaPhiM(HPSTauPt->at(t),HPSTauEta->at(t),HPSTauPhi->at(t),s_TauMass);
			vg.SetPtEtaPhiE(GenJetPt->at(index_gen),GenJetEta->at(index_gen),GenJetPhi->at(index_gen),GenJetEnergy->at(index_gen));
		
			double f = 0;
			if(up) f = max(vg.E() + (1+unc)*(v_tau.E()-vg.E()),0.)/v_tau.E();
			else f = max(vg.E() + (1-unc)*(v_tau.E()-vg.E()),0.)/v_tau.E();
			
			v_tau *= f;
			HPSTauPt->at(t) = v_tau.Pt();
		}
	}
}

/*
//---------------
//Tau ER smearing
void LQClass::TauER(bool up){
	double unc = 0.1;

	for(unsigned t = 0; t < HPSTauPt->size(); t++){
		double min_dR = 1000;
		bool matchedGenJetFound = false;
		double pt_gen = 0;

		for (unsigned g = 0; g < GenJetPt->size(); g++){
			double dR = DeltaR(HPSTauPhi->at(t),HPSTauEta->at(t),GenJetPhi->at(g),GenJetEta->at(g));
			if(dR < min_dR) {
				min_dR = dR;
				pt_gen = GenJetPt->at(g);
			}
		}
		if (min_dR < 0.05) matchedGenJetFound = true;

		if (matchedGenJetFound) {
			if(up) HPSTauPt->at(t) = max(pt_gen + (1+unc)*(HPSTauPt->at(t) - pt_gen),0.);
			else HPSTauPt->at(t) = max(pt_gen + (1-unc)*(HPSTauPt->at(t) - pt_gen),0.);
		}
	}
}
*/

//-------------------------------
//Scale jet pT by JEC uncertainty
void LQClass::JetES(bool up){
	for(unsigned j = 0; j < PFJetPt->size(); j++){
		double unc = PFJetJECUnc->at(j);
		TLorentzVector vj;
		vj.SetPtEtaPhiE(PFJetPt->at(j),PFJetEta->at(j),PFJetPhi->at(j),PFJetEnergy->at(j));
		//smear 4-vector (nonzero jet mass)
		if(up) vj *= (1+unc);
		else vj *= (1-unc);
		PFJetPt->at(j) = vj.Pt();
		PFJetEnergy->at(j) = vj.E();
	}
}

//------------
//JER smearing
void LQClass::JetER(bool up){
	for(unsigned j = 0; j < PFJetPt->size(); j++){
		double min_dR = 1000;
		bool matchedGenJetFound = false;
		int index_gen = -1;

		for (unsigned g = 0; g < GenJetPt->size(); g++){
			double dR = DeltaR(PFJetPhi->at(j),PFJetEta->at(j),GenJetPhi->at(g),GenJetEta->at(g));
			if(dR < min_dR) {
				min_dR = dR;
				index_gen = g;
			}
		}
		if (min_dR < 0.4) matchedGenJetFound = true;
		
		if(matchedGenJetFound){
			TLorentzVector vj, vg;
			vj.SetPtEtaPhiE(PFJetPt->at(j),PFJetEta->at(j),PFJetPhi->at(j),PFJetEnergy->at(j));
			vg.SetPtEtaPhiE(GenJetPt->at(index_gen),GenJetEta->at(index_gen),GenJetPhi->at(index_gen),GenJetEnergy->at(index_gen));
			
			vj = SmearJER(vj,vg,up);
			PFJetPt->at(j) = vj.Pt();
			PFJetEnergy->at(j) = vj.E();
		}
	}
}

//-----------------------
//helper for JER smearing
TLorentzVector LQClass::SmearJER(TLorentzVector vj, TLorentzVector vg, bool up){
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
	
	//TLorentzVector vj2;
	//if(up) vj2 = vg + c*(vj-vg);
	//else vj2 = vg + (1.-(c-1.))*(vj-vg);
	//check for quantities < 0?
	
	return f*vj;
}

/*

//------------
//JER smearing
void LQClass::JetER(bool up){
	for(unsigned j = 0; j < PFJetPt->size(); j++){
		double min_dR = 1000;
		bool matchedGenJetFound = false;
		double pt_gen = 0;
		double en_gen = 0;

		for (unsigned g = 0; g < GenJetPt->size(); g++){
			double dR = DeltaR(PFJetPhi->at(j),PFJetEta->at(j),GenJetPhi->at(g),GenJetEta->at(g));
			if(dR < min_dR) {
				min_dR = dR;
				pt_gen = GenJetPt->at(g);
				en_gen = GenJetEnergy->at(g);
			}
		}
		if (min_dR < 0.4) matchedGenJetFound = true;

		PFJetPt->at(j) = SmearJER(PFJetPt->at(j),PFJetEta->at(j),pt_gen,up,matchedGenJetFound);
		PFJetEnergy->at(j) = SmearJER(PFJetEnergy->at(j),PFJetEta->at(j),en_gen,up,matchedGenJetFound);
	}
}

//-----------------------
//helper for JER smearing
double LQClass::SmearJER(double ptreco, double eta, double ptgen, bool up, bool doSmear){
	double c = 1.;
	double abseta = TMath::Abs(eta);
	if (abseta <= 0.5) c = 1.052;
	else if (abseta > 0.5 && abseta <= 1.1) c = 1.057;
	else if (abseta > 1.1 && abseta <= 1.7) c = 1.096;
	else if (abseta > 1.7 && abseta <= 2.3) c = 1.134;
	else c = 1.288;
	
	double ptsmeared = 0;
	if (doSmear) {
		if(up) ptsmeared = max(ptgen + c*(ptreco-ptgen),0.);
		else ptsmeared = max(ptgen + (1.-(c-1.))*(ptreco-ptgen),0.);
	}
	else ptsmeared = ptreco;
	
	return ptsmeared;
}

*/

//------------------
//modular selections

//--------------------------
//HLT trigger line selection
//default: HLTLine = "HLT_IsoMU24"
Bool_t LQClass::HLTSelection(string HLTLine){
	//loop over trigger names
	bool goodTrigger = false;
	for(unsigned h = 0; h < HLTInsideDatasetTriggerNames->size(); h++){
		//check:
		//1) if the line matches the current inside dataset line
		//2) if the decision was true (the line fired)
		//3) if the line was not prescaled
		if(HLTInsideDatasetTriggerNames->at(h).compare(0,HLTLine.size(),HLTLine)==0 && HLTInsideDatasetTriggerDecisions->at(h) && HLTInsideDatasetTriggerPrescales->at(h)==1) {
			goodTrigger = true;
			break;
		}
	}
	//skip event if finished searching and no HLT lines found
	return goodTrigger;
}

//-----------
//tight muons
//default: pt = 30
Bool_t LQClass::MuonIDTight(int m, double pt){
	//boolean to check requirements
	bool goodMuon = (MuonPt->at(m)>pt) //pT cut in GeV
	&& (fabs(MuonEta->at(m))<2.1) //eta cut to match trigger IsoMu24_eta2p1 (2012A)
	&& (MuonIsPF->at(m)==1) //require PF muon
	&& (MuonPassID->at(m)==1) //require global, chi2 < 10, chamber hits
	//&& (MuonIsGlobal->at(m)==1) //require global
	//&& (MuonGlobalChi2->at(m)<10) //require global fit chi2/ndof < 10
	//&& (MuonSegmentMatches->at(m)-MuonStationMatches->at(m)>=1) //require chamber hits (segment - station)
	&& (MuonStationMatches->at(m)>=1) //require station hits
	&& (fabs(MuonBestTrackVtxDistXY->at(m))<0.2) //require impact parameter < 0.2
	&& (fabs(MuonBestTrackVtxDistZ->at(m))<0.5) //require longitudinal distance of track < 5 mm = .5 cm
	&& (MuonPixelHits->at(m)>0) //require pixel hits
	&& (MuonTrackLayersWithMeasurement->at(m)>5) //require 5+ tracker layers with hits
	&& ((MuonPFIsoR04ChargedHadron->at(m) + max(MuonPFIsoR04Photon->at(m) + MuonPFIsoR04NeutralHadron->at(m) - 0.5*MuonPFIsoR04PU->at(m),0.))/MuonPt->at(m)<0.12); //require tight (<0.12 GeV) relative PF isolation

	return goodMuon;
}

//-----------
//loose muons
//default: pt = 20
Bool_t LQClass::MuonIDLoose(int m, double pt){
	//boolean to check requirements
	bool looseMuon = (MuonPt->at(m)>pt) //pT cut in GeV
	&& (fabs(MuonEta->at(m))<2.4) //eta cut to ensure good ID
	&& (MuonIsPF->at(m)==1) //require PF muon
	&& (MuonIsGlobal->at(m)==1 || MuonIsTracker->at(m)==1) //require global OR tracker
	&& ((MuonPFIsoR04ChargedHadron->at(m) + max(MuonPFIsoR04Photon->at(m) + MuonPFIsoR04NeutralHadron->at(m) - 0.5*MuonPFIsoR04PU->at(m),0.))/MuonPt->at(m)<0.3); //require loose (<0.3) relative PF isolation

	return looseMuon;
}

//------------------------
//muon selection (leading)
Bool_t LQClass::MuonSelection(){
	vector<int> goodMuons;
	for(unsigned m = 0; m < MuonPt->size(); m++){
		//boolean to check requirements
		bool goodMuon = MuonIDTight(m,30);
		
		//if it passed, store it
		if(goodMuon) goodMuons.push_back(m);
	}
	//skip event if there is not at least one good muon
	if(goodMuons.size()<1) return false;
	else { //otherwise, store leading muon (lowest index) and vectors
		theGoodMuon = goodMuons[0];
		theGoodMuons = goodMuons;
		return true;
	}
}

//------------------------------------------------
//muon selection (control region: loose not tight)
Bool_t LQClass::MuonSelectionLooseNotTight(){
	vector<int> goodMuons;
	for(unsigned m = 0; m < MuonPt->size(); m++){
		//boolean to check requirements
		bool tightMuon = MuonIDTight(m,30);
		bool looseMuon = MuonIDLoose(m,30);
		
		//if it passed, store it
		if(looseMuon && !tightMuon) goodMuons.push_back(m);
	}
	//skip event if there is not at least one good muon
	if(goodMuons.size()<1) return false;
	else { //otherwise, store leading muon (lowest index) and vectors
		theGoodMuon = goodMuons[0];
		theGoodMuons = goodMuons;
		return true;
	}
}

//-----------------------
//muon selection (second)
//default: pt = 25
Bool_t LQClass::MuonSelectionSecond(double pt){
	vector<int> goodMuons;
	for(unsigned m = 0; m < MuonPt->size(); m++){
		//boolean to check requirements
		bool goodMuon = MuonIDLoose(m,pt);
		
		//comparison with leading muon
		double dR = DeltaR(MuonPhi->at(theGoodMuon),MuonEta->at(theGoodMuon),MuonPhi->at(m),MuonEta->at(m));
		goodMuon &= (m != theGoodMuon) //different muon
		&& (MuonCharge->at(m) != MuonCharge->at(theGoodMuon)) //opposite charge
		//&& (fabs(MuonBestTrackVtxDistZ->at(m) - muonZPV) < 0.2); //same vertex
		&& (MuonBestTrackVtxIndex->at(m) == MuonBestTrackVtxIndex->at(theGoodMuon)); //same vertex
		
		//if it passed, store it
		if(goodMuon) goodMuons.push_back(m);
	}
	//skip event if there is not at least one good muon
	if(goodMuons.size()<1) return false;
	else { //otherwise, store leading muon (lowest index) and vectors
		theGoodMuon2 = goodMuons[0];
		theGoodMuons2 = goodMuons;
		return true;
	}
}

//------------------
//loose OS muon veto
Bool_t LQClass::MuonVetoOS(){
	for(unsigned m = 0; m < MuonPt->size(); m++){
		//boolean to check requirements
		bool looseMuon = MuonIDLoose(m,20);
		
		//skip event if there are two OS muons
		if(looseMuon && MuonCharge->at(theGoodMuon) != MuonCharge->at(m)) return false;
	}
	return true;
}

//------------------
//loose muon veto
Bool_t LQClass::MuonVetoLoose(){
	for(unsigned m = 0; m < MuonPt->size(); m++){
		//boolean to check requirements
		bool looseMuon = MuonIDLoose(m,20);
		
		//skip event if there are multiple loose muons
		if(looseMuon) return false;
	}
	return true;
}

//--------------------
//vertex quality check
Bool_t LQClass::VertexQuality(){
	unsigned assocVtxIndex = MuonBestTrackVtxIndex->at(theGoodMuon);
	bool goodMuonVtx = false;
	nGoodVertices = 0;
	for(unsigned ivtx = 0; ivtx != VertexZ->size(); ++ivtx) {
		//double chi2 = VertexChi2->at(ivtx);
		double ndf  = VertexNDF->at(ivtx);
		double z    = VertexZ->at(ivtx);
		double rho  = VertexRho->at(ivtx);
		
		if (!(VertexIsFake->at(ivtx)) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2){
			++nGoodVertices;
			if(ivtx==assocVtxIndex) goodMuonVtx = true;
		}
	}
	//skip event if there are no good vertices
	if(nGoodVertices==0) return false;
	//or if the muon vertex is not good
	else if(!goodMuonVtx) return false;
	else return true;
}

//-------------------------------------------------------------
//opposite-sign electron veto (to avoid overlap w/ emu channel)
Bool_t LQClass::ElectronVeto(){
	for(unsigned e = 0; e < ElectronPt->size(); e++){
		bool goodEle = (ElectronPt->at(e)>30) //pT cut in GeV
		&& (fabs(ElectronEta->at(e))<1.444||(fabs(ElectronEta->at(e))>1.56&&fabs(ElectronEta->at(e))<2.5)) //ECAL eta requirements
		&& (ElectronPassEGammaIDMedium->at(e)==1023); //EGammaID, medium working point (TestWP rather than PassWP)
		
		//cut events with a good electron that has opposite sign charge w/ the leading muon
		if(goodEle && ElectronCharge->at(e)*MuonCharge->at(theGoodMuon)==-1) return false;
	}
	return true;
}

//-------------------
//loose electron veto
Bool_t LQClass::ElectronVetoLoose(){
	for(unsigned e = 0; e < ElectronPt->size(); e++){
		bool goodEle = true;
		goodEle &= (ElectronPt->at(e)>20); //pT cut in GeV
		goodEle &= (fabs(ElectronEta->at(e))<1.444||(fabs(ElectronEta->at(e))>1.56&&fabs(ElectronEta->at(e))<2.5)); //ECAL eta requirements
		goodEle &= (ElectronPassEGammaIDLoose->at(e)==1023); //EGammaID, loose working point
		
		//cut events with any extra electrons
		if(goodEle) return false;
	}
	return true;
}

//-------------
//tau selection
//default: iso_option = 1, chargesign = -1
Bool_t LQClass::TauSelection(int iso_option, int chargesign){
	vector<int> goodTaus;
	for(unsigned t = 0; t < HPSTauPt->size(); t++){
		double dR = DeltaR(MuonPhi->at(theGoodMuon),MuonEta->at(theGoodMuon),HPSTauPhi->at(t),HPSTauEta->at(t));
		double dR2 = (theGoodMuon2 != -1) ? DeltaR(MuonPhi->at(theGoodMuon2),MuonEta->at(theGoodMuon2),HPSTauPhi->at(t),HPSTauEta->at(t)) : 100;
		
		//boolean to check requirements
		bool goodTau = (HPSTauPt->at(t)>30) //pT cut in GeV
		&& (fabs(HPSTauEta->at(t))<2.3) //require eta < 2.3
		&& ((iso_option==-1 || HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(t)==iso_option)) //require loose isolation: 3Hits -> lower fake rate
		//iso_option -1 -> no isolation required, iso_option 1 -> isolation, iso_option 0 -> anti-isolation
		//&& (HPSTauAgainstElectronLooseDiscr->at(t)==1) //require loose electron-tau discrimination
		&& (HPSTauagainstElectronLooseMVA3->at(t)==1) //require loose electron-tau discrimination: MVA3 -> lower fake rate
		&& (HPSTauagainstMuonTight2->at(t)==1) //require tight (version 2) muon-tau discrimination
		//comparison of leading muon and tau
		&& (dR > 0.5) //require dR > 0.5 with leading muon
		&& (dR2 > 0.5) //require dR > 0.5 with second muon (if selected)
		&& (HPSTauVtxIndex->at(t)==MuonBestTrackVtxIndex->at(theGoodMuon)) //require same vertex
		&& (chargesign==0 || MuonCharge->at(theGoodMuon)*HPSTauCharge->at(t)==chargesign); //charge sign requirement (no req if chargesign==0)
		
		//if it passed, store it
		if(goodTau) goodTaus.push_back(t);
	}
	//skip event if there is not at least one good tau
	if(goodTaus.size()<1) return false;
	else { //otherwise, store leading tau (lowest index) and vector
		theGoodTau = goodTaus[0];
		theGoodTaus = goodTaus;
		return true;
	}
}

//--------------------------------
//iso tau veto for anti-iso region
Bool_t LQClass::TauIsoVeto(){
	for(unsigned t = 0; t < theGoodTaus.size(); t++){
		if(HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(theGoodTaus[t])) return false;
	}
	return true;
}

//-------------
//jet selection
//default: ntau = 1
Bool_t LQClass::JetSelection(int ntau){
	theMainJet = -1;
	theMainBJet = -1;
	vector<int> goodJets;
	for(unsigned j = 0; j < PFJetPt->size(); j++){
		double dRm = DeltaR(MuonPhi->at(theGoodMuon),MuonEta->at(theGoodMuon),PFJetPhi->at(j),PFJetEta->at(j));
		double dRm2 = (theGoodMuon2 != -1) ? DeltaR(MuonPhi->at(theGoodMuon2),MuonEta->at(theGoodMuon2),PFJetPhi->at(j),PFJetEta->at(j)) : 100;
		
		bool goodJet = (PFJetPt->at(j)>30) //pT cut
		&& (fabs(PFJetEta->at(j))<2.4) //eta cut
		&& PFJetPassLooseID->at(j) //loose ID
		&& (dRm > 0.5) //separation from muon
		&& (dRm2 > 0.5); //separation from second muon (if selected)
		
		//separation from leading good tau or all good taus
		if(ntau==-1) ntau = theGoodTaus.size();
		for(int t = 0; t < ntau; t++){
			double dRt = DeltaR(HPSTauPhi->at(theGoodTaus[t]),HPSTauEta->at(theGoodTaus[t]),PFJetPhi->at(j),PFJetEta->at(j));
			goodJet &= (dRt > 0.5); 
		}
		
		//if it passed, store it
		if(goodJet){
			goodJets.push_back(j); //jets vector *does* contain the main 2 jets
		
			if(theMainBJet==-1 && PFJetCombinedSecondaryVertexBTag->at(j)>0.244) theMainBJet = j;
			else if(theMainJet==-1) theMainJet = j;
		}
	}
	
	theGoodJets = goodJets;
	return true;
}

//----------------
//gen-level checks
Bool_t LQClass::GenSelection(){
	//check for fake taus in MC
	realTau.resize(theGoodTaus.size());
	fakeTauLep.resize(theGoodTaus.size());
	fakeTauJet.resize(theGoodTaus.size());
	for(int t = 0; t < theGoodTaus.size(); t++){
		realTau[t] = fakeTauLep[t] = false;
		fakeTauJet[t] = true; //first, assume jet faking tau
		if(motherId==-1) { realTau[t] = true; fakeTauJet[t] = fakeTauLep[t] = false; } //assume all real taus for data
		else {
			vector<int> goodGens(6,-1); //indices 0..5 correspond to: 15, -15, 13, -13, 11, -11
		
			//loop over genparticles to find good leptons
			for(unsigned g = 0; g < GenParticlePdgId->size(); g++){
				bool goodGen = (GenParticleMotherIndex->at(g)>=0 && fabs(GenParticlePdgId->at(GenParticleMotherIndex->at(g)))==motherId); //check mother PDGID = specified mother ID, w/ safety check for -1 (no mother)
				if(goodGen){
					if(GenParticlePdgId->at(g)==15) goodGens[0] = g;
					else if(GenParticlePdgId->at(g)==-15) goodGens[1] = g;
					else if(GenParticlePdgId->at(g)==13) goodGens[2] = g;
					else if(GenParticlePdgId->at(g)==-13) goodGens[3] = g;
					else if(GenParticlePdgId->at(g)==11) goodGens[4] = g;
					else if(GenParticlePdgId->at(g)==-11) goodGens[5] = g;
				}
			}
			
			//loop over good gen particles to compare w/ reco tau
			for(unsigned g = 0; g < goodGens.size(); g++){
				if(goodGens[g]==-1) continue;
				double dR = DeltaR(GenParticlePhi->at(goodGens[g]),GenParticleEta->at(goodGens[g]),HPSTauPhi->at(theGoodTaus[t]),HPSTauEta->at(theGoodTaus[t]));
				if(dR < 0.1){
					if(fabs(GenParticlePdgId->at(goodGens[g]))==11 || fabs(GenParticlePdgId->at(goodGens[g]))==13) { fakeTauLep[t] = true; fakeTauJet[t] = realTau[t] = false; } //check for ele or mu PDGID
					else if(fabs(GenParticlePdgId->at(goodGens[g]))==15) { realTau[t] = true; fakeTauLep[t] = fakeTauJet[t] = false; } //check for tau PDGID
					//if none of these conditions are met, default assumption of jet faking tau will be kept
					break;
				}
			}
		}
	}
	
	return true;
}

//----------------------
//gen-level checks for b
Bool_t LQClass::GenSelectionB(){
	fakeTauBJet.resize(theGoodTaus.size());
	for(int t = 0; t < theGoodTaus.size(); t++){
		fakeTauBJet[t] = false; //first, assume tau not fake from bjet
		vector<int> goodGens;
	
		//loop over genparticles to find good b quarks
		for(unsigned g = 0; g < GenParticlePdgId->size(); g++){
			bool goodGen = (GenParticleMotherIndex->at(g)>=0 && fabs(GenParticlePdgId->at(GenParticleMotherIndex->at(g)))==6); //check mother PDGID = top, w/ safety check for -1 (no mother)
			if(goodGen && fabs(GenParticlePdgId->at(g))==5) goodGens.push_back(g);
		}
		
		//loop over good gen particles to compare w/ reco tau
		for(unsigned g = 0; g < goodGens.size(); g++){
			double dR = DeltaR(GenParticlePhi->at(goodGens[g]),GenParticleEta->at(goodGens[g]),HPSTauPhi->at(theGoodTaus[t]),HPSTauEta->at(theGoodTaus[t]));
			cout << "dR(b,tau) = " << dR << endl;
			if(dR < 0.1){
				fakeTauBJet[t] = true;
				break;
			}
		}
	}
	
	return true;
}

Bool_t LQClass::Cut(int s)
{
// This function may be called from Loop. It uses whatever tree entry is currently loaded.
// returns true if entry is accepted.
// returns false otherwise.
	bool goodEvent = false;

	//reset object index lists
	theGoodMuons.clear(); theGoodMuons2.clear();
	theGoodTaus.clear(); theGoodJets.clear();
	fakeTauJet.clear(); fakeTauLep.clear(); realTau.clear(); fakeTauBJet.clear();	
	theGoodMuon = theGoodMuon2 = theGoodTau = theMainJet = theMainBJet = -1;

	//uncType is a member variable of the class, set in Loop() before calling Cut()
	if(uncType.size()==0) {}
	else if(uncType=="TauESUp") TauES(true);
	else if(uncType=="TauESDown") TauES(false);
	else if(uncType=="TauERUp") TauER(true);
	else if(uncType=="TauERDown") TauER(false);
	else if(uncType=="JESUp") JetES(true);
	else if(uncType=="JESDown") JetES(false);
	else if(uncType=="JERUp") JetER(true);
	else if(uncType=="JERDown") JetER(false);
	
	//selType is a member variable of the class, set in Loop() before calling Cut()
	if(selType=="main") { //preselection/main selection
		goodEvent = (HLTSelection() && ++ctr[s][0])
				 && (MuonSelection() && ++ctr[s][1])
				 && (MuonVetoOS() && ++ctr[s][2])
				 && (VertexQuality() && ++ctr[s][3])
				 && (ElectronVeto() && ++ctr[s][4])
				 && (TauSelection(1) && ++ctr[s][5])
				 && JetSelection()
				 && GenSelection();
	}
	else if(selType=="main-bcheck") { //preselection/main selection w/ fake tau check against b quarks for ttbar
		goodEvent = HLTSelection()
				 && MuonSelection()
				 && MuonVetoOS()
				 && VertexQuality()
				 && ElectronVeto()
				 && TauSelection(1)
				 && JetSelection()
				 && GenSelection()
				 && GenSelectionB();
	}
	else if(selType=="main-notauiso") { //preselection/main selection for tau fake rate check
		goodEvent = HLTSelection()
				 && MuonSelection()
				 && MuonVetoOS()
				 && VertexQuality()
				 && ElectronVeto()
				 && TauSelection(-1) //tau isolation not required
				 && JetSelection(-1) //check jet separation from ALL good taus
				 && GenSelection();
	}
	else if(selType=="muonLNT") { //loose-not-tight muon control region for tau fake rate check
		goodEvent = HLTSelection()
				 && MuonSelectionLooseNotTight()
				 && MuonVetoOS()
				 && VertexQuality()
				 && ElectronVeto()
				 && TauSelection(-1) //tau isolation not required
				 && JetSelection(-1) //check jet separation from ALL good taus
				 && GenSelection();
	}	
	else if(selType=="ttbar") { //ttbar selection - same sign taus
		goodEvent = HLTSelection()
				 && MuonSelection()
				 && MuonVetoOS()
				 && VertexQuality()
				 && ElectronVeto()
				 && TauSelection(-1,1) //tau isolation not required, same-sign events
				 && JetSelection()
				 && GenSelection();
	}
	else if(selType=="zjets") { //z+jet selection for tau fake rate
		goodEvent = (HLTSelection() && ++ctr[s][0])
				 && (MuonSelection() && ++ctr[s][1])
				 && (MuonSelectionSecond() && ++ctr[s][2]) //select a second OS muon for Z->mumu
				 && (VertexQuality() && ++ctr[s][3])
				 && (TauSelection(-1,0) && ++ctr[s][5]) //tau isolation not required, use either-sign events
				 && JetSelection(-1) //check jet separation from ALL good taus
				 && GenSelection();
	}
	else if(selType=="zjets-nojetsep") { //z+jet selection for tau fake rate (no jet sep)
		goodEvent = HLTSelection()
				 && MuonSelection()
				 && VertexQuality()
				 && MuonSelectionSecond() //select a second OS muon for Z->mumu
				 && TauSelection(-1,0) //tau isolation not required, use either-sign events
				 && JetSelection(0) //check jet separation from NO good taus (will be checked object-by-object later)
				 && GenSelection();
	}
	else if(selType=="anti-iso"){ //anti-isolated sample for fake tau yield
		goodEvent = (HLTSelection() && ++ctr[s][0])
				 && (MuonSelection() && ++ctr[s][1])
				 && (MuonVetoOS() && ++ctr[s][2])
				 && (VertexQuality() && ++ctr[s][3])
				 && (ElectronVeto() && ++ctr[s][4])
				 && TauSelection(-1) //no iso req for taus (anti-iso req applied when plotting)
				 && (TauIsoVeto() && ++ctr[s][5])
				 && JetSelection(-1) //check jet separation from ALL good taus
				 && GenSelection();
	}
	else if(selType=="anti-iso-nojetsep"){ //anti-isolated sample for fake tau yield (no jet sep)
		goodEvent = HLTSelection()
				 && MuonSelection()
				 && MuonVetoOS()
				 && VertexQuality()
				 && ElectronVeto()
				 && TauSelection(-1) //no iso req for taus (anti-iso req applied when plotting)
				 && TauIsoVeto()
				 && JetSelection(0) //check jet separation from NO good taus (will be checked object-by-object later)
				 && GenSelection();
	}	
	else if(selType=="wjets"){ //w+jet selection for tau fake rate
		goodEvent = HLTSelection()
				 && MuonSelection()
				 && MuonVetoLoose() //veto any extra loose leptons (muons)
				 && VertexQuality()
				 && ElectronVetoLoose() //veto any extra loose leptons (electrons)
				 && TauSelection(-1) //tau isolation not required, use opposite-sign events
				 && JetSelection()
				 && GenSelection();
	}
	else if(selType=="wjets7tev"){ //w+jet selection for tau fake rate (7 TeV version, mu-tau trigger)
		goodEvent = HLTSelection("HLT_IsoMu17_eta2p1_LooseIsoPFTau20")
				 && MuonSelection()
				 && MuonVetoLoose() //veto any extra loose leptons (muons)
				 && VertexQuality()
				 && ElectronVetoLoose() //veto any extra loose leptons (electrons)
				 && TauSelection(-1,1) //tau isolation not required, use same-sign events
				 && JetSelection()
				 && GenSelection();
	}
	else if(selType=="anti-iso7tev"){ //anti-isolated sample for fake tau yield (7 TeV version, mu-tau trigger)
		goodEvent = HLTSelection("HLT_IsoMu17_eta2p1_LooseIsoPFTau20")
				 && MuonSelection()
				 && MuonVetoOS()
				 && VertexQuality()
				 && ElectronVeto()
				 && TauSelection(-1) //no iso req for taus (anti-iso req applied when plotting)
				 && JetSelection(-1) //check jet separation from ALL good taus
				 && GenSelection();
	}
	if(selType=="main7tev") { //preselection/main selection
		goodEvent = HLTSelection("HLT_IsoMu17_eta2p1_LooseIsoPFTau20")
				 && MuonSelection()
				 && MuonVetoOS()
				 && VertexQuality()
				 && ElectronVeto()
				 && TauSelection(1)
				 && JetSelection()
				 && GenSelection();
	}
	else {} 
	
	return goodEvent;
}

void LQClass::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L LQClass.C
//      Root > LQClass t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
	if (fChain == 0) return;

	//---------------------------------
	//set up tree output of observables
	
	//variables
	double           s_MuonEnergy;
	double           s_MuonEta;
	double           s_MuonPhi;
	double           s_MuonPt;
	double           s_MuonPFRelIsoTight;
	int              s_MuonCharge;
	int              s_MuonVtxIndex;
	int              s_MuonMultiplicity;
	double           s_SecondMuonEnergy;
	double           s_SecondMuonEta;
	double           s_SecondMuonPhi;
	double           s_SecondMuonPt;
	double           s_SecondMuonPFRelIsoTight;
	int              s_SecondMuonCharge;
	int              s_SecondMuonMultiplicity;	
	vector<double>  *s_HPSTauEt = 0;
	vector<double>  *s_HPSTauEta = 0;
	vector<double>  *s_HPSTauPhi = 0;
	vector<double>  *s_HPSTauPt = 0;
	vector<int>     *s_HPSTauCharge = 0;
	int              s_HPSTauMultiplicity;
	vector<int>     *s_HPSTauDecayMode = 0;
	vector<bool>    *s_HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits = 0;	
	vector<bool>    *s_HPSTauFakeJet = 0;
	vector<bool>    *s_HPSTauFakeLep = 0;
	vector<bool>    *s_HPSTauReal = 0;
	vector<bool>    *s_HPSTauFakeBJet = 0;
	double           s_PFMET;
	double           s_PFMETPatType1;
	//double           s_PFMETType01Cor;
	double           s_PFMETPhi;
	double           s_PFMETPhiPatType1;
	//double           s_PFMETPhiType01Cor;
	vector<double>  *s_PFJetEnergy = 0;
	vector<double>  *s_PFJetPt = 0;
	vector<double>  *s_PFJetEta = 0;
	vector<double>  *s_PFJetPhi = 0;
	vector<double>  *s_PFJetCSVBTag = 0;
	vector<double>  *s_PFJetPartonFlavour = 0;
	vector<int>     *s_PFJetVtxIndex = 0;
	int              s_PFJetMultiplicity;
	double           s_PFMainJetPt[2];
	double           s_PFMainJetEnergy[2];
	double           s_PFMainJetEta[2];
	double           s_PFMainJetPhi[2];
	double           s_PFMainJetCSVBTag[2];
	double           s_PFMainJetPartonFlavour[2];
	int              s_nGoodVertices;
	int              s_run;
	int              s_event;
	int              s_ls;
	double           s_trueNInteraction;
	int              s_CharginoMultiplicity;
	double           s_MassMuonMuon;
	double           s_MassMuonTau;
	double           s_PtMuonTau;
	int              s_IndexTauJet;
	double           s_MassTauJet;
	double           s_PtTauJet;
	int              s_IndexMuonJet;
	double           s_MassMuonJet;
	double           s_PtMuonJet;	
	
	//setup ctr
	ctr = vector<vector<int> >(selTypes.size(),vector<int>(nCtr,0));
	
	//open file and output tree
	vector<TFile*> out_file(outNames.size(),NULL);
	vector<TTree*> s_tree(selTypes.size(),NULL);
	for(unsigned s = 0; s < selTypes.size(); s++){
		out_file[s] = new TFile(outNames[s].c_str(), "RECREATE");
		string treedesc = "selected observables for mu+tau_h+b+b, " + selTypes[s];
		s_tree[s] = new TTree("tree", treedesc.c_str());

		//setup output tree
		s_tree[s]->Branch("MuonEnergy",&s_MuonEnergy,"s_MuonEnergy/D");
		s_tree[s]->Branch("MuonEta",&s_MuonEta,"s_MuonEta/D");
		s_tree[s]->Branch("MuonPhi",&s_MuonPhi,"s_MuonPhi/D");
		s_tree[s]->Branch("MuonPt",&s_MuonPt,"s_MuonPt/D");
		s_tree[s]->Branch("MuonPFRelIsoTight",&s_MuonPFRelIsoTight,"s_MuonPFRelIsoTight/D");
		s_tree[s]->Branch("MuonMultiplicity",&s_MuonMultiplicity,"s_MuonMultiplicity/I");
		s_tree[s]->Branch("MuonCharge",&s_MuonCharge,"s_MuonCharge/I");
		s_tree[s]->Branch("MuonVtxIndex",&s_MuonVtxIndex,"s_MuonVtxIndex/I");
		s_tree[s]->Branch("SecondMuonEnergy",&s_SecondMuonEnergy,"s_SecondMuonEnergy/D");
		s_tree[s]->Branch("SecondMuonEta",&s_SecondMuonEta,"s_SecondMuonEta/D");
		s_tree[s]->Branch("SecondMuonPhi",&s_SecondMuonPhi,"s_SecondMuonPhi/D");
		s_tree[s]->Branch("SecondMuonPt",&s_SecondMuonPt,"s_SecondMuonPt/D");
		s_tree[s]->Branch("SecondMuonPFRelIsoTight",&s_SecondMuonPFRelIsoTight,"s_SecondMuonPFRelIsoTight/D");
		s_tree[s]->Branch("SecondMuonMultiplicity",&s_SecondMuonMultiplicity,"s_SecondMuonMultiplicity/I");
		s_tree[s]->Branch("SecondMuonCharge",&s_SecondMuonCharge,"s_SecondMuonCharge/I");		
		s_tree[s]->Branch("HPSTauEt","std::vector<double>",&s_HPSTauEt);
		s_tree[s]->Branch("HPSTauEta","std::vector<double>",&s_HPSTauEta);
		s_tree[s]->Branch("HPSTauPhi","std::vector<double>",&s_HPSTauPhi);
		s_tree[s]->Branch("HPSTauPt","std::vector<double>",&s_HPSTauPt);
		s_tree[s]->Branch("HPSTauMultiplicity",&s_HPSTauMultiplicity,"s_HPSTauMultiplicity/I");
		s_tree[s]->Branch("HPSTauCharge","std::vector<int>",&s_HPSTauCharge);
		s_tree[s]->Branch("HPSTauDecayMode","std::vector<int>",&s_HPSTauDecayMode);
		s_tree[s]->Branch("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits","std::vector<bool>",&s_HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits);
		s_tree[s]->Branch("HPSTauFakeJet","std::vector<bool>",&s_HPSTauFakeJet);
		s_tree[s]->Branch("HPSTauFakeLep","std::vector<bool>",&s_HPSTauFakeLep);
		s_tree[s]->Branch("HPSTauReal","std::vector<bool>",&s_HPSTauReal);
		s_tree[s]->Branch("HPSTauFakeBJet","std::vector<bool>",&s_HPSTauFakeBJet);
		s_tree[s]->Branch("PFMET",&s_PFMET,"s_PFMET/D");
		s_tree[s]->Branch("PFMETPatType1",&s_PFMETPatType1,"s_PFMETPatType1/D");
		//s_tree[s]->Branch("PFMETType01Cor",&s_PFMETType01Cor,"s_PFMETType01Cor/D");
		s_tree[s]->Branch("PFMETPhi",&s_PFMETPhi,"s_PFMETPhi/D");
		s_tree[s]->Branch("PFMETPhiPatType1",&s_PFMETPhiPatType1,"s_PFMETPhiPatType1/D");
		//s_tree[s]->Branch("PFMETPhiType01Cor",&s_PFMETPhiType01Cor,"s_PFMETPhiType01Cor/D");
		s_tree[s]->Branch("PFJetEnergy","std::vector<double>",&s_PFJetEnergy);
		s_tree[s]->Branch("PFJetPt","std::vector<double>",&s_PFJetPt);
		s_tree[s]->Branch("PFJetEta","std::vector<double>",&s_PFJetEta);
		s_tree[s]->Branch("PFJetPhi","std::vector<double>",&s_PFJetPhi);
		s_tree[s]->Branch("PFJetCSVBTag","std::vector<double>",&s_PFJetCSVBTag);
		s_tree[s]->Branch("PFJetPartonFlavour","std::vector<double>",&s_PFJetPartonFlavour);
		s_tree[s]->Branch("PFJetVtxIndex","std::vector<int>",&s_PFJetVtxIndex);
		s_tree[s]->Branch("PFJetMultiplicity",&s_PFJetMultiplicity,"s_PFJetMultiplicity/I");
		s_tree[s]->Branch("PFMainJetPt",s_PFMainJetPt,"s_PFMainJetPt[2]/D");
		s_tree[s]->Branch("PFMainJetEnergy",s_PFMainJetEnergy,"s_PFMainJetEnergy[2]/D");
		s_tree[s]->Branch("PFMainJetEta",s_PFMainJetEta,"s_PFMainJetEta[2]/D");
		s_tree[s]->Branch("PFMainJetPhi",s_PFMainJetPhi,"s_PFMainJetPhi[2]/D");
		s_tree[s]->Branch("PFMainJetCSVBTag",s_PFMainJetCSVBTag,"s_PFMainJetCSVBTag[2]/D");
		s_tree[s]->Branch("PFMainJetPartonFlavour",s_PFMainJetPartonFlavour,"s_PFMainJetPartonFlavour[2]/D");
		s_tree[s]->Branch("nGoodVertices",&s_nGoodVertices,"s_nGoodVertices/I");
		s_tree[s]->Branch("run",&s_run,"s_run/I");
		s_tree[s]->Branch("event",&s_event,"s_event/I");
		s_tree[s]->Branch("ls",&s_ls,"s_ls/I");
		s_tree[s]->Branch("trueNInteraction",&s_trueNInteraction,"s_trueNInteraction/D");
		s_tree[s]->Branch("CharginoMultiplicity",&s_CharginoMultiplicity,"s_CharginoMultiplicity/I");
		s_tree[s]->Branch("MassMuonMuon",&s_MassMuonMuon,"s_MassMuonMuon/D");
		s_tree[s]->Branch("MassMuonTau",&s_MassMuonTau,"s_MassMuonTau/D");
		s_tree[s]->Branch("PtMuonTau",&s_PtMuonTau,"s_PtMuonTau/D");
		s_tree[s]->Branch("IndexTauJet",&s_IndexTauJet,"s_IndexTauJet/I");
		s_tree[s]->Branch("MassTauJet",&s_MassTauJet,"s_MassTauJet/D");
		s_tree[s]->Branch("PtTauJet",&s_PtTauJet,"s_PtTauJet/D");
		s_tree[s]->Branch("IndexMuonJet",&s_IndexMuonJet,"s_IndexMuonJet/I");
		s_tree[s]->Branch("MassMuonJet",&s_MassMuonJet,"s_MassMuonJet/D");
		s_tree[s]->Branch("PtMuonJet",&s_PtMuonJet,"s_PtMuonJet/D");
	}

	//loop over ntuple tree
	Long64_t nentries = fChain->GetEntriesFast();
	//if(nentries>10000) nentries = 10000;
	
	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
		//skip some corrupted events in Z3Jets exclusive
		if(inFile=="Z3Jets_2l_muTau"){
			if(jentry==131634) jentry = 131891;
			else if(jentry==444118) jentry = 444387;
		}
		
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);   nbytes += nb;
		
		//clear and fill vectors of original pt values (for syst)
		StoreOrigVars();
		
		//call cut function, which does selection and stores good object numbers
		vector<Bool_t> cutResult(selTypes.size(),false);
		for(unsigned s = 0; s < selTypes.size(); s++){
			string tmp = selTypes[s];
			vector<string> fields;
			process(tmp,'_',fields);
			selType = fields[0];
			if(fields.size()>1) uncType = fields[1];
			else uncType = "";
			
			cutResult[s] = Cut(s);
			if(!cutResult[s]) continue;
			
			//-------------------------------------------------------
			//if we get to this point in the loop, it is a good event
			//(all selections passed)
			
			//first, clear tree variables
			//jets
			delete s_PFJetEnergy; s_PFJetEnergy = new vector<double>();
			delete s_PFJetPt; s_PFJetPt = new vector<double>();
			delete s_PFJetPhi; s_PFJetPhi = new vector<double>();
			delete s_PFJetEta; s_PFJetEta = new vector<double>();
			delete s_PFJetCSVBTag; s_PFJetCSVBTag = new vector<double>();
			delete s_PFJetPartonFlavour; s_PFJetPartonFlavour = new vector<double>();
			delete s_PFJetVtxIndex; s_PFJetVtxIndex = new vector<int>();
			//main jets
			s_PFMainJetPt[0] = s_PFMainJetEnergy[0] = s_PFMainJetPhi[0] = s_PFMainJetEta[0] = s_PFMainJetCSVBTag[0] = s_PFMainJetPartonFlavour[0] = 0; //jet
			s_PFMainJetPt[1] = s_PFMainJetEnergy[1] = s_PFMainJetPhi[1] = s_PFMainJetEta[1] = s_PFMainJetCSVBTag[1] = s_PFMainJetPartonFlavour[1] = 0; //bjet	
			//muon
			s_MuonEnergy = s_MuonEta = s_MuonPhi = s_MuonPt = 0;
			s_MuonCharge = s_MuonMultiplicity = s_MuonVtxIndex = 0;
			//second muon
			s_SecondMuonEnergy = s_SecondMuonEta = s_SecondMuonPhi = s_SecondMuonPt = 0;
			s_SecondMuonCharge = s_SecondMuonMultiplicity = 0;
			//tau
			delete s_HPSTauEt; s_HPSTauEt = new vector<double>();
			delete s_HPSTauEta; s_HPSTauEta = new vector<double>();
			delete s_HPSTauPhi; s_HPSTauPhi = new vector<double>();
			delete s_HPSTauPt; s_HPSTauPt = new vector<double>();
			delete s_HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits; s_HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits = new vector<bool>();
			delete s_HPSTauFakeJet; s_HPSTauFakeJet = new vector<bool>();
			delete s_HPSTauFakeLep; s_HPSTauFakeLep = new vector<bool>();
			delete s_HPSTauReal; s_HPSTauReal = new vector<bool>();
			delete s_HPSTauFakeBJet; s_HPSTauFakeBJet = new vector<bool>();
			delete s_HPSTauCharge; s_HPSTauCharge = new vector<int>();
			s_HPSTauMultiplicity = 0;
			delete s_HPSTauDecayMode; s_HPSTauDecayMode = new vector<int>();
			//misc - ints, doubles
			s_IndexTauJet = s_IndexMuonJet = s_run = s_event = 0; //ints
			s_trueNInteraction = s_MassMuonMuon = s_MassMuonTau = s_PtMuonTau = s_MassTauJet = s_PtTauJet = s_MassMuonJet = s_PtMuonJet = 0; //doubles
			s_PFMET = s_PFMETPatType1 = s_PFMETPhi = s_PFMETPhiPatType1 = 0; //more doubles
			
			//store muon observables
			s_MuonEnergy = MuonEnergy->at(theGoodMuon);
			s_MuonEta = MuonEta->at(theGoodMuon);
			s_MuonPhi = MuonPhi->at(theGoodMuon);
			s_MuonPt = MuonPt->at(theGoodMuon);
			s_MuonPFRelIsoTight = (MuonPFIsoR04ChargedHadron->at(theGoodMuon) +
									max(MuonPFIsoR04Photon->at(theGoodMuon) + MuonPFIsoR04NeutralHadron->at(theGoodMuon) - 0.5*MuonPFIsoR04PU->at(theGoodMuon),0.))/MuonPt->at(theGoodMuon);
			s_MuonCharge = MuonCharge->at(theGoodMuon);
			s_MuonMultiplicity = theGoodMuons.size();
			s_MuonVtxIndex = MuonBestTrackVtxIndex->at(theGoodMuon);
			
			//store tau observables
			if(fakeTauBJet.size()==0) fakeTauBJet = vector<bool>(theGoodTaus.size(),false);
			for(int t = 0; t < theGoodTaus.size(); t++){
				s_HPSTauEt->push_back(HPSTauEt->at(theGoodTaus[t]));
				s_HPSTauEta->push_back(HPSTauEta->at(theGoodTaus[t]));
				s_HPSTauPhi->push_back(HPSTauPhi->at(theGoodTaus[t]));
				s_HPSTauPt->push_back(HPSTauPt->at(theGoodTaus[t]));
				s_HPSTauCharge->push_back(HPSTauCharge->at(theGoodTaus[t]));
				s_HPSTauDecayMode->push_back(HPSTauDecayMode->at(theGoodTaus[t]));
				s_HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->push_back(HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits->at(theGoodTaus[t]));
				s_HPSTauFakeJet->push_back(fakeTauJet[t]);
				s_HPSTauFakeLep->push_back(fakeTauLep[t]);
				s_HPSTauReal->push_back(realTau[t]);
				s_HPSTauFakeBJet->push_back(fakeTauBJet[t]);
			}
			s_HPSTauMultiplicity = theGoodTaus.size();
			
			//store invariant mass & pT of (highest pT) muon and tau
			TLorentzVector v_mu;
			v_mu.SetPtEtaPhiM(s_MuonPt,s_MuonEta,s_MuonPhi,s_MuonMass);
			TLorentzVector v_tau;
			if(s_HPSTauPt->size()>0) v_tau.SetPtEtaPhiM(s_HPSTauPt->at(0),s_HPSTauEta->at(0),s_HPSTauPhi->at(0),s_TauMass);
			else v_tau.SetPtEtaPhiM(0,0,0,0);
			TLorentzVector v_tot;
			v_tot = v_mu+v_tau; //sum up
			s_MassMuonTau = v_tot.M();
			s_PtMuonTau = v_tot.Pt();

			//store second muon observables (if applicable)
			if(theGoodMuon2 != -1){
				s_SecondMuonEnergy = MuonEnergy->at(theGoodMuon2);
				s_SecondMuonEta = MuonEta->at(theGoodMuon2);
				s_SecondMuonPhi = MuonPhi->at(theGoodMuon2);
				s_SecondMuonPt = MuonPt->at(theGoodMuon2);
				s_SecondMuonPFRelIsoTight = (MuonPFIsoR04ChargedHadron->at(theGoodMuon2) +
										max(MuonPFIsoR04Photon->at(theGoodMuon2) + MuonPFIsoR04NeutralHadron->at(theGoodMuon2) - 0.5*MuonPFIsoR04PU->at(theGoodMuon2),0.))/MuonPt->at(theGoodMuon2);
				s_SecondMuonCharge = MuonCharge->at(theGoodMuon2);
				s_SecondMuonMultiplicity = theGoodMuons2.size();
				
				TLorentzVector v_mu2;
				v_mu2.SetPtEtaPhiM(s_SecondMuonPt,s_SecondMuonEta,s_SecondMuonPhi,s_MuonMass);
				TLorentzVector v_mumu;
				v_mumu = v_mu + v_mu2; //sum up
				s_MassMuonMuon = v_mumu.M();
			}
			
			//store MET observables
			s_PFMET = PFMET->at(0);
			s_PFMETPatType1 = PFMETPatType1->at(0);
			//s_PFMETType01Cor = PFMETType01Cor->at(0);
			s_PFMETPhi = PFMETPhi->at(0);
			s_PFMETPhiPatType1 = PFMETPhiPatType1->at(0);
			//s_PFMETPhiType01Cor = PFMETPhiType01Cor->at(0);
			
			//store jet observables
			s_PFJetMultiplicity = 0;
			//jets only w/ pT > 30 GeV & dR > 0.5 w/ muon and tau
			for(unsigned j = 0; j < theGoodJets.size(); j++){
				s_PFJetMultiplicity++;
				s_PFJetEnergy->push_back(PFJetEnergy->at(theGoodJets[j]));
				s_PFJetPt->push_back(PFJetPt->at(theGoodJets[j]));
				s_PFJetPhi->push_back(PFJetPhi->at(theGoodJets[j]));
				s_PFJetEta->push_back(PFJetEta->at(theGoodJets[j]));
				s_PFJetCSVBTag->push_back(PFJetCombinedSecondaryVertexBTag->at(theGoodJets[j]));
				s_PFJetPartonFlavour->push_back(PFJetPartonFlavour->at(theGoodJets[j]));
				s_PFJetVtxIndex->push_back(PFJetBestVertexTrackAssociationIndex->at(theGoodJets[j]));
			}

			//store main jet & bjet observables
			//bjet is always index 1, other jet is always index 0
			if(theMainJet!=-1 && theMainBJet!=-1){
				int theJets[2] = {theMainJet,theMainBJet};
				TLorentzVector v_jet[2];
				TLorentzVector v_mujet[2];
				TLorentzVector v_taujet[2];
				for(int j = 0; j < 2; j++){
					s_PFMainJetPt[j] = PFJetPt->at(theJets[j]);
					s_PFMainJetEnergy[j] = PFJetEnergy->at(theJets[j]);
					s_PFMainJetPhi[j] = PFJetPhi->at(theJets[j]);
					s_PFMainJetEta[j] = PFJetEta->at(theJets[j]);
					s_PFMainJetCSVBTag[j] = PFJetCombinedSecondaryVertexBTag->at(theJets[j]);
					s_PFMainJetPartonFlavour[j] = PFJetPartonFlavour->at(theJets[j]);
					
					//make jet vector and lepton+jet combination vectors
					v_jet[j].SetPtEtaPhiE(s_PFMainJetPt[j],s_PFMainJetEta[j],s_PFMainJetPhi[j],s_PFMainJetEnergy[j]);
					v_mujet[j] = v_mu + v_jet[j];
					v_taujet[j] = v_tau + v_jet[j];
				}

				//minimize delta(M_tj - M_mj) (pairing of leptons and jets)
				double pair_t0m1 = fabs(v_taujet[0].M()-v_mujet[1].M());
				double pair_t1m0 = fabs(v_taujet[1].M()-v_mujet[0].M());
				if(pair_t0m1 < pair_t1m0){
					s_IndexTauJet = 0;
					s_IndexMuonJet = 1;
				}
				else{
					s_IndexTauJet = 1;
					s_IndexMuonJet = 0;
				}
				s_MassTauJet = v_taujet[s_IndexTauJet].M();
				s_PtTauJet = v_taujet[s_IndexTauJet].Pt();
				s_MassMuonJet = v_mujet[s_IndexMuonJet].M();
				s_PtMuonJet = v_mujet[s_IndexMuonJet].Pt();
			}

			//store run/event info
			s_run = run;
			s_ls = ls;
			s_event = event;
			s_trueNInteraction = trueNInteraction;
			
			//store # primary vertices
			s_nGoodVertices = nGoodVertices;

			//check for 2 charginos from RPV stops
			int CharginoMultiplicity = 0;
			if(motherId==1000006){
				for(unsigned g = 0; g < GenParticlePdgId->size(); g++){
					if(fabs(GenParticlePdgId->at(g))==1000024 && GenParticleMotherIndex->at(g)>=0 && fabs(GenParticlePdgId->at(GenParticleMotherIndex->at(g)))==motherId) CharginoMultiplicity++;
				}
			}
			s_CharginoMultiplicity = CharginoMultiplicity;
			
			//fill output tree
			if(cutResult[s]) s_tree[s]->Fill();
			
			//restore original pt values to standard vars (in case multiple systematics are assessed)
			if(uncType.size()>0) RestoreOrigVars();
		}
	}
	
	//save output trees
	for(unsigned s = 0; s < selTypes.size(); s++){
		out_file[s]->cd();
		if(nEventProc) nEventProc->Write();
		s_tree[s]->Write();
		out_file[s]->Close();
		
		//output yields
		string yieldnames[nCtr] = {"HLT","Muon","SecondMuonVeto","Vertex","ElectronVeto","Tau"};
		cout << selTypes[s] << ":" << endl;
		cout << fixed << setprecision(2);
		for(int i = 0; i < nCtr; i++){
			cout << yieldnames[i] << ": " << ((double)ctr[s][i])/((double)nentries)*100 << endl;
		}
	}
	
	if(doBatch) sleep(30);
}
