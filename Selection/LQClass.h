#ifndef LQClass_h
#define LQClass_h
#define NtupleClass_cxx

#include "NtupleClass.h"
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TLorentzVector.h>

// Header file for the classes stored in the TTree if any.
#include <string>
#include <vector>

using namespace std;

void NtupleClass::Loop() {}

class LQClass : public NtupleClass {
	public :
		TH1            *nEventProc;
		
		vector<int>    theGoodMuons;
		vector<int>    theGoodMuons2;
		int            theGoodMuon;
		int            theGoodMuon2;
		double         muonZPV;
		vector<int>    theGoodTaus;
		int            theGoodTau;
		vector<bool>   fakeTauJet, fakeTauLep, realTau, fakeTauBJet;
		vector<int>    theGoodJets;
		int            theMainJet;
		int            theMainBJet;
		int            nGoodVertices;
		bool           doBatch;
		int            motherId;
		string         inFile;
		vector<string> selTypes;
		string         selType;
		string         uncType;
		vector<string> outNames;
		vector<double> PFJetPtOrig;
		vector<double> PFJetEnergyOrig;
		vector<double> HPSTauPtOrig;
		double         s_MuonMass, s_TauMass;
		
		LQClass(TTree* tree, TH1* nEventProc_, int motherId_) : NtupleClass(tree), nEventProc(nEventProc_), doBatch(false), motherId(motherId_) { 
			//PDG mass values in GeV
			double s_MuonMass = 0.1056583715;
			double s_TauMass = 1.77682;	
		}
		virtual ~LQClass() {}
		Bool_t Cut();
		void StoreOrigVars();
		void RestoreOrigVars();
		void TauES(bool up);
		void TauER(bool up);
		void JetES(bool up);
		void JetER(bool up);
		TLorentzVector SmearJER(TLorentzVector vj, TLorentzVector vg, bool up);
		Bool_t HLTSelection(string HLTLine="HLT_IsoMu24");
		Bool_t MuonIDTight(int m, double pt=30);
		Bool_t MuonIDLoose(int m, double pt=20);
		Bool_t MuonSelection();
		Bool_t MuonSelectionLooseNotTight();
		Bool_t MuonSelectionSecond(double pt=25);
		Bool_t MuonVetoOS();
		Bool_t MuonVetoLoose();
		Bool_t VertexQuality();
		Bool_t ElectronVeto();
		Bool_t ElectronVetoLoose();
		Bool_t TauSelection(int iso_option=1, int chargesign=-1);
		Bool_t TauIsoVeto();
		Bool_t JetSelection(int ntau=1);
		Bool_t GenSelection();
		Bool_t GenSelectionB();
		virtual void Loop();
		void SetBatch(bool b) { doBatch = b; }
		bool GetBatch() { return doBatch; }
		void SetInFile(string in) { inFile = in; }
		void AddSelType(string stype, string oname){
			selTypes.push_back(stype);
			outNames.push_back(oname);
		}
		double phi(double x, double y);
		double DeltaPhi(double phi1, double phi2);
		double DeltaR(double phi1, double eta1, double phi2, double eta2);
		void process(string line, char delim, vector<string>& fields);
};

#endif
