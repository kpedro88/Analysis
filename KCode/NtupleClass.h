//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat May  9 11:44:27 2015 by ROOT version 5.34/18
// from TTree PreSelection/PreSelection
// found on file: root://cmseos.fnal.gov//store/user/awhitbe1/RA2synch_May7_2015_V2/PHYS14.SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_0_RA2AnalysisTree.root
//////////////////////////////////////////////////////////

#ifndef NtupleClass_h
#define NtupleClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class NtupleClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          RunNum;
   UInt_t          LumiBlockNum;
   UInt_t          EvtNum;
   Int_t           NVtx;
   Int_t           isoElectronTracks;
   Int_t           isoMuonTracks;
   Int_t           isoPionTracks;
   Int_t           Leptons;
   Int_t           LeptonsNoMiniIsolation;
   Int_t           NJets;
   Int_t           BTags;
   Int_t           Photons;
   Int_t           NJetsNoPhotons;
   Float_t         Weight;
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
   Float_t         minDeltaPhi;
   Float_t         ak1p2Jets_sumJetMass;
   Float_t         ak1p2JetsPt15Reclust_sumJetMass;
   Float_t         ak1p2JetsPt20Reclust_sumJetMass;
   Float_t         ak1p2JetsPt30Reclust_sumJetMass;
   Float_t         HTnoPhotons;
   Float_t         MHTnoPhotons;
   Float_t         DeltaPhi1noPhotons;
   Float_t         DeltaPhi2noPhotons;
   Float_t         DeltaPhi3noPhotons;
   Float_t         DeltaPhiN1noPhotons;
   Float_t         DeltaPhiN2noPhotons;
   Float_t         DeltaPhiN3noPhotons;
   Float_t         minDeltaPhiNnoPhotons;
   Float_t         METnoPhotonsPt;
   Float_t         METnoPhotonsPhi;
   UChar_t         JetID;
   vector<int>     *genParticles_PDGid;
   vector<int>     *ak1p2Jets_nSubjets;
   vector<int>     *ak4Jets_chargedHadMult;
   vector<int>     *ak4Jets_neutralHadMult;
   vector<int>     *ak4Jets_photonMult;
   vector<int>     *ak4Jets_flavor;
   vector<int>     *ak4JetsRaw_chargedHadMult;
   vector<int>     *ak4JetsRaw_neutralHadMult;
   vector<int>     *ak4JetsRaw_photonMult;
   vector<int>     *ak4JetsRaw_flavor;
   vector<double>  *deltaPhiN;
   vector<double>  *photon_isEB;
   vector<double>  *photon_genMatched;
   vector<double>  *photon_hadTowOverEM;
   vector<double>  *photon_hasPixelSeed;
   vector<double>  *photon_pfChargedIso;
   vector<double>  *photon_pfChargedIsoRhoCorr;
   vector<double>  *photon_pfGammaIso;
   vector<double>  *photon_pfGammaIsoRhoCorr;
   vector<double>  *photon_pfNeutralIso;
   vector<double>  *photon_pfNeutralIsoRhoCorr;
   vector<double>  *photon_sigmaIetaIeta;
   vector<double>  *ak1p2Jets_tau1;
   vector<double>  *ak1p2Jets_tau2;
   vector<double>  *ak1p2Jets_tau3;
   vector<double>  *ak1p2Jets_tau4;
   vector<double>  *ak4Jets_CSVdisc;
   vector<double>  *ak4Jets_chargeHadEfrac;
   vector<double>  *ak4Jets_neutralHadEfrac;
   vector<double>  *ak4Jets_photonEfrac;
   vector<double>  *ak4JetsRaw_CSVdisc;
   vector<double>  *ak4JetsRaw_chargeHadEfrac;
   vector<double>  *ak4JetsRaw_neutralHadEfrac;
   vector<double>  *ak4JetsRaw_photonEfrac;
   vector<TLorentzVector> *Muons;
   vector<TLorentzVector> *Electrons;
   vector<TLorentzVector> *genParticles;
   vector<TLorentzVector> *photonProd;
   vector<TLorentzVector> *ak1p2Jets;
   vector<TLorentzVector> *ak1p2JetsPt15Reclust;
   vector<TLorentzVector> *ak1p2JetsPt20Reclust;
   vector<TLorentzVector> *ak1p2JetsPt30Reclust;
   vector<TLorentzVector> *ak4Jets;
   vector<TLorentzVector> *ak4JetsRaw;
   vector<TLorentzVector> *bestPhoton;
   vector<TLorentzVector> *ak4JetsNoPhotons;
   vector<TLorentzVector> *ak4GenJets;

   // List of branches
   TBranch        *b_RunNum;   //!
   TBranch        *b_LumiBlockNum;   //!
   TBranch        *b_EvtNum;   //!
   TBranch        *b_NVtx;   //!
   TBranch        *b_isoElectronTracks;   //!
   TBranch        *b_isoMuonTracks;   //!
   TBranch        *b_isoPionTracks;   //!
   TBranch        *b_Leptons;   //!
   TBranch        *b_LeptonsNoMiniIsolation;   //!
   TBranch        *b_NJets;   //!
   TBranch        *b_BTags;   //!
   TBranch        *b_Photons;   //!
   TBranch        *b_NJetsNoPhotons;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_MHT;   //!
   TBranch        *b_DeltaPhi1;   //!
   TBranch        *b_DeltaPhi2;   //!
   TBranch        *b_DeltaPhi3;   //!
   TBranch        *b_minDeltaPhiN;   //!
   TBranch        *b_DeltaPhiN1;   //!
   TBranch        *b_DeltaPhiN2;   //!
   TBranch        *b_DeltaPhiN3;   //!
   TBranch        *b_METPt;   //!
   TBranch        *b_METPhi;   //!
   TBranch        *b_minDeltaPhi;   //!
   TBranch        *b_ak1p2Jets_sumJetMass;   //!
   TBranch        *b_ak1p2JetsPt15Reclust_sumJetMass;   //!
   TBranch        *b_ak1p2JetsPt20Reclust_sumJetMass;   //!
   TBranch        *b_ak1p2JetsPt30Reclust_sumJetMass;   //!
   TBranch        *b_HTnoPhotons;   //!
   TBranch        *b_MHTnoPhotons;   //!
   TBranch        *b_DeltaPhi1noPhotons;   //!
   TBranch        *b_DeltaPhi2noPhotons;   //!
   TBranch        *b_DeltaPhi3noPhotons;   //!
   TBranch        *b_DeltaPhiN1noPhotons;   //!
   TBranch        *b_DeltaPhiN2noPhotons;   //!
   TBranch        *b_DeltaPhiN3noPhotons;   //!
   TBranch        *b_minDeltaPhiNnoPhotons;   //!
   TBranch        *b_METnoPhotonsPt;   //!
   TBranch        *b_METnoPhotonsPhi;   //!
   TBranch        *b_JetID;   //!
   TBranch        *b_genParticles_PDGid;   //!
   TBranch        *b_ak1p2Jets_nSubjets;   //!
   TBranch        *b_ak4Jets_chargedHadMult;   //!
   TBranch        *b_ak4Jets_neutralHadMult;   //!
   TBranch        *b_ak4Jets_photonMult;   //!
   TBranch        *b_ak4Jets_flavor;   //!
   TBranch        *b_ak4JetsRaw_chargedHadMult;   //!
   TBranch        *b_ak4JetsRaw_neutralHadMult;   //!
   TBranch        *b_ak4JetsRaw_photonMult;   //!
   TBranch        *b_ak4JetsRaw_flavor;   //!
   TBranch        *b_deltaPhiN;   //!
   TBranch        *b_photon_isEB;   //!
   TBranch        *b_photon_genMatched;   //!
   TBranch        *b_photon_hadTowOverEM;   //!
   TBranch        *b_photon_hasPixelSeed;   //!
   TBranch        *b_photon_pfChargedIso;   //!
   TBranch        *b_photon_pfChargedIsoRhoCorr;   //!
   TBranch        *b_photon_pfGammaIso;   //!
   TBranch        *b_photon_pfGammaIsoRhoCorr;   //!
   TBranch        *b_photon_pfNeutralIso;   //!
   TBranch        *b_photon_pfNeutralIsoRhoCorr;   //!
   TBranch        *b_photon_sigmaIetaIeta;   //!
   TBranch        *b_ak1p2Jets_tau1;   //!
   TBranch        *b_ak1p2Jets_tau2;   //!
   TBranch        *b_ak1p2Jets_tau3;   //!
   TBranch        *b_ak1p2Jets_tau4;   //!
   TBranch        *b_ak4Jets_CSVdisc;   //!
   TBranch        *b_ak4Jets_chargeHadEfrac;   //!
   TBranch        *b_ak4Jets_neutralHadEfrac;   //!
   TBranch        *b_ak4Jets_photonEfrac;   //!
   TBranch        *b_ak4JetsRaw_CSVdisc;   //!
   TBranch        *b_ak4JetsRaw_chargeHadEfrac;   //!
   TBranch        *b_ak4JetsRaw_neutralHadEfrac;   //!
   TBranch        *b_ak4JetsRaw_photonEfrac;   //!
   TBranch        *b_Muons;   //!
   TBranch        *b_Electrons;   //!
   TBranch        *b_genParticles;   //!
   TBranch        *b_photonProd;   //!
   TBranch        *b_ak1p2Jets;   //!
   TBranch        *b_ak1p2JetsPt15Reclust;   //!
   TBranch        *b_ak1p2JetsPt20Reclust;   //!
   TBranch        *b_ak1p2JetsPt30Reclust;   //!
   TBranch        *b_ak4Jets;   //!
   TBranch        *b_ak4JetsRaw;   //!
   TBranch        *b_bestPhoton;   //!
   TBranch        *b_ak4JetsNoPhotons;   //!
   TBranch        *b_ak4GenJets;   //!

   NtupleClass(TTree *tree=0);
   virtual ~NtupleClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef NtupleClass_cxx
NtupleClass::NtupleClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://cmseos.fnal.gov//store/user/awhitbe1/RA2synch_May7_2015_V2/PHYS14.SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_0_RA2AnalysisTree.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root://cmseos.fnal.gov//store/user/awhitbe1/RA2synch_May7_2015_V2/PHYS14.SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_0_RA2AnalysisTree.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("root://cmseos.fnal.gov//store/user/awhitbe1/RA2synch_May7_2015_V2/PHYS14.SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_0_RA2AnalysisTree.root:/TreeMaker2");
      dir->GetObject("PreSelection",tree);

   }
   Init(tree);
}

NtupleClass::~NtupleClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NtupleClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NtupleClass::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void NtupleClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   genParticles_PDGid = 0;
   ak1p2Jets_nSubjets = 0;
   ak4Jets_chargedHadMult = 0;
   ak4Jets_neutralHadMult = 0;
   ak4Jets_photonMult = 0;
   ak4Jets_flavor = 0;
   ak4JetsRaw_chargedHadMult = 0;
   ak4JetsRaw_neutralHadMult = 0;
   ak4JetsRaw_photonMult = 0;
   ak4JetsRaw_flavor = 0;
   deltaPhiN = 0;
   photon_isEB = 0;
   photon_genMatched = 0;
   photon_hadTowOverEM = 0;
   photon_hasPixelSeed = 0;
   photon_pfChargedIso = 0;
   photon_pfChargedIsoRhoCorr = 0;
   photon_pfGammaIso = 0;
   photon_pfGammaIsoRhoCorr = 0;
   photon_pfNeutralIso = 0;
   photon_pfNeutralIsoRhoCorr = 0;
   photon_sigmaIetaIeta = 0;
   ak1p2Jets_tau1 = 0;
   ak1p2Jets_tau2 = 0;
   ak1p2Jets_tau3 = 0;
   ak1p2Jets_tau4 = 0;
   ak4Jets_CSVdisc = 0;
   ak4Jets_chargeHadEfrac = 0;
   ak4Jets_neutralHadEfrac = 0;
   ak4Jets_photonEfrac = 0;
   ak4JetsRaw_CSVdisc = 0;
   ak4JetsRaw_chargeHadEfrac = 0;
   ak4JetsRaw_neutralHadEfrac = 0;
   ak4JetsRaw_photonEfrac = 0;
   Muons = 0;
   Electrons = 0;
   genParticles = 0;
   photonProd = 0;
   ak1p2Jets = 0;
   ak1p2JetsPt15Reclust = 0;
   ak1p2JetsPt20Reclust = 0;
   ak1p2JetsPt30Reclust = 0;
   ak4Jets = 0;
   ak4JetsRaw = 0;
   bestPhoton = 0;
   ak4JetsNoPhotons = 0;
   ak4GenJets = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("RunNum", &RunNum, &b_RunNum);
   fChain->SetBranchAddress("LumiBlockNum", &LumiBlockNum, &b_LumiBlockNum);
   fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
   fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
   fChain->SetBranchAddress("isoElectronTracks", &isoElectronTracks, &b_isoElectronTracks);
   fChain->SetBranchAddress("isoMuonTracks", &isoMuonTracks, &b_isoMuonTracks);
   fChain->SetBranchAddress("isoPionTracks", &isoPionTracks, &b_isoPionTracks);
   fChain->SetBranchAddress("Leptons", &Leptons, &b_Leptons);
   fChain->SetBranchAddress("LeptonsNoMiniIsolation", &LeptonsNoMiniIsolation, &b_LeptonsNoMiniIsolation);
   fChain->SetBranchAddress("NJets", &NJets, &b_NJets);
   fChain->SetBranchAddress("BTags", &BTags, &b_BTags);
   fChain->SetBranchAddress("Photons", &Photons, &b_Photons);
   fChain->SetBranchAddress("NJetsNoPhotons", &NJetsNoPhotons, &b_NJetsNoPhotons);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("MHT", &MHT, &b_MHT);
   fChain->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
   fChain->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
   fChain->SetBranchAddress("DeltaPhi3", &DeltaPhi3, &b_DeltaPhi3);
   fChain->SetBranchAddress("minDeltaPhiN", &minDeltaPhiN, &b_minDeltaPhiN);
   fChain->SetBranchAddress("DeltaPhiN1", &DeltaPhiN1, &b_DeltaPhiN1);
   fChain->SetBranchAddress("DeltaPhiN2", &DeltaPhiN2, &b_DeltaPhiN2);
   fChain->SetBranchAddress("DeltaPhiN3", &DeltaPhiN3, &b_DeltaPhiN3);
   fChain->SetBranchAddress("METPt", &METPt, &b_METPt);
   fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
   fChain->SetBranchAddress("minDeltaPhi", &minDeltaPhi, &b_minDeltaPhi);
   fChain->SetBranchAddress("ak1p2Jets_sumJetMass", &ak1p2Jets_sumJetMass, &b_ak1p2Jets_sumJetMass);
   fChain->SetBranchAddress("ak1p2JetsPt15Reclust_sumJetMass", &ak1p2JetsPt15Reclust_sumJetMass, &b_ak1p2JetsPt15Reclust_sumJetMass);
   fChain->SetBranchAddress("ak1p2JetsPt20Reclust_sumJetMass", &ak1p2JetsPt20Reclust_sumJetMass, &b_ak1p2JetsPt20Reclust_sumJetMass);
   fChain->SetBranchAddress("ak1p2JetsPt30Reclust_sumJetMass", &ak1p2JetsPt30Reclust_sumJetMass, &b_ak1p2JetsPt30Reclust_sumJetMass);
   fChain->SetBranchAddress("HTnoPhotons", &HTnoPhotons, &b_HTnoPhotons);
   fChain->SetBranchAddress("MHTnoPhotons", &MHTnoPhotons, &b_MHTnoPhotons);
   fChain->SetBranchAddress("DeltaPhi1noPhotons", &DeltaPhi1noPhotons, &b_DeltaPhi1noPhotons);
   fChain->SetBranchAddress("DeltaPhi2noPhotons", &DeltaPhi2noPhotons, &b_DeltaPhi2noPhotons);
   fChain->SetBranchAddress("DeltaPhi3noPhotons", &DeltaPhi3noPhotons, &b_DeltaPhi3noPhotons);
   fChain->SetBranchAddress("DeltaPhiN1noPhotons", &DeltaPhiN1noPhotons, &b_DeltaPhiN1noPhotons);
   fChain->SetBranchAddress("DeltaPhiN2noPhotons", &DeltaPhiN2noPhotons, &b_DeltaPhiN2noPhotons);
   fChain->SetBranchAddress("DeltaPhiN3noPhotons", &DeltaPhiN3noPhotons, &b_DeltaPhiN3noPhotons);
   fChain->SetBranchAddress("minDeltaPhiNnoPhotons", &minDeltaPhiNnoPhotons, &b_minDeltaPhiNnoPhotons);
   fChain->SetBranchAddress("METnoPhotonsPt", &METnoPhotonsPt, &b_METnoPhotonsPt);
   fChain->SetBranchAddress("METnoPhotonsPhi", &METnoPhotonsPhi, &b_METnoPhotonsPhi);
   fChain->SetBranchAddress("JetID", &JetID, &b_JetID);
   fChain->SetBranchAddress("genParticles_PDGid", &genParticles_PDGid, &b_genParticles_PDGid);
   fChain->SetBranchAddress("ak1p2Jets_nSubjets", &ak1p2Jets_nSubjets, &b_ak1p2Jets_nSubjets);
   fChain->SetBranchAddress("ak4Jets_chargedHadMult", &ak4Jets_chargedHadMult, &b_ak4Jets_chargedHadMult);
   fChain->SetBranchAddress("ak4Jets_neutralHadMult", &ak4Jets_neutralHadMult, &b_ak4Jets_neutralHadMult);
   fChain->SetBranchAddress("ak4Jets_photonMult", &ak4Jets_photonMult, &b_ak4Jets_photonMult);
   fChain->SetBranchAddress("ak4Jets_flavor", &ak4Jets_flavor, &b_ak4Jets_flavor);
   fChain->SetBranchAddress("ak4JetsRaw_chargedHadMult", &ak4JetsRaw_chargedHadMult, &b_ak4JetsRaw_chargedHadMult);
   fChain->SetBranchAddress("ak4JetsRaw_neutralHadMult", &ak4JetsRaw_neutralHadMult, &b_ak4JetsRaw_neutralHadMult);
   fChain->SetBranchAddress("ak4JetsRaw_photonMult", &ak4JetsRaw_photonMult, &b_ak4JetsRaw_photonMult);
   fChain->SetBranchAddress("ak4JetsRaw_flavor", &ak4JetsRaw_flavor, &b_ak4JetsRaw_flavor);
   fChain->SetBranchAddress("deltaPhiN", &deltaPhiN, &b_deltaPhiN);
   fChain->SetBranchAddress("photon_isEB", &photon_isEB, &b_photon_isEB);
   fChain->SetBranchAddress("photon_genMatched", &photon_genMatched, &b_photon_genMatched);
   fChain->SetBranchAddress("photon_hadTowOverEM", &photon_hadTowOverEM, &b_photon_hadTowOverEM);
   fChain->SetBranchAddress("photon_hasPixelSeed", &photon_hasPixelSeed, &b_photon_hasPixelSeed);
   fChain->SetBranchAddress("photon_pfChargedIso", &photon_pfChargedIso, &b_photon_pfChargedIso);
   fChain->SetBranchAddress("photon_pfChargedIsoRhoCorr", &photon_pfChargedIsoRhoCorr, &b_photon_pfChargedIsoRhoCorr);
   fChain->SetBranchAddress("photon_pfGammaIso", &photon_pfGammaIso, &b_photon_pfGammaIso);
   fChain->SetBranchAddress("photon_pfGammaIsoRhoCorr", &photon_pfGammaIsoRhoCorr, &b_photon_pfGammaIsoRhoCorr);
   fChain->SetBranchAddress("photon_pfNeutralIso", &photon_pfNeutralIso, &b_photon_pfNeutralIso);
   fChain->SetBranchAddress("photon_pfNeutralIsoRhoCorr", &photon_pfNeutralIsoRhoCorr, &b_photon_pfNeutralIsoRhoCorr);
   fChain->SetBranchAddress("photon_sigmaIetaIeta", &photon_sigmaIetaIeta, &b_photon_sigmaIetaIeta);
   fChain->SetBranchAddress("ak1p2Jets_tau1", &ak1p2Jets_tau1, &b_ak1p2Jets_tau1);
   fChain->SetBranchAddress("ak1p2Jets_tau2", &ak1p2Jets_tau2, &b_ak1p2Jets_tau2);
   fChain->SetBranchAddress("ak1p2Jets_tau3", &ak1p2Jets_tau3, &b_ak1p2Jets_tau3);
   fChain->SetBranchAddress("ak1p2Jets_tau4", &ak1p2Jets_tau4, &b_ak1p2Jets_tau4);
   fChain->SetBranchAddress("ak4Jets_CSVdisc", &ak4Jets_CSVdisc, &b_ak4Jets_CSVdisc);
   fChain->SetBranchAddress("ak4Jets_chargeHadEfrac", &ak4Jets_chargeHadEfrac, &b_ak4Jets_chargeHadEfrac);
   fChain->SetBranchAddress("ak4Jets_neutralHadEfrac", &ak4Jets_neutralHadEfrac, &b_ak4Jets_neutralHadEfrac);
   fChain->SetBranchAddress("ak4Jets_photonEfrac", &ak4Jets_photonEfrac, &b_ak4Jets_photonEfrac);
   fChain->SetBranchAddress("ak4JetsRaw_CSVdisc", &ak4JetsRaw_CSVdisc, &b_ak4JetsRaw_CSVdisc);
   fChain->SetBranchAddress("ak4JetsRaw_chargeHadEfrac", &ak4JetsRaw_chargeHadEfrac, &b_ak4JetsRaw_chargeHadEfrac);
   fChain->SetBranchAddress("ak4JetsRaw_neutralHadEfrac", &ak4JetsRaw_neutralHadEfrac, &b_ak4JetsRaw_neutralHadEfrac);
   fChain->SetBranchAddress("ak4JetsRaw_photonEfrac", &ak4JetsRaw_photonEfrac, &b_ak4JetsRaw_photonEfrac);
   fChain->SetBranchAddress("Muons", &Muons, &b_Muons);
   fChain->SetBranchAddress("Electrons", &Electrons, &b_Electrons);
   fChain->SetBranchAddress("genParticles", &genParticles, &b_genParticles);
   fChain->SetBranchAddress("photonProd", &photonProd, &b_photonProd);
   fChain->SetBranchAddress("ak1p2Jets", &ak1p2Jets, &b_ak1p2Jets);
   fChain->SetBranchAddress("ak1p2JetsPt15Reclust", &ak1p2JetsPt15Reclust, &b_ak1p2JetsPt15Reclust);
   fChain->SetBranchAddress("ak1p2JetsPt20Reclust", &ak1p2JetsPt20Reclust, &b_ak1p2JetsPt20Reclust);
   fChain->SetBranchAddress("ak1p2JetsPt30Reclust", &ak1p2JetsPt30Reclust, &b_ak1p2JetsPt30Reclust);
   fChain->SetBranchAddress("ak4Jets", &ak4Jets, &b_ak4Jets);
   fChain->SetBranchAddress("ak4JetsRaw", &ak4JetsRaw, &b_ak4JetsRaw);
   fChain->SetBranchAddress("bestPhoton", &bestPhoton, &b_bestPhoton);
   fChain->SetBranchAddress("ak4JetsNoPhotons", &ak4JetsNoPhotons, &b_ak4JetsNoPhotons);
   fChain->SetBranchAddress("ak4GenJets", &ak4GenJets, &b_ak4GenJets);
   Notify();
}

Bool_t NtupleClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NtupleClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NtupleClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef NtupleClass_cxx
