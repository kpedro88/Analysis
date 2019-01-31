#ifndef KBUILDERSELECTORS_H
#define KBUILDERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KCommonSelectors.h"
#include "KMath.h"
#include "KHisto.h"
#include "../btag/BTagCorrector.h"
#include "../corrections/EventListFilter.h"
#include "../corrections/GetTriggerEffCorr.C"
#include "../corrections/Quad76x.h"
#include "../corrections/Double76x.h"
#include "../corrections/TriggerEfficiencySextet.cpp"
#include "../corrections/ISRCorrector.h"
#include "../corrections/PileupAcceptanceUncertainty.h"
#include "../corrections/Flattener.h"

//ROOT headers
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

//STL headers
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <set>

using namespace std;

//base class for Selectors is in KSelection.h

//--------------------------------------------------------------------------
//does cross section normalization, data/MC corrections, uncertainties, etc.
class KMCWeightSelector : public KSelector {
	public:
		//constructor
		KMCWeightSelector() : KSelector() {}
		KMCWeightSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
		}
		//enum for flattening qtys
		enum flatqtys { noflatqty = 0, leadjetAK8pt = 1, subleadjetAK8pt = 2, bothjetAK8pt = 3, thirdjetAK8pt = 4, fourthjetAK8pt = 5 };
		enum svbqtys { nosvbqty = 0, MTAK8 = 1 };
		virtual void CheckBase(){
			//standard weight options
			normtype = ""; base->GetLocalOpt()->Get("normtype",normtype); GetNormTypeEnum();
			unweighted = base->GetLocalOpt()->Get("unweighted",false);
			useTreeWeight = base->GetGlobalOpt()->Get("useTreeWeight",false);
			useKFactor = base->GetLocalOpt()->Get("kfactor",kfactor);
			nEventProc = 0; got_nEventProc = base->GetLocalOpt()->Get("nEventProc",nEventProc);
			xsection = 0; got_xsection = base->GetLocalOpt()->Get("xsection",xsection);
			norm = 0; got_luminorm = base->GetGlobalOpt()->Get("luminorm",norm);
			debugWeight = base->GetGlobalOpt()->Get("debugWeight",false); didDebugWeight = false;
			fastsim = base->GetLocalOpt()->Get("fastsim",false);
			
			//PU options
			pucorr = localOpt->Get("pucorr",false);
			puunc = 0; localOpt->Get("puunc",puunc);
			puhist = NULL;
			puhistUp = NULL;
			puhistDown = NULL;
			if(pucorr){
				string puname = ""; localOpt->Get("puname",puname);
				HistoMap* hmtmp = puhistMap().Get(puname);
				if(puname.size()==0){
					cout << "Input error: no pileup weight file specified!" << endl;
				}
				else if(hmtmp){
					puhist = hmtmp->Get("puhist");
					puhistUp = hmtmp->Get("puhistUp");
					puhistDown = hmtmp->Get("puhistDown");
				}
				else{
					//store correction root files in map
					TFile* pufile = TFile::Open(puname.c_str(),"READ");
					if(pufile){
						hmtmp = new HistoMap();
						puhist = (TH1*)pufile->Get("pu_weights_central"); puhist->SetDirectory(0); hmtmp->Add("puhist",puhist);
						puhistUp = (TH1*)pufile->Get("pu_weights_up"); puhistUp->SetDirectory(0); hmtmp->Add("puhistUp",puhistUp);
						puhistDown = (TH1*)pufile->Get("pu_weights_down"); puhistDown->SetDirectory(0); hmtmp->Add("puhistDown",puhistDown);
						puhistMap().Add(puname,hmtmp);
						pufile->Close();
					}
					else {
						cout << "Input error: could not open pileup weight file " << puname << "." << endl;
					}
				}
			}
			
			//alternative PU option - acceptance uncertainty
			puacccorr = localOpt->Get("puacccorr",false);
			puaccunc = 0; localOpt->Get("puaccunc",puaccunc);
			if(puacccorr){
				TH1* h_nvtx = NULL;
				string nvtxname = ""; localOpt->Get("nvtxname",nvtxname);
				TH1* htmp = nvtxhistMap().Get(nvtxname);
				if(nvtxname.size()==0){
					cout << "Input error: no nvtx distribution file specified!" << endl;
				}
				else if(htmp){
					h_nvtx = htmp;
				}
				else{
					TFile* nvtxfile = TFile::Open(nvtxname.c_str(),"READ");
					if(nvtxfile){
						h_nvtx = (TH1*)nvtxfile->Get("nvertex_SingleElectron"); h_nvtx->SetDirectory(0); nvtxhistMap().Add(nvtxname,h_nvtx);
						nvtxfile->Close();
					}
					else{
						cout << "Input error: could not open nvtx distribution file " << nvtxname << "." << endl;
					}
				}
				puacc.SetInputs((TGraphErrors*)base->GetFile()->Get("pileupAccBand"),h_nvtx,localOpt->Get("debugpuacc",false));
			}
			
			//trig corr options
			trigcorr = localOpt->Get("trigcorr",false);
			string treedir = ""; base->GetGlobalOpt()->Get("treedir",treedir);
			if(treedir.find("genMHT")!=string::npos) trigcorr = false; //disabled for genMHT variation
			trigunc = 0; localOpt->Get("trigunc", trigunc);
			realMET = base->GetLocalOpt()->Get("realMET",true);
			signal = base->GetLocalOpt()->Get("signal",false);
			
			//ISR corr options
			isrcorr = localOpt->Get("isrcorr",false);
			base->GetLocalOpt()->Get("mother",mother);
			isrunc = 0; localOpt->Get("isrunc", isrunc);
			if(isrcorr){
				TH1* isrhist = NULL;
				TH1* isrhistUp = NULL;
				TH1* isrhistDown = NULL;
				string isrname = ""; localOpt->Get("isrname",isrname);
				HistoMap* hmtmp = isrhistMap().Get(isrname);
				if(isrname.size()==0){
					cout << "Input error: no ISR weight file specified!" << endl;
				}
				else if(hmtmp){
					isrhist = hmtmp->Get("isrhist");
					isrhistUp = hmtmp->Get("isrhistUp");
					isrhistDown = hmtmp->Get("isrhistDown");
				}
				else{
					//store correction files centrally
					TFile* isrfile = TFile::Open(isrname.c_str(),"READ");
					if(isrfile){
						hmtmp = new HistoMap();
						isrhist = (TH1*)isrfile->Get("isr_weights_central"); isrhist->SetDirectory(0); hmtmp->Add("isrhist",isrhist);
						isrhistUp = (TH1*)isrfile->Get("isr_weights_up"); isrhistUp->SetDirectory(0); hmtmp->Add("isrhistUp",isrhistUp);
						isrhistDown = (TH1*)isrfile->Get("isr_weights_down"); isrhistDown->SetDirectory(0); hmtmp->Add("isrhistDown",isrhistDown);
						isrhistMap().Add(isrname,hmtmp);
						isrfile->Close();
					}
					else {
						cout << "Input error: could not open ISR weight file " << isrname << "." << endl;
					}
				}
				TH1* isrtmp = NULL;
				if(isrunc==1) isrtmp = isrhistUp;
				else if(isrunc==-1) isrtmp = isrhistDown;
				else isrtmp = isrhist;
				isrcorror.SetWeights(isrtmp,(TH1*)base->GetFile()->Get("NJetsISR"));
			}
			
			//flattening options
			flatten = localOpt->Get("flatten",false);
			if(flatten){
				flatqty = noflatqty;
				TH1* flathist = NULL;
				string flatname; localOpt->Get("flatname",flatname);
				TFile* flatfile = TFile::Open(flatname.c_str(),"READ");
				if(flatfile){
					localOpt->Get("flatqty",sflatqty);
					string flatsuff;
					if(!base->GetLocalOpt()->Get("flatsuff",flatsuff)) flatsuff = base->GetName();
					string flatdist = sflatqty + "_" + flatsuff;
					flathist = (TH1*)flatfile->Get(flatdist.c_str());
					flattener.SetDist(flathist);
					
					//get enum for flatqty
					if(sflatqty=="leadjetAK8pt") flatqty = leadjetAK8pt;
					else if(sflatqty=="subleadjetAK8pt") flatqty = subleadjetAK8pt;
					else if(sflatqty=="bothjetAK8pt") flatqty = bothjetAK8pt;
					else if(sflatqty=="thirdjetAK8pt") flatqty = thirdjetAK8pt;
					else if(sflatqty=="fourthjetAK8pt") flatqty = fourthjetAK8pt;
					else cout << "Input error: unknown flatqty " << sflatqty << endl;
				}
				else{
					cout << "Input error: could not open flattening file " << flatname << endl;
				}
			}

			//svb weighting options
			svbweight = localOpt->Get("svbweight",false);
			hsvb = NULL;
			if(svbweight){
				svbqty = nosvbqty;
				string svbname; localOpt->Get("svbname",svbname);
				string svbnumer; localOpt->Get("svbnumer",svbnumer);
				string svbdenom; localOpt->Get("svbdenom",svbdenom);
				TFile* svbfile = TFile::Open(svbname.c_str(),"READ");
				if(svbfile and !svbnumer.empty() and !svbdenom.empty()){
					//get enum
					string ssvbqty; localOpt->Get("svbqty",ssvbqty);
					if(ssvbqty=="MTAK8") svbqty = MTAK8;

					//use ratio object w/ built-in calculations
					TH1F* numer = (TH1F*)svbfile->Get((ssvbqty+"_"+svbnumer).c_str());
					TH1F* denom = (TH1F*)svbfile->Get((ssvbqty+"_"+svbdenom).c_str());
					string svbcalc; localOpt->Get("svbcalc",svbcalc);
					KSetRatio svbratio("svb",nullptr,nullptr);
					svbratio.SetCalc(svbcalc);
					svbratio.Build(ssvbqty,numer,denom);
					hsvb = svbratio.GetHisto();

					//check the assignment of this set (if not numer, then denom)
					bool svb_isnumer = base->GetLocalOpt()->Get("svbnumer",false);

					//normalize weights to preserve total # events in sample
					if(localOpt->Get("svbnorm",false)){
						TH1F* svbnorm = svb_isnumer ? (TH1F*)numer->Clone() : (TH1F*)denom->Clone();
						svbnorm->Scale(1.0/svbnorm->Integral(0,svbnorm->GetNbinsX()+1));
						hsvb->Multiply(svbnorm);
					}

				}
			}

			//jet ID corr options - only for fastsim
			jetidcorrval = 1;
			jetidcorr = localOpt->Get("jetidcorr",false);
			int jetidunc = 0; localOpt->Get("jetidunc",jetidunc);
			vector<double> jetidcorrvals;
			if(fastsim && localOpt->Get("jetidcorrvals",jetidcorrvals) && jetidcorrvals.size()==3){
				//vector has down, central, up
				jetidcorrval = jetidunc==0 ? jetidcorrvals[1] : ( jetidunc==1 ? jetidcorrvals[2] : jetidcorrvals[0] );
			}
			
			//isotrack eff corr options - only for signals with leptonic decays
			isotrackcorrval = 1;
			isotrackcorr = localOpt->Get("isotrackcorr",false);
			int isotrackunc = 0; localOpt->Get("isotrackunc",isotrackunc);
			vector<string> isotracksiglist; localOpt->Get("isotracksiglist",isotracksiglist);
			bool inisotracksiglist = false;
			for(const auto& sig : isotracksiglist){
				if(base->GetName().find(sig)!=string::npos){
					inisotracksiglist = true;
					break;
				}
			}
			vector<double> isotrackcorrvals;
			if(inisotracksiglist && localOpt->Get("isotrackcorrvals",isotrackcorrvals) && isotrackcorrvals.size()==3){
				//vector has down, central, up
				isotrackcorrval = isotrackunc==0 ? isotrackcorrvals[1] : ( isotrackunc==1 ? isotrackcorrvals[2] : isotrackcorrvals[0] );
			}
			
			//lumi corr options
			lumicorrval = 1;
			lumicorr = localOpt->Get("lumicorr",false);
			int lumiunc = 0; localOpt->Get("lumiunc",lumiunc);
			vector<double> lumicorrvals;
			if(localOpt->Get("lumicorrvals",lumicorrvals) && lumicorrvals.size()==3){
				//vector has down, central, up
				lumicorrval = lumiunc==0 ? lumicorrvals[1] : ( lumiunc==1 ? lumicorrvals[2] : lumicorrvals[0] );
			}
			
			//btag corr options - used in BTagSF
			btagcorr = localOpt->Get("btagcorr",false);
			btagSFunc = 0; localOpt->Get("btagSFunc",btagSFunc);
			mistagSFunc = 0; localOpt->Get("mistagSFunc",mistagSFunc);
			btagCFunc = 0; localOpt->Get("btagCFunc",btagCFunc);
			ctagCFunc = 0; localOpt->Get("ctagCFunc",ctagCFunc);
			mistagCFunc = 0; localOpt->Get("mistagCFunc",mistagCFunc);
			
			//other uncertainty options
			pdfunc = 0; localOpt->Get("pdfunc",pdfunc);
			scaleunc = 0; localOpt->Get("scaleunc",scaleunc);
			if(pdfunc!=0 || scaleunc!=0){
				//get the normalizations for pdf/scale uncertainties
				TH1F* h_norm = (TH1F*)base->GetFile()->Get("PDFNorm");
				pdfnorms = vector<double>(4,1.0);
				//0: PDF up, 1: PDF down, 2: scale up, 3: scale down
				if(h_norm){
					double nominal = h_norm->GetBinContent(1);
					for(unsigned n = 0; n < 4; ++n){
						//(bin in histo = index + 2)
						pdfnorms[n] = nominal/h_norm->GetBinContent(n+2);
					}
				}
			}
		}
		virtual void CheckBranches(){
			//force enable branches needed for cuts/weights/etc.
			looper->fChain->SetBranchStatus("Weight",1); //needed for negative weights even if useTreeWeight==false
			if(pucorr){
				looper->fChain->SetBranchStatus("TrueNumInteractions",1);
				looper->fChain->SetBranchStatus("NVtx",1);
				if(puunc==1) looper->fChain->SetBranchStatus("puSysUp",1);
				else if(puunc==-1) looper->fChain->SetBranchStatus("puSysDown",1);
				else looper->fChain->SetBranchStatus("puWeight",1);
			}
			if(isrcorr){
				looper->fChain->SetBranchStatus("NJetsISR",1);
			}
			if(flatten){
				if(flatqty>noflatqty and flatqty<=fourthjetAK8pt) looper->fChain->SetBranchStatus("JetsAK8",1);
			}
			if(svbweight){
				if(svbqty==MTAK8) looper->fChain->SetBranchStatus("MT_AK8",1);
			}
			if(NTenum==ttbarLowHTLowMET || NTenum==ttbarLowHTHighMET || NTenum==ttbarLowHThad || NTenum==ttbarHighHT || NTenum==wjetsLowHT || NTenum==wjetsHighHT){
				looper->fChain->SetBranchStatus("madHT",1);
			}
			if(NTenum==ttbarLowHTLowMET || NTenum==ttbarLowHTHighMET){
				looper->fChain->SetBranchStatus("GenMET",1);
			}
			if(NTenum==ttbarLowHThad){
				looper->fChain->SetBranchStatus("GenElectrons",1);
				looper->fChain->SetBranchStatus("GenMuons",1);
				looper->fChain->SetBranchStatus("GenTaus",1);
			}
			if(pdfunc!=0){
				looper->fChain->SetBranchStatus("PDFweights",1);
			}
			if(scaleunc!=0){
				looper->fChain->SetBranchStatus("ScaleWeights",1);
			}
		}
		//enum for normtypes
		enum normtypes { NoNT=0, ttbarLowHTLowMET=1, ttbarLowHTHighMET=2, ttbarLowHThad=3, ttbarHighHT=4, wjetsLowHT=5, wjetsHighHT=6 };
		//convert normtype from string to enum for quicker compares
		void GetNormTypeEnum(){
			if(normtype=="ttbarLowHTLowMET") NTenum = ttbarLowHTLowMET;
			else if(normtype=="ttbarLowHTHighMET") NTenum = ttbarLowHTHighMET;
			else if(normtype=="ttbarLowHThad") NTenum = ttbarLowHThad;
			else if(normtype=="ttbarHighHT") NTenum = ttbarHighHT;
			else if(normtype=="wjetsLowHT") NTenum = wjetsLowHT;
			else if(normtype=="wjetsHighHT") NTenum = wjetsHighHT;
			else NTenum = NoNT;
		}
		double GetWeight(){
			double w = 1.;
			if(unweighted) return w;
			
			//check option in case correction types are disabled globally
			//(enabled by default
			//(*disabled* until 2015 data is available)
			
			if(pucorr) {
				//use TreeMaker weights if no histo provided
				if(puunc==1){
					w *= puhistUp ? puhistUp->GetBinContent(puhistUp->GetXaxis()->FindBin(min(looper->TrueNumInteractions,puhistUp->GetBinLowEdge(puhistUp->GetNbinsX()+1)))) : looper->puSysUp;
				}
				else if(puunc==-1){
					w *= puhistDown ? puhistDown->GetBinContent(puhistDown->GetXaxis()->FindBin(min(looper->TrueNumInteractions,puhistDown->GetBinLowEdge(puhistDown->GetNbinsX()+1)))) : looper->puSysDown;
				}
				else {
					w *= puhist ? puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(looper->TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1)))) : looper->puWeight;
				}
			}
			
			if(puacccorr){
				w *= puacc.GetCorrection(puaccunc);
			}
			
			if(trigcorr){
				unsigned effindex = trigunc==-1 ? 2 : trigunc;
				w *= Eff_MetMhtSextetReal_CenterUpDown(looper->HT, looper->MHT, looper->NJets)[effindex];
			}
			
			if(isrcorr){
				w *= isrcorror.GetCorrection(looper->NJetsISR);
			}

			if(pdfunc!=0){
				if(pdfunc==1) w *= *(TMath::LocMax(looper->PDFweights->begin(),looper->PDFweights->end()))*pdfnorms[0];
				else if(pdfunc==-1) w *= *(TMath::LocMin(looper->PDFweights->begin(),looper->PDFweights->end()))*pdfnorms[1];
			}
			
			if(scaleunc!=0){
				vector<double> ScaleWeightsMod = *looper->ScaleWeights;
				//remove unwanted variations
				if(ScaleWeightsMod.size()>7) ScaleWeightsMod.erase(ScaleWeightsMod.begin()+7);
				if(ScaleWeightsMod.size()>5) ScaleWeightsMod.erase(ScaleWeightsMod.begin()+5);
				if(ScaleWeightsMod.size()>0) ScaleWeightsMod.erase(ScaleWeightsMod.begin());
				
				if(scaleunc==1) w *= *(TMath::LocMax(ScaleWeightsMod.begin(),ScaleWeightsMod.end()))*pdfnorms[2];
				else if(scaleunc==-1) w *= *(TMath::LocMin(ScaleWeightsMod.begin(),ScaleWeightsMod.end()))*pdfnorms[3];
			}
			
			//correct for expected FullSim PFJetID efficiency
			if(jetidcorr){
				w *= jetidcorrval;
			}
			
			if(isotrackcorr){
				w *= isotrackcorrval;
			}
			
			if(lumicorr){
				w *= lumicorrval;
			}

			if(svbweight){
				double qty = 0;
				if(svbqty==MTAK8) qty = looper->MT_AK8;
				w *= hsvb->GetBinContent(hsvb->GetXaxis()->FindBin(min(qty,hsvb->GetBinLowEdge(hsvb->GetNbinsX()+1))));
			}
			
			//now do scaling: norm*xsection/nevents
			if(useTreeWeight && !fastsim) w *= looper->Weight;
			else if(got_nEventProc && nEventProc>0 && got_xsection){
				w *= xsection/nEventProc;
				//account for negative weight events
				if(looper->Weight<0) w *= -1;
				
				//debugging
				if(debugWeight && !didDebugWeight){
					int oldprec = cout.precision(20);
					cout << base->GetName() << endl;
					cout << "TreeMaker: " << fabs(looper->Weight) << endl;
					cout << "    KCode: " << xsection/nEventProc << " = " << xsection << " / " << nEventProc << endl;
					didDebugWeight = true;
					cout.precision(oldprec);
				}
			}
			if(useKFactor) w *= kfactor;
			
			//use lumi norm (default)
			if(got_luminorm) w *= norm;
			
			return w;
		}
		//static members - kept in functions for safety
		static HistoMapMap& puhistMap(){
			static HistoMapMap puhistMap_;
			return puhistMap_;
		}
		static HistoMapMap& isrhistMap(){
			static HistoMapMap isrhistMap_;
			return isrhistMap_;
		}
		static HistoMap& nvtxhistMap(){
			static HistoMap nvtxhistMap_;
			return nvtxhistMap_;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool goodEvent = true;
			
			//check normalization type here
			if(NTenum==ttbarLowHTLowMET) { goodEvent &= looper->madHT < 600 and looper->GenMET < 150; }
			else if(NTenum==ttbarLowHTHighMET) { goodEvent &= looper->madHT < 600 and looper->GenMET >= 150; }
			else if(NTenum==ttbarLowHThad) { goodEvent &= looper->madHT < 600 && looper->GenElectrons->size()==0 && looper->GenMuons->size()==0 && looper->GenTaus->size()==0; }
			else if(NTenum==ttbarHighHT) { goodEvent &= looper->madHT >= 600; }
			else if(NTenum==wjetsLowHT) { goodEvent &= looper->madHT < 100; }
			else if(NTenum==wjetsHighHT) { goodEvent &= looper->madHT >= 100; }
		
			return goodEvent;
		}
		
		//member variables
		bool unweighted, got_nEventProc, got_xsection, got_luminorm, useTreeWeight, useKFactor, debugWeight, didDebugWeight;
		bool pucorr, trigcorr, isrcorr, realMET, signal, fastsim, jetidcorr, isotrackcorr, lumicorr, btagcorr, puacccorr, flatten, svbweight;
		double jetidcorrval, isotrackcorrval, lumicorrval;
		int puunc, pdfunc, isrunc, scaleunc, trigunc, btagSFunc, mistagSFunc, btagCFunc, ctagCFunc, mistagCFunc, puaccunc;
		vector<int> mother;
		TH1 *puhist, *puhistUp, *puhistDown;
		vector<double> pdfnorms;
		string normtype;
		normtypes NTenum;
		int nEventProc;
		double xsection, norm, kfactor;
		ISRCorrector isrcorror;
		PileupAcceptanceUncertainty puacc;
		Flattener flattener;
		string sflatqty;
		flatqtys flatqty;
		svbqtys svbqty;
		TH1* hsvb;
};
REGISTER_SELECTOR(MCWeight);

//avoid unwanted dependency
void KRA2BinSelector::CheckDeps(){
	CheckLabels();
	//check other options
	MCWeight = sel->Get<KMCWeightSelector*>("MCWeight");
}
void KRA2BinSelector::CheckBase(){
	DoBTagSF = MCWeight ? MCWeight->btagcorr : false;
}

//----------------------------------------------------
//selects events based on specified jet pT range
class KPTRangeSelector : public KSelector {
	public:
		//constructor
		KPTRangeSelector() : KSelector() { }
		KPTRangeSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(1), ptmin(1000), ptmax(1200) { 
			//check for option
			localOpt->Get("njet",njet);
			localOpt->Get("ptmin",ptmin);
			localOpt->Get("ptmax",ptmax);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("JetsAK8",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->JetsAK8->size()<njet) return false;
			double jetpt = looper->JetsAK8->at(njet-1).Pt();
			return ptmin < jetpt and jetpt <= ptmax;
		}
		
		//member variables
		int njet;
		double ptmin, ptmax;
};
REGISTER_SELECTOR(PTRange);


//----------------------------------------------------
//simulates some interesting triggers
class KFakeHLTSelector : public KSelector {
	public:
		//enum for trigger names
		enum faketrigger { t_PFHT350_PFMET100=0, t_QuadJet45_TripleCSV067=1, t_DoubleJet90_Quad30_TripleCSV067=2, t_MET110_CSV07=3 };
		//constructor
		KFakeHLTSelector() : KSelector() { }
		KFakeHLTSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
			//get selected trigger from options
			vector<string> s_trigger;
			localOpt->Get("trigger",s_trigger);
			for(unsigned t = 0; t < s_trigger.size(); ++t){
				if(s_trigger[t]=="PFHT350_PFMET100") trigger.push_back(t_PFHT350_PFMET100);
				else if(s_trigger[t]=="QuadJet45_TripleCSV067") trigger.push_back(t_QuadJet45_TripleCSV067);
				else if(s_trigger[t]=="DoubleJet90_Quad30_TripleCSV067") trigger.push_back(t_DoubleJet90_Quad30_TripleCSV067);
				else if(s_trigger[t]=="MET110_CSV07") trigger.push_back(t_MET110_CSV07);
			}
		}
		virtual void CheckBranches(){
			//looper->fChain->SetBranchStatus("METPt",1);
			looper->fChain->SetBranchStatus("MHT",1);
			looper->fChain->SetBranchStatus("HT",1);
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("Jets_bDiscriminatorCSV",1);
		}
		virtual void CheckBase(){
			//check MC stuff
			realMET = base->GetLocalOpt()->Get("realMET",true);
			signal = base->GetLocalOpt()->Get("signal",false);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			vector<double> weights;
			//simulate trigger via offline cuts: require to be on plateau or assign a weight
			for(unsigned t = 0; t < trigger.size(); ++t){
				if(trigger[t]==t_PFHT350_PFMET100){
					//plateau requirements + residual inefficiency weight
					bool passTrigger = (looper->HT > 500 && looper->MHT > 200);
					if(!passTrigger) weights.push_back(0.);
					else weights.push_back(GetTriggerEffCorr(signal, looper->MHT, realMET));
				}
				else if(trigger[t]==t_QuadJet45_TripleCSV067){
					//weight for inefficiency w/ functions (from Caterina, AN2015_108)
					double jcounter = 0;
					vector<double> jetpt;
					set<double> csv_sorted;
					for(unsigned j = 0; j < looper->Jets->size(); ++j){
						if(fabs(looper->Jets->at(j).Eta())<2.6){
							if(jcounter < 4){
								jetpt.push_back(looper->Jets->at(j).Pt());
								++jcounter;
							}
							csv_sorted.insert(looper->Jets_bDiscriminatorCSV->at(j));
						}
					}
					//find CSV3
					set<double>::reverse_iterator rit = csv_sorted.rbegin();
					++rit; ++rit;
					double csv3 = *rit;
					
					weights.push_back(TurnOnQuad(jetpt[0],jetpt[1],jetpt[2],jetpt[3],csv3));
				}
				else if(trigger[t]==t_DoubleJet90_Quad30_TripleCSV067){
					//weight for inefficiency w/ functions (from Caterina, AN2015_108)
					double jcounter = 0;
					vector<double> jetpt;
					set<double> csv_sorted;
					for(unsigned j = 0; j < looper->Jets->size(); ++j){
						if(fabs(looper->Jets->at(j).Eta())<2.6){
							if(jcounter < 4){
								jetpt.push_back(looper->Jets->at(j).Pt());
								++jcounter;
							}
							csv_sorted.insert(looper->Jets_bDiscriminatorCSV->at(j));
						}
					}
					//find CSV3
					set<double>::reverse_iterator rit = csv_sorted.rbegin();
					++rit; ++rit;
					double csv3 = *rit;
					
					weights.push_back(TurnOnDouble(jetpt[0],jetpt[1],jetpt[2],jetpt[3],csv3));
				}
				else if(trigger[t]==t_MET110_CSV07){
					//MET on plateau, use CSV function from above
					bool passTrigger = (looper->MHT > 200);
					
					set<double> csv_sorted;
					for(unsigned j = 0; j < looper->Jets->size(); ++j){
						if(fabs(looper->Jets->at(j).Eta())<2.6){
							csv_sorted.insert(looper->Jets_bDiscriminatorCSV->at(j));
						}
					}
					//find CSV1
					set<double>::reverse_iterator rit = csv_sorted.rbegin();
					double csv1 = *rit;

					if(!passTrigger) weights.push_back(0.);
					else {
						csv1 = min(max(csv1,0.),1.);
						double wtmp = QuaJet_CSV3(-log(1-csv1+1.e-7));
						if(TMath::IsNaN(wtmp)) wtmp = 0.0;
						weights.push_back(wtmp);
					}
				}
			}
			
			if(weights.empty()) weight = 0.;
			else weight = weights[0];
			//OR: P(A||B)=P(A)+P(B)-P(A)*P(B)
			if(weights.size()>1){
				for(unsigned w = 1; w < weights.size(); ++w){
					weight = weight + weights[w] - weight*weights[w];
				}
			}
			
			return true;
		}
		
		//member variables
		vector<faketrigger> trigger;
		bool signal, realMET;
		double weight;
};
REGISTER_SELECTOR(FakeHLT);

//avoid unwanted dependency
double KHisto::GetWeight(){
	double w = 1.0;
	if(MCWeight) w = MCWeight->GetWeight();
	if(FakeHLT) w *= FakeHLT->weight;
	return w;
}
double KHisto::GetWeightPerJet(unsigned index){
	double w = 1.0;
	//range of flatqty for AK8 jet pt
	if(MCWeight and MCWeight->flatten and MCWeight->flatqty>KMCWeightSelector::noflatqty and MCWeight->flatqty<=KMCWeightSelector::fourthjetAK8pt) {
		w *= MCWeight->flattener.GetWeight(looper->JetsAK8->at(index).Pt());
		if( (MCWeight->flatqty==KMCWeightSelector::leadjetAK8pt and index!=0) or
			(MCWeight->flatqty==KMCWeightSelector::subleadjetAK8pt and index!=1) or
			(MCWeight->flatqty==KMCWeightSelector::bothjetAK8pt and (index!=0 and index!=1)) or
			(MCWeight->flatqty==KMCWeightSelector::thirdjetAK8pt and index!=2) or
			(MCWeight->flatqty==KMCWeightSelector::fourthjetAK8pt and index!=3) )
		{
			cout << "Input error: flattening qty is " << MCWeight->sflatqty << " but jet index is " << index << endl;
		}
	}
	return w;
}

//----------------------------------------------------
//selects events based on leading jet pt
class KLeadJetPtSelector : public KSelector {
	public:
		//constructor
		KLeadJetPtSelector() : KSelector() { }
		KLeadJetPtSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), pTmin(200) { 
			//check for option
			localOpt->Get("pTmin",pTmin);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Jets",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->Jets->size()>0 && looper->Jets->at(0).Pt() > pTmin;
		}
		
		//member variables
		double pTmin;
};
REGISTER_SELECTOR(LeadJetPt);

//---------------------------------------------------------------
//checks for double-counted events
class KDoubleCountSelector : public KSelector {
	public:
		//constructor
		KDoubleCountSelector() : KSelector() { }
		KDoubleCountSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("RunNum",1);
			looper->fChain->SetBranchStatus("EvtNum",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			pair<unsigned,unsigned long long> tmp(looper->RunNum,looper->EvtNum);
			map<pair<unsigned,unsigned long long>,int>::iterator cmit = countmap.find(tmp);
			if(cmit!=countmap.end()) return false; //fails the event if it finds a double count
			else {
				countmap[tmp] = 1;
				return true;
			}
		}
		
		//member variables
		map<pair<unsigned,unsigned long long>,int> countmap;
};
REGISTER_SELECTOR(DoubleCount);

//---------------------------------------------------------------
//applies photon ID to candidates
class KPhotonIDSelector : public KSelector {
	public:
		//constructor
		KPhotonIDSelector() : KSelector() { }
		KPhotonIDSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
			canfail = false;
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Photons",1);
			looper->fChain->SetBranchStatus("Photons_hadTowOverEM",1);
			looper->fChain->SetBranchStatus("Photons_hasPixelSeed",1);
			looper->fChain->SetBranchStatus("Photons_isEB",1);
			looper->fChain->SetBranchStatus("Photons_pfChargedIsoRhoCorr",1);
			looper->fChain->SetBranchStatus("Photons_pfGammaIsoRhoCorr",1);
			looper->fChain->SetBranchStatus("Photons_pfNeutralIsoRhoCorr",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			goodPhotons.clear();
			for(unsigned p = 0; p < looper->Photons->size(); ++p){
				bool goodPhoton = (abs(looper->Photons->at(p).Eta())<1.4442 || ((abs(looper->Photons->at(p).Eta())>1.566 && abs(looper->Photons->at(p).Eta())<2.5)))
									&& looper->Photons->at(p).Pt()>100.
									&& ((looper->Photons_hadTowOverEM->at(p)<0.028 && !looper->Photons_hasPixelSeed->at(p) && looper->Photons_isEB->at(p)) 
										|| (looper->Photons_hadTowOverEM->at(p)<0.093 && !looper->Photons_hasPixelSeed->at(p) && !looper->Photons_isEB->at(p)))
									&& looper->Photons_pfChargedIsoRhoCorr->at(p)<(2.67*looper->Photons_isEB->at(p) + 1.79*!looper->Photons_isEB->at(p))
									&& looper->Photons_pfGammaIsoRhoCorr->at(p)<((2.11+0.0014*looper->Photons->at(p).Pt())*looper->Photons_isEB->at(p)
																					+ (3.09+0.0091*looper->Photons->at(p).Pt())*!looper->Photons_isEB->at(p))
									&& looper->Photons_pfNeutralIsoRhoCorr->at(p)<((7.23+exp(0.0028*looper->Photons->at(p).Pt()+0.5408))*looper->Photons_isEB->at(p)
																					+ (8.89+0.01725*looper->Photons->at(p).Pt())*!looper->Photons_isEB->at(p));
				if(goodPhoton) goodPhotons.push_back(p);
			}
			return true;
		}
		
		//member variables
		vector<unsigned> goodPhotons;
};
REGISTER_SELECTOR(PhotonID);

//---------------------------------------------------------------
//individual MET filters
class KNVtxFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void CheckBranches(){ looper->fChain->SetBranchStatus("NVtx",1); }
		virtual bool Cut() { return looper->NVtx > 0; }
};
REGISTER_SELECTOR(NVtxFilter);

class KeeBadScFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void CheckBranches(){ looper->fChain->SetBranchStatus("eeBadScFilter",1); }
		virtual bool Cut() { return looper->eeBadScFilter==1; }
};
REGISTER_SELECTOR(eeBadScFilter);

class KEcalDeadCellTriggerPrimitiveFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void CheckBranches(){ looper->fChain->SetBranchStatus("EcalDeadCellTriggerPrimitiveFilter",1); }
		virtual bool Cut() { return looper->EcalDeadCellTriggerPrimitiveFilter==1; }
};
REGISTER_SELECTOR(EcalDeadCellTriggerPrimitiveFilter);

class KHBHENoiseFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void CheckBranches(){ looper->fChain->SetBranchStatus("HBHENoiseFilter",1); }
		virtual bool Cut() { return looper->HBHENoiseFilter==1; }
};
REGISTER_SELECTOR(HBHENoiseFilter);

class KHBHEIsoNoiseFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void CheckBranches(){ looper->fChain->SetBranchStatus("HBHEIsoNoiseFilter",1); }
		virtual bool Cut() { return looper->HBHEIsoNoiseFilter==1; }
};
REGISTER_SELECTOR(HBHEIsoNoiseFilter);

class KglobalSuperTightHalo2016FilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void CheckBranches(){ looper->fChain->SetBranchStatus("globalSuperTightHalo2016Filter",1); }
		virtual bool Cut() { return looper->globalSuperTightHalo2016Filter==1; }
};
REGISTER_SELECTOR(globalSuperTightHalo2016Filter);

class KBadChargedCandidateFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void CheckBranches(){ looper->fChain->SetBranchStatus("BadChargedCandidateFilter",1); }
		virtual bool Cut() { return looper->BadChargedCandidateFilter==1; }
};
REGISTER_SELECTOR(BadChargedCandidateFilter);

class KBadPFMuonFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void CheckBranches(){ looper->fChain->SetBranchStatus("BadPFMuonFilter",1); }
		virtual bool Cut() { return looper->BadPFMuonFilter==1; }
};
REGISTER_SELECTOR(BadPFMuonFilter);

//---------------------------------------------------------------
//eta regions for PFJetID: 0 = 0.0 < |eta| < 2.4; 1 = 0.0 < |eta| < 3.0; 2 = 3.0 < |eta|
//all require pt > 30
class KJetEtaRegionSelector : public KSelector {
	public:
		//constructor
		KJetEtaRegionSelector() : KSelector() { }
		KJetEtaRegionSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), region(0) { 
			localOpt->Get("region",region);
			canfail = false;
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Jets",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//reset mask
			mask.clear(); mask.resize(looper->Jets->size(),false);
			//check eta for each jet
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				if(looper->Jets->at(j).Pt() <= 30) continue;
				
				if(region==0 && fabs(looper->Jets->at(j).Eta()) <= 2.4) mask[j] = true;
				else if(region==1 && fabs(looper->Jets->at(j).Eta()) > 2.4 && fabs(looper->Jets->at(j).Eta()) <= 3.0) mask[j] = true;
				else if(region==2 && fabs(looper->Jets->at(j).Eta()) > 3.0) mask[j] = true;
			}
			return true;
		}
		
		//member variables
		int region;
		vector<bool> mask;
};
REGISTER_SELECTOR(JetEtaRegion);

//---------------------------------------------------------------
//recalculate NJets and BTags in a hemisphere around MHT_Phi
class KHemisphereSelector : public KSelector {
	public:
		//constructor
		KHemisphereSelector() : KSelector() { }
		KHemisphereSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
			canfail = false;
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("MHT_Phi",1);
			looper->fChain->SetBranchStatus("Jets_HTMask",1);
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("Jets_bDiscriminatorCSV",1);
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//reset output vars
			NJets = 0; BTags = 0;
			NJetsOpp = 0; BTagsOpp = 0;
			//check dphi for each jet
			for(unsigned j = 0; j < looper->Jets->size(); ++j){
				//HT jet cuts
				if(!looper->Jets_HTMask->at(j)) continue;
				
				double dphi = KMath::DeltaPhi(looper->MHTPhi,looper->Jets->at(j).Phi());
				if(fabs(dphi)<=TMath::Pi()/2){
					++NJets;
					if(looper->Jets_bDiscriminatorCSV->at(j) > 0.890) ++BTags;
				}
				else {
					++NJetsOpp;
					if(looper->Jets_bDiscriminatorCSV->at(j) > 0.890) ++BTagsOpp;
				}
			}
			return true;
		}
		
		//member variables
		int NJets, BTags;
		int NJetsOpp, BTagsOpp;
};
REGISTER_SELECTOR(Hemisphere);

//---------------------------------------------------------------
//calculates btag scale factors
class KBTagSFSelector : public KSelector {
	public:
		//constructor
		KBTagSFSelector() : KSelector() { }
		KBTagSFSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), debug(0), MCWeight(NULL)
		{ 
			canfail = false;
			
			//check for option
			localOpt->Get("debug",debug);
			if(debug==2) btagcorr.SetDebug(true);
			
			has_calib = localOpt->Get("calib",calib);
			if(!has_calib) localOpt->Get("calibs",calibs);
			has_calibfast = localOpt->Get("calibfast",calibfast);
			if(!has_calibfast) localOpt->Get("calibsfast",calibsfast);
		}
		virtual void CheckDeps(){
			//set dependencies here
			MCWeight = sel->Get<KMCWeightSelector*>("MCWeight");
			if(!MCWeight) depfailed = true;
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("Jets_HTMask",1);
			looper->fChain->SetBranchStatus("Jets",1);
			looper->fChain->SetBranchStatus("Jets_hadronFlavor",1);
			if(debug) looper->fChain->SetBranchStatus("BTagsDeepCSV",1);
		}
		virtual void CheckBase(){
			//don't even bother
			if(depfailed) return;

			//check for year
			if(!has_calib){
				if(base->GetName().find("MC2016")!=std::string::npos) calib = calibs[0];
				else if(base->GetName().find("MC2017")!=std::string::npos) calib = calibs[1];
				else if(base->GetName().find("MC2018")!=std::string::npos) calib = calibs[2];
				else calib = "";
			}
			//initialize btag corrector calibrations
			btagcorr.SetCalib(calib);

			//check for option
			int btagSFunc = MCWeight->btagSFunc; btagcorr.SetBtagSFunc(btagSFunc);
			int mistagSFunc = MCWeight->mistagSFunc; btagcorr.SetMistagSFunc(mistagSFunc);
			
			//get efficiency histograms
			btagcorr.SetEffs(base->GetFile());
			
			if(!btagcorr.h_eff_b || !btagcorr.h_eff_c || !btagcorr.h_eff_udsg){
				cout << "Input error: b-tag efficiency histograms missing!" << endl;
				depfailed = true;
			}
			
			//check fastsim stuff
			bool fastsim = base->GetLocalOpt()->Get("fastsim",false); btagcorr.SetFastSim(fastsim);
			if(fastsim){
				//check for year
				if(!has_calibfast){
					if(base->GetName().find("MC2016")!=std::string::npos) calibfast = calibsfast[0];
					else if(base->GetName().find("MC2017")!=std::string::npos) calibfast = calibsfast[1];
					else if(base->GetName().find("MC2018")!=std::string::npos) calibfast = calibsfast[2];
					else calibfast = "";
				}
				//initialize btag corrector fastsim calibrations
				//todo: check the sample name and choose the appropriate CFs (once available)
				if(!calibfast.empty()) btagcorr.SetCalibFastSim(calibfast);
				
				//check for option
				int btagCFunc = MCWeight->btagCFunc; btagcorr.SetBtagCFunc(btagCFunc);
				int ctagCFunc = MCWeight->ctagCFunc; btagcorr.SetCtagCFunc(ctagCFunc);
				int mistagCFunc = MCWeight->mistagCFunc;  btagcorr.SetMistagCFunc(mistagCFunc);
			}			
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(depfailed) return false;
			//get probabilities
			prob = btagcorr.GetCorrections(looper->Jets,looper->Jets_hadronFlavor,looper->Jets_HTMask);
			if(debug>0) {
				cout << "BTags = " << looper->BTagsDeepCSV << endl;
				cout << "prob = ";
				copy(prob.begin(),prob.end(),ostream_iterator<double>(cout," "));
				cout << endl;
			}
			return true;
		}
		
		//member variables
		int debug;
		string calib, calibfast;
		bool has_calib, has_calibfast;
		vector<string> calibs, calibsfast;
		BTagCorrector btagcorr;
		vector<double> prob;
		KMCWeightSelector* MCWeight;
};
REGISTER_SELECTOR(BTagSF);

//----------------------------------------------------
//vetoes events with gen leptons
class KGenLeptonVetoSelector : public KSelector {
	public:
		//constructor
		KGenLeptonVetoSelector() : KSelector() { }
		KGenLeptonVetoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { }
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("GenElectrons",1);
			looper->fChain->SetBranchStatus("GenMuons",1);
			looper->fChain->SetBranchStatus("GenTaus",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return looper->GenElectrons->size()==0 && looper->GenMuons->size()==0 && looper->GenTaus->size()==0;
		}
		
		//member variables
};
REGISTER_SELECTOR(GenLeptonVeto);

//---------------------------------------------------------------
//associate hidden sector gen-level info with reco jets
class KDarkHadronSelector : public KSelector {
	public:
		//constructor
		KDarkHadronSelector() : KSelector() { }
		KDarkHadronSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(2) {
			canfail = false;
			//check options
			localOpt->Get("njet",njet);
			useSubjets = localOpt->Get("useSubjets",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("JetsAK8",1);
			if(useSubjets) looper->fChain->SetBranchStatus("JetsAK8_subjets",1);
			looper->fChain->SetBranchStatus("GenParticles",1);
			looper->fChain->SetBranchStatus("GenParticles_PdgId",1);
			looper->fChain->SetBranchStatus("GenParticles_ParentId",1);
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			unsigned njet_ = min(njet,(unsigned)looper->JetsAK8->size());
			//clear
			n_stable.clear(); n_stable.reserve(njet_);
			n_unstable.clear(); n_unstable.reserve(njet_);
			n_visible.clear(); n_visible.reserve(njet_);
			rinv.clear(); rinv.reserve(njet_);
			for(unsigned j = 0; j < njet_; ++j){
				n_stable.push_back(0);
				n_unstable.push_back(0);
				n_visible.push_back(0);
				int n_invisible = 0;
				for(unsigned g = 0; g < looper->GenParticles->size(); ++g){
					int pdgid = abs(looper->GenParticles_PdgId->at(g));
					int parentid = abs(looper->GenParticles_ParentId->at(g));
					if(abs(pdgid-4900000)>1000) continue;
					double dR = 1000;
					double mindR = 0.8;
					if(useSubjets){
						mindR = 0.4;
						for(const auto& subjet: looper->JetsAK8_subjets->at(j)){
							double dRtmp = KMath::DeltaR(subjet.Phi(),subjet.Eta(),
											   looper->GenParticles->at(g).Phi(),looper->GenParticles->at(g).Eta());
							if(dRtmp < dR) dR = dRtmp;
						}
					}
					else {
						dR = KMath::DeltaR(looper->JetsAK8->at(j).Phi(),looper->JetsAK8->at(j).Eta(),
										   looper->GenParticles->at(g).Phi(),looper->GenParticles->at(g).Eta());
					}
					if(dR<mindR){
						if(pdgid==4900211){
							++(n_stable[j]);
							if(parentid==4900111) ++n_invisible;
						}
						else if(pdgid==4900111) ++(n_unstable[j]);
					}
				}
				//visible = unstable - invisible/2 (computed by invisible decays, unstable decays to 2 stable)
				n_visible[j] = n_unstable[j] - n_invisible/2;
				//rinv = 1 - (visible/unstable)
				rinv.push_back(1.0 - double(n_visible[j])/double(n_unstable[j]));
			}
			return true;
		}
		
		//member variables
		unsigned njet;
		bool useSubjets;
		vector<int> n_stable, n_unstable, n_visible;
		vector<double> rinv;
};
REGISTER_SELECTOR(DarkHadron);

//----------------------------------------------------
//final selector to fill histograms
//(just calls KHisto methods)
class KHistoSelector : public KSelector {
	public:
		//constructor
		KHistoSelector() : KSelector() { }
		KHistoSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_) { 
			canfail = false;
		}
		
		virtual void CheckBranches(){
			for(auto& hit : base->GetKTable()){
				hit.second->CheckBranches();
			}
		}
		//used for non-dummy selectors
		virtual bool Cut() {
			for(auto& hit : base->GetKTable()){
				hit.second->Fill();
			}

			return true;
		}
		virtual void Finalize(TFile* file){
			for(auto& hit : base->GetKTable()){
				hit.second->Finalize();
			}
		}
};
REGISTER_SELECTOR(Histo);

//---------------------------------------------------------------
//applies extra filters
class KExtraFilterSelector : public KSelector {
	public:
		//constructor
		KExtraFilterSelector() : KSelector() { }
		KExtraFilterSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), fastsim(false), data(false), data2017(false) {
			//check option
			onlydata = localOpt->Get("onlydata",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("METRatioFilter",1);
			looper->fChain->SetBranchStatus("MuonJetFilter",1);
			looper->fChain->SetBranchStatus("EcalNoiseJetFilter",1);
			looper->fChain->SetBranchStatus("HTRatioDPhiTightFilter",1);
			looper->fChain->SetBranchStatus("LowNeutralJetFilter",1);
			looper->fChain->SetBranchStatus("FakeJetFilter",1);
		}
		virtual void CheckBase(){
			//check fastsim stuff
			if(base->GetLocalOpt()->Get("fastsim",false)) fastsim = true;
			if(base->IsData()) {
				data = true;
				if(base->GetName().find("2017")!=string::npos) data2017 = true;
			}
			if(onlydata and !data){
				//disable this for non-data if desired
				dummy = true;
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool METRatioFilter = looper->METRatioFilter;
			bool MuonJetFilter = looper->MuonJetFilter;
			bool EcalNoiseJetFilter = (!data2017) or looper->EcalNoiseJetFilter;
			bool HTRatioDPhiTightFilter = looper->HTRatioDPhiTightFilter;
			bool LowNeutralJetFilter = looper->LowNeutralJetFilter;
			bool FakeJetFilter = (!fastsim) or looper->FakeJetFilter;

			return METRatioFilter and MuonJetFilter and EcalNoiseJetFilter and HTRatioDPhiTightFilter and LowNeutralJetFilter and FakeJetFilter;
		}
		
		//member variables
		bool fastsim, data, data2017;
		bool onlydata;
};
REGISTER_SELECTOR(ExtraFilter);

#endif
