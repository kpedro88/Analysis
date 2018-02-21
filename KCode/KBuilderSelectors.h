#ifndef KBUILDERSELECTORS_H
#define KBUILDERSELECTORS_H

//custom headers
#include "KSelection.h"
#include "KCommonSelectors.h"
#include "KMath.h"
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
		virtual void CheckBase(){
			//standard weight options
			normtype = ""; base->GetLocalOpt()->Get("normtype",normtype); GetNormTypeEnum();
			unweighted = base->GetLocalOpt()->Get("unweighted",false);
			useTreeWeight = base->GetGlobalOpt()->Get("useTreeWeight",false);
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
				TH1* flathist = NULL;
				string flatname; localOpt->Get("flatname",flatname);
				TFile* flatfile = TFile::Open(flatname.c_str(),"READ");
				if(flatfile){
					flatqty=""; localOpt->Get("flatqty",flatqty);
					string flatsuff;
					if(!base->GetLocalOpt()->Get("flatsuff",flatsuff)) flatsuff = base->GetName();
					string flatdist = flatqty + "_" + flatsuff;
					flathist = (TH1*)flatfile->Get(flatdist.c_str());
					flattener.SetDist(flathist);
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
				if(flatqty=="leadjetAK8pt" || flatqty=="subleadjetAK8pt" || flatqty=="bothjetAK8pt") looper->fChain->SetBranchStatus("JetsAK8");
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
			
			if(flatten){
				if(flatqty=="leadjetAK8pt") w *= flattener.GetWeight(looper->JetsAK8->at(0).Pt());
				else if(flatqty=="subleadjetAK8pt") w *= flattener.GetWeight(looper->JetsAK8->at(1).Pt());
				//for bothjetAK8pt, need to weight per jet
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
			else if(NTenum==ttbarLowHTLowMET) { goodEvent &= looper->madHT < 600 and looper->GenMET >= 150; }
			else if(NTenum==ttbarLowHThad) { goodEvent &= looper->madHT < 600 && looper->GenElectrons->size()==0 && looper->GenMuons->size()==0 && looper->GenTaus->size()==0; }
			else if(NTenum==ttbarHighHT) { goodEvent &= looper->madHT >= 600; }
			else if(NTenum==wjetsLowHT) { goodEvent &= looper->madHT < 100; }
			else if(NTenum==wjetsHighHT) { goodEvent &= looper->madHT >= 100; }
		
			return goodEvent;
		}
		
		//member variables
		bool unweighted, got_nEventProc, got_xsection, got_luminorm, useTreeWeight, debugWeight, didDebugWeight;
		bool pucorr, trigcorr, isrcorr, realMET, signal, fastsim, jetidcorr, isotrackcorr, lumicorr, btagcorr, puacccorr, flatten;
		double jetidcorrval, isotrackcorrval, lumicorrval;
		int puunc, pdfunc, isrunc, scaleunc, trigunc, btagSFunc, mistagSFunc, btagCFunc, ctagCFunc, mistagCFunc, puaccunc;
		vector<int> mother;
		TH1 *puhist, *puhistUp, *puhistDown;
		vector<double> pdfnorms;
		string normtype;
		normtypes NTenum;
		int nEventProc;
		double xsection, norm;
		ISRCorrector isrcorror;
		PileupAcceptanceUncertainty puacc;
		Flattener flattener;
		string flatqty;
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
//applies MET filters
class KMETFilterSelector : public KSelector {
	public:
		//constructor
		KMETFilterSelector() : KSelector() { }
		KMETFilterSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), skipHTRatio(false) {
			localOpt->Get("filterfiles",filterfiles);
			for(unsigned f = 0; f < filterfiles.size(); ++f){
				filters.push_back(new EventListFilter(filterfiles[f]));
			}
			onlydata = localOpt->Get("onlydata",false);
			filter2015 = localOpt->Get("filter2015",false);
		}
		virtual void CheckBranches(){
			looper->fChain->SetBranchStatus("HT",1);
			looper->fChain->SetBranchStatus("HT5",1);
			looper->fChain->SetBranchStatus("NVtx",1);
			looper->fChain->SetBranchStatus("eeBadScFilter",1);
			looper->fChain->SetBranchStatus("eeBadSc4Filter",1);
			looper->fChain->SetBranchStatus("HBHENoiseFilter",1);
			looper->fChain->SetBranchStatus("HBHEIsoNoiseFilter",1);
			looper->fChain->SetBranchStatus("CSCTightHaloFilter",1);
			looper->fChain->SetBranchStatus("EcalDeadCellTriggerPrimitiveFilter",1);
			looper->fChain->SetBranchStatus("globalTightHalo2016Filter",1);
			looper->fChain->SetBranchStatus("BadChargedCandidateFilter",1);
			looper->fChain->SetBranchStatus("BadPFMuonFilter",1);
			if(filters.size()>0 && filters[0]->Initialized()){
				looper->fChain->SetBranchStatus("RunNum",1);
				looper->fChain->SetBranchStatus("LumiBlockNum",1);
				looper->fChain->SetBranchStatus("EvtNum",1);				
			}
			if(!looper->fChain->GetBranchStatus("HT5") or !looper->fChain->GetBranch("HT5")) skipHTRatio = true;
		}
		virtual void CheckBase(){
			//check fastsim stuff
			bool fastsim = base->GetLocalOpt()->Get("fastsim",false);
			if(fastsim){
				//disable this for fastsim
				dummy = true;
			}
			if(onlydata and !base->IsData()){
				//disable this for non-data if desired
				dummy = true;
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			bool TightHaloFilter = looper->globalTightHalo2016Filter==1;
			bool HBHENoiseFilter = looper->HBHENoiseFilter==1;
			bool HBHEIsoNoiseFilter = looper->HBHEIsoNoiseFilter==1;
			bool EcalDeadCellTriggerPrimitiveFilter = looper->EcalDeadCellTriggerPrimitiveFilter==1;
			bool eeBadScFilter = looper->eeBadScFilter==1;
			bool BadChargedCandidateFilter = looper->BadChargedCandidateFilter;
			bool BadPFMuonFilter = looper->BadPFMuonFilter;
			bool HTRatioFilter = skipHTRatio;// or (looper->HT5/looper->HT <= 2.0);
			bool otherFilters = true;
			for(unsigned f = 0; f < filters.size(); ++f){
				otherFilters &= filters[f]->CheckEvent(looper->RunNum,looper->LumiBlockNum,looper->EvtNum);
			}
			if(filter2015) return looper->NVtx > 0 && eeBadScFilter && HBHENoiseFilter && HBHEIsoNoiseFilter && looper->CSCTightHaloFilter==1 && EcalDeadCellTriggerPrimitiveFilter && otherFilters;
			return looper->NVtx > 0 && eeBadScFilter && HBHENoiseFilter && HBHEIsoNoiseFilter && TightHaloFilter 
				&& EcalDeadCellTriggerPrimitiveFilter && BadChargedCandidateFilter && BadPFMuonFilter && HTRatioFilter && otherFilters;
		}
		
		//member variables
		bool onlydata, filter2015, skipHTRatio;
		vector<string> filterfiles;
		vector<EventListFilter*> filters;
};
REGISTER_SELECTOR(METFilter);

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
			
			//initialize btag corrector calibrations
			btagcorr.SetCalib("btag/CSVv2_Moriond17_B_H_mod.csv");
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
			if(debug) looper->fChain->SetBranchStatus("BTags",1);
		}
		virtual void CheckBase(){
			//don't even bother
			if(depfailed) return;
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
				//initialize btag corrector fastsim calibrations
				//todo: check the sample name and choose the appropriate CFs (once available)
				btagcorr.SetCalibFastSim("btag/fastsim_csvv2_ttbar_26_1_2017.csv");
				
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
				cout << "BTags = " << looper->BTags << endl;
				cout << "prob = ";
				copy(prob.begin(),prob.end(),ostream_iterator<double>(cout," "));
				cout << endl;
			}
			return true;
		}
		
		//member variables
		int debug;
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
			looper->fChain->SetBranchStatus("JetsAK8_subjets",1);
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

//---------------------------------------------------------------
//little class to store value & weight pairs for filling histos
class KValue {
	public:
		//constructor
		KValue() : values(0), weights(0) {}
		//accessors
		void Fill(double v, double w=1){
			values.push_back(v);
			weights.push_back(w);
		}
		double & GetValue(int iv) { return values[iv]; }
		double & GetWeight(int iw) { return weights[iw]; }
		int GetSize() { return values.size(); }
		
	protected:
		//member variables
		vector<double> values;
		vector<double> weights;
	
};

//----------------------------------------------------
//final selector to fill histograms
//(accounts for dependence on other selectors)
class KHistoSelector : public KSelector {
	public:
		//constructor
		KHistoSelector() : KSelector() { }
		KHistoSelector(string name_, OptionMap* localOpt_) : 
			KSelector(name_,localOpt_), initialized(false), MCWeight(NULL), RA2Bin(NULL), PhotonID(NULL), BTagSF(NULL), JetEtaRegion(NULL), Hemisphere(NULL), FakeHLT(NULL), DarkHadron(NULL)
		{ 
			canfail = false;
		}
		
		virtual void CheckDeps(){
			//set dependencies here
			MCWeight = sel->Get<KMCWeightSelector*>("MCWeight");
			RA2Bin = sel->Get<KRA2BinSelector*>("RA2Bin");
			PhotonID = sel->Get<KPhotonIDSelector*>("PhotonID");
			
			BTagSF = sel->Get<KBTagSFSelector*>("BTagSF");
			JetEtaRegion = sel->Get<KJetEtaRegionSelector*>("JetEtaRegion");
			Hemisphere = sel->Get<KHemisphereSelector*>("Hemisphere");
			FakeHLT = sel->Get<KFakeHLTSelector*>("FakeHLT");
			DarkHadron = sel->Get<KDarkHadronSelector*>("DarkHadron");
		}
		virtual void CheckBranches(){
			if(RA2Bin && RA2Bin->debug){
				looper->fChain->SetBranchStatus("RunNum",1);
				looper->fChain->SetBranchStatus("LumiBlockNum",1);
				looper->fChain->SetBranchStatus("EvtNum",1);
			}
		}
		virtual void CheckBase(){
			base->GetLocalOpt()->Get("mother",mother);
			deltaM = 0; base->GetLocalOpt()->Get("deltaM",deltaM);
			//do not use MCWeight with data
			if(base->IsData()) MCWeight = NULL;
			//but require it for MC
			else if(base->IsMC() && !MCWeight) depfailed = true;
			bool DoBTagSF = MCWeight ? MCWeight->btagcorr : false;
			if(!DoBTagSF) BTagSF = NULL;
			
			if(depfailed) canfail = true;
		}
		void Initialize(){
			if(initialized) return;
			//initial loop to get histo variables
			int table_size = base->GetTable().size();
			vars.clear(); vars.reserve(table_size);
			htmp.clear(); htmp.reserve(table_size);
			for(auto& sit : base->GetTable()){
				//get histo name
				string stmp = sit.first;
				htmp.push_back(sit.second);
				//split up histo variable names
				vector<string> vars_tmp;
				KParser::process(stmp,'_',vars_tmp);
				vars.push_back(vars_tmp);
			}
			initialized = true;
		}
		bool IsPerJet(const string& vname, KValue& value, double w){
			bool leadjet = false, subleadjet = false, thirdjet = false, fourthjet = false;
			string vname2;
			
			if(vname.compare(0,10,"leadjetAK8")==0) {
				leadjet = true;
				vname2 = vname.substr(10,string::npos);
			}
			else if(vname.compare(0,13,"subleadjetAK8")==0){
				subleadjet = true;
				vname2 = vname.substr(13,string::npos);
			}
			else if(vname.compare(0,10,"bothjetAK8")==0) {
				leadjet = true;
				subleadjet = true;
				vname2 = vname.substr(10,string::npos);
			}
			else if(vname.compare(0,11,"thirdjetAK8")==0) {
				thirdjet = true;
				vname2 = vname.substr(11,string::npos);
			}
			else if(vname.compare(0,12,"fourthjetAK8")==0) {
				fourthjet = true;
				vname2 = vname.substr(12,string::npos);
			}

			if(leadjet) FillPerJet(vname2,value,w,0);
			if(subleadjet) FillPerJet(vname2,value,w,1);
			if(thirdjet) FillPerJet(vname2,value,w,2);
			if(fourthjet) FillPerJet(vname2,value,w,3);
			
			return (leadjet or subleadjet or thirdjet or fourthjet);
		}
		void FillPerJet(const string& vname, KValue& value, double w, unsigned index){
			if(looper->JetsAK8->size()>index){
				//
				if(MCWeight and MCWeight->flatten and MCWeight->flatqty=="bothjetAK8pt")  w *= MCWeight->flattener.GetWeight(looper->JetsAK8->at(index).Pt());

				if(vname=="pt") value.Fill(looper->JetsAK8->at(index).Pt(),w);
				else if(vname=="eta") value.Fill(looper->JetsAK8->at(index).Eta(),w);
				else if(vname=="abseta") value.Fill(abs(looper->JetsAK8->at(index).Eta()),w);
				else if(vname=="axisminor") value.Fill(looper->JetsAK8_axisminor->at(index),w);
				else if(vname=="axismajor") value.Fill(looper->JetsAK8_axismajor->at(index),w);
				else if(vname=="girth") value.Fill(looper->JetsAK8_girth->at(index),w);
				else if(vname=="mhalf") value.Fill(looper->JetsAK8_momenthalf->at(index),w);
				else if(vname=="mult") value.Fill(looper->JetsAK8_multiplicity->at(index),w);
				else if(vname=="overflow") value.Fill(looper->JetsAK8_overflow->at(index),w);
				else if(vname=="ptD") value.Fill(looper->JetsAK8_ptD->at(index),w);
				//derived substructure variables
				else if(vname=="tau21"){
					if(looper->JetsAK8_NsubjettinessTau1->at(index)>0) value.Fill(looper->JetsAK8_NsubjettinessTau2->at(index)/looper->JetsAK8_NsubjettinessTau1->at(index),w);
				}
				else if(vname=="tau32"){
					if(looper->JetsAK8_NsubjettinessTau2->at(index)>0) value.Fill(looper->JetsAK8_NsubjettinessTau3->at(index)/looper->JetsAK8_NsubjettinessTau2->at(index),w);
				}
				else if(vname=="msd") value.Fill(looper->JetsAK8_softDropMass->at(index),w);
				else if(vname=="mass") value.Fill(looper->JetsAK8->at(index).M(),w);
				else if(vname=="nstable" and DarkHadron) value.Fill(DarkHadron->n_stable.at(index),w);
				else if(vname=="nunstable" and DarkHadron) value.Fill(DarkHadron->n_unstable.at(index),w);
				else if(vname=="nvisible" and DarkHadron) value.Fill(DarkHadron->n_visible.at(index),w);
				else if(vname=="rinv" and DarkHadron) value.Fill(DarkHadron->rinv.at(index),w);
				else if(vname=="nsubjet") value.Fill(looper->JetsAK8_subjets->at(index).size(),w);
				else { //do nothing
				}
			}
		}
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(!initialized) Initialize();
			if(depfailed) return false;
			
			double w = 1.0;
			if(MCWeight) w = MCWeight->GetWeight();
			if(FakeHLT) w *= FakeHLT->weight;
			
			for(unsigned h = 0; h < htmp.size(); h++){
				unsigned vsize = vars[h].size();
				vector<KValue> values(vsize);				
			
				for(unsigned i = 0; i < vsize; i++){
					string vname = vars[h][i];
					//list of cases for histo calculation and filling
					if(vname=="RA2bin" && RA2Bin){ //plot yield vs. bin of RA2 search -> depends on RA2Bin selector
						if(RA2Bin->RA2Exclusive) {
							if(RA2Bin->debug==1){
								cout << "Run = " << looper->RunNum << ", LS = " << looper->LumiBlockNum << ", Evt = " << looper->EvtNum;
								for(unsigned q = 0; q < RA2Bin->RA2VarNames.size(); ++q){
									if(RA2Bin->RA2VarNames[q]=="NJets") cout << ", NJets = " << looper->NJets;
									else if(RA2Bin->RA2VarNames[q]=="BTags") cout << ", BTags = " << looper->BTags;
									else if(RA2Bin->RA2VarNames[q]=="MHT") cout << ", MHT = " << looper->MHT;
									else if(RA2Bin->RA2VarNames[q]=="HT") cout << ", HT = " << looper->HT;
								}
								cout << endl;
							}
							else if(RA2Bin->debug==2){
								//RA2bin number starts at 1
								cout << looper->RunNum << "\t" << looper->LumiBlockNum << "\t" << looper->EvtNum << "\t" << RA2Bin->RA2bins[0] << "\t" << RA2Bin->labels[RA2Bin->RA2bins[0]-1] << endl;
							}
							values[i].Fill(RA2Bin->RA2bins[0],w);
						}
						else {
							for(unsigned b = 0; b < RA2Bin->RA2bins.size(); b++){
								double wb = w;
								//weight by btag scale factor probability if available
								if(BTagSF) {
									int nb = RA2Bin->GetBin("BTags",RA2Bin->RA2binVec[b]);
									if(nb>=0 && ((unsigned)nb)<BTagSF->prob.size()) wb *= BTagSF->prob[nb];
									else wb = 0; //btag sf failed
								}
								values[i].Fill(RA2Bin->RA2bins[b],wb);
							}
						}
					}
					else if(vname=="njets"){//jet multiplicity
						values[i].Fill(looper->NJets,w);
					}
					else if(vname=="njetsisr"){//ISR jet multiplicity
						values[i].Fill(looper->NJetsISR,w);
					}
					else if(vname=="nbjets"){//b-jet multiplicity
						if(BTagSF){
							for(unsigned b = 0; b < BTagSF->prob.size(); b++){
								//weight by btag scale factor probability if available
								double wb = w*BTagSF->prob[b];
								values[i].Fill(b,wb);
							}
						}
						else values[i].Fill(looper->BTags,w);
					}
					else if(vname=="njetshemi"){//jet multiplicity
						if(Hemisphere) values[i].Fill(Hemisphere->NJets,w);
					}
					else if(vname=="nbjetshemi"){//b-jet multiplicity
						if(Hemisphere) values[i].Fill(Hemisphere->BTags,w);
					}
					else if(vname=="njetsopphemi"){//jet multiplicity
						if(Hemisphere) values[i].Fill(Hemisphere->NJetsOpp,w);
					}
					else if(vname=="nbjetsopphemi"){//b-jet multiplicity
						if(Hemisphere) values[i].Fill(Hemisphere->BTagsOpp,w);
					}
					else if(vname=="ht"){//sum of jet pt
						values[i].Fill(looper->HT,w);
					}
					else if(vname=="mht"){//missing hadronic energy
						values[i].Fill(looper->MHT,w);
					}
					else if(vname=="genmht"){//missing hadronic energy
						values[i].Fill(looper->GenMHT,w);
					}
					else if(vname=="met"){//missing energy
						values[i].Fill(looper->MET,w);
					}
					else if(vname=="nleptons"){//# leptons (mu or ele)
						values[i].Fill(looper->NMuons+looper->NElectrons,w);
					}
					else if(vname=="nelectrons"){//# electrons
						values[i].Fill(looper->NElectrons,w);
					}
					else if(vname=="nmuons"){//# muons
						values[i].Fill(looper->NMuons,w);
					}
					else if(vname=="nisotrack"){//# iso tracks
						values[i].Fill(looper->isoElectronTracks+looper->isoMuonTracks+looper->isoPionTracks,w);
					}
					else if(vname=="nvertex"){//# good vertices
						values[i].Fill(looper->NVtx,w);
					}
					else if(vname=="numint"){//# interactions
						values[i].Fill(looper->NumInteractions,w);
					}
					else if(vname=="madht"){//madgraph HT
						values[i].Fill(looper->madHT,w);
					}
					else if(vname=="leadjetpt"){//pT of leading jet
						if(looper->Jets->size()>0){
							values[i].Fill(looper->Jets->at(0).Pt(),w);
						}
					}
					else if(vname=="mht-leadjetpt-ratio"){//ratio of MHT & pT of leading jet
						if(looper->Jets->size()>0){
							values[i].Fill(looper->MHT/looper->Jets->at(0).Pt(),w);
						}
					}
					else if(vname=="met-leadjetpt-ratio"){//ratio of MET & pT of leading jet
						if(looper->Jets->size()>0){
							values[i].Fill(looper->MET/looper->Jets->at(0).Pt(),w);
						}
					}
					else if(vname=="leadbhadronjetpt"){//pT of leading jet w/ hadronFlavor==5, |eta|<2.4
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(abs(looper->Jets_hadronFlavor->at(j))==5 && fabs(looper->Jets->at(j).Eta())<2.4){
								values[i].Fill(looper->Jets->at(j).Pt(),w);
								break;
							}
						}
					}
					else if(vname=="deltaphi1"){//deltaphi of leading jet
						values[i].Fill(looper->DeltaPhi1,w);
					}
					else if(vname=="deltaphi2"){//deltaphi of 2nd jet
						values[i].Fill(looper->DeltaPhi2,w);
					}
					else if(vname=="deltaphi3"){//deltaphi of 3rd jet
						values[i].Fill(looper->DeltaPhi3,w);
					}
					else if(vname=="deltaphi4"){//deltaphi of 4th jet
						values[i].Fill(looper->DeltaPhi4,w);
					}
					else if(vname=="deltaM"){//difference between mMother and mLSP
						values[i].Fill(deltaM,w);
					}
					else if(vname=="motherpt"){//pT of each mother particle
						//loop over genparticles
						for(unsigned g = 0; g < looper->GenParticles_PdgId->size(); ++g){
							if(binary_search(mother.begin(),mother.end(),abs(looper->GenParticles_PdgId->at(g)))){
								values[i].Fill(looper->GenParticles->at(g).Pt(),w);
							}
						}
					}
					else if(vname=="recoil"){//pT of mother particle system recoiling against ISR jets
						//loop over genparticles
						TLorentzVector vgen;
						vgen.SetPtEtaPhiE(0,0,0,0);
						for(unsigned g = 0; g < looper->GenParticles_PdgId->size(); ++g){
							if(binary_search(mother.begin(),mother.end(),abs(looper->GenParticles_PdgId->at(g)))){
								vgen += looper->GenParticles->at(g);
							}
						}
						values[i].Fill(vgen.Pt(),w);
					}
					else if(vname=="deltaphirecoil"){//delta phi of mother particle system with MHT
						//loop over genparticles
						TLorentzVector vgen;
						vgen.SetPtEtaPhiE(0,0,0,0);
						for(unsigned g = 0; g < looper->GenParticles_PdgId->size(); ++g){
							if(binary_search(mother.begin(),mother.end(),abs(looper->GenParticles_PdgId->at(g)))){
								vgen += looper->GenParticles->at(g);
							}
						}
						values[i].Fill(KMath::DeltaPhi(vgen.Phi(),looper->MHTPhi),w);
					}
					else if(vname=="sigmaietaieta"){//sigma ieta ieta variable for all photon candidates
						if(PhotonID){
							for(unsigned p = 0; p < PhotonID->goodPhotons.size(); ++p){
								values[i].Fill(looper->Photons_sigmaIetaIeta->at(PhotonID->goodPhotons[p]),w);
							}
						}
						else { //if no ID applied, just plot everything
							for(unsigned p = 0; p < looper->Photons_sigmaIetaIeta->size(); ++p){
								values[i].Fill(looper->Photons_sigmaIetaIeta->at(p),w);
							}
						}
					}
					else if(vname=="bestsigmaietaieta"){//sigma ieta ieta variable for best photon
						double best_sieie=0;
						double best_pt=0;
						for(unsigned p = 0; p < looper->Photons->size(); ++p){
							if(looper->Photons->at(p).Pt()>best_pt){
								best_pt = looper->Photons->at(p).Pt();
								best_sieie = looper->Photons_sigmaIetaIeta->at(p);
							}
						}
						values[i].Fill(best_sieie,w);
					}
					else if(vname=="pdfrms"){
						values[i].Fill(TMath::RMS(looper->PDFweights->begin(),looper->PDFweights->end()),w);
					}
					else if(vname=="pdfweight"){
						for(unsigned w = 0; w < looper->PDFweights->size(); ++w){
							values[i].Fill(looper->PDFweights->at(w),w);
						}
					}
					//jet ID quantities... (w/ optional eta region specification)
					else if(vname=="neufrac"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_neutralHadronEnergyFraction->at(j),w);
						}
					}
					else if(vname=="phofrac"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_photonEnergyFraction->at(j),w);
						}
					}
					else if(vname=="chgfrac"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_chargedHadronEnergyFraction->at(j),w);
						}
					}
					else if(vname=="chgemfrac"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_chargedEmEnergyFraction->at(j),w);
						}
					}
					else if(vname=="chgmulti"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_chargedMultiplicity->at(j),w);
						}
					}
					else if(vname=="neumulti"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_neutralMultiplicity->at(j),w);
						}
					}
					else if(vname=="nconstit"){
						for(unsigned j = 0; j < looper->Jets->size(); ++j){
							if(!JetEtaRegion || JetEtaRegion->mask[j]) values[i].Fill(looper->Jets_chargedMultiplicity->at(j)+looper->Jets_neutralMultiplicity->at(j),w);
						}
					}
					else if(vname=="MTAK8"){//transverse mass
						values[i].Fill(looper->MT_AK8,w);
					}
					else if(vname=="MJJAK8"){//dijet mass
						values[i].Fill(looper->MJJ_AK8,w);
					}
					else if (vname=="MJJSDAK8"){//dijet mass from softdrop
						TLorentzVector vjjjj;
						if(looper->JetsAK8_subjets->size()>1){
							for(unsigned j = 0; j < 2; ++j){
								for(const auto& subjet: looper->JetsAK8_subjets->at(j)){
									vjjjj += subjet;
								}
							}
							values[i].Fill(vjjjj.M(),w);
						}
					}
					else if(vname=="MTSDAK8"){//transverse mass from softdrop
						TLorentzVector vjjjj;
						if(looper->JetsAK8_subjets->size()>1){
							for(unsigned j = 0; j < 2; ++j){
								for(const auto& subjet: looper->JetsAK8_subjets->at(j)){
									vjjjj += subjet;
								}
							}
							double MT = KMath::TransverseMass(vjjjj.Px(),vjjjj.Py(),vjjjj.M(),looper->MET*cos(looper->METPhi),looper->MET*sin(looper->METPhi),0);
							values[i].Fill(MT,w);
						}						
					}
					else if(vname=="MmcAK8"){//dijet+truth mass
						values[i].Fill(looper->Mmc_AK8,w);
					}
					else if(IsPerJet(vname,values[i],w)){
						//per-jet histos (leading, subleading, or both)
						//nothing to do - histo is filled as a side effect
					}
					else if(vname=="deltaetaAK8"){//deta(j1,j2)
						if(looper->JetsAK8->size()>1) values[i].Fill(abs(looper->JetsAK8->at(0).Eta()-looper->JetsAK8->at(1).Eta()),w);
					}
					else if(vname=="deltaphi1AK8"){//dphi(j1,MET)
						values[i].Fill(looper->DeltaPhi1_AK8,w);
					}
					else if(vname=="deltaphi2AK8"){//dphi(j2,MET)
						values[i].Fill(looper->DeltaPhi2_AK8,w);
					}
					else if(vname=="deltaphiminAK8"){//min dphi(j1/2,MET)
						values[i].Fill(looper->DeltaPhiMin_AK8,w);
					}
					else if(vname=="ptAsymAK8"){//AK8 dijet pT asymmetry
						if(looper->JetsAK8->size()>1) values[i].Fill((looper->JetsAK8->at(0).Pt()-looper->JetsAK8->at(1).Pt())/(looper->JetsAK8->at(0).Pt()+looper->JetsAK8->at(1).Pt()),w);
					}
					else if(vname=="msdAsymAK8"){//AK8 dijet m_sd asymmetry
						if(looper->JetsAK8->size()>1) values[i].Fill(abs(looper->JetsAK8_softDropMass->at(0)-looper->JetsAK8_softDropMass->at(1))/(looper->JetsAK8_softDropMass->at(0)+looper->JetsAK8_softDropMass->at(1)),w);
					}
					else if(vname=="htAK8"){//HT from AK8 jets
						double htak8 = 0.0;
						for(const auto& jet : *(looper->JetsAK8)){
							if(abs(jet.Eta())<2.4) htak8 += jet.Pt();
						}
						values[i].Fill(htak8,w);
					}
					else if(vname=="njetAK8"){//number of AK8 jets
						int num = 0;
						for(const auto& jet : *(looper->JetsAK8)){
							if(abs(jet.Eta())<2.4) ++num;
						}
						values[i].Fill(num,w);
					}
					else if(vname=="metMTratio"){//MET/MT
						values[i].Fill(looper->MT_AK8>0?looper->MET/looper->MT_AK8:0.0,w);
					}
					else if(vname=="metsig"){//MET significance
						values[i].Fill(looper->METSignificance,w);
					}
					else if(vname=="logmetsig"){//log(MET significance)
						values[i].Fill(log(looper->METSignificance),w);
					}
					else if(vname=="genmet"){//gen MET
						values[i].Fill(looper->GenMET,w);
					}
					else if(vname=="madht"){//madgraph HT
						values[i].Fill(looper->madHT,w);
					}
					else { //if it's a histogram with no known variable or calculation, do nothing
					}
				}
				
				//now fill the histogram
				if(vsize==1){
					for(int ix = 0; ix < values[0].GetSize(); ix++){
						htmp[h]->Fill(values[0].GetValue(ix), values[0].GetWeight(ix));
					}
				}
				else if(vsize==2){
					//need to cast in order to use Fill(x,y,w)
					//these three cases allow for various x vs. y comparisons: same # entries per event, or 1 vs. N per event
					if(values[0].GetSize()==values[1].GetSize()) {
						for(int i = 0; i < values[0].GetSize(); i++){
							if(htmp[h]->GetDimension()==1)
								static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
							else if(htmp[h]->GetDimension()==2)
								static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(i), values[1].GetValue(i), values[0].GetWeight(i)); //pick the x weight by default
						}
					}
					else if(values[0].GetSize()==1){
						for(int iy = 0; iy < values[1].GetSize(); iy++){
							if(htmp[h]->GetDimension()==1)
								static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
							else if(htmp[h]->GetDimension()==2)
								static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(0), values[1].GetValue(iy), values[1].GetWeight(iy));
						}
					}
					else if(values[1].GetSize()==1){
						for(int ix = 0; ix < values[0].GetSize(); ix++){
							if(htmp[h]->GetDimension()==1)
								static_cast<TProfile*>(htmp[h])->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
							else if(htmp[h]->GetDimension()==2)
								static_cast<TH2*>(htmp[h])->Fill(values[0].GetValue(ix), values[1].GetValue(0), values[0].GetWeight(ix));
						}
					}
				}
				else { //no support for other # of vars
				}
			}
			
			return true;
		}
		virtual void Finalize(TFile* file){
			if(sel->GetGlobalOpt()->Get("plotoverflow",false)){
				for(unsigned h = 0; h < htmp.size(); h++){
					if(vars[h].size()==2) continue; //not implemented for 2D histos or profiles yet
					
					//temporary histo to calculate error correctly when adding overflow bin to last bin
					TH1* otmp = (TH1*)htmp[h]->Clone();
					otmp->Reset("ICEM");
					int ovbin = htmp[h]->GetNbinsX()+1;
					double err = 0.;
					otmp->SetBinContent(ovbin-1,htmp[h]->IntegralAndError(ovbin,ovbin,err));
					otmp->SetBinError(ovbin-1,err);
					
					//add overflow bin to last bin
					htmp[h]->Add(otmp);
					
					//remove overflow bin from htmp[h] (for consistent integral/yield)
					htmp[h]->SetBinContent(ovbin,0);
					htmp[h]->SetBinError(ovbin,0);
					
					delete otmp;
				}
			}
		}
		
		//member variables
		bool initialized;
		vector<vector<string> > vars;
		vector<TH1*> htmp;
		KMCWeightSelector* MCWeight;
		KRA2BinSelector* RA2Bin;
		KPhotonIDSelector* PhotonID;
		KBTagSFSelector* BTagSF;
		KJetEtaRegionSelector* JetEtaRegion;
		KHemisphereSelector* Hemisphere;
		KFakeHLTSelector* FakeHLT;
		KDarkHadronSelector* DarkHadron;
		vector<int> mother;
		double deltaM;
};
REGISTER_SELECTOR(Histo);

#endif
