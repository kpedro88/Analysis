//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul  6 16:24:11 2017 by ROOT version 6.06/01
// from TTree tree/tree
// found on file: root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/GenProductionV1/genmassanalysis_mZprime-3000_mDark-20_rinv-0.3_alpha-0.2_n-10000_part-1.root
//////////////////////////////////////////////////////////

#ifndef NtupleClass_h
#define NtupleClass_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>
#include <vector>
#include <string>

class NtupleClass {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        DeltaPhi1;
   Double_t        DeltaPhi2;
   Double_t        DeltaPhiMin;
   vector<TLorentzVector> *GenJetsAK8;
   vector<double>  *GenJetsAK8_AxisAverage;
   vector<double>  *GenJetsAK8_AxisMajor;
   vector<double>  *GenJetsAK8_AxisMinor;
   vector<double>  *GenJetsAK8_ECF1;
   vector<double>  *GenJetsAK8_ECF2;
   vector<double>  *GenJetsAK8_ECF3;
   vector<double>  *GenJetsAK8_MomentGirth;
   vector<double>  *GenJetsAK8_MomentHalf;
   vector<int>     *GenJetsAK8_Multiplicity;
   vector<double>  *GenJetsAK8_Overflow;
   vector<double>  *GenJetsAK8_PtD;
   vector<double>  *GenJetsAK8_Tau1;
   vector<double>  *GenJetsAK8_Tau2;
   vector<double>  *GenJetsAK8_Tau3;
   vector<TLorentzVector> *HVMesons;
   Double_t        MAOS;
   vector<TLorentzVector> *METSystems;
   Double_t        MET;
   Double_t        METPhi;
   Double_t        MJJ;
   Double_t        Mmc;
   Double_t        MT;
   Double_t        MT2;

   // List of branches
   TBranch        *b_DeltaPhi1;   //!
   TBranch        *b_DeltaPhi2;   //!
   TBranch        *b_DeltaPhiMin;   //!
   TBranch        *b_GenJetsAK8;   //!
   TBranch        *b_GenJetsAK8_AxisAverage;   //!
   TBranch        *b_GenJetsAK8_AxisMajor;   //!
   TBranch        *b_GenJetsAK8_AxisMinor;   //!
   TBranch        *b_GenJetsAK8_ECF1;   //!
   TBranch        *b_GenJetsAK8_ECF2;   //!
   TBranch        *b_GenJetsAK8_ECF3;   //!
   TBranch        *b_GenJetsAK8_MomentGirth;   //!
   TBranch        *b_GenJetsAK8_MomentHalf;   //!
   TBranch        *b_GenJetsAK8_Multiplicity;   //!
   TBranch        *b_GenJetsAK8_Overflow;   //!
   TBranch        *b_GenJetsAK8_PtD;   //!
   TBranch        *b_GenJetsAK8_Tau1;   //!
   TBranch        *b_GenJetsAK8_Tau2;   //!
   TBranch        *b_GenJetsAK8_Tau3;   //!
   TBranch        *b_HVMesons;   //!
   TBranch        *b_MAOS;   //!
   TBranch        *b_METSystems;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_METPhi;   //!
   TBranch        *b_MJJ;   //!
   TBranch        *b_Mmc;   //!
   TBranch        *b_MT;   //!
   TBranch        *b_MT2;   //!

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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/GenProductionV1/genmassanalysis_mZprime-3000_mDark-20_rinv-0.3_alpha-0.2_n-10000_part-1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/GenProductionV1/genmassanalysis_mZprime-3000_mDark-20_rinv-0.3_alpha-0.2_n-10000_part-1.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/GenProductionV1/genmassanalysis_mZprime-3000_mDark-20_rinv-0.3_alpha-0.2_n-10000_part-1.root:/GenMassAnalyzer");
      dir->GetObject("tree",tree);

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
   GenJetsAK8 = 0;
   GenJetsAK8_AxisAverage = 0;
   GenJetsAK8_AxisMajor = 0;
   GenJetsAK8_AxisMinor = 0;
   GenJetsAK8_ECF1 = 0;
   GenJetsAK8_ECF2 = 0;
   GenJetsAK8_ECF3 = 0;
   GenJetsAK8_MomentGirth = 0;
   GenJetsAK8_MomentHalf = 0;
   GenJetsAK8_Multiplicity = 0;
   GenJetsAK8_Overflow = 0;
   GenJetsAK8_PtD = 0;
   GenJetsAK8_Tau1 = 0;
   GenJetsAK8_Tau2 = 0;
   GenJetsAK8_Tau3 = 0;
   HVMesons = 0;
   METSystems = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
   fChain->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
   fChain->SetBranchAddress("DeltaPhiMin", &DeltaPhiMin, &b_DeltaPhiMin);
   fChain->SetBranchAddress("GenJetsAK8", &GenJetsAK8, &b_GenJetsAK8);
   fChain->SetBranchAddress("GenJetsAK8_AxisAverage", &GenJetsAK8_AxisAverage, &b_GenJetsAK8_AxisAverage);
   fChain->SetBranchAddress("GenJetsAK8_AxisMajor", &GenJetsAK8_AxisMajor, &b_GenJetsAK8_AxisMajor);
   fChain->SetBranchAddress("GenJetsAK8_AxisMinor", &GenJetsAK8_AxisMinor, &b_GenJetsAK8_AxisMinor);
   fChain->SetBranchAddress("GenJetsAK8_ECF1", &GenJetsAK8_ECF1, &b_GenJetsAK8_ECF1);
   fChain->SetBranchAddress("GenJetsAK8_ECF2", &GenJetsAK8_ECF2, &b_GenJetsAK8_ECF2);
   fChain->SetBranchAddress("GenJetsAK8_ECF3", &GenJetsAK8_ECF3, &b_GenJetsAK8_ECF3);
   fChain->SetBranchAddress("GenJetsAK8_MomentGirth", &GenJetsAK8_MomentGirth, &b_GenJetsAK8_MomentGirth);
   fChain->SetBranchAddress("GenJetsAK8_MomentHalf", &GenJetsAK8_MomentHalf, &b_GenJetsAK8_MomentHalf);
   fChain->SetBranchAddress("GenJetsAK8_Multiplicity", &GenJetsAK8_Multiplicity, &b_GenJetsAK8_Multiplicity);
   fChain->SetBranchAddress("GenJetsAK8_Overflow", &GenJetsAK8_Overflow, &b_GenJetsAK8_Overflow);
   fChain->SetBranchAddress("GenJetsAK8_PtD", &GenJetsAK8_PtD, &b_GenJetsAK8_PtD);
   fChain->SetBranchAddress("GenJetsAK8_Tau1", &GenJetsAK8_Tau1, &b_GenJetsAK8_Tau1);
   fChain->SetBranchAddress("GenJetsAK8_Tau2", &GenJetsAK8_Tau2, &b_GenJetsAK8_Tau2);
   fChain->SetBranchAddress("GenJetsAK8_Tau3", &GenJetsAK8_Tau3, &b_GenJetsAK8_Tau3);
   fChain->SetBranchAddress("HVMesons", &HVMesons, &b_HVMesons);
   fChain->SetBranchAddress("MAOS", &MAOS, &b_MAOS);
   fChain->SetBranchAddress("METSystems", &METSystems, &b_METSystems);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
   fChain->SetBranchAddress("MJJ", &MJJ, &b_MJJ);
   fChain->SetBranchAddress("Mmc", &Mmc, &b_Mmc);
   fChain->SetBranchAddress("MT", &MT, &b_MT);
   fChain->SetBranchAddress("MT2", &MT2, &b_MT2);
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
