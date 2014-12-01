//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Oct 14 19:37:14 2013 by ROOT version 5.32/00
// from TTree tree/selected observables for mu+tau_h+b+b, main
// found on file: tree_rpvstop_LQD333_Stop300_chargino.root
//////////////////////////////////////////////////////////

#ifndef TreeClass_h
#define TreeClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class TreeClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        MuonEnergy;
   Double_t        MuonEta;
   Double_t        MuonPhi;
   Double_t        MuonPt;
   Double_t        MuonPFRelIsoTight;
   Int_t           MuonMultiplicity;
   Int_t           MuonCharge;
   Int_t           MuonVtxIndex;
   Double_t        SecondMuonEnergy;
   Double_t        SecondMuonEta;
   Double_t        SecondMuonPhi;
   Double_t        SecondMuonPt;
   Double_t        SecondMuonPFRelIsoTight;
   Int_t           SecondMuonMultiplicity;
   Int_t           SecondMuonCharge;
   vector<double>  *HPSTauEt;
   vector<double>  *HPSTauEta;
   vector<double>  *HPSTauPhi;
   vector<double>  *HPSTauPt;
   Int_t           HPSTauMultiplicity;
   vector<int>     *HPSTauCharge;
   vector<int>     *HPSTauDecayMode;
   vector<bool>    *HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits;
   vector<bool>    *HPSTauFakeJet;
   vector<bool>    *HPSTauFakeLep;
   vector<bool>    *HPSTauReal;
   vector<bool>    *HPSTauFakeBJet;
   Double_t        PFMET;
   Double_t        PFMETPatType1;
   Double_t        PFMETPhi;
   Double_t        PFMETPhiPatType1;
   vector<double>  *PFJetEnergy;
   vector<double>  *PFJetPt;
   vector<double>  *PFJetEta;
   vector<double>  *PFJetPhi;
   vector<double>  *PFJetCSVBTag;
   vector<double>  *PFJetPartonFlavour;
   vector<int>     *PFJetVtxIndex;
   Int_t           PFJetMultiplicity;
   Double_t        PFMainJetPt[2];
   Double_t        PFMainJetEnergy[2];
   Double_t        PFMainJetEta[2];
   Double_t        PFMainJetPhi[2];
   Double_t        PFMainJetCSVBTag[2];
   Double_t        PFMainJetPartonFlavour[2];
   Int_t           nGoodVertices;
   Int_t           run;
   Int_t           event;
   Int_t           ls;
   Double_t        trueNInteraction;
   Int_t           CharginoMultiplicity;
   Double_t        MassMuonMuon;
   Double_t        MassMuonTau;
   Double_t        PtMuonTau;
   Int_t           IndexTauJet;
   Double_t        MassTauJet;
   Double_t        PtTauJet;
   Int_t           IndexMuonJet;
   Double_t        MassMuonJet;
   Double_t        PtMuonJet;

   // List of branches
   TBranch        *b_s_MuonEnergy;   //!
   TBranch        *b_s_MuonEta;   //!
   TBranch        *b_s_MuonPhi;   //!
   TBranch        *b_s_MuonPt;   //!
   TBranch        *b_s_MuonPFRelIsoTight;   //!
   TBranch        *b_s_MuonMultiplicity;   //!
   TBranch        *b_s_MuonCharge;   //!
   TBranch        *b_s_MuonVtxIndex;   //!
   TBranch        *b_s_SecondMuonEnergy;   //!
   TBranch        *b_s_SecondMuonEta;   //!
   TBranch        *b_s_SecondMuonPhi;   //!
   TBranch        *b_s_SecondMuonPt;   //!
   TBranch        *b_s_SecondMuonPFRelIsoTight;   //!
   TBranch        *b_s_SecondMuonMultiplicity;   //!
   TBranch        *b_s_SecondMuonCharge;   //!
   TBranch        *b_HPSTauEt;   //!
   TBranch        *b_HPSTauEta;   //!
   TBranch        *b_HPSTauPhi;   //!
   TBranch        *b_HPSTauPt;   //!
   TBranch        *b_s_HPSTauMultiplicity;   //!
   TBranch        *b_HPSTauCharge;   //!
   TBranch        *b_HPSTauDecayMode;   //!
   TBranch        *b_HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits;   //!
   TBranch        *b_HPSTauFakeJet;   //!
   TBranch        *b_HPSTauFakeLep;   //!
   TBranch        *b_HPSTauReal;   //!
   TBranch        *b_HPSTauFakeBJet;   //!
   TBranch        *b_s_PFMET;   //!
   TBranch        *b_s_PFMETPatType1;   //!
   TBranch        *b_s_PFMETPhi;   //!
   TBranch        *b_s_PFMETPhiPatType1;   //!
   TBranch        *b_PFJetEnergy;   //!
   TBranch        *b_PFJetPt;   //!
   TBranch        *b_PFJetEta;   //!
   TBranch        *b_PFJetPhi;   //!
   TBranch        *b_PFJetCSVBTag;   //!
   TBranch        *b_PFJetPartonFlavour;   //!
   TBranch        *b_PFJetVtxIndex;   //!
   TBranch        *b_s_PFJetMultiplicity;   //!
   TBranch        *b_s_PFMainJetPt;   //!
   TBranch        *b_s_PFMainJetEnergy;   //!
   TBranch        *b_s_PFMainJetEta;   //!
   TBranch        *b_s_PFMainJetPhi;   //!
   TBranch        *b_s_PFMainJetCSVBTag;   //!
   TBranch        *b_s_PFMainJetPartonFlavour;   //!
   TBranch        *b_s_nGoodVertices;   //!
   TBranch        *b_s_run;   //!
   TBranch        *b_s_event;   //!
   TBranch        *b_s_ls;   //!
   TBranch        *b_s_trueNInteraction;   //!
   TBranch        *b_s_CharginoMultiplicity;   //!
   TBranch        *b_s_MassMuonMuon;   //!
   TBranch        *b_s_MassMuonTau;   //!
   TBranch        *b_s_PtMuonTau;   //!
   TBranch        *b_s_IndexTauJet;   //!
   TBranch        *b_s_MassTauJet;   //!
   TBranch        *b_s_PtTauJet;   //!
   TBranch        *b_s_IndexMuonJet;   //!
   TBranch        *b_s_MassMuonJet;   //!
   TBranch        *b_s_PtMuonJet;   //!

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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tree_rpvstop_LQD333_Stop300_chargino.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("tree_rpvstop_LQD333_Stop300_chargino.root");
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
   HPSTauEt = 0;
   HPSTauEta = 0;
   HPSTauPhi = 0;
   HPSTauPt = 0;
   HPSTauCharge = 0;
   HPSTauDecayMode = 0;
   HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits = 0;
   HPSTauFakeJet = 0;
   HPSTauFakeLep = 0;
   HPSTauReal = 0;
   HPSTauFakeBJet = 0;
   PFJetEnergy = 0;
   PFJetPt = 0;
   PFJetEta = 0;
   PFJetPhi = 0;
   PFJetCSVBTag = 0;
   PFJetPartonFlavour = 0;
   PFJetVtxIndex = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("MuonEnergy", &MuonEnergy, &b_s_MuonEnergy);
   fChain->SetBranchAddress("MuonEta", &MuonEta, &b_s_MuonEta);
   fChain->SetBranchAddress("MuonPhi", &MuonPhi, &b_s_MuonPhi);
   fChain->SetBranchAddress("MuonPt", &MuonPt, &b_s_MuonPt);
   fChain->SetBranchAddress("MuonPFRelIsoTight", &MuonPFRelIsoTight, &b_s_MuonPFRelIsoTight);
   fChain->SetBranchAddress("MuonMultiplicity", &MuonMultiplicity, &b_s_MuonMultiplicity);
   fChain->SetBranchAddress("MuonCharge", &MuonCharge, &b_s_MuonCharge);
   fChain->SetBranchAddress("MuonVtxIndex", &MuonVtxIndex, &b_s_MuonVtxIndex);
   fChain->SetBranchAddress("SecondMuonEnergy", &SecondMuonEnergy, &b_s_SecondMuonEnergy);
   fChain->SetBranchAddress("SecondMuonEta", &SecondMuonEta, &b_s_SecondMuonEta);
   fChain->SetBranchAddress("SecondMuonPhi", &SecondMuonPhi, &b_s_SecondMuonPhi);
   fChain->SetBranchAddress("SecondMuonPt", &SecondMuonPt, &b_s_SecondMuonPt);
   fChain->SetBranchAddress("SecondMuonPFRelIsoTight", &SecondMuonPFRelIsoTight, &b_s_SecondMuonPFRelIsoTight);
   fChain->SetBranchAddress("SecondMuonMultiplicity", &SecondMuonMultiplicity, &b_s_SecondMuonMultiplicity);
   fChain->SetBranchAddress("SecondMuonCharge", &SecondMuonCharge, &b_s_SecondMuonCharge);
   fChain->SetBranchAddress("HPSTauEt", &HPSTauEt, &b_HPSTauEt);
   fChain->SetBranchAddress("HPSTauEta", &HPSTauEta, &b_HPSTauEta);
   fChain->SetBranchAddress("HPSTauPhi", &HPSTauPhi, &b_HPSTauPhi);
   fChain->SetBranchAddress("HPSTauPt", &HPSTauPt, &b_HPSTauPt);
   fChain->SetBranchAddress("HPSTauMultiplicity", &HPSTauMultiplicity, &b_s_HPSTauMultiplicity);
   fChain->SetBranchAddress("HPSTauCharge", &HPSTauCharge, &b_HPSTauCharge);
   fChain->SetBranchAddress("HPSTauDecayMode", &HPSTauDecayMode, &b_HPSTauDecayMode);
   fChain->SetBranchAddress("HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits", &HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits, &b_HPSTaubyLooseCombinedIsolationDeltaBetaCorr3Hits);
   fChain->SetBranchAddress("HPSTauFakeJet", &HPSTauFakeJet, &b_HPSTauFakeJet);
   fChain->SetBranchAddress("HPSTauFakeLep", &HPSTauFakeLep, &b_HPSTauFakeLep);
   fChain->SetBranchAddress("HPSTauReal", &HPSTauReal, &b_HPSTauReal);
   fChain->SetBranchAddress("HPSTauFakeBJet", &HPSTauFakeBJet, &b_HPSTauFakeBJet);
   fChain->SetBranchAddress("PFMET", &PFMET, &b_s_PFMET);
   fChain->SetBranchAddress("PFMETPatType1", &PFMETPatType1, &b_s_PFMETPatType1);
   fChain->SetBranchAddress("PFMETPhi", &PFMETPhi, &b_s_PFMETPhi);
   fChain->SetBranchAddress("PFMETPhiPatType1", &PFMETPhiPatType1, &b_s_PFMETPhiPatType1);
   fChain->SetBranchAddress("PFJetEnergy", &PFJetEnergy, &b_PFJetEnergy);
   fChain->SetBranchAddress("PFJetPt", &PFJetPt, &b_PFJetPt);
   fChain->SetBranchAddress("PFJetEta", &PFJetEta, &b_PFJetEta);
   fChain->SetBranchAddress("PFJetPhi", &PFJetPhi, &b_PFJetPhi);
   fChain->SetBranchAddress("PFJetCSVBTag", &PFJetCSVBTag, &b_PFJetCSVBTag);
   fChain->SetBranchAddress("PFJetPartonFlavour", &PFJetPartonFlavour, &b_PFJetPartonFlavour);
   fChain->SetBranchAddress("PFJetVtxIndex", &PFJetVtxIndex, &b_PFJetVtxIndex);
   fChain->SetBranchAddress("PFJetMultiplicity", &PFJetMultiplicity, &b_s_PFJetMultiplicity);
   fChain->SetBranchAddress("PFMainJetPt", PFMainJetPt, &b_s_PFMainJetPt);
   fChain->SetBranchAddress("PFMainJetEnergy", PFMainJetEnergy, &b_s_PFMainJetEnergy);
   fChain->SetBranchAddress("PFMainJetEta", PFMainJetEta, &b_s_PFMainJetEta);
   fChain->SetBranchAddress("PFMainJetPhi", PFMainJetPhi, &b_s_PFMainJetPhi);
   fChain->SetBranchAddress("PFMainJetCSVBTag", PFMainJetCSVBTag, &b_s_PFMainJetCSVBTag);
   fChain->SetBranchAddress("PFMainJetPartonFlavour", PFMainJetPartonFlavour, &b_s_PFMainJetPartonFlavour);
   fChain->SetBranchAddress("nGoodVertices", &nGoodVertices, &b_s_nGoodVertices);
   fChain->SetBranchAddress("run", &run, &b_s_run);
   fChain->SetBranchAddress("event", &event, &b_s_event);
   fChain->SetBranchAddress("ls", &ls, &b_s_ls);
   fChain->SetBranchAddress("trueNInteraction", &trueNInteraction, &b_s_trueNInteraction);
   fChain->SetBranchAddress("CharginoMultiplicity", &CharginoMultiplicity, &b_s_CharginoMultiplicity);
   fChain->SetBranchAddress("MassMuonMuon", &MassMuonMuon, &b_s_MassMuonMuon);
   fChain->SetBranchAddress("MassMuonTau", &MassMuonTau, &b_s_MassMuonTau);
   fChain->SetBranchAddress("PtMuonTau", &PtMuonTau, &b_s_PtMuonTau);
   fChain->SetBranchAddress("IndexTauJet", &IndexTauJet, &b_s_IndexTauJet);
   fChain->SetBranchAddress("MassTauJet", &MassTauJet, &b_s_MassTauJet);
   fChain->SetBranchAddress("PtTauJet", &PtTauJet, &b_s_PtTauJet);
   fChain->SetBranchAddress("IndexMuonJet", &IndexMuonJet, &b_s_IndexMuonJet);
   fChain->SetBranchAddress("MassMuonJet", &MassMuonJet, &b_s_MassMuonJet);
   fChain->SetBranchAddress("PtMuonJet", &PtMuonJet, &b_s_PtMuonJet);
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
