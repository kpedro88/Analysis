//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Oct  5 11:23:27 2015 by ROOT version 5.34/18
// from TTree PreSelection/PreSelection
// found on file: root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV2/Spring15.SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root
//////////////////////////////////////////////////////////

#ifndef NtupleClass_h
#define NtupleClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>

// Header file for the classes stored in the TTree if any.
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
   vector<TLorentzVector> *bestPhoton;
   Int_t           BTags;
   Int_t           BTagsclean;
   Int_t           CSCTightHaloFilter;
   Double_t        DeltaPhi1;
   Double_t        DeltaPhi1clean;
   Double_t        DeltaPhi2;
   Double_t        DeltaPhi2clean;
   Double_t        DeltaPhi3;
   Double_t        DeltaPhi3clean;
   Double_t        DeltaPhi4;
   Double_t        DeltaPhi4clean;
   Double_t        DeltaPhiN1;
   Double_t        DeltaPhiN2;
   Double_t        DeltaPhiN3;
   Int_t           EcalDeadCellTriggerPrimitiveFilter;
   Int_t           eeBadScFilter;
   vector<int>     *ElectronCharge;
   vector<TLorentzVector> *Electrons;
   vector<double>  *GenDeltaPhi;
   vector<int>     *GenElec_GenElecFromTau;
   vector<double>  *GenElec_MT2Activity;
   vector<double>  *GenElec_RA2Activity;
   vector<TLorentzVector> *GenEls;
   Double_t        genHT;
   vector<TLorentzVector> *GenJets;
   Double_t        GenMETPhi;
   Double_t        GenMETPt;
   Double_t        GenMHT;
   Double_t        GenMHT_Phi;
   vector<double>  *GenMinDeltaPhiEta24;
   vector<double>  *GenMinDeltaPhiEta5;
   vector<int>     *GenMinDeltaPhiIndexEta24;
   vector<int>     *GenMinDeltaPhiIndexEta5;
   vector<int>     *GenMu_GenMuFromTau;
   vector<double>  *GenMu_MT2Activity;
   vector<double>  *GenMu_RA2Activity;
   vector<TLorentzVector> *GenMus;
   vector<TLorentzVector> *genParticles;
   vector<int>     *genParticles_PDGid;
   vector<int>     *GenTau_GenTauHad;
   vector<double>  *GenTau_MT2Activity;
   vector<double>  *GenTau_RA2Activity;
   vector<TLorentzVector> *GenTauNu;
   vector<TLorentzVector> *GenTaus;
   Bool_t          HBHENoiseFilter;
   Double_t        HT;
   Double_t        HTclean;
   Int_t           isoElectronTracks;
   vector<TLorentzVector> *IsolatedElectronTracksVeto;
   vector<double>  *IsolatedElectronTracksVeto_MTW;
   vector<TLorentzVector> *IsolatedMuonTracksVeto;
   vector<double>  *IsolatedMuonTracksVeto_MTW;
   vector<TLorentzVector> *IsolatedPionTracksVeto;
   vector<double>  *IsolatedPionTracksVeto_MTW;
   Int_t           isoMuonTracks;
   Int_t           isoPionTracks;
   Bool_t          JetID;
   vector<TLorentzVector> *Jets;
   vector<double>  *Jets_bDiscriminatorCSV;
   vector<double>  *Jets_bDiscriminatorMVA;
   vector<double>  *Jets_chargedEmEnergyFraction;
   vector<double>  *Jets_chargedHadronEnergyFraction;
   vector<int>     *Jets_chargedHadronMultiplicity;
   vector<int>     *Jets_electronMultiplicity;
   vector<int>     *Jets_flavor;
   vector<double>  *Jets_jetArea;
   vector<double>  *Jets_muonEnergyFraction;
   vector<int>     *Jets_muonMultiplicity;
   vector<double>  *Jets_neutralEmEnergyFraction;
   vector<int>     *Jets_neutralHadronMultiplicity;
   vector<double>  *Jets_photonEnergyFraction;
   vector<int>     *Jets_photonMultiplicity;
   Int_t           Leptons;
   Int_t           METFilters;
   Double_t        METPhi;
   Double_t        METPt;
   Double_t        METPtclean;
   Double_t        MHT;
   Double_t        MHT_Phi;
   Double_t        MHTclean;
   Double_t        minDeltaPhiN;
   vector<string>  *minDeltaPhiNames;
   Double_t        minDeltaPhiNclean;
   vector<int>     *MuonCharge;
   vector<TLorentzVector> *Muons;
   Int_t           nAllVertices;
   vector<TLorentzVector> *NeutrinoLorentzVector;
   vector<int>     *NeutrinoMotherPdg;
   vector<int>     *NeutrinoPdg;
   Int_t           NJets;
   Int_t           NJetsclean;
   Int_t           NumPhotons;
   Int_t           NVtx;
   vector<double>  *photon_genMatched;
   vector<double>  *photon_hadTowOverEM;
   vector<double>  *photon_hasPixelSeed;
   vector<double>  *photon_isEB;
   vector<bool>    *photon_nonPrompt;
   vector<double>  *photon_passElectronVeto;
   vector<double>  *photon_pfChargedIso;
   vector<double>  *photon_pfChargedIsoRhoCorr;
   vector<double>  *photon_pfGammaIso;
   vector<double>  *photon_pfGammaIsoRhoCorr;
   vector<double>  *photon_pfNeutralIso;
   vector<double>  *photon_pfNeutralIsoRhoCorr;
   vector<double>  *photon_sigmaIetaIeta;
   vector<TLorentzVector> *photonCands;
   vector<double>  *RJetDeltaPhi;
   vector<double>  *RJetMinDeltaPhiEta24;
   vector<double>  *RJetMinDeltaPhiEta5;
   vector<int>     *RJetMinDeltaPhiIndexEta24;
   vector<int>     *RJetMinDeltaPhiIndexEta5;
   vector<TLorentzVector> *selectedIDElectrons;
   vector<bool>    *selectedIDElectrons_mediumID;
   vector<double>  *selectedIDElectrons_MiniIso;
   vector<double>  *selectedIDElectrons_MT2Activity;
   vector<double>  *selectedIDElectrons_MTW;
   vector<double>  *selectedIDElectrons_RA2Activity;
   vector<bool>    *selectedIDIsoElectrons_mediumID;
   vector<double>  *selectedIDIsoElectrons_MT2Activity;
   vector<double>  *selectedIDIsoElectrons_MTW;
   vector<double>  *selectedIDIsoElectrons_PTW;
   vector<double>  *selectedIDIsoElectrons_RA2Activity;
   vector<double>  *selectedIDIsoMuons_MT2Activity;
   vector<double>  *selectedIDIsoMuons_MTW;
   vector<double>  *selectedIDIsoMuons_PTW;
   vector<double>  *selectedIDIsoMuons_RA2Activity;
   vector<TLorentzVector> *selectedIDMuons;
   vector<double>  *selectedIDMuons_MiniIso;
   vector<double>  *selectedIDMuons_MT2Activity;
   vector<double>  *selectedIDMuons_MTW;
   vector<double>  *selectedIDMuons_RA2Activity;
   vector<TLorentzVector> *slimJet;
   vector<int>     *slimJet_slimJetID;
   Double_t        SusyLSPMass;
   Double_t        SusyMotherMass;
   Int_t           TagLeptonHighPT;
   vector<TLorentzVector> *TauDecayCands;
   vector<int>     *TauDecayCands_pdgID;
   vector<string>  *TriggerNames;
   vector<bool>    *TriggerPass;
   vector<int>     *TriggerPrescales;
   Double_t        Weight;
   Int_t           ZNum;
   vector<TLorentzVector> *Zp4;

   // List of branches
   TBranch        *b_RunNum;   //!
   TBranch        *b_LumiBlockNum;   //!
   TBranch        *b_EvtNum;   //!
   TBranch        *b_bestPhoton;   //!
   TBranch        *b_BTags;   //!
   TBranch        *b_BTagsclean;   //!
   TBranch        *b_CSCTightHaloFilter;   //!
   TBranch        *b_DeltaPhi1;   //!
   TBranch        *b_DeltaPhi1clean;   //!
   TBranch        *b_DeltaPhi2;   //!
   TBranch        *b_DeltaPhi2clean;   //!
   TBranch        *b_DeltaPhi3;   //!
   TBranch        *b_DeltaPhi3clean;   //!
   TBranch        *b_DeltaPhi4;   //!
   TBranch        *b_DeltaPhi4clean;   //!
   TBranch        *b_DeltaPhiN1;   //!
   TBranch        *b_DeltaPhiN2;   //!
   TBranch        *b_DeltaPhiN3;   //!
   TBranch        *b_EcalDeadCellTriggerPrimitiveFilter;   //!
   TBranch        *b_eeBadScFilter;   //!
   TBranch        *b_ElectronCharge;   //!
   TBranch        *b_Electrons;   //!
   TBranch        *b_GenDeltaPhi;   //!
   TBranch        *b_GenElec_GenElecFromTau;   //!
   TBranch        *b_GenElec_MT2Activity;   //!
   TBranch        *b_GenElec_RA2Activity;   //!
   TBranch        *b_GenEls;   //!
   TBranch        *b_genHT;   //!
   TBranch        *b_GenJets;   //!
   TBranch        *b_GenMETPhi;   //!
   TBranch        *b_GenMETPt;   //!
   TBranch        *b_GenMHT;   //!
   TBranch        *b_GenMHT_Phi;   //!
   TBranch        *b_GenMinDeltaPhiEta24;   //!
   TBranch        *b_GenMinDeltaPhiEta5;   //!
   TBranch        *b_GenMinDeltaPhiIndexEta24;   //!
   TBranch        *b_GenMinDeltaPhiIndexEta5;   //!
   TBranch        *b_GenMu_GenMuFromTau;   //!
   TBranch        *b_GenMu_MT2Activity;   //!
   TBranch        *b_GenMu_RA2Activity;   //!
   TBranch        *b_GenMus;   //!
   TBranch        *b_genParticles;   //!
   TBranch        *b_genParticles_PDGid;   //!
   TBranch        *b_GenTau_GenTauHad;   //!
   TBranch        *b_GenTau_MT2Activity;   //!
   TBranch        *b_GenTau_RA2Activity;   //!
   TBranch        *b_GenTauNu;   //!
   TBranch        *b_GenTaus;   //!
   TBranch        *b_HBHENoiseFilter;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_HTclean;   //!
   TBranch        *b_isoElectronTracks;   //!
   TBranch        *b_IsolatedElectronTracksVeto;   //!
   TBranch        *b_IsolatedElectronTracksVeto_MTW;   //!
   TBranch        *b_IsolatedMuonTracksVeto;   //!
   TBranch        *b_IsolatedMuonTracksVeto_MTW;   //!
   TBranch        *b_IsolatedPionTracksVeto;   //!
   TBranch        *b_IsolatedPionTracksVeto_MTW;   //!
   TBranch        *b_isoMuonTracks;   //!
   TBranch        *b_isoPionTracks;   //!
   TBranch        *b_JetID;   //!
   TBranch        *b_Jets;   //!
   TBranch        *b_Jets_bDiscriminatorCSV;   //!
   TBranch        *b_Jets_bDiscriminatorMVA;   //!
   TBranch        *b_Jets_chargedEmEnergyFraction;   //!
   TBranch        *b_Jets_chargedHadronEnergyFraction;   //!
   TBranch        *b_Jets_chargedHadronMultiplicity;   //!
   TBranch        *b_Jets_electronMultiplicity;   //!
   TBranch        *b_Jets_flavor;   //!
   TBranch        *b_Jets_jetArea;   //!
   TBranch        *b_Jets_muonEnergyFraction;   //!
   TBranch        *b_Jets_muonMultiplicity;   //!
   TBranch        *b_Jets_neutralEmEnergyFraction;   //!
   TBranch        *b_Jets_neutralHadronMultiplicity;   //!
   TBranch        *b_Jets_photonEnergyFraction;   //!
   TBranch        *b_Jets_photonMultiplicity;   //!
   TBranch        *b_Leptons;   //!
   TBranch        *b_METFilters;   //!
   TBranch        *b_METPhi;   //!
   TBranch        *b_METPt;   //!
   TBranch        *b_METPtclean;   //!
   TBranch        *b_MHT;   //!
   TBranch        *b_MHT_Phi;   //!
   TBranch        *b_MHTclean;   //!
   TBranch        *b_minDeltaPhiN;   //!
   TBranch        *b_minDeltaPhiNames;   //!
   TBranch        *b_minDeltaPhiNclean;   //!
   TBranch        *b_MuonCharge;   //!
   TBranch        *b_Muons;   //!
   TBranch        *b_nAllVertices;   //!
   TBranch        *b_NeutrinoLorentzVector;   //!
   TBranch        *b_NeutrinoMotherPdg;   //!
   TBranch        *b_NeutrinoPdg;   //!
   TBranch        *b_NJets;   //!
   TBranch        *b_NJetsclean;   //!
   TBranch        *b_NumPhotons;   //!
   TBranch        *b_NVtx;   //!
   TBranch        *b_photon_genMatched;   //!
   TBranch        *b_photon_hadTowOverEM;   //!
   TBranch        *b_photon_hasPixelSeed;   //!
   TBranch        *b_photon_isEB;   //!
   TBranch        *b_photon_nonPrompt;   //!
   TBranch        *b_photon_passElectronVeto;   //!
   TBranch        *b_photon_pfChargedIso;   //!
   TBranch        *b_photon_pfChargedIsoRhoCorr;   //!
   TBranch        *b_photon_pfGammaIso;   //!
   TBranch        *b_photon_pfGammaIsoRhoCorr;   //!
   TBranch        *b_photon_pfNeutralIso;   //!
   TBranch        *b_photon_pfNeutralIsoRhoCorr;   //!
   TBranch        *b_photon_sigmaIetaIeta;   //!
   TBranch        *b_photonCands;   //!
   TBranch        *b_RJetDeltaPhi;   //!
   TBranch        *b_RJetMinDeltaPhiEta24;   //!
   TBranch        *b_RJetMinDeltaPhiEta5;   //!
   TBranch        *b_RJetMinDeltaPhiIndexEta24;   //!
   TBranch        *b_RJetMinDeltaPhiIndexEta5;   //!
   TBranch        *b_selectedIDElectrons;   //!
   TBranch        *b_selectedIDElectrons_mediumID;   //!
   TBranch        *b_selectedIDElectrons_MiniIso;   //!
   TBranch        *b_selectedIDElectrons_MT2Activity;   //!
   TBranch        *b_selectedIDElectrons_MTW;   //!
   TBranch        *b_selectedIDElectrons_RA2Activity;   //!
   TBranch        *b_selectedIDIsoElectrons_mediumID;   //!
   TBranch        *b_selectedIDIsoElectrons_MT2Activity;   //!
   TBranch        *b_selectedIDIsoElectrons_MTW;   //!
   TBranch        *b_selectedIDIsoElectrons_PTW;   //!
   TBranch        *b_selectedIDIsoElectrons_RA2Activity;   //!
   TBranch        *b_selectedIDIsoMuons_MT2Activity;   //!
   TBranch        *b_selectedIDIsoMuons_MTW;   //!
   TBranch        *b_selectedIDIsoMuons_PTW;   //!
   TBranch        *b_selectedIDIsoMuons_RA2Activity;   //!
   TBranch        *b_selectedIDMuons;   //!
   TBranch        *b_selectedIDMuons_MiniIso;   //!
   TBranch        *b_selectedIDMuons_MT2Activity;   //!
   TBranch        *b_selectedIDMuons_MTW;   //!
   TBranch        *b_selectedIDMuons_RA2Activity;   //!
   TBranch        *b_slimJet;   //!
   TBranch        *b_slimJet_slimJetID;   //!
   TBranch        *b_SusyLSPMass;   //!
   TBranch        *b_SusyMotherMass;   //!
   TBranch        *b_TagLeptonHighPT;   //!
   TBranch        *b_TauDecayCands;   //!
   TBranch        *b_TauDecayCands_pdgID;   //!
   TBranch        *b_TriggerNames;   //!
   TBranch        *b_TriggerPass;   //!
   TBranch        *b_TriggerPrescales;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_ZNum;   //!
   TBranch        *b_Zp4;   //!

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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV2/Spring15.SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV2/Spring15.SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV2/Spring15.SMS-T1tttt_mGluino-1500_mLSP-100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_0_RA2AnalysisTree.root:/TreeMaker2");
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
   bestPhoton = 0;
   ElectronCharge = 0;
   Electrons = 0;
   GenDeltaPhi = 0;
   GenElec_GenElecFromTau = 0;
   GenElec_MT2Activity = 0;
   GenElec_RA2Activity = 0;
   GenEls = 0;
   GenJets = 0;
   GenMinDeltaPhiEta24 = 0;
   GenMinDeltaPhiEta5 = 0;
   GenMinDeltaPhiIndexEta24 = 0;
   GenMinDeltaPhiIndexEta5 = 0;
   GenMu_GenMuFromTau = 0;
   GenMu_MT2Activity = 0;
   GenMu_RA2Activity = 0;
   GenMus = 0;
   genParticles = 0;
   genParticles_PDGid = 0;
   GenTau_GenTauHad = 0;
   GenTau_MT2Activity = 0;
   GenTau_RA2Activity = 0;
   GenTauNu = 0;
   GenTaus = 0;
   IsolatedElectronTracksVeto = 0;
   IsolatedElectronTracksVeto_MTW = 0;
   IsolatedMuonTracksVeto = 0;
   IsolatedMuonTracksVeto_MTW = 0;
   IsolatedPionTracksVeto = 0;
   IsolatedPionTracksVeto_MTW = 0;
   Jets = 0;
   Jets_bDiscriminatorCSV = 0;
   Jets_bDiscriminatorMVA = 0;
   Jets_chargedEmEnergyFraction = 0;
   Jets_chargedHadronEnergyFraction = 0;
   Jets_chargedHadronMultiplicity = 0;
   Jets_electronMultiplicity = 0;
   Jets_flavor = 0;
   Jets_jetArea = 0;
   Jets_muonEnergyFraction = 0;
   Jets_muonMultiplicity = 0;
   Jets_neutralEmEnergyFraction = 0;
   Jets_neutralHadronMultiplicity = 0;
   Jets_photonEnergyFraction = 0;
   Jets_photonMultiplicity = 0;
   minDeltaPhiNames = 0;
   MuonCharge = 0;
   Muons = 0;
   NeutrinoLorentzVector = 0;
   NeutrinoMotherPdg = 0;
   NeutrinoPdg = 0;
   photon_genMatched = 0;
   photon_hadTowOverEM = 0;
   photon_hasPixelSeed = 0;
   photon_isEB = 0;
   photon_nonPrompt = 0;
   photon_passElectronVeto = 0;
   photon_pfChargedIso = 0;
   photon_pfChargedIsoRhoCorr = 0;
   photon_pfGammaIso = 0;
   photon_pfGammaIsoRhoCorr = 0;
   photon_pfNeutralIso = 0;
   photon_pfNeutralIsoRhoCorr = 0;
   photon_sigmaIetaIeta = 0;
   photonCands = 0;
   RJetDeltaPhi = 0;
   RJetMinDeltaPhiEta24 = 0;
   RJetMinDeltaPhiEta5 = 0;
   RJetMinDeltaPhiIndexEta24 = 0;
   RJetMinDeltaPhiIndexEta5 = 0;
   selectedIDElectrons = 0;
   selectedIDElectrons_mediumID = 0;
   selectedIDElectrons_MiniIso = 0;
   selectedIDElectrons_MT2Activity = 0;
   selectedIDElectrons_MTW = 0;
   selectedIDElectrons_RA2Activity = 0;
   selectedIDIsoElectrons_mediumID = 0;
   selectedIDIsoElectrons_MT2Activity = 0;
   selectedIDIsoElectrons_MTW = 0;
   selectedIDIsoElectrons_PTW = 0;
   selectedIDIsoElectrons_RA2Activity = 0;
   selectedIDIsoMuons_MT2Activity = 0;
   selectedIDIsoMuons_MTW = 0;
   selectedIDIsoMuons_PTW = 0;
   selectedIDIsoMuons_RA2Activity = 0;
   selectedIDMuons = 0;
   selectedIDMuons_MiniIso = 0;
   selectedIDMuons_MT2Activity = 0;
   selectedIDMuons_MTW = 0;
   selectedIDMuons_RA2Activity = 0;
   slimJet = 0;
   slimJet_slimJetID = 0;
   TauDecayCands = 0;
   TauDecayCands_pdgID = 0;
   TriggerNames = 0;
   TriggerPass = 0;
   TriggerPrescales = 0;
   Zp4 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("RunNum", &RunNum, &b_RunNum);
   fChain->SetBranchAddress("LumiBlockNum", &LumiBlockNum, &b_LumiBlockNum);
   fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
   fChain->SetBranchAddress("bestPhoton", &bestPhoton, &b_bestPhoton);
   fChain->SetBranchAddress("BTags", &BTags, &b_BTags);
   fChain->SetBranchAddress("BTagsclean", &BTagsclean, &b_BTagsclean);
   fChain->SetBranchAddress("CSCTightHaloFilter", &CSCTightHaloFilter, &b_CSCTightHaloFilter);
   fChain->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
   fChain->SetBranchAddress("DeltaPhi1clean", &DeltaPhi1clean, &b_DeltaPhi1clean);
   fChain->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
   fChain->SetBranchAddress("DeltaPhi2clean", &DeltaPhi2clean, &b_DeltaPhi2clean);
   fChain->SetBranchAddress("DeltaPhi3", &DeltaPhi3, &b_DeltaPhi3);
   fChain->SetBranchAddress("DeltaPhi3clean", &DeltaPhi3clean, &b_DeltaPhi3clean);
   fChain->SetBranchAddress("DeltaPhi4", &DeltaPhi4, &b_DeltaPhi4);
   fChain->SetBranchAddress("DeltaPhi4clean", &DeltaPhi4clean, &b_DeltaPhi4clean);
   fChain->SetBranchAddress("DeltaPhiN1", &DeltaPhiN1, &b_DeltaPhiN1);
   fChain->SetBranchAddress("DeltaPhiN2", &DeltaPhiN2, &b_DeltaPhiN2);
   fChain->SetBranchAddress("DeltaPhiN3", &DeltaPhiN3, &b_DeltaPhiN3);
   fChain->SetBranchAddress("EcalDeadCellTriggerPrimitiveFilter", &EcalDeadCellTriggerPrimitiveFilter, &b_EcalDeadCellTriggerPrimitiveFilter);
   fChain->SetBranchAddress("eeBadScFilter", &eeBadScFilter, &b_eeBadScFilter);
   fChain->SetBranchAddress("ElectronCharge", &ElectronCharge, &b_ElectronCharge);
   fChain->SetBranchAddress("Electrons", &Electrons, &b_Electrons);
   fChain->SetBranchAddress("GenDeltaPhi", &GenDeltaPhi, &b_GenDeltaPhi);
   fChain->SetBranchAddress("GenElec_GenElecFromTau", &GenElec_GenElecFromTau, &b_GenElec_GenElecFromTau);
   fChain->SetBranchAddress("GenElec_MT2Activity", &GenElec_MT2Activity, &b_GenElec_MT2Activity);
   fChain->SetBranchAddress("GenElec_RA2Activity", &GenElec_RA2Activity, &b_GenElec_RA2Activity);
   fChain->SetBranchAddress("GenEls", &GenEls, &b_GenEls);
   fChain->SetBranchAddress("genHT", &genHT, &b_genHT);
   fChain->SetBranchAddress("GenJets", &GenJets, &b_GenJets);
   fChain->SetBranchAddress("GenMETPhi", &GenMETPhi, &b_GenMETPhi);
   fChain->SetBranchAddress("GenMETPt", &GenMETPt, &b_GenMETPt);
   fChain->SetBranchAddress("GenMHT", &GenMHT, &b_GenMHT);
   fChain->SetBranchAddress("GenMHT_Phi", &GenMHT_Phi, &b_GenMHT_Phi);
   fChain->SetBranchAddress("GenMinDeltaPhiEta24", &GenMinDeltaPhiEta24, &b_GenMinDeltaPhiEta24);
   fChain->SetBranchAddress("GenMinDeltaPhiEta5", &GenMinDeltaPhiEta5, &b_GenMinDeltaPhiEta5);
   fChain->SetBranchAddress("GenMinDeltaPhiIndexEta24", &GenMinDeltaPhiIndexEta24, &b_GenMinDeltaPhiIndexEta24);
   fChain->SetBranchAddress("GenMinDeltaPhiIndexEta5", &GenMinDeltaPhiIndexEta5, &b_GenMinDeltaPhiIndexEta5);
   fChain->SetBranchAddress("GenMu_GenMuFromTau", &GenMu_GenMuFromTau, &b_GenMu_GenMuFromTau);
   fChain->SetBranchAddress("GenMu_MT2Activity", &GenMu_MT2Activity, &b_GenMu_MT2Activity);
   fChain->SetBranchAddress("GenMu_RA2Activity", &GenMu_RA2Activity, &b_GenMu_RA2Activity);
   fChain->SetBranchAddress("GenMus", &GenMus, &b_GenMus);
   fChain->SetBranchAddress("genParticles", &genParticles, &b_genParticles);
   fChain->SetBranchAddress("genParticles_PDGid", &genParticles_PDGid, &b_genParticles_PDGid);
   fChain->SetBranchAddress("GenTau_GenTauHad", &GenTau_GenTauHad, &b_GenTau_GenTauHad);
   fChain->SetBranchAddress("GenTau_MT2Activity", &GenTau_MT2Activity, &b_GenTau_MT2Activity);
   fChain->SetBranchAddress("GenTau_RA2Activity", &GenTau_RA2Activity, &b_GenTau_RA2Activity);
   fChain->SetBranchAddress("GenTauNu", &GenTauNu, &b_GenTauNu);
   fChain->SetBranchAddress("GenTaus", &GenTaus, &b_GenTaus);
   fChain->SetBranchAddress("HBHENoiseFilter", &HBHENoiseFilter, &b_HBHENoiseFilter);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("HTclean", &HTclean, &b_HTclean);
   fChain->SetBranchAddress("isoElectronTracks", &isoElectronTracks, &b_isoElectronTracks);
   fChain->SetBranchAddress("IsolatedElectronTracksVeto", &IsolatedElectronTracksVeto, &b_IsolatedElectronTracksVeto);
   fChain->SetBranchAddress("IsolatedElectronTracksVeto_MTW", &IsolatedElectronTracksVeto_MTW, &b_IsolatedElectronTracksVeto_MTW);
   fChain->SetBranchAddress("IsolatedMuonTracksVeto", &IsolatedMuonTracksVeto, &b_IsolatedMuonTracksVeto);
   fChain->SetBranchAddress("IsolatedMuonTracksVeto_MTW", &IsolatedMuonTracksVeto_MTW, &b_IsolatedMuonTracksVeto_MTW);
   fChain->SetBranchAddress("IsolatedPionTracksVeto", &IsolatedPionTracksVeto, &b_IsolatedPionTracksVeto);
   fChain->SetBranchAddress("IsolatedPionTracksVeto_MTW", &IsolatedPionTracksVeto_MTW, &b_IsolatedPionTracksVeto_MTW);
   fChain->SetBranchAddress("isoMuonTracks", &isoMuonTracks, &b_isoMuonTracks);
   fChain->SetBranchAddress("isoPionTracks", &isoPionTracks, &b_isoPionTracks);
   fChain->SetBranchAddress("JetID", &JetID, &b_JetID);
   fChain->SetBranchAddress("Jets", &Jets, &b_Jets);
   fChain->SetBranchAddress("Jets_bDiscriminatorCSV", &Jets_bDiscriminatorCSV, &b_Jets_bDiscriminatorCSV);
   fChain->SetBranchAddress("Jets_bDiscriminatorMVA", &Jets_bDiscriminatorMVA, &b_Jets_bDiscriminatorMVA);
   fChain->SetBranchAddress("Jets_chargedEmEnergyFraction", &Jets_chargedEmEnergyFraction, &b_Jets_chargedEmEnergyFraction);
   fChain->SetBranchAddress("Jets_chargedHadronEnergyFraction", &Jets_chargedHadronEnergyFraction, &b_Jets_chargedHadronEnergyFraction);
   fChain->SetBranchAddress("Jets_chargedHadronMultiplicity", &Jets_chargedHadronMultiplicity, &b_Jets_chargedHadronMultiplicity);
   fChain->SetBranchAddress("Jets_electronMultiplicity", &Jets_electronMultiplicity, &b_Jets_electronMultiplicity);
   fChain->SetBranchAddress("Jets_flavor", &Jets_flavor, &b_Jets_flavor);
   fChain->SetBranchAddress("Jets_jetArea", &Jets_jetArea, &b_Jets_jetArea);
   fChain->SetBranchAddress("Jets_muonEnergyFraction", &Jets_muonEnergyFraction, &b_Jets_muonEnergyFraction);
   fChain->SetBranchAddress("Jets_muonMultiplicity", &Jets_muonMultiplicity, &b_Jets_muonMultiplicity);
   fChain->SetBranchAddress("Jets_neutralEmEnergyFraction", &Jets_neutralEmEnergyFraction, &b_Jets_neutralEmEnergyFraction);
   fChain->SetBranchAddress("Jets_neutralHadronMultiplicity", &Jets_neutralHadronMultiplicity, &b_Jets_neutralHadronMultiplicity);
   fChain->SetBranchAddress("Jets_photonEnergyFraction", &Jets_photonEnergyFraction, &b_Jets_photonEnergyFraction);
   fChain->SetBranchAddress("Jets_photonMultiplicity", &Jets_photonMultiplicity, &b_Jets_photonMultiplicity);
   fChain->SetBranchAddress("Leptons", &Leptons, &b_Leptons);
   fChain->SetBranchAddress("METFilters", &METFilters, &b_METFilters);
   fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
   fChain->SetBranchAddress("METPt", &METPt, &b_METPt);
   fChain->SetBranchAddress("METPtclean", &METPtclean, &b_METPtclean);
   fChain->SetBranchAddress("MHT", &MHT, &b_MHT);
   fChain->SetBranchAddress("MHT_Phi", &MHT_Phi, &b_MHT_Phi);
   fChain->SetBranchAddress("MHTclean", &MHTclean, &b_MHTclean);
   fChain->SetBranchAddress("minDeltaPhiN", &minDeltaPhiN, &b_minDeltaPhiN);
   fChain->SetBranchAddress("minDeltaPhiNames", &minDeltaPhiNames, &b_minDeltaPhiNames);
   fChain->SetBranchAddress("minDeltaPhiNclean", &minDeltaPhiNclean, &b_minDeltaPhiNclean);
   fChain->SetBranchAddress("MuonCharge", &MuonCharge, &b_MuonCharge);
   fChain->SetBranchAddress("Muons", &Muons, &b_Muons);
   fChain->SetBranchAddress("nAllVertices", &nAllVertices, &b_nAllVertices);
   fChain->SetBranchAddress("NeutrinoLorentzVector", &NeutrinoLorentzVector, &b_NeutrinoLorentzVector);
   fChain->SetBranchAddress("NeutrinoMotherPdg", &NeutrinoMotherPdg, &b_NeutrinoMotherPdg);
   fChain->SetBranchAddress("NeutrinoPdg", &NeutrinoPdg, &b_NeutrinoPdg);
   fChain->SetBranchAddress("NJets", &NJets, &b_NJets);
   fChain->SetBranchAddress("NJetsclean", &NJetsclean, &b_NJetsclean);
   fChain->SetBranchAddress("NumPhotons", &NumPhotons, &b_NumPhotons);
   fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
   fChain->SetBranchAddress("photon_genMatched", &photon_genMatched, &b_photon_genMatched);
   fChain->SetBranchAddress("photon_hadTowOverEM", &photon_hadTowOverEM, &b_photon_hadTowOverEM);
   fChain->SetBranchAddress("photon_hasPixelSeed", &photon_hasPixelSeed, &b_photon_hasPixelSeed);
   fChain->SetBranchAddress("photon_isEB", &photon_isEB, &b_photon_isEB);
   fChain->SetBranchAddress("photon_nonPrompt", &photon_nonPrompt, &b_photon_nonPrompt);
   fChain->SetBranchAddress("photon_passElectronVeto", &photon_passElectronVeto, &b_photon_passElectronVeto);
   fChain->SetBranchAddress("photon_pfChargedIso", &photon_pfChargedIso, &b_photon_pfChargedIso);
   fChain->SetBranchAddress("photon_pfChargedIsoRhoCorr", &photon_pfChargedIsoRhoCorr, &b_photon_pfChargedIsoRhoCorr);
   fChain->SetBranchAddress("photon_pfGammaIso", &photon_pfGammaIso, &b_photon_pfGammaIso);
   fChain->SetBranchAddress("photon_pfGammaIsoRhoCorr", &photon_pfGammaIsoRhoCorr, &b_photon_pfGammaIsoRhoCorr);
   fChain->SetBranchAddress("photon_pfNeutralIso", &photon_pfNeutralIso, &b_photon_pfNeutralIso);
   fChain->SetBranchAddress("photon_pfNeutralIsoRhoCorr", &photon_pfNeutralIsoRhoCorr, &b_photon_pfNeutralIsoRhoCorr);
   fChain->SetBranchAddress("photon_sigmaIetaIeta", &photon_sigmaIetaIeta, &b_photon_sigmaIetaIeta);
   fChain->SetBranchAddress("photonCands", &photonCands, &b_photonCands);
   fChain->SetBranchAddress("RJetDeltaPhi", &RJetDeltaPhi, &b_RJetDeltaPhi);
   fChain->SetBranchAddress("RJetMinDeltaPhiEta24", &RJetMinDeltaPhiEta24, &b_RJetMinDeltaPhiEta24);
   fChain->SetBranchAddress("RJetMinDeltaPhiEta5", &RJetMinDeltaPhiEta5, &b_RJetMinDeltaPhiEta5);
   fChain->SetBranchAddress("RJetMinDeltaPhiIndexEta24", &RJetMinDeltaPhiIndexEta24, &b_RJetMinDeltaPhiIndexEta24);
   fChain->SetBranchAddress("RJetMinDeltaPhiIndexEta5", &RJetMinDeltaPhiIndexEta5, &b_RJetMinDeltaPhiIndexEta5);
   fChain->SetBranchAddress("selectedIDElectrons", &selectedIDElectrons, &b_selectedIDElectrons);
   fChain->SetBranchAddress("selectedIDElectrons_mediumID", &selectedIDElectrons_mediumID, &b_selectedIDElectrons_mediumID);
   fChain->SetBranchAddress("selectedIDElectrons_MiniIso", &selectedIDElectrons_MiniIso, &b_selectedIDElectrons_MiniIso);
   fChain->SetBranchAddress("selectedIDElectrons_MT2Activity", &selectedIDElectrons_MT2Activity, &b_selectedIDElectrons_MT2Activity);
   fChain->SetBranchAddress("selectedIDElectrons_MTW", &selectedIDElectrons_MTW, &b_selectedIDElectrons_MTW);
   fChain->SetBranchAddress("selectedIDElectrons_RA2Activity", &selectedIDElectrons_RA2Activity, &b_selectedIDElectrons_RA2Activity);
   fChain->SetBranchAddress("selectedIDIsoElectrons_mediumID", &selectedIDIsoElectrons_mediumID, &b_selectedIDIsoElectrons_mediumID);
   fChain->SetBranchAddress("selectedIDIsoElectrons_MT2Activity", &selectedIDIsoElectrons_MT2Activity, &b_selectedIDIsoElectrons_MT2Activity);
   fChain->SetBranchAddress("selectedIDIsoElectrons_MTW", &selectedIDIsoElectrons_MTW, &b_selectedIDIsoElectrons_MTW);
   fChain->SetBranchAddress("selectedIDIsoElectrons_PTW", &selectedIDIsoElectrons_PTW, &b_selectedIDIsoElectrons_PTW);
   fChain->SetBranchAddress("selectedIDIsoElectrons_RA2Activity", &selectedIDIsoElectrons_RA2Activity, &b_selectedIDIsoElectrons_RA2Activity);
   fChain->SetBranchAddress("selectedIDIsoMuons_MT2Activity", &selectedIDIsoMuons_MT2Activity, &b_selectedIDIsoMuons_MT2Activity);
   fChain->SetBranchAddress("selectedIDIsoMuons_MTW", &selectedIDIsoMuons_MTW, &b_selectedIDIsoMuons_MTW);
   fChain->SetBranchAddress("selectedIDIsoMuons_PTW", &selectedIDIsoMuons_PTW, &b_selectedIDIsoMuons_PTW);
   fChain->SetBranchAddress("selectedIDIsoMuons_RA2Activity", &selectedIDIsoMuons_RA2Activity, &b_selectedIDIsoMuons_RA2Activity);
   fChain->SetBranchAddress("selectedIDMuons", &selectedIDMuons, &b_selectedIDMuons);
   fChain->SetBranchAddress("selectedIDMuons_MiniIso", &selectedIDMuons_MiniIso, &b_selectedIDMuons_MiniIso);
   fChain->SetBranchAddress("selectedIDMuons_MT2Activity", &selectedIDMuons_MT2Activity, &b_selectedIDMuons_MT2Activity);
   fChain->SetBranchAddress("selectedIDMuons_MTW", &selectedIDMuons_MTW, &b_selectedIDMuons_MTW);
   fChain->SetBranchAddress("selectedIDMuons_RA2Activity", &selectedIDMuons_RA2Activity, &b_selectedIDMuons_RA2Activity);
   fChain->SetBranchAddress("slimJet", &slimJet, &b_slimJet);
   fChain->SetBranchAddress("slimJet_slimJetID", &slimJet_slimJetID, &b_slimJet_slimJetID);
   fChain->SetBranchAddress("SusyLSPMass", &SusyLSPMass, &b_SusyLSPMass);
   fChain->SetBranchAddress("SusyMotherMass", &SusyMotherMass, &b_SusyMotherMass);
   fChain->SetBranchAddress("TagLeptonHighPT", &TagLeptonHighPT, &b_TagLeptonHighPT);
   fChain->SetBranchAddress("TauDecayCands", &TauDecayCands, &b_TauDecayCands);
   fChain->SetBranchAddress("TauDecayCands_pdgID", &TauDecayCands_pdgID, &b_TauDecayCands_pdgID);
   fChain->SetBranchAddress("TriggerNames", &TriggerNames, &b_TriggerNames);
   fChain->SetBranchAddress("TriggerPass", &TriggerPass, &b_TriggerPass);
   fChain->SetBranchAddress("TriggerPrescales", &TriggerPrescales, &b_TriggerPrescales);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("ZNum", &ZNum, &b_ZNum);
   fChain->SetBranchAddress("Zp4", &Zp4, &b_Zp4);
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
