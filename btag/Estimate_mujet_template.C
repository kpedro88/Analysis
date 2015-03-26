#define Estimate_mujet_placeholder_cxx
#include "Estimate_mujet_placeholder.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <Math/LorentzVector.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "/Volumes/Tbilisi/Work/LQ/GetPUWeight.C"
#include "/Volumes/Tbilisi/Work/LQ/tauTriggers.C"
#include "/Volumes/Tbilisi/Work/LQ/btagSF.C"
#include "/Volumes/Tbilisi/Work/LQ/btagEFF.C"
#include "/Volumes/Tbilisi/Work/LQ/SFlightFuncs.C"
#include "/Volumes/Tbilisi/Work/LQ/mistagSF.C"
#include "/Volumes/Tbilisi/Work/LQ/mistagEFF.C"
#include "/Volumes/Tbilisi/Work/LQ/BTagWeight4.h"
#include "/Volumes/Tbilisi/Work/LQ/ResidualModifier.h"
#include "/Volumes/Tbilisi/Work/LQ/smearJER.C"

/*
TFile* f_puweights = TFile::Open("../reweight_173692.root");
TH1D* puweights = (TH1D*)f_puweights->Get("plot_data_div_MC");
static float puweight(float npu) {
  if (npu<0) return 1.0;
  return puweights->GetBinContent(puweights->FindBin(npu));
}
*/

double returnMax(double iso) {
  if (iso-1 > 0)
    return (iso-1);
  else
    return 0;
}

typedef ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<double> > LV;

float* computeZeta(LV leg1, LV leg2, LV MEt){
  
  float* result = new float[2];
  
  float cos1 = leg1.Px()/leg1.Pt();
  float cos2 = leg2.Px()/leg2.Pt();
  float sen1 = leg1.Py()/leg1.Pt();
  float sen2 = leg2.Py()/leg2.Pt();

  float bisecX = cos1 + cos2;
  float bisecY = sen1 + sen2;

  float norm = TMath::Sqrt(bisecX*bisecX + bisecY*bisecY);

  if(norm>0.){
    bisecX /= norm;
    bisecY /= norm;
  }

  float pZetaVis = (leg1+leg2).Px()*bisecX + (leg1+leg2).Py()*bisecY;
  float pZeta    = (leg1+leg2+MEt).Px()*bisecX + (leg1+leg2+MEt).Py()*bisecY; 

  result[0] = pZetaVis;
  result[1] = pZeta;

  return result;
}

double ptSum(double sumptchargedh, double sumptgamma, double rho){

  double r = 0.5;
  double removedPU = sumptgamma-TMath::Pi()*rho*r*r;
  if (removedPU<0) removedPU = 0;
  double ptsum = sumptchargedh+removedPU;
  return ptsum;
}

double delta_phi(double phi1, double phi2) {
  double PI = 3.1415926;
  double TWOPI = PI*2;
  double PHI = TMath::Abs(phi1-phi2);
  return (PHI<=PI)? PHI : TWOPI-PHI;
}

double getPhi(double phi) {
  double PI = 3.1415926;
  if (phi < PI) 
    return phi+PI;
  else
    return phi-PI;
}

void Estimate_mujet_placeholder::Loop()
{
   if (fChain == 0) return;
   BTagWeight b(1);
     
   TString name = "placeholder";
   //This is for recoil correction
   int motherId = 0;
   if(name == "DYJets")
     motherId = 23;
   if(name == "WJets")
     motherId = 24;
   if(name == "SMH")
     motherId = 25;
   if(name == "MSSMH")
     motherId = 36;

   //============= Define histograms
   
   TH1F* h_gen_M_Ztt = new TH1F("h_gen_M_Ztt","",100,40,140);
   TH1F* h_gen_M_Zmm = new TH1F("h_gen_M_Zmm","",100,40,140);

   TH1F* h_nPU = new TH1F("h_nPU","",51,-0.5,50.5);
   h_nPU->Sumw2();

   //============= Define tree
   TFile* out_tree = new TFile(name+"_tree.root","RECREATE");
   TTree* tree = new TTree("mytree","mytree");
   
   int Run = 0;
   tree->Branch("Run",&Run,"Run/I");
   int Event = 0;
   tree->Branch("Event",&Event,"Event/I");
   int LumiSec = 0;
   tree->Branch("LumiSec",&LumiSec,"LumiSec/I");

   float Weight = 0;
   tree->Branch("Weight",&Weight,"Weight/F");

   float Wtag_2bjet = 0;
   tree->Branch("Wtag_2bjet",&Wtag_2bjet,"Wtag_2bjet/F");

   float Wtag = 0;
   tree->Branch("Wtag",&Wtag,"Wtag/F");

   float Wtag_1bjet = 0;
   tree->Branch("Wtag_1bjet",&Wtag_1bjet,"Wtag_1bjet/F");
   
   float SFtrig = 0;
   tree->Branch("SFtrig",&SFtrig,"SFtrig/F");
   
   int pass_trigger = 0;
   tree->Branch("pass_trigger",&pass_trigger,"pass_trigger/I");   
   
   //Gen particle info
   float pt_genlep1 = 0;
   tree->Branch("pt_genlep1",&pt_genlep1,"pt_genlep1/F");
   float phi_genlep1 = 0;
   tree->Branch("phi_genlep1",&phi_genlep1,"phi_genlep1/F");
   float eta_genlep1 = 0;
   tree->Branch("eta_genlep1",&eta_genlep1,"eta_genlep1/F");

   float pt_genlep2 = 0;
   tree->Branch("pt_genlep2",&pt_genlep2,"pt_genlep2/F");
   float phi_genlep2 = 0;
   tree->Branch("phi_genlep2",&phi_genlep2,"phi_genlep2/F");
   float eta_genlep2 = 0;
   tree->Branch("eta_genlep2",&eta_genlep2,"eta_genlep2/F");

   int isztt = 0;
   tree->Branch("isztt",&isztt,"isztt/I");
   int iszll = 0;
   tree->Branch("iszll",&iszll,"iszll/I");
   
   //
   float m_lep_tau = 0;
   tree->Branch("m_lep_tau",&m_lep_tau,"m_lep_tau/F");
   float m_lep_tau_met = 0;
   tree->Branch("m_lep_tau_met",&m_lep_tau_met,"m_lep_tau_met/F");

   float pt_lep = 0;
   tree->Branch("pt_lep",&pt_lep,"pt_lep/F");
   float eta_lep = 0;
   tree->Branch("eta_lep",&eta_lep,"eta_lep/F");
   float phi_lep = 0;
   tree->Branch("phi_lep",&phi_lep,"phi_lep/F");
   int charge_lep = 0;
   tree->Branch("charge_lep",&charge_lep,"charge_lep/I");
   float iso_lep = 0;
   tree->Branch("iso_lep",&iso_lep,"iso_lep/F");
   float d0_lep = 0;
   tree->Branch("d0_lep",&d0_lep,"d0_lep/F");
   float dz_lep = 0;
   tree->Branch("dz_lep",&dz_lep,"dz_lep/F");
   int isfromjet_lep = 0;
   tree->Branch("isfromjet_lep",&isfromjet_lep,"isfromjet_lep/I");

   float pt_tau = 0;
   tree->Branch("pt_tau",&pt_tau,"pt_tau/F");
   float eta_tau = 0;
   tree->Branch("eta_tau",&eta_tau,"eta_tau/F");
   float phi_tau = 0;
   tree->Branch("phi_tau",&phi_tau,"phi_tau/F");
   int charge_tau = 0;
   tree->Branch("charge_tau",&charge_tau,"charge_tau/I");
   float dz_tau = 0;
   tree->Branch("dz_tau",&dz_tau,"dz_tau/F");
   float d0_tau = 0;
   tree->Branch("d0_tau",&d0_tau,"d0_tau/F");
   float iso_tau = 0;
   tree->Branch("iso_tau",&iso_tau,"iso_tau/F");
   int isfromjet_tau = 0;
   tree->Branch("isfromjet_tau",&isfromjet_tau,"isfromjet_tau/I");
   
   int njets = 0;
   tree->Branch("njets",&njets,"njets/I");
   
   int nbtagjetsl = 0;
   tree->Branch("nbtagjetsl",&nbtagjetsl,"nbtagjetsl/I");
   int nbtagjetsm = 0;
   tree->Branch("nbtagjetsm",&nbtagjetsm,"nbtagjetsm/I");
   int nbtagjetst = 0;
   tree->Branch("nbtagjetst",&nbtagjetst,"nbtagjetst/I");

   //
   float pt_btaggedjet = 0;
   tree->Branch("pt_btaggedjet",&pt_btaggedjet,"pt_btaggedjet/F");
   float eta_btaggedjet = 0;
   tree->Branch("eta_btaggedjet",&eta_btaggedjet,"eta_btaggedjet/F");
   float phi_btaggedjet = 0;
   tree->Branch("phi_btaggedjet",&phi_btaggedjet,"phi_btaggedjet/F");
   float tche_btaggedjet = 0;
   tree->Branch("tche_btaggedjet",&tche_btaggedjet,"tche_btaggedjet/F");
   float tchp_btaggedjet = 0;
   tree->Branch("tchp_btaggedjet",&tchp_btaggedjet,"tchp_btaggedjet/F");
   
   float pt_closejet = 0;
   tree->Branch("pt_closejet",&pt_closejet,"pt_closejet/F");
   //
   float pt_jet1_2bjet = 0;
   tree->Branch("pt_jet1_2bjet",&pt_jet1_2bjet,"pt_jet1_2bjet/F");
   float eta_jet1_2bjet = 0;
   tree->Branch("eta_jet1_2bjet",&eta_jet1_2bjet,"eta_jet1_2bjet/F");
   float phi_jet1_2bjet = 0;
   tree->Branch("phi_jet1_2bjet",&phi_jet1_2bjet,"phi_jet1_2bjet/F");

   float pt_jet2_2bjet = 0;
   tree->Branch("pt_jet2_2bjet",&pt_jet2_2bjet,"pt_jet2_2bjet/F");
   float eta_jet2_2bjet = 0;
   tree->Branch("eta_jet2_2bjet",&eta_jet2_2bjet,"eta_jet2_2bjet/F");
   float phi_jet2_2bjet = 0;
   tree->Branch("phi_jet2_2bjet",&phi_jet2_2bjet,"phi_jet2_2bjet/F");
   //
   float pt_jet1_1bjet = 0;
   tree->Branch("pt_jet1_1bjet",&pt_jet1_1bjet,"pt_jet1_1bjet/F");
   float eta_jet1_1bjet = 0;
   tree->Branch("eta_jet1_1bjet",&eta_jet1_1bjet,"eta_jet1_1bjet/F");
   float phi_jet1_1bjet = 0;
   tree->Branch("phi_jet1_1bjet",&phi_jet1_1bjet,"phi_jet1_1bjet/F");

   float pt_jet2_1bjet = 0;
   tree->Branch("pt_jet2_1bjet",&pt_jet2_1bjet,"pt_jet2_1bjet/F");
   float eta_jet2_1bjet = 0;
   tree->Branch("eta_jet2_1bjet",&eta_jet2_1bjet,"eta_jet2_1bjet/F");
   float phi_jet2_1bjet = 0;
   tree->Branch("phi_jet2_1bjet",&phi_jet2_1bjet,"phi_jet2_1bjet/F");
   //
   int found_jet1 = 0;
   tree->Branch("found_jet1",&found_jet1,"found_jet1/I");
   float pt_jet1 = 0;
   tree->Branch("pt_jet1",&pt_jet1,"pt_jet1/F");
   float eta_jet1 = 0;
   tree->Branch("eta_jet1",&eta_jet1,"eta_jet1/F");
   float phi_jet1 = 0;
   tree->Branch("phi_jet1",&phi_jet1,"phi_jet1/F");
   int btag_jet1 = 0;
   tree->Branch("btag_jet1", &btag_jet1, "btag_jet1/I");
   float TCHE_jet1 = 0;
   tree->Branch("TCHE_jet1",&TCHE_jet1,"TCHE_jet1/F");
   float TCHP_jet1 = 0;
   tree->Branch("TCHP_jet1",&TCHP_jet1,"TCHP_jet1/F");

   int found_jet2 = 0;
   tree->Branch("found_jet2",&found_jet2,"found_jet2/I");
   float pt_jet2 = 0;
   tree->Branch("pt_jet2",&pt_jet2,"pt_jet2/F");
   float eta_jet2 = 0;
   tree->Branch("eta_jet2",&eta_jet2,"eta_jet2/F");
   float phi_jet2 = 0;
   tree->Branch("phi_jet2",&phi_jet2,"phi_jet2/F");
   int btag_jet2 = 0;
   tree->Branch("btag_jet2", &btag_jet2, "btag_jet2/I");
   float TCHE_jet2 = 0;
   tree->Branch("TCHE_jet2",&TCHE_jet2,"TCHE_jet2/F");
   float TCHP_jet2 = 0;
   tree->Branch("TCHP_jet2",&TCHP_jet2,"TCHP_jet2/F");

   float pfMET = 0;
   tree->Branch("pfMET",&pfMET,"pfMET/F");
   float pfMETPhi = 0;
   tree->Branch("pfMETPhi",&pfMETPhi,"pfMETPhi/F");   
   
   double pfMET_corr = 0;
   tree->Branch("pfMET_corr",&pfMET_corr,"pfMET_corr/D");
   double pfMETPhi_corr = 0;
   tree->Branch("pfMETPhi_corr",&pfMETPhi_corr,"pfMETPhi_corr/D");

   float dphi_leptau_met = 0;
   tree->Branch("dphi_leptau_met",&dphi_leptau_met,"dphi_leptau_met/F");   
   float dphi_lep_tau = 0;
   tree->Branch("dphi_lep_tau",&dphi_lep_tau,"dphi_lep_tau/F");   
   float deta_lep_tau = 0;
   tree->Branch("deta_lep_tau",&deta_lep_tau,"deta_lep_tau/F");   
   float dr_lep_tau = 0;
   tree->Branch("dr_lep_tau",&dr_lep_tau,"dr_lep_tau/F");   
   float dphi_met_lep = 0;
   tree->Branch("dphi_met_lep",&dphi_met_lep,"dphi_met_lep/F");   
   float dphi_met_tau = 0;
   tree->Branch("dphi_met_tau",&dphi_met_tau,"dphi_met_tau/F");   
   float dphi_lep_bjet1 = 0;
   tree->Branch("dphi_lep_bjet1",&dphi_lep_bjet1,"dphi_lep_bjet1/F");   
   float dphi_tau_bjet1 = 0;
   tree->Branch("dphi_tau_bjet1",&dphi_tau_bjet1,"dphi_tau_bjet1/F");   
   float deta_lep_bjet1 = 0;
   tree->Branch("deta_lep_bjet1",&deta_lep_bjet1,"deta_lep_bjet1/F");   
   float deta_tau_bjet1 = 0;
   tree->Branch("deta_tau_bjet1",&deta_tau_bjet1,"deta_tau_bjet1/F");   
   float dr_lep_bjet1 = 0;
   tree->Branch("dr_lep_bjet1",&dr_lep_bjet1,"dr_lep_bjet1/F");   
   float dr_tau_bjet1 = 0;
   tree->Branch("dr_tau_bjet1",&dr_tau_bjet1,"dr_tau_bjet1/F");   
   float deta_bjet1_jets = 0;
   tree->Branch("deta_bejt1_jets",&deta_bjet1_jets,"deta_bjet1_jets/F");   

   float st_1bjet = 0;
   tree->Branch("st_1bjet",&st_1bjet,"st_1bjet/F");   
   float st_2bjet = 0;
   tree->Branch("st_2bjet",&st_2bjet,"st_2bjet/F");   
   float st_2jet = 0;
   tree->Branch("st_2jet",&st_2jet,"st_2jet/F");   

   float m11 = 0;
   tree->Branch("m11",&m11,"m11/F");
   float m12 = 0;
   tree->Branch("m12",&m12,"m12/F");
   float m21 = 0;
   tree->Branch("m21",&m21,"m21/F");
   float m22 = 0;
   tree->Branch("m22",&m22,"m22/F");

   float m1_reco_2bjet = 0;
   tree->Branch("m1_reco_2bjet",&m1_reco_2bjet,"m1_reco_2bjet/F");
   float m2_reco_2bjet = 0;
   tree->Branch("m2_reco_2bjet",&m2_reco_2bjet,"m2_reco_2bjet/F");

   float m1_reco_1bjet = 0;
   tree->Branch("m1_reco_1bjet",&m1_reco_1bjet,"m1_reco_1bjet/F");
   float m2_reco_1bjet = 0;
   tree->Branch("m2_reco_1bjet",&m2_reco_1bjet,"m2_reco_1bjet/F");

   float m1_reco_2jet = 0;
   tree->Branch("m1_reco_2jet",&m1_reco_2jet,"m1_reco_2jet/F");
   float m2_reco_2jet = 0;
   tree->Branch("m2_reco_2jet",&m2_reco_2jet,"m2_reco_2jet/F");

   float tm = 0;
   tree->Branch("tm",&tm,"tm/F");   
   float tm_corr = 0;
   tree->Branch("tm_corr",&tm_corr,"tm_corr/F");   
   float p_zeta_old = 0;
   tree->Branch("p_zeta_old",&p_zeta_old,"p_zeta_old/F");
   float p_zeta = 0;
   tree->Branch("p_zeta",&p_zeta,"p_zeta/F");
   float p_zeta_corr = 0;
   tree->Branch("p_zeta_corr",&p_zeta_corr,"p_zeta_corr/F");

   int npv = 0;
   tree->Branch("npv",&npv,"npv/I");
   float zpv = 0;
   tree->Branch("zpv",&zpv,"zpv/F");

   TTree* gentree = new TTree("mygentree","mygentree");
   
   float pt_bquark_lead = 0;
   gentree->Branch("pt_bquark_lead", &pt_bquark_lead, "pt_bquark_lead/F");
   float pt_bquark_trail = 0;
   gentree->Branch("pt_bquark_trail", &pt_bquark_trail, "pt_bquark_trail/F");

   float eta_bquark_lead = 0;
   gentree->Branch("eta_bquark_lead", &eta_bquark_lead, "eta_bquark_lead/F");
   float eta_bquark_trail = 0;
   gentree->Branch("eta_bquark_trail", &eta_bquark_trail, "eta_bquark_trail/F");

   float pt_cquark_lead = 0;
   gentree->Branch("pt_cquark_lead", &pt_cquark_lead, "pt_cquark_lead/F");
   float pt_cquark_trail = 0;
   gentree->Branch("pt_cquark_trail", &pt_cquark_trail, "pt_cquark_trail/F");

   float eta_cquark_lead = 0;
   gentree->Branch("eta_cquark_lead", &eta_cquark_lead, "eta_cquark_lead/F");
   float eta_cquark_trail = 0;
   gentree->Branch("eta_cquark_trail", &eta_cquark_trail, "eta_cquark_trail/F");


   //For Acceptance x Efficiency
   double count_allevents = 0;
   double count_leptaufound = 0;

   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nentriesReal = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      ++count_allevents;

      if (jentry%10000==0)
	std::cout << "Processed " << jentry << " out of " << nentriesReal << std::endl;

      //==========================================
      // Define scale factors
      //==========================================
      // Muon
      // from AN360 
      //0.9933+/-0.0003 (Barrel);
      //0.9982+/-0.0004 (Endcap);
      double muonIdEB = 0.993;  // |eta| < 1.2
      double muonIdEE = 0.975;  // |eta| > 1.2 && |eta| < 2.4
      // Electron
      // from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SimpleCutBasedEleID2011#Efficiency_data_MC_scale_factors
      double eleIdEB = 0.98;
      double eleIdEE = 1.0;
      // b-jets
      // from BTV-11-001: 0.95+/-0.01+/-0.10
      double SFb = 0.95;
      // from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPerformanceDBV2
      double SFlight = 1.09;

      // Integrated luminosity
      double lumi = desired_luminosity;
      double Nevents = Numberofevents;
      double sigma = crosssection;
      int isSignal = SampleIsSignal;

      Run = run;
      Event = event;
      LumiSec = ls;

      //================ Get Generator event info
      int ngenpart = GenParticleEnergy->size();
      bool genZmm = false;
      bool genZtt = false;
      int genMu1 = -1;
      int genMu2 = -1;
      int genTau1 = -1;
      int genTau2 = -1;
      int boson = -1;
      //bool bquark1Found = false;
      //bool bquark2Found = false;
      //TLorentzVector gen_bquark1, gen_bquark2;
      TLorentzVector gen_lep1, gen_lep2;
      TLorentzVector gen_tau1, gen_tau2;
      TLorentzVector gen_Z;
      if (name=="DYJets") {
	for(int ip = 0; ip != ngenpart; ++ip) {
	  int pdgId = GenParticlePdgId->at(ip);
	  int motherIndex = GenParticleMotherIndex->at(ip);
	  if ( motherIndex == -1 ) continue; // ISR
	  if (motherIndex >= ngenpart) continue;
	  int pdgIdMother = GenParticlePdgId->at(motherIndex);
	  //If Z->ee
	  if ( pdgId == 13 && pdgIdMother == 23) {
	    genMu1 = ip;
	    gen_lep1.SetPtEtaPhiM(GenParticlePt->at(ip),
				  GenParticleEta->at(ip),
				  GenParticlePhi->at(ip),
				  0);
	  }
	  if ( pdgId == -13 && pdgIdMother == 23) {
	    genMu2 = ip;
	    gen_lep2.SetPtEtaPhiM(GenParticlePt->at(ip),
				  GenParticleEta->at(ip),
				  GenParticlePhi->at(ip),
				  0);
	  }
	  if ( pdgId == 15 && pdgIdMother == 23) {
	    genTau1 = ip;
	    gen_tau1.SetPtEtaPhiM(GenParticlePt->at(ip),
				  GenParticleEta->at(ip),
				  GenParticlePhi->at(ip),
				  0);
	  }
	  if ( pdgId == -15 && pdgIdMother == 23) {
	    genTau2 = ip;
	    gen_tau2.SetPtEtaPhiM(GenParticlePt->at(ip),
				  GenParticleEta->at(ip),
				  GenParticlePhi->at(ip),
				  0);
	  }
	  
	  if(pdgIdMother == motherId) {
	    boson = ip;
	    gen_Z.SetPtEtaPhiM(GenParticlePt->at(motherIndex),
			       GenParticleEta->at(motherIndex),
			       GenParticlePhi->at(motherIndex),
			       0);
	  }
	}//End of loop over genParticles
      }
      
      iszll = 0;
      isztt = 0;
      if (genMu1!=-1 && genMu2!=-1){
	genZmm = true;
	iszll = 1;
      }
      if (genTau1!=-1 && genTau2!=-1){
	genZtt = true;
	isztt = 1;
      }

      //      if (!genZmm) continue;
      //      if (!genZtt) continue;

      /*
      //Fill gen lepton info
      pt_genlep1 = -10;
      phi_genlep1 = -10;
      eta_genlep1 = -10;
      pt_genlep2 = -10;
      phi_genlep2 = -10;
      eta_genlep2 = -10;
      if (genTau1!=-1 && genTau2!=-1) {
      pt_genlep1 = gen_tau1.Pt();
      phi_genlep1 = gen_tau1.Phi();
      eta_genlep1 = gen_tau1.Eta();
      pt_genlep2 = gen_tau2.Pt();
      phi_genlep2 = gen_tau2.Phi();
      eta_genlep2 = gen_tau2.Eta();
      }
      */

      //      double rho = rhoFastJet;	      
      
      // =================================================
      // Require triggers
      // =================================================
      bool triggerFired = false;
      /*
      if (HLTResults->size() > 0) {
	if ( HLTResults->at(13)==1  
	     || HLTResults->at(14)==1 
	     || HLTResults->at(15)==1 
	     || HLTResults->at(16)==1
	     || HLTResults->at(17)==1
	     || HLTResults->at(18)==1
	     || HLTResults->at(19)==1
	     || HLTResults->at(20)==1
	     || HLTResults->at(21)==1
	     || HLTResults->at(23)==1
	     || HLTResults->at(24)==1) triggerFired = true;
      } 
      pass_trigger = 0;
      if (triggerFired) pass_trigger = 1;
      */
      pass_trigger = 1;

      // =================================================
      //Get PFMET
      // =================================================
      double pfMet = PFMET->at(0);
      double pfMetPhi = PFMETPhi->at(0);      

      //Initialize
      pfMET = -10;
      pfMETPhi = -10;
      pfMET_corr = -10;
      pfMETPhi_corr = -10;
      //Fill
      pfMET = pfMet;
      pfMETPhi = pfMetPhi;
      pfMET_corr = pfMet;
      pfMETPhi_corr = pfMetPhi;

      /*
      // =================================================
      // Get Vertex
      // =================================================
      int nGoodVertices = 0;
      npv = 0;
      zpv = -100;
      for(unsigned int ivtx = 0; ivtx != VertexZ->size(); ++ivtx) {
	double chi2 = VertexChi2->at(ivtx);
	double ndf  = VertexNDF->at(ivtx);
	double z    = VertexZ->at(ivtx);
	double rho  = VertexRho->at(ivtx);
	
	if (!(VertexIsFake->at(ivtx)) && ndf > 4 && fabs(z) < 24 && fabs(rho)< 2) {
	  ++nGoodVertices;
	  if (nGoodVertices==1)
	    zpv = z;
	}
      }
      if (nGoodVertices==0) continue;
      npv = nGoodVertices;
      */

      // =================================================
      // Loop over muons
      // =================================================
      vector<int> v_loosemuons;  
      vector<int> v_muons;
      for (int imu=0; imu<MuonPt->size(); ++imu) {
	
	double MuonPFRelIso = (MuonPFIsoR04Photon->at(imu) +
			       MuonPFIsoR04ChargedHadron->at(imu) +
			       MuonPFIsoR04NeutralHadron->at(imu) - 
			       MuonPFIsoR04PU->at(imu))/MuonPt->at(imu);
	

	bool quality_loose = MuonPt->at(imu) > 25
	  && MuonPassID->at(imu) == 1
	  && MuonPFRelIso < 0.3; 
	if (quality_loose)
	  v_loosemuons.push_back(imu);
	
	// Final Pt and eta cuts
	bool kinematics = (*MuonPt)[imu] > 30
	  && TMath::Abs((*MuonEta)[imu]) < 2.1;
	if (!kinematics) continue;
	
	//compute d0 for a muon
	//	double dist3d = MuonVtxDist3D->at(imu);
	//	double distz = MuonVtxDistZ->at(imu);
	//	double distxy = sqrt(dist3d*dist3d - distz*distz);

	// Final selection on muon object
	bool quality = MuonPassID->at(imu) == 1
	  && MuonIsGlobal->at(imu) == 1
	  && MuonIsTracker->at(imu) == 1
	  && MuonTrkHits->at(imu) > 10
	  && MuonPixelHits->at(imu) > 0
	  && TMath::Abs(MuonBestTrackVtxDistXY->at(imu)) < 0.2
	  //	  && TMath::Abs(distxy) < 0.045
	  && MuonSegmentMatches->at(imu) > 1
	  && TMath::Abs(MuonBestTrackVtxDistZ->at(imu)) < 0.5
	  //	  && TMath::Abs(distz) < 0.2
	  && MuonPFRelIso < 0.1;
	
	if (!quality) continue;
	
	//Calculate MT for muon and MET
	//	double MT = sqrt(2*MuonPt->at(imu)*pfMet*(1-cos(delta_phi(MuonPhi->at(imu),pfMetPhi))));
	
	v_muons.push_back(imu);
      }//End of loop over muons
      
      //If no good muon found in event, continue;
      if (v_muons.size()==0) continue; 
      //If more than one global muon found in event with opposite charge, continue;
      bool secondMuonOS = false;
      for (int i=0; i<v_loosemuons.size(); ++i) {
	if (MuonCharge->at(v_muons[0])*MuonCharge->at(v_loosemuons[i])<0)
	  secondMuonOS = true;
      }      
      if(secondMuonOS) continue;
      
      int imu1 = v_muons[0];
      TLorentzVector muon;
      muon.SetPtEtaPhiM(MuonPt->at(imu1),
			MuonEta->at(imu1),
			MuonPhi->at(imu1),
			0);
      int muon1_charge = MuonCharge->at(imu1);
      double muon1_pfreliso = (MuonPFIsoR04Photon->at(imu1) +
			       MuonPFIsoR04ChargedHadron->at(imu1) +
			       MuonPFIsoR04NeutralHadron->at(imu1) - 
			       MuonPFIsoR04PU->at(imu1))/MuonPt->at(imu1);
	
      //      double muon1_pfreliso = MuonPFRelIso->at(imu1);
      //      double muon1_d0 = MuonTrkD0->at(imu1);
      //      double muon1_dz = MuonTrkDz->at(imu1);
      int assocVtxIndex = MuonVtxIndex->at(imu1);

      //Now check if tau is not coming from neither gen_tau nor gen_lep
      isfromjet_lep = 1;
      bool isFromGenTau = false;
      bool isFromGenLep = false;
      /*
      if (genZtt) 
	if (muon.DeltaR(gen_tau1)<0.1 || muon.DeltaR(gen_tau2) < 0.1)
	  isFromGenTau = true;
      if (genZmm) 
	if (muon.DeltaR(gen_lep1)<0.1 || muon.DeltaR(gen_lep2) < 0.1)
	  isFromGenLep = true;

      if (isFromGenTau || isFromGenLep) 
	isfromjet_lep = 0;
      */

      // =================================================
      // Get Vertex
      // =================================================
      int nGoodVertices = 0;
      npv = 0;
      zpv = -100;
      for(unsigned int ivtx = 0; ivtx != VertexZ->size(); ++ivtx) {
	double chi2 = VertexChi2->at(ivtx);
	double ndf  = VertexNDF->at(ivtx);
	double z    = VertexZ->at(ivtx);
	double rho  = VertexRho->at(ivtx);
	
	if (!(VertexIsFake->at(ivtx)) 
	    && ndf > 4 
	    && fabs(z) < 24 
	    && fabs(rho)< 2) {
	  ++nGoodVertices;
	  if (ivtx==assocVtxIndex) 
	    zpv = z;
	}
      }
      if (nGoodVertices==0) continue;
      npv = nGoodVertices;
      
      // ========================================
      // Loop over Taus
      // ========================================
      vector<int> v_taus;
      vector<TLorentzVector> lv_taus;
      for (int itau=0; itau< HPSTauPt->size(); ++itau) {
	
	double tauPt = HPSTauPt->at(itau);
	double tauEta = HPSTauEta->at(itau);
	double tauPhi = HPSTauPhi->at(itau);
	if (TMath::Abs(tauEta) > 2.3) continue;
	
	//Scale tau pT by TauES uncertainty
	double unc = 0.;
	double base = 0.03; 
	//	unc = base;
	double newtauPt = tauPt*(1+unc);
	
	//Tau ER smearing
	double minDR = 1000;
	bool matchedGenJetFound = false;
	double ptgen = 0;
	for (int igenjet=0; igenjet<GenJetPt->size();++igenjet){
	  TLorentzVector recotau;
	  recotau.SetPtEtaPhiM(tauPt,
			       tauEta,
			       tauPhi,
			       0);
	  TLorentzVector genjet;
	  genjet.SetPtEtaPhiM(GenJetPt->at(igenjet),
			      GenJetEta->at(igenjet),
			      GenJetPhi->at(igenjet),
			      0);
	  if (recotau.DeltaR(genjet) > minDR) continue;
	  minDR = recotau.DeltaR(genjet);
	  ptgen = GenJetPt->at(igenjet);
	}
	if (minDR < 0.05) 
	  matchedGenJetFound = true;
	
	if (matchedGenJetFound) {
	  double c = 0.9;
	  //	  newtauPt = max(ptgen + c*(tauPt-ptgen));
	}

	bool kinematics = newtauPt > 30
	  && TMath::Abs(tauEta) < 2.3;
	if (!kinematics) continue;
		
	bool quality =HPSTauDecayMode->at(itau) == 1
	  && HPSTauLooseCombinedIsolationDeltaBetaCorrDiscr->at(itau) == 1
	  && HPSTauAgainstElectronLooseDiscr->at(itau) == 1
	  && HPSTauAgainstMuonTightDiscr->at(itau) == 1
	  && TMath::Abs(HPSTauVertexZ->at(itau)-zpv) < 0.2;
	if (!quality) continue; 
	

	TLorentzVector thisTau;
	thisTau.SetPtEtaPhiM(newtauPt,
			     HPSTauEta->at(itau),
			     HPSTauPhi->at(itau),
			     0);
	
	//Check that the tau candidate does is away from any muon from loosemuons
	//by at least dR=0.5
	bool tauCloseToMuon = false;
	for (int i=0; i<v_loosemuons.size(); ++i){	  
	  int i_muon = v_loosemuons[i];
	  TLorentzVector loosemuon;
	  loosemuon.SetPtEtaPhiM(MuonPt->at(i_muon),
				 MuonEta->at(i_muon),
				 MuonPhi->at(i_muon),
				 0);
	  if (thisTau.DeltaR(loosemuon) < 0.5)
	    tauCloseToMuon = true;
	}
	
	if (tauCloseToMuon) continue;             
	//	if (muon1_charge*HPSTauCharge->at(itau) > 0) continue;
	lv_taus.push_back(thisTau);
	v_taus.push_back(itau);
      }//End of loop over taus
      
      // Move to next event if zero tau candidate found passing selection
      if (v_taus.size()==0) continue;
      int itau1 = v_taus[0];
      TLorentzVector tau;
      tau = lv_taus[0];
      int tau1_charge = HPSTauCharge->at(itau1);
      //      double tau1_trkdz = HPSTauLeadTrackdz->at(itau1);
      //      double tau1_trkd0 = HPSTauLeadTrackd0->at(itau1);
      //      double tau1_ptsum = ptSum(HPSTauPtSumPFChargedHadronsIsoCone->at(itau1),
      //				HPSTauPtSumPhotonsIsoCone->at(itau1),
      //				rho);
				
      ++count_leptaufound;
      
      //Now check if tau is not coming from neither gen_tau nor gen_lep
      isfromjet_tau = 1;
      isFromGenTau = false;
      isFromGenLep = false;
      if (genZtt) 
	if (tau.DeltaR(gen_tau1)<0.1 || tau.DeltaR(gen_tau2) < 0.1)
	  isFromGenTau = true;
      if (genZmm) 
	if (tau.DeltaR(gen_lep1)<0.1 || tau.DeltaR(gen_lep2) < 0.1)
	  isFromGenLep = true;

      if (isFromGenTau || isFromGenLep) 
	isfromjet_tau = 0;      

      //==================== Caclucalte transverse mass of muon and MET
      tm = -10;
      tm_corr = -10;
      tm = sqrt(2*muon.Pt()*pfMET*(1-cos(delta_phi(muon.Phi(),pfMETPhi))));
      tm_corr = sqrt(2*muon.Pt()*pfMET_corr*(1-cos(delta_phi(muon.Phi(),pfMETPhi_corr))));

      //      if (tm > 40) continue;      
      //      if (tau1_charge*muon1_charge>0)continue;  

      // ========================================
      // Loop over PFJets
      // ========================================
      vector<int> v_pfjets;
      vector<int> v_pfbtagjets;
      vector<TLorentzVector> lv_pfbtagjets;
      int nbjets_l = 0;
      int nbjets_m = 0;
      int nbjets_t = 0;
      TLorentzVector btaggedjet;
      bool btaggedJetFound = false;
      for (int ijet=0; ijet<PFJetPt->size(); ++ijet) {

	double jetEta = PFJetEta->at(ijet);
	double jetPt = PFJetPt->at(ijet);	
	double jetPhi = PFJetPhi->at(ijet);	
	
	if(TMath::Abs(jetEta) > 2.4) continue;
	
	//Scale jet pT by JEC uncertainty
	double unc = 0.;
	double base = 0.03; //PFJetJECUnc->at(ijet);
	double baseandresid = NewJesUncertainty(base,jetPt, jetEta);
	double baseandresidandflav = sqrt(baseandresid*baseandresid
					  +
					  0.01*0.01);
	//	unc = baseandresidandflav;
	double newjetPt = jetPt*(1-unc);
	//	cout << jetPt << endl;
	
	//For JER smearing
	double minDR = 1000;
	bool matchedGenJetFound = false;
	double ptgen = 0;
	for (int igenjet=0; igenjet<GenJetPt->size();++igenjet){
	  TLorentzVector recojet;
	  recojet.SetPtEtaPhiM(jetPt,
			       jetEta,
			       jetPhi,
			       0);
	  TLorentzVector genjet;
	  genjet.SetPtEtaPhiM(GenJetPt->at(igenjet),
			      GenJetEta->at(igenjet),
			      GenJetPhi->at(igenjet),
			      0);
	  if (recojet.DeltaR(genjet) > minDR) continue;
	  minDR = recojet.DeltaR(genjet);
	  ptgen = GenJetPt->at(igenjet);
	}
	if (minDR < 0.4) 
	  matchedGenJetFound = true;
	
	//	newjetPt = smearJER(jetPt,jetEta,ptgen,1,matchedGenJetFound);

	bool kinematics = newjetPt > 30
	  && TMath::Abs(jetEta) < 2.4;
	if (!kinematics) continue;
	
	if (PFJetPassLooseID->at(ijet) != 1) continue;

	TLorentzVector jet;
	jet.SetPtEtaPhiM(newjetPt,
			 PFJetEta->at(ijet),
			 PFJetPhi->at(ijet),
			 0);
	
	if (tau.DeltaR(jet) < 0.5) continue;
	if (muon.DeltaR(jet) < 0.5) continue;
	
	if (PFJetCombinedSecondaryVertexBTag->at(ijet) > 0.244) {
	  ++nbjets_l;
	  v_pfbtagjets.push_back(ijet);
	  lv_pfbtagjets.push_back(jet);
	}

	//CSVM
	if (PFJetCombinedSecondaryVertexBTag->at(ijet) > 0.679) ++nbjets_m;	
	//	CSVT
	if (PFJetCombinedSecondaryVertexBTag->at(ijet) > 0.898) ++nbjets_t;

	v_pfjets.push_back(ijet);	
      }//End of loop over PF jets


      //=========================================================
      //============= Fill histograms and tree variables
      //=========================================================
      //Get nPU
      //      double NPU = (*nPU)[0];
      //      h_nPU->Fill(NPU);
      //      float puWeight = puweight(NPU);
      //      float puWeight = pileupWeight_Fall11_InTime((*nPU)[1]);
      // +++++++++++++++++++++++++ Initialize tree variables for every event      
      //cross section normalization
      //      if (isSignal==1)
      double weight = 1./Nevents;

      //Trigger weight
      double muoneta = muon.Eta();
      double muonpt = muon.Pt();
      double taueta = tau.Eta();
      double taupt = tau.Pt();
      SFtrig = muonTriggerEfficiency(muonpt,muoneta,taupt,taueta);

      /*
      if (fabs(muon.Eta())<1.2)
	Weight = lumi*weight*puWeight*muonIdEB;
      else
	Weight = lumi*weight*puWeight*muonIdEE;
      */
      
      Weight = lumi*weight;
      
      //Compute weight due to btag/mistag
      Wtag = 1.;
      vector<vector<BTagWeight::JetInfo> > j;
      for (int i=0;i<v_pfjets.size();++i){
	
	int index = v_pfjets[i];
	double jetpt = PFJetPt->at(index);
	double jeteta = PFJetEta->at(index);
	double jetflavor = TMath::Abs(PFJetPartonFlavour->at(index));
	double discr = 1.7;
	
	double eff = 1.;
	double sf = 1.;
	if (jetflavor==5){
	  eff = btagEFF(discr,1);
	  sf = btagSF(jetpt,1);
	}else if (jetflavor==4){
	  eff = btagEFF(discr,0);
	  sf = btagSF(jetpt,0);
	}else{
	  eff = mistagEFF(jetpt,jeteta);
	  sf = mistagSF(jetpt,jeteta);
	}
	
	BTagWeight::JetInfo jetinfo(eff,sf);
	vector<BTagWeight::JetInfo> jetInfoForAllOPs;
	jetInfoForAllOPs.push_back(jetinfo);
	j.push_back(jetInfoForAllOPs);

      }// End of loop over PF-jets
      Wtag = b.weight(j);
      
      //
      m_lep_tau = -10;
      m_lep_tau_met = -10;
      //
      pt_lep = -10;
      eta_lep = -10;
      phi_lep = -10;
      charge_lep = -10;
      iso_lep = -10;
      d0_lep = -10;
      dz_lep = -10;
      //
      pt_tau = -10;
      eta_tau = -10;
      phi_tau = -10;
      charge_tau = -10;
      d0_tau = -10;
      dz_tau = -10;
      iso_tau = -10;
      //
      njets = -10;
      nbtagjetsl = -10;
      nbtagjetsm = -10;
      nbtagjetst = -10;
      //
      pt_jet1_2bjet = -10;
      eta_jet1_2bjet = -10;
      phi_jet1_2bjet = -10;      
      pt_jet2_2bjet = -10;
      eta_jet2_2bjet = -10;
      phi_jet2_2bjet = -10;
      //
      pt_jet1_1bjet = -10;
      eta_jet1_1bjet = -10;
      phi_jet1_1bjet = -10;      
      pt_jet2_1bjet = -10;
      eta_jet2_1bjet = -10;
      phi_jet2_1bjet = -10;      
      //
      found_jet1 = -10;
      pt_jet1 = -10;
      eta_jet1 = -10;
      phi_jet1 = -10;
      btag_jet1 = -10;
      TCHE_jet1 = -10;
      TCHP_jet1 = -10;
      //
      found_jet2 = -10;
      pt_jet2 = -10;
      eta_jet2 = -10;
      phi_jet2 = -10;
      btag_jet2 = -10;
      TCHE_jet2 = -10;
      TCHP_jet2 = -10;
      //
      dphi_leptau_met = -10;
      dphi_lep_tau = -10;
      deta_lep_tau = -10;
      dr_lep_tau = -10;
      dphi_met_lep = -10;
      dphi_met_tau = -10;
      dphi_lep_bjet1 = -10;
      dphi_tau_bjet1 = -10;
      deta_lep_bjet1 = -10;
      deta_tau_bjet1 = -10;
      dr_lep_bjet1 = -10;
      dr_tau_bjet1 = -10;      
      deta_bjet1_jets = -10;
      //
      
      //++++++++++++++++ Now fill tree variables
      
      pt_lep = muon.Pt();
      eta_lep = muon.Eta();
      phi_lep = muon.Phi();
      charge_lep = muon1_charge;      
      iso_lep = muon1_pfreliso;
      //      d0_lep = muon1_d0;
      //      dz_lep = muon1_dz;

      pt_tau = tau.Pt();
      eta_tau = tau.Eta();
      phi_tau = tau.Phi();
      charge_tau = tau1_charge;
      //      d0_tau = tau1_trkd0;
      //      dz_tau = tau1_trkdz;
      //      iso_tau = tau1_ptsum;


      m_lep_tau = (muon+tau).M();      
      
      //Store dPhi between lepton/tau/met
      dphi_lep_tau = acos(cos(muon.Phi()-tau.Phi()));
      deta_lep_tau = TMath::Abs(muon.Eta()-tau.Eta());
      dr_lep_tau = muon.DeltaR(tau);
      dphi_met_lep = acos(cos(pfMetPhi-muon.Phi()));
      dphi_met_tau = acos(cos(pfMetPhi-tau.Phi()));
      double phi_leptau = acos((muon.Pt()*cos(muon.Phi())+tau.Pt()*cos(tau.Phi()))/(muon.Pt()+tau.Pt()));
      dphi_leptau_met = delta_phi(phi_leptau,pfMetPhi);

      //Calculate MET projection on lepton and tau
      double leptonMetDPhi = TMath::Abs(acos(cos(pfMetPhi-muon.Phi())));
      double metOnLepton = pfMet*cos(leptonMetDPhi);
      
      double tauMetDPhi = TMath::Abs(acos(cos(pfMetPhi-tau.Phi())));
      double metOnTau = pfMet*cos(tauMetDPhi);
      
      TLorentzVector leptonnu;
      leptonnu.SetPtEtaPhiM(metOnLepton,
			    muon.Eta(),
			    getPhi(muon.Phi()),
			    0);

      TLorentzVector taunu;
      taunu.SetPtEtaPhiM(metOnTau,
			 tau.Eta(),
			 getPhi(tau.Phi()),
			 0);
      
      m_lep_tau_met = (muon+tau+leptonnu+taunu).M();   
      
      njets = v_pfjets.size();
      //Fill b-tagging flags
      nbtagjetsl = nbjets_l;
      nbtagjetsm = nbjets_m;
      nbtagjetst = nbjets_t;
      
      st_1bjet = -10;
      st_2bjet = -10;
      st_2jet = -10;
      m11 = -10;
      m12 = -10;
      m21 = -10;
      m22 = -10;
      m1_reco_2bjet = -10;
      m2_reco_2bjet = -10;
      m1_reco_1bjet = -10;
      m2_reco_1bjet = -10;
      m1_reco_2jet = -10;
      m2_reco_2jet = -10;
      Wtag_2bjet = 1.;
      Wtag_1bjet = 1.;
      if (v_pfbtagjets.size() >= 2) {
	int ijet1 = v_pfbtagjets[0];
	int ijet2 = v_pfbtagjets[1];
	
	/*
	//Scale jet pT by JEC uncertainty
	//Jet 1
	double unc = 0.;
	double base = 0.03;//PFJetJECUnc->at(ijet1);
	double baseandresid = NewJesUncertainty(base,
						PFJetPt->at(ijet1), 
						PFJetEta->at(ijet1));
 
	double baseandresidandflav = sqrt(baseandresid*baseandresid
					  +
					  0.01*0.01);
	//	unc = baseandresidandflav;
	//PFJetPt->at(ijet1)*(1-unc);
	*/
	double jet1Pt = lv_pfbtagjets[0].Pt();
	
	//Jet 2
	/*
	unc = 0.;
	base = 0.03;//PFJetJECUnc->at(ijet2);
	baseandresid = NewJesUncertainty(base,
					 PFJetPt->at(ijet2), 
					 PFJetEta->at(ijet2));	
	baseandresidandflav = sqrt(baseandresid*baseandresid
				   +
				   0.01*0.01);
	//	unc = baseandresidandflav;
	//PFJetPt->at(ijet2)*(1-unc);
	*/
	double jet2Pt = lv_pfbtagjets[1].Pt();

	//
	st_2bjet = muon.Pt() + tau.Pt() + jet1Pt + jet2Pt;

	TLorentzVector bjet1, bjet2;
	bjet1.SetPtEtaPhiM(jet1Pt,
			   PFJetEta->at(ijet1),
			   PFJetPhi->at(ijet1),
			   0);
	bjet2.SetPtEtaPhiM(jet2Pt,
			   PFJetEta->at(ijet2),
			   PFJetPhi->at(ijet2),
			   0);
	pt_jet1_2bjet = bjet1.Pt();
	eta_jet1_2bjet = bjet1.Eta();
	phi_jet1_2bjet = bjet1.Phi();
	pt_jet2_2bjet = bjet2.Pt();
	eta_jet2_2bjet = bjet2.Eta();
	phi_jet2_2bjet = bjet2.Phi();

	//Now store btag/mistag scale factors corresponding to jet1 and jet2
	double SFjet1 = 1.;
	double SFjet2 = 1.;
	if (TMath::Abs(PFJetPartonFlavour->at(ijet1))==5)
	  SFjet1 = btagSF(bjet1.Pt(),1);
	else
	  SFjet1 = mistagSF(bjet1.Pt(),bjet1.Eta());

	if (TMath::Abs(PFJetPartonFlavour->at(ijet2))==5)
	  SFjet2 = btagSF(bjet2.Pt(),1);
	else
	  SFjet2 = mistagSF(bjet2.Pt(),bjet2.Eta());

	Wtag_2bjet = SFjet1*SFjet2;

	m11 = (muon+bjet1).M();
	m12 = (muon+bjet2).M();
	m21 = (tau+bjet1).M();
	m22 = (tau+bjet2).M();
	if (fabs(m11-m22) < fabs(m12-m21)) {
	  m1_reco_2bjet = m11;
	  m2_reco_2bjet = m22;
	}else{
	  m1_reco_2bjet = m12;
	  m2_reco_2bjet = m21;
	}
      }
      
      if (v_pfbtagjets.size() >= 1) {
	int ijet1 = v_pfbtagjets[0];
	int ijet2 = -1;
	for (int i = 0; i<v_pfjets.size(); ++i) {
	  if (ijet1 == v_pfjets[i]) continue;
	  ijet2 = v_pfjets[i];
	  break;
	}
	if (ijet2!=-1) {
	  st_1bjet = muon.Pt() + tau.Pt() + PFJetPt->at(ijet1) + PFJetPt->at(ijet2);
	  TLorentzVector bjet1, jet2;
	  bjet1.SetPtEtaPhiM(PFJetPt->at(ijet1),
			     PFJetEta->at(ijet1),
			     PFJetPhi->at(ijet1),
			     0);
	  jet2.SetPtEtaPhiM(PFJetPt->at(ijet2),
			    PFJetEta->at(ijet2),
			    PFJetPhi->at(ijet2),
			    0);

	  pt_jet1_1bjet = bjet1.Pt();
	  eta_jet1_1bjet = bjet1.Eta();
	  phi_jet1_1bjet = bjet1.Phi();
	  pt_jet2_1bjet = jet2.Pt();
	  eta_jet2_1bjet = jet2.Eta();
	  phi_jet2_1bjet = jet2.Phi();

	  double SFjet1 = 1.;
	  if (TMath::Abs(PFJetPartonFlavour->at(ijet1))==5)
	    SFjet1 = btagSF(bjet1.Pt(),1);
	  else
	    SFjet1 = mistagSF(bjet1.Pt(),bjet1.Eta());
	  
	  Wtag_2bjet = SFjet1;
	  
	  double M11 = (muon+bjet1).M();
	  double M12 = (muon+jet2).M();
	  double M21 = (tau+bjet1).M();
	  double M22 = (tau+jet2).M();
	  if (fabs(M11-M22) < fabs(M12-M21)) {
	    m1_reco_1bjet = M11;
	    m2_reco_1bjet = M22;
	  }else{
	    m1_reco_1bjet = M12;
	    m2_reco_1bjet = M21;
	  }
	}
      }
      
      if (v_pfjets.size() >= 2) {
	int ijet1 = v_pfjets[0];
	int ijet2 = v_pfjets[1];
	TLorentzVector jet1, jet2;
	jet1.SetPtEtaPhiM(PFJetPt->at(ijet1),
			  PFJetEta->at(ijet1),
			  PFJetPhi->at(ijet1),
			  0);
	jet2.SetPtEtaPhiM(PFJetPt->at(ijet2),
			  PFJetEta->at(ijet2),
			  PFJetPhi->at(ijet2),
			  0);

	st_2jet = muon.Pt() + tau.Pt() + jet1.Pt() + jet2.Pt();


	double M11 = (muon+jet1).M();
	double M12 = (muon+jet2).M();
	double M21 = (tau+jet1).M();
	double M22 = (tau+jet2).M();
	
	if (fabs(M11-M22) < fabs(M12-M21)) {
	  m1_reco_2jet = M11;
	  m2_reco_2jet = M22;
	}else{
	  m1_reco_2jet = M12;
	  m2_reco_2jet = M21;
	}
      }

      if (v_pfjets.size() >= 1) {
	
	found_jet1 = 1;
	int ijet1 = v_pfjets[0];
	pt_jet1 = PFJetPt->at(ijet1);
	eta_jet1 = PFJetEta->at(ijet1);
	phi_jet1 = PFJetPhi->at(ijet1);
	TCHE_jet1 = PFJetTrackCountingHighEffBTag->at(ijet1);	
	TCHP_jet1 = PFJetTrackCountingHighPurBTag->at(ijet1);	
	if (TCHE_jet1 > 1.7)
	  btag_jet1 = 1;
	
	if (v_pfjets.size() >= 2) {
	  
	  found_jet2 = 1;
	  int ijet2 = v_pfjets[1];
	  pt_jet2 = PFJetPt->at(ijet2);
	  eta_jet2 = PFJetEta->at(ijet2);
	  phi_jet2 = PFJetPhi->at(ijet2);
	  TCHE_jet2 = PFJetTrackCountingHighEffBTag->at(ijet2);
	  TCHP_jet2 = PFJetTrackCountingHighPurBTag->at(ijet2);
	  if (TCHE_jet2 > 1.7)
	    btag_jet2 = 1;
	}// v_pfjets.size()>=2
      }// v_pfjets.size()>=1
      
      tree->Fill();
   }//End of loop over events
   
   tree->Write();
   //   gentree->Write();
   //   h_gen_M_Ztt->Write();
   //   h_gen_M_Zmm->Write();
   out_tree->Close();
	
   cout << "count_allevents " << count_allevents << endl;
   cout << "count_leptaufound " << count_leptaufound << endl;
  
}


int main(){
  Estimate_mujet_placeholder t;
  t.Loop();
  return 0;
}
