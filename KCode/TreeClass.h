//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jun  3 22:30:24 2015 by ROOT version 5.34/18
// from TTree tree/all observables, signal
// found on file: root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/tree_signal/tree_T1tttt_1500_100.root
//////////////////////////////////////////////////////////

#ifndef TreeClass_h
#define TreeClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class TreeClass {
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
   Int_t           NJetsclean;
   Int_t           BTagsclean;
   Int_t           NumPhotons;
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
   Float_t         HTclean;
   Float_t         MHTclean;
   Float_t         minDeltaPhiNclean;
   Float_t         METPtclean;
   Float_t         rho;
   Float_t         ak1p2Jets_sumJetMass;
   Float_t         ak1p2JetsPt15Reclust_sumJetMass;
   Float_t         ak1p2JetsPt20Reclust_sumJetMass;
   Float_t         ak1p2JetsPt30Reclust_sumJetMass;
   UChar_t         JetID;
   vector<int>     *MuonCharge;
   vector<int>     *ElectronCharge;
   vector<int>     *genParticles_PDGid;
   vector<int>     *ak1p2Jets_nSubjets;
   vector<int>     *ak4Jets_chargedHadMult;
   vector<int>     *ak4Jets_neutralHadMult;
   vector<int>     *ak4Jets_photonMult;
   vector<int>     *ak4Jets_flavor;
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
   vector<double>  *ak4Jets_MVAdisc;
   vector<double>  *ak4Jets_chargeHadEfrac;
   vector<double>  *ak4Jets_neutralHadEfrac;
   vector<double>  *ak4Jets_photonEfrac;
   vector<TLorentzVector> *Muons;
   vector<TLorentzVector> *Electrons;
   vector<TLorentzVector> *genParticles;
   vector<TLorentzVector> *photonCands;
   vector<TLorentzVector> *bestPhoton;
   vector<TLorentzVector> *ak1p2Jets;
   vector<TLorentzVector> *ak1p2JetsPt15Reclust;
   vector<TLorentzVector> *ak1p2JetsPt20Reclust;
   vector<TLorentzVector> *ak1p2JetsPt30Reclust;
   vector<TLorentzVector> *ak4Jets;
   vector<TLorentzVector> *ak4JetsRaw;
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
   TBranch        *b_NJetsclean;   //!
   TBranch        *b_BTagsclean;   //!
   TBranch        *b_NumPhotons;   //!
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
   TBranch        *b_HTclean;   //!
   TBranch        *b_MHTclean;   //!
   TBranch        *b_minDeltaPhiNclean;   //!
   TBranch        *b_METPtclean;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_ak1p2Jets_sumJetMass;   //!
   TBranch        *b_ak1p2JetsPt15Reclust_sumJetMass;   //!
   TBranch        *b_ak1p2JetsPt20Reclust_sumJetMass;   //!
   TBranch        *b_ak1p2JetsPt30Reclust_sumJetMass;   //!
   TBranch        *b_JetID;   //!
   TBranch        *b_MuonCharge;   //!
   TBranch        *b_ElectronCharge;   //!
   TBranch        *b_genParticles_PDGid;   //!
   TBranch        *b_ak1p2Jets_nSubjets;   //!
   TBranch        *b_ak4Jets_chargedHadMult;   //!
   TBranch        *b_ak4Jets_neutralHadMult;   //!
   TBranch        *b_ak4Jets_photonMult;   //!
   TBranch        *b_ak4Jets_flavor;   //!
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
   TBranch        *b_ak4Jets_MVAdisc;   //!
   TBranch        *b_ak4Jets_chargeHadEfrac;   //!
   TBranch        *b_ak4Jets_neutralHadEfrac;   //!
   TBranch        *b_ak4Jets_photonEfrac;   //!
   TBranch        *b_Muons;   //!
   TBranch        *b_Electrons;   //!
   TBranch        *b_genParticles;   //!
   TBranch        *b_photonCands;   //!
   TBranch        *b_bestPhoton;   //!
   TBranch        *b_ak1p2Jets;   //!
   TBranch        *b_ak1p2JetsPt15Reclust;   //!
   TBranch        *b_ak1p2JetsPt20Reclust;   //!
   TBranch        *b_ak1p2JetsPt30Reclust;   //!
   TBranch        *b_ak4Jets;   //!
   TBranch        *b_ak4JetsRaw;   //!
   TBranch        *b_ak4GenJets;   //!

   TreeClass(TTree *tree=0);
   virtual ~TreeClass();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TreeClass_cxx
TreeClass::TreeClass(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/tree_signal/tree_T1tttt_1500_100.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/tree_signal/tree_T1tttt_1500_100.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

TreeClass::~TreeClass()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t TreeClass::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TreeClass::LoadTree(Long64_t entry)
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

void TreeClass::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   MuonCharge = 0;
   ElectronCharge = 0;
   genParticles_PDGid = 0;
   ak1p2Jets_nSubjets = 0;
   ak4Jets_chargedHadMult = 0;
   ak4Jets_neutralHadMult = 0;
   ak4Jets_photonMult = 0;
   ak4Jets_flavor = 0;
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
   ak4Jets_MVAdisc = 0;
   ak4Jets_chargeHadEfrac = 0;
   ak4Jets_neutralHadEfrac = 0;
   ak4Jets_photonEfrac = 0;
   Muons = 0;
   Electrons = 0;
   genParticles = 0;
   photonCands = 0;
   bestPhoton = 0;
   ak1p2Jets = 0;
   ak1p2JetsPt15Reclust = 0;
   ak1p2JetsPt20Reclust = 0;
   ak1p2JetsPt30Reclust = 0;
   ak4Jets = 0;
   ak4JetsRaw = 0;
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
   fChain->SetBranchAddress("NJetsclean", &NJetsclean, &b_NJetsclean);
   fChain->SetBranchAddress("BTagsclean", &BTagsclean, &b_BTagsclean);
   fChain->SetBranchAddress("NumPhotons", &NumPhotons, &b_NumPhotons);
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
   fChain->SetBranchAddress("HTclean", &HTclean, &b_HTclean);
   fChain->SetBranchAddress("MHTclean", &MHTclean, &b_MHTclean);
   fChain->SetBranchAddress("minDeltaPhiNclean", &minDeltaPhiNclean, &b_minDeltaPhiNclean);
   fChain->SetBranchAddress("METPtclean", &METPtclean, &b_METPtclean);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("ak1p2Jets_sumJetMass", &ak1p2Jets_sumJetMass, &b_ak1p2Jets_sumJetMass);
   fChain->SetBranchAddress("ak1p2JetsPt15Reclust_sumJetMass", &ak1p2JetsPt15Reclust_sumJetMass, &b_ak1p2JetsPt15Reclust_sumJetMass);
   fChain->SetBranchAddress("ak1p2JetsPt20Reclust_sumJetMass", &ak1p2JetsPt20Reclust_sumJetMass, &b_ak1p2JetsPt20Reclust_sumJetMass);
   fChain->SetBranchAddress("ak1p2JetsPt30Reclust_sumJetMass", &ak1p2JetsPt30Reclust_sumJetMass, &b_ak1p2JetsPt30Reclust_sumJetMass);
   fChain->SetBranchAddress("JetID", &JetID, &b_JetID);
   fChain->SetBranchAddress("MuonCharge", &MuonCharge, &b_MuonCharge);
   fChain->SetBranchAddress("ElectronCharge", &ElectronCharge, &b_ElectronCharge);
   fChain->SetBranchAddress("genParticles_PDGid", &genParticles_PDGid, &b_genParticles_PDGid);
   fChain->SetBranchAddress("ak1p2Jets_nSubjets", &ak1p2Jets_nSubjets, &b_ak1p2Jets_nSubjets);
   fChain->SetBranchAddress("ak4Jets_chargedHadMult", &ak4Jets_chargedHadMult, &b_ak4Jets_chargedHadMult);
   fChain->SetBranchAddress("ak4Jets_neutralHadMult", &ak4Jets_neutralHadMult, &b_ak4Jets_neutralHadMult);
   fChain->SetBranchAddress("ak4Jets_photonMult", &ak4Jets_photonMult, &b_ak4Jets_photonMult);
   fChain->SetBranchAddress("ak4Jets_flavor", &ak4Jets_flavor, &b_ak4Jets_flavor);
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
   fChain->SetBranchAddress("ak4Jets_MVAdisc", &ak4Jets_MVAdisc, &b_ak4Jets_MVAdisc);
   fChain->SetBranchAddress("ak4Jets_chargeHadEfrac", &ak4Jets_chargeHadEfrac, &b_ak4Jets_chargeHadEfrac);
   fChain->SetBranchAddress("ak4Jets_neutralHadEfrac", &ak4Jets_neutralHadEfrac, &b_ak4Jets_neutralHadEfrac);
   fChain->SetBranchAddress("ak4Jets_photonEfrac", &ak4Jets_photonEfrac, &b_ak4Jets_photonEfrac);
   fChain->SetBranchAddress("Muons", &Muons, &b_Muons);
   fChain->SetBranchAddress("Electrons", &Electrons, &b_Electrons);
   fChain->SetBranchAddress("genParticles", &genParticles, &b_genParticles);
   fChain->SetBranchAddress("photonCands", &photonCands, &b_photonCands);
   fChain->SetBranchAddress("bestPhoton", &bestPhoton, &b_bestPhoton);
   fChain->SetBranchAddress("ak1p2Jets", &ak1p2Jets, &b_ak1p2Jets);
   fChain->SetBranchAddress("ak1p2JetsPt15Reclust", &ak1p2JetsPt15Reclust, &b_ak1p2JetsPt15Reclust);
   fChain->SetBranchAddress("ak1p2JetsPt20Reclust", &ak1p2JetsPt20Reclust, &b_ak1p2JetsPt20Reclust);
   fChain->SetBranchAddress("ak1p2JetsPt30Reclust", &ak1p2JetsPt30Reclust, &b_ak1p2JetsPt30Reclust);
   fChain->SetBranchAddress("ak4Jets", &ak4Jets, &b_ak4Jets);
   fChain->SetBranchAddress("ak4JetsRaw", &ak4JetsRaw, &b_ak4JetsRaw);
   fChain->SetBranchAddress("ak4GenJets", &ak4GenJets, &b_ak4GenJets);
   Notify();
}

Bool_t TreeClass::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TreeClass::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TreeClass::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TreeClass_cxx
