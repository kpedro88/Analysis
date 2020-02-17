#ifndef KBUILDERSELECTORS_H
#define KBUILDERSELECTORS_H

//custom headers
#include "KParser.h"
#include "KSelection.h"
#include "KCommonSelectors.h"
#include "KMath.h"
#include "KHisto.h"
#include "../btag/BTagCorrector.h"
#include "../corrections/EventListFilter.h"
#include "../corrections/TriggerEfficiencySextet.cpp"
#include "../corrections/ISRCorrector.h"
#include "../corrections/TriggerCorrector.h"
#include "../corrections/TriggerFuncCorrector.h"
#include "../corrections/LeptonCorrector.h"
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
#include <array>
#include <tuple>
#include <functional>
#include <unordered_set>

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
		virtual void CheckDeps(){
			//if norm type not already provided, run it as a subordinate selector
			NormType = sel->Get<KNormTypeSelector*>("NormType");
			if(!NormType){
				NormType = new KNormTypeSelector("NormType", new OptionMap());
				internalNormType = true;
				NormType->SetSelection(sel);
			}
			else {
				internalNormType = false;
				canfail = false;
			}
		}
		virtual void CheckBase(){
			//standard weight options
			if(internalNormType){
				NormType->SetBase(base);
				NormType->CheckBase();
			}
			unweighted = base->GetLocalOpt()->Get("unweighted",false);
			useTreeWeight = base->GetGlobalOpt()->Get("useTreeWeight",false);
			useTreeXsec = base->GetGlobalOpt()->Get("useTreeXsec",false);
			useKFactor = base->GetLocalOpt()->Get("kfactor",kfactor);
			nEventProc = 0; got_nEventProc = base->GetLocalOpt()->Get("nEventProc",nEventProc);
			xsection = 0; got_xsection = base->GetLocalOpt()->Get("xsection",xsection);
			norm = 0; got_luminorm = base->GetGlobalOpt()->Get("luminorm",norm);
			debugWeight = base->GetGlobalOpt()->Get("debugWeight",false); didDebugWeight = false;
			fastsim = base->GetLocalOpt()->Get("fastsim",false);
			
			//PU options
			pucorr = localOpt->Get("pucorr",false);
			putree = localOpt->Get("putree",false); //option to use puWeight from tree
			puunc = 0; localOpt->Get("puunc",puunc);
			puhist = NULL;
			puhistUp = NULL;
			puhistDown = NULL;
			if(pucorr and !putree){
				string puname = ""; localOpt->Get("puname",puname);
				HistoMap* hmtmp = puhistMap().Get(puname);
				if(puname.empty()){
					throw runtime_error("no pileup weight file specified!");
				}
				else if(hmtmp){
					puhist = hmtmp->Get("puhist");
					puhistUp = hmtmp->Get("puhistUp");
					puhistDown = hmtmp->Get("puhistDown");
				}
				else{
					//store correction root files in map
					TFile* pufile = KOpen(puname);
					hmtmp = new HistoMap();
					puhist = KGet<TH1>(pufile,"pu_weights_central"); puhist->SetDirectory(0); hmtmp->Add("puhist",puhist);
					puhistUp = KGet<TH1>(pufile,"pu_weights_up"); puhistUp->SetDirectory(0); hmtmp->Add("puhistUp",puhistUp);
					puhistDown = KGet<TH1>(pufile,"pu_weights_down"); puhistDown->SetDirectory(0); hmtmp->Add("puhistDown",puhistDown);
					puhistMap().Add(puname,hmtmp);
					pufile->Close();
				}
			}

			//update existing PU correction using data ratio
			//only necessary because of 2017 wrong PU MC
			puupdcorr = localOpt->Get("puupdcorr",false);
			puupdunc = 0; localOpt->Get("puupdunc",puupdunc);
			puupdhist = NULL;
			puupdhistUp = NULL;
			puupdhistDown = NULL;
			if(puupdcorr){
				string puname1; localOpt->Get("puname1",puname1);
				string puname2; localOpt->Get("puname2",puname2);
				HistoMap* hmtmp = puhistMap().Get(puname1+puname2);
				if(puname1.empty() or puname2.empty()){
					throw runtime_error("expected pileup weight file not specified!");
				}
				else if(hmtmp){
					puupdhist = hmtmp->Get("puupdhist");
					puupdhistUp = hmtmp->Get("puupdhistUp");
					puupdhistDown = hmtmp->Get("puupdhistDown");
				}
				else {
					TFile* pufile2 = KOpen(puname2);
					hmtmp = new HistoMap();
					puupdhist = KGet<TH1>(pufile2,"data_pu_central"); puupdhist->SetDirectory(0);
					puupdhistUp = KGet<TH1>(pufile2,"data_pu_up"); puupdhistUp->SetDirectory(0);
					puupdhistDown = KGet<TH1>(pufile2,"data_pu_down"); puupdhistDown->SetDirectory(0);
					pufile2->Close();

					TFile* pufile1 = KOpen(puname1);
					//correct puWeight branch (data1/mc) by data2/data1 to get data2/mc
					puupdhist->Divide(KGet<TH1>(pufile1,"data_pu_central"));
					puupdhistUp->Divide(KGet<TH1>(pufile1,"data_pu_up"));
					puupdhistDown->Divide(KGet<TH1>(pufile1,"data_pu_down"));
					pufile1->Close();

					hmtmp->Add("puupdhist",puupdhist);
					hmtmp->Add("puupdhistUp",puupdhistUp);
					hmtmp->Add("puupdhistDown",puupdhistDown);
				}
			}
			
			//alternative PU option - acceptance uncertainty
			puacccorr = localOpt->Get("puacccorr",false);
			puaccunc = 0; localOpt->Get("puaccunc",puaccunc);
			if(puacccorr){
				TH1* h_nvtx = NULL;
				string nvtxname = ""; localOpt->Get("nvtxname",nvtxname);
				TH1* htmp = nvtxhistMap().Get(nvtxname);
				if(nvtxname.empty()){
					throw runtime_error("no nvtx distribution file specified!");
				}
				else if(htmp){
					h_nvtx = htmp;
				}
				else{
					TFile* nvtxfile = KOpen(nvtxname);
					h_nvtx = KGet<TH1>(nvtxfile,"nvertex_SingleElectron"); h_nvtx->SetDirectory(0); nvtxhistMap().Add(nvtxname,h_nvtx);
					nvtxfile->Close();
				}
				puacc.SetInputs(KGet<TGraphErrors>(base->GetFile(),"pileupAccBand"),h_nvtx,localOpt->Get("debugpuacc",false));
			}
			
			//trig corr options
			trigcorr = localOpt->Get("trigcorr",false);
			trigunc = 0; localOpt->Get("trigunc", trigunc);
			if(trigcorr){
				string trigfile; localOpt->Get("trigfile",trigfile);
				string trigeff; localOpt->Get("trigeff",trigeff);
				trigcorror.SetEff(trigfile,trigeff);
				trigcorror.debug = localOpt->Get("trigdebug",false);
			}
			trigsystcorrval = 1;
			trigsystcorr = localOpt->Get("trigsystcorr",false);
			int trigsystunc = 0; localOpt->Get("trigsystunc", trigsystunc);
			vector<double> trigsystcorrvals;
			if(localOpt->Get("trigsystcorrvals",trigsystcorrvals) && trigsystcorrvals.size()==3){
				//vector has down, central, up
				trigsystcorrval = trigsystunc==0 ? trigsystcorrvals[1] : ( trigsystunc==1 ? trigsystcorrvals[2] : trigsystcorrvals[0] );
			}
			trigfncorr = localOpt->Get("trigfncorr",false);
			trigfnunc = 0; localOpt->Get("trigfnunc",trigfnunc);
			if(trigfncorr){
				string trigfnfile; localOpt->Get("trigfnfile",trigfnfile);
				string trigfneff; localOpt->Get("trigfneff",trigfneff);
				string trigfnerr; localOpt->Get("trigfnerr",trigfnerr);
				trigfncorror.SetFunc(trigfnfile,trigfneff,trigfnerr);
			}
			
			//ISR corr options
			isrcorr = localOpt->Get("isrcorr",false);
			base->GetLocalOpt()->Get("mother",mother);
			isrunc = 0; localOpt->Get("isrunc", isrunc);
			useisrflat = localOpt->Get("isrflat", isrflat);
			if(isrcorr and not useisrflat){
				TH1* isrhist = NULL;
				TH1* isrhistUp = NULL;
				TH1* isrhistDown = NULL;
				string isrname = ""; localOpt->Get("isrname",isrname);
				HistoMap* hmtmp = isrhistMap().Get(isrname);
				if(isrname.empty()){
					throw runtime_error("no ISR weight file specified!");
				}
				else if(hmtmp){
					isrhist = hmtmp->Get("isrhist");
					isrhistUp = hmtmp->Get("isrhistUp");
					isrhistDown = hmtmp->Get("isrhistDown");
				}
				else{
					//store correction files centrally
					TFile* isrfile = KOpen(isrname);
					hmtmp = new HistoMap();
					isrhist = KGet<TH1>(isrfile,"isr_weights_central"); isrhist->SetDirectory(0); hmtmp->Add("isrhist",isrhist);
					isrhistUp = KGet<TH1>(isrfile,"isr_weights_up"); isrhistUp->SetDirectory(0); hmtmp->Add("isrhistUp",isrhistUp);
					isrhistDown = KGet<TH1>(isrfile,"isr_weights_down"); isrhistDown->SetDirectory(0); hmtmp->Add("isrhistDown",isrhistDown);
					isrhistMap().Add(isrname,hmtmp);
					isrfile->Close();
				}
				TH1* isrtmp = NULL;
				if(isrunc==1) isrtmp = isrhistUp;
				else if(isrunc==-1) isrtmp = isrhistDown;
				else isrtmp = isrhist;
				isrcorror.SetWeights(isrtmp,KGet<TH1>(base->GetFile(),"NJetsISR"));
			}
			
			//flattening options
			flatten = localOpt->Get("flatten",false);
			if(flatten){
				flatqty = noflatqty;
				string flatname; localOpt->Get("flatname",flatname);
				TFile* flatfile = KOpen(flatname);

				localOpt->Get("flatqty",sflatqty);
				string flatsuff;
				if(!base->GetLocalOpt()->Get("flatsuff",flatsuff)) flatsuff = base->GetName();
				string flatdist = sflatqty + "_" + flatsuff;
				TH1* flathist = KGet<TH1>(flatfile,flatdist); flathist->SetDirectory(0);

				//optional numer
				string flatnumer; localOpt->Get("flatnumer",flatnumer);
				TH1* flatnumerhist = NULL;
				if(!flatnumer.empty()) {
					string flatnumerdist = sflatqty + "_" + flatnumer;
					flatnumerhist = KGet<TH1>(flatfile,flatnumerdist);
					flatnumerhist->SetDirectory(0);
				}

				flatfile->Close();
				flattener.SetDist(flathist,flatnumerhist);
					
				//get enum for flatqty
				if(sflatqty=="leadjetAK8pt") flatqty = leadjetAK8pt;
				else if(sflatqty=="subleadjetAK8pt") flatqty = subleadjetAK8pt;
				else if(sflatqty=="bothjetAK8pt") flatqty = bothjetAK8pt;
				else if(sflatqty=="thirdjetAK8pt") flatqty = thirdjetAK8pt;
				else if(sflatqty=="fourthjetAK8pt") flatqty = fourthjetAK8pt;
				else throw runtime_error("unknown flatqty " + sflatqty);
			}

			//svb weighting options
			svbweight = localOpt->Get("svbweight",false);
			hsvb = NULL;
			if(svbweight){
				svbqty = nosvbqty;
				string svbname; localOpt->Get("svbname",svbname);
				string svbnumer; localOpt->Get("svbnumer",svbnumer);
				string svbdenom; localOpt->Get("svbdenom",svbdenom);
				TFile* svbfile = KOpen(svbname);
				if(svbfile and !svbnumer.empty() and !svbdenom.empty()){
					//get enum
					string ssvbqty; localOpt->Get("svbqty",ssvbqty);
					if(ssvbqty=="MTAK8") svbqty = MTAK8;

					//use ratio object w/ built-in calculations
					TH1F* numer = KGet<TH1F>(svbfile,ssvbqty+"_"+svbnumer);
					TH1F* denom = KGet<TH1F>(svbfile,ssvbqty+"_"+svbdenom);
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

			//prefire corr options
			prefirecorr = localOpt->Get("prefirecorr",false);
			prefireunc = 0; localOpt->Get("prefireunc",prefireunc);

			//hem veto options
			hemvetocorr = localOpt->Get("hemvetocorr",false);
			hemvetounc = 0; localOpt->Get("hemvetounc",hemvetounc);

			//lepton SF options
			lepcorr = localOpt->Get("lepcorr",false);
			lepidunc = 0; localOpt->Get("lepidunc",lepidunc);
			lepisounc = 0; localOpt->Get("lepisounc",lepisounc);
			leptrkunc = 0; localOpt->Get("leptrkunc",leptrkunc);
			if(lepcorr){
				int lepyear; localOpt->Get("lepyear",lepyear);
				if(lepyear==2016){
					elcorrors = {
						LeptonCorrector(LeptonCorrector::LCtype::id,
							"corrections/leptonSFs/ElectronScaleFactors_Run2016_190404.root",
							"Run2016_CutBasedVetoNoIso94XV2",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							lepidunc
						),
						LeptonCorrector(LeptonCorrector::LCtype::iso,
							"corrections/leptonSFs/ElectronScaleFactors_Run2016_190404.root",
							"Run2016_Mini",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							lepisounc
						),
						LeptonCorrector(LeptonCorrector::LCtype::trk,
							"corrections/leptonSFs/EGM2D_BtoH_low_RecoSF_Legacy2016_190404.root",
							"EGamma_SF2D",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							leptrkunc,
							0., -1, 20
						),
						LeptonCorrector(LeptonCorrector::LCtype::trk,
							"corrections/leptonSFs/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016_190404.root",
							"EGamma_SF2D",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							leptrkunc,
							0., 20, -1
						),
					};
					mucorrors = {
						LeptonCorrector(LeptonCorrector::LCtype::id,
							"corrections/leptonSFs/RunBCDEFGH_weightedAvg_SF_ID_Mu_2016_190404.root",
							"NUM_MediumID_DEN_genTracks_eta_pt",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							lepidunc,
							0.017
						),
						LeptonCorrector(LeptonCorrector::LCtype::iso,
							"corrections/leptonSFs/RunBCDEFGH_weightedAvg_SF_ISO_Mu_2016_190404.root",
							"NUM_TightRelIso_DEN_MediumID_eta_pt",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							lepisounc,
							0.017
						),
					};
				}
				else if(lepyear==2017){
					elcorrors = {
						LeptonCorrector(LeptonCorrector::LCtype::id,
							"corrections/leptonSFs/ElectronScaleFactors_Run2017_190404.root",
							"Run2017_CutBasedVetoNoIso94XV2",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							lepidunc
						),
						LeptonCorrector(LeptonCorrector::LCtype::iso,
							"corrections/leptonSFs/ElectronScaleFactors_Run2017_190404.root",
							"Run2017_MVAVLooseTightIP2DMini",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							lepisounc
						),
						LeptonCorrector(LeptonCorrector::LCtype::trk,
							"corrections/leptonSFs/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_lowEt_2017_190404.root",
							"EGamma_SF2D",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							leptrkunc,
							0., -1, 20
						),
						LeptonCorrector(LeptonCorrector::LCtype::trk,
							"corrections/leptonSFs/egammaEffi.txt_EGM2D_runBCDEF_passingRECO_2017_190404.root",
							"EGamma_SF2D",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							leptrkunc,
							0., 20, -1
						)
					};
					mucorrors = {
						//trk SF included w/ id SF
						LeptonCorrector(LeptonCorrector::LCtype::id,
							"corrections/leptonSFs/RunBCDEF_SF_ID_Mu_2017_190404.root",
							"NUM_MediumID_DEN_genTracks_pt_abseta",
							{LeptonCorrector::LCaxes::pt, LeptonCorrector::LCaxes::abseta},
							lepidunc,
							0.017
						),
						LeptonCorrector(LeptonCorrector::LCtype::iso,
							"corrections/leptonSFs/RunBCDEF_SF_ISO_Mu_2017_190404.root",
							"NUM_TightRelIso_DEN_MediumID_pt_abseta",
							{LeptonCorrector::LCaxes::pt, LeptonCorrector::LCaxes::abseta},
							lepisounc
						)
					};
				}
				else if(lepyear==2018){
					elcorrors = {
						LeptonCorrector(LeptonCorrector::LCtype::id,
							"corrections/leptonSFs/ElectronScaleFactors_Run2018_190404.root",
							"Run2018_CutBasedVetoNoIso94XV2",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							lepidunc
						),
						LeptonCorrector(LeptonCorrector::LCtype::iso,
							"corrections/leptonSFs/ElectronScaleFactors_Run2018_190404.root",
							"Run2018_Mini",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							lepisounc
						),
						LeptonCorrector(LeptonCorrector::LCtype::trk,
							"corrections/leptonSFs/egammaEffi.txt_EGM2D_updatedAll_Above10GeV_2018_190404.root",
							"EGamma_SF2D",
							{LeptonCorrector::LCaxes::eta, LeptonCorrector::LCaxes::pt},
							leptrkunc,
							0., 10, -1
						)
					};
					mucorrors = {
						//trk SF included w/ id SF
						LeptonCorrector(LeptonCorrector::LCtype::id,
							"corrections/leptonSFs/RunABCD_SF_ID_Mu_2018_190404.root",
							"NUM_MediumID_DEN_TrackerMuons_pt_abseta",
							{LeptonCorrector::LCaxes::pt, LeptonCorrector::LCaxes::abseta},
							lepidunc,
							0.017
						),
						LeptonCorrector(LeptonCorrector::LCtype::iso,
							"corrections/leptonSFs/RunABCD_SF_ISO_Mu_2018_190404.root",
							"NUM_TightRelIso_DEN_MediumID_pt_abseta",
							{LeptonCorrector::LCaxes::pt, LeptonCorrector::LCaxes::abseta},
							lepisounc
						)
					};
				}
			}
			
			//other uncertainty options
			pdfunc = 0; localOpt->Get("pdfunc",pdfunc);
			scaleunc = 0; localOpt->Get("scaleunc",scaleunc);
			if(pdfunc!=0 || scaleunc!=0){
				//get the normalizations for pdf/scale uncertainties
				TH1F* h_norm = KGet<TH1F>(base->GetFile(),"PDFNorm");
				pdfnorms = vector<double>(4,1.0);
				//0: PDF up, 1: PDF down, 2: scale up, 3: scale down
				double nominal = h_norm->GetBinContent(1);
				for(unsigned n = 0; n < 4; ++n){
					//(bin in histo = index + 2)
					pdfnorms[n] = nominal/h_norm->GetBinContent(n+2);
				}
			}
			pdfallunc = 0; localOpt->Get("pdfallunc",pdfallunc);
			if(pdfallunc!=0){
				TH1F* h_norm = KGet<TH1F>(base->GetFile(),"PDFAllNorm");
				int nEventProc = h_norm->GetBinContent(1);
				pdfallnorms = vector<double>(h_norm->GetNbinsX()-1,0.);
				for(int n = 2; n <= h_norm->GetNbinsX(); ++n){
					pdfallnorms[n-2] = double(nEventProc)/h_norm->GetBinContent(n);
				}
			}
		}
		virtual void ListBranches(){
			//force enable branches needed for cuts/weights/etc.
			branches.push_back("Weight"); //needed for negative weights even if useTreeWeight==false
			if(useTreeXsec) branches.push_back("CrossSection");
			if(internalNormType) NormType->CheckBranches();
			if(pucorr){
				branches.push_back("TrueNumInteractions");
				if(putree){
					punew = puunc==1 ? looper->fChain->GetBranch("puSysUpNew") : puunc==-1 ? looper->fChain->GetBranch("puSysDownNew") : looper->fChain->GetBranch("puWeightNew");
					if(puunc==1) branches.push_back(punew ? "puSysUpNew" : "puSysUp");
					else if(puunc==-1) branches.push_back(punew ? "puSysDownNew" : "puSysDown");
					else branches.push_back(punew ? "puWeightNew" : "puWeight");
				}
			}
			if(puupdcorr){
				branches.push_back("TrueNumInteractions");
				if(puupdunc==1) branches.push_back("puSysUp");
				else if(puupdunc==-1) branches.push_back("puSysDown");
				else branches.push_back("puWeight");
			}
			if(isrcorr){
				branches.push_back("NJetsISR");
			}
			if(flatten){
				if(flatqty>noflatqty and flatqty<=fourthjetAK8pt) branches.push_back("JetsAK8");
			}
			if(svbweight){
				if(svbqty==MTAK8) branches.push_back("MT_AK8");
			}
			if(pdfunc!=0 or pdfallunc!=0){
				branches.push_back("PDFweights");
			}
			if(scaleunc!=0){
				branches.push_back("ScaleWeights");
			}
			if(trigfncorr){
				branches.push_back("MT_AK8");
			}
			if(prefirecorr){
				if(prefireunc==0) branches.push_back("NonPrefiringProb");
				else if(prefireunc>0) branches.push_back("NonPrefiringProbUp");
				else if(prefireunc<0) branches.push_back("NonPrefiringProbDown");
			}
			if(hemvetocorr){
				if(hemvetounc<1) branches.push_back("HEMDPhiVetoFilter");
			}
			if(lepcorr){
				branches.insert(branches.end(),{
					"GenElectrons",
					"GenMuons",
					"NElectrons",
					"Electrons",
					"Electrons_passIso",
					"NMuons",
					"Muons",
					"Muons_mediumID",
					"Muons_passIso",
				});
			}
		}
		double GetBinContentBounded(TH1* hist, double val){
			return hist->GetBinContent(hist->GetXaxis()->FindBin(min(val,hist->GetBinLowEdge(hist->GetNbinsX()+1))));
		}
		double GetWeight(){
			double w = 1.;
			if(unweighted) return w;
			
			//check option in case correction types are disabled globally
			
			if(pucorr) {
				//use TreeMaker weights if no histo provided
				if(puunc==1) w *= puhistUp ? GetBinContentBounded(puhistUp,looper->TrueNumInteractions) : punew ? looper->puSysUpNew : looper->puSysUp;
				else if(puunc==-1) w *= puhistDown ? GetBinContentBounded(puhistDown,looper->TrueNumInteractions) : punew ? looper->puSysDownNew : looper->puSysDown;
				else w *= puhist ? GetBinContentBounded(puhist,looper->TrueNumInteractions) : punew ? looper->puWeightNew : looper->puWeight;
			}

			if(puupdcorr){
				if(puupdunc==1) w *= (puupdhistUp ? GetBinContentBounded(puupdhistUp,looper->TrueNumInteractions) : 1.0) * looper->puSysUp;
				else if(puupdunc==-1) w *= (puupdhistDown ? GetBinContentBounded(puupdhistDown,looper->TrueNumInteractions) : 1.0) * looper->puSysDown;
				else w *= (puupdhist ? GetBinContentBounded(puupdhist,looper->TrueNumInteractions) : 1.0) * looper->puWeight;
			}
			
			if(puacccorr){
				w *= puacc.GetCorrection(puaccunc);
			}
			
			if(trigcorr){
//				BNN not yet available
//				unsigned effindex = trigunc==-1 ? 2 : trigunc;
//				w *= Eff_MetMhtSextetReal_CenterUpDown(looper->HT, looper->MHT, looper->NJets)[effindex];
				//special case: no correction, 5% unc
				if(looper->NJets==2){
					if(trigunc>0) w *= 1.05;
					else if(trigunc<0) w *= .95;
				}
				else {
					w *= trigcorror.GetCorrection(looper->HT,trigunc);
				}
			}

			if(trigfncorr){
				w *= trigfncorror.GetCorrection(looper->MT_AK8,trigfnunc);
			}
			
			if(trigsystcorr){
				w *= trigsystcorrval;
			}

			if(isrcorr){
				if(useisrflat){
					if(isrunc<0) w *= (1 - isrflat);
					else if(isrunc>0) w *= (1 + isrflat);
				}
				else w *= isrcorror.GetCorrection(looper->NJetsISR);
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
			
			if(pdfallunc!=0){
				auto PDFweightsNorm = *looper->PDFweights;
				std::transform(looper->PDFweights->begin(),looper->PDFweights->end(),pdfallnorms.begin(),PDFweightsNorm.begin(),multiplies<double>());
				double mean, rms;
				tie(mean,rms) = KMath::MeanRMS(PDFweightsNorm.begin(),PDFweightsNorm.end());
				w *= (pdfallunc>0 ? mean+rms : mean-rms);
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

			if(prefirecorr){
				if(prefireunc==0) w *= looper->NonPrefiringProb;
				else if(prefireunc>0) w *= looper->NonPrefiringProbUp;
				else if(prefireunc<0) w *= looper->NonPrefiringProbDown;
			}

			if(hemvetocorr){
				//nominal: hem veto, up: no veto, dn: hem veto - (no veto - hem veto) = 2*hem veto - no veto
				//to implement uncDown on per-event basis, remove vetoed event "twice" (weight of 0 = removed, weight of -1 = removed again)
				if(hemvetounc==0) w *= looper->HEMDPhiVetoFilter;
				else if(hemvetounc>0) w *= 1;
				else if(hemvetounc<0) {
					if(!looper->HEMDPhiVetoFilter) w *= -1;
					else w *= 1;
				}
			}

			if(lepcorr){
				//assume SL CR
				double gen_pt_min = 5;
				double gen_eta_max = 2.5;
				double delta_pt_max = 0.1;
				double delta_r_max = 0.03;
				if(looper->NElectrons==1){
					vector<vector<bool>> masks{*looper->Electrons_passIso};
					auto matched = LeptonCorrector::MatchGenRec(
						*looper->GenElectrons, gen_pt_min, gen_eta_max,
						*looper->Electrons, masks, delta_pt_max, delta_r_max
					);
					if(!matched.empty()){
						for(const auto& cor : elcorrors){
							w *= cor.GetSF(matched[0]);
						}
					}
				}
				else if(looper->NMuons==1){
					vector<vector<bool>> masks{*looper->Muons_mediumID,*looper->Muons_passIso};
					auto matched = LeptonCorrector::MatchGenRec(
						*looper->GenMuons, gen_pt_min, gen_eta_max,
						*looper->Muons, masks, delta_pt_max, delta_r_max
					);
					if(!matched.empty()){
						for(const auto& cor : mucorrors){
							w *= cor.GetSF(matched[0]);
						}
					}
				}
			}

			if(svbweight){
				double qty = 0;
				if(svbqty==MTAK8) qty = looper->MT_AK8;
				w *= hsvb->GetBinContent(hsvb->GetXaxis()->FindBin(min(qty,hsvb->GetBinLowEdge(hsvb->GetNbinsX()+1))));
			}
			
			//now do scaling: norm*xsection/nevents
			if(useTreeWeight && !fastsim) w *= looper->Weight;
			else if(got_nEventProc && nEventProc>0 && (got_xsection or useTreeXsec)){
				if(useTreeXsec and !got_xsection) xsection = looper->CrossSection;

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

			if(internalNormType) goodEvent &= NormType->Cut();
			
			return goodEvent;
		}
		
		//member variables
		KNormTypeSelector* NormType;
		bool internalNormType;
		bool unweighted, got_nEventProc, got_xsection, got_luminorm, useTreeWeight, useTreeXsec, useKFactor, debugWeight, didDebugWeight;
		bool pucorr, putree, punew, puupdcorr, trigcorr, trigsystcorr, trigfncorr, isrcorr, useisrflat, fastsim, jetidcorr, isotrackcorr, lumicorr, btagcorr, puacccorr, flatten, svbweight, prefirecorr, hemvetocorr, lepcorr;
		double jetidcorrval, isotrackcorrval, trigsystcorrval, lumicorrval, isrflat;
		int puunc, puupdunc, pdfunc, pdfallunc, isrunc, scaleunc, trigunc, trigyear, trigfnunc, btagSFunc, mistagSFunc, btagCFunc, ctagCFunc, mistagCFunc, puaccunc, prefireunc, hemvetounc, lepidunc, lepisounc, leptrkunc;
		vector<int> mother;
		TH1 *puhist, *puhistUp, *puhistDown;
		TH1 *puupdhist, *puupdhistUp, *puupdhistDown;
		vector<double> pdfnorms, pdfallnorms;
		int nEventProc;
		double xsection, norm, kfactor;
		ISRCorrector isrcorror;
		TriggerCorrector trigcorror;
		TriggerFuncCorrector trigfncorror;
		vector<LeptonCorrector> elcorrors, mucorrors;
		PileupAcceptanceUncertainty puacc;
		Flattener flattener;
		string sflatqty;
		flatqtys flatqty;
		svbqtys svbqty;
		TH1* hsvb;
};
REGISTER_SELECTOR(MCWeight);

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
		virtual void ListBranches(){
			branches.push_back("JetsAK8");
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			if(looper->JetsAK8->size()<njet) return false;
			double jetpt = looper->JetsAK8->at(njet-1).Pt();
			return ptmin < jetpt and jetpt <= ptmax;
		}
		
		//member variables
		unsigned njet;
		double ptmin, ptmax;
};
REGISTER_SELECTOR(PTRange);

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
		virtual void ListBranches(){
			branches.push_back("Jets");
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
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"RunNum",
				"EvtNum",
			});
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
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"Photons",
				"Photons_hadTowOverEM",
				"Photons_hasPixelSeed",
				"Photons_isEB",
				"Photons_pfChargedIsoRhoCorr",
				"Photons_pfGammaIsoRhoCorr",
				"Photons_pfNeutralIsoRhoCorr",
			});
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
		virtual void ListBranches(){ branches.push_back("NVtx"); }
		virtual bool Cut() { return looper->NVtx > 0; }
};
REGISTER_SELECTOR(NVtxFilter);

class KeeBadScFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void ListBranches(){ branches.push_back("eeBadScFilter"); }
		virtual bool Cut() { return looper->eeBadScFilter==1; }
};
REGISTER_SELECTOR(eeBadScFilter);

class KEcalDeadCellTriggerPrimitiveFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void ListBranches(){ branches.push_back("EcalDeadCellTriggerPrimitiveFilter"); }
		virtual bool Cut() { return looper->EcalDeadCellTriggerPrimitiveFilter==1; }
};
REGISTER_SELECTOR(EcalDeadCellTriggerPrimitiveFilter);

class KHBHENoiseFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void ListBranches(){ branches.push_back("HBHENoiseFilter"); }
		virtual bool Cut() { return looper->HBHENoiseFilter==1; }
};
REGISTER_SELECTOR(HBHENoiseFilter);

class KHBHEIsoNoiseFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void ListBranches(){ branches.push_back("HBHEIsoNoiseFilter"); }
		virtual bool Cut() { return looper->HBHEIsoNoiseFilter==1; }
};
REGISTER_SELECTOR(HBHEIsoNoiseFilter);

class KglobalSuperTightHalo2016FilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void ListBranches(){ branches.push_back("globalSuperTightHalo2016Filter"); }
		virtual bool Cut() { return looper->globalSuperTightHalo2016Filter==1; }
};
REGISTER_SELECTOR(globalSuperTightHalo2016Filter);

class KBadChargedCandidateFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void ListBranches(){ branches.push_back("BadChargedCandidateFilter"); }
		virtual bool Cut() { return looper->BadChargedCandidateFilter==1; }
};
REGISTER_SELECTOR(BadChargedCandidateFilter);

class KBadPFMuonFilterSelector : public KSelector {
	public:
		using KSelector::KSelector;
		virtual void ListBranches(){ branches.push_back("BadPFMuonFilter"); }
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
		virtual void ListBranches(){
			branches.push_back("Jets");
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
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"MHT_Phi",
				"Jets_HTMask",
				"Jets",
				"Jets_bDiscriminatorCSV",
			});
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
			
			localOpt->Get("calib",calib);
			localOpt->Get("calibfast",calibfast);
		}
		virtual void CheckDeps(){
			//set dependencies here
			MCWeight = sel->Get<KMCWeightSelector*>("MCWeight");
			if(!MCWeight) depfailed = true;
		}
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"Jets_HTMask",
				"Jets",
				"Jets_hadronFlavor",
			});
			if(debug) branches.push_back("BTagsDeepCSV");
		}
		virtual void CheckBase(){
			//don't even bother
			if(depfailed) return;

			//initialize btag corrector calibrations
			btagcorr.SetCalib(calib);

			//check for option
			int btagSFunc = MCWeight->btagSFunc; btagcorr.SetBtagSFunc(btagSFunc);
			int mistagSFunc = MCWeight->mistagSFunc; btagcorr.SetMistagSFunc(mistagSFunc);
			
			//get efficiency histograms
			btagcorr.SetEffs(base->GetFile());
			
			//check fastsim stuff
			bool fastsim = base->GetLocalOpt()->Get("fastsim",false);
			if(fastsim){
				//initialize btag corrector fastsim calibrations
				//todo: check the sample name and choose the appropriate CFs (once available)
				if(!calibfast.empty()) {
					btagcorr.SetFastSim(true);
					btagcorr.SetCalibFastSim(calibfast);
				
					//check for option
					int btagCFunc = MCWeight->btagCFunc; btagcorr.SetBtagCFunc(btagCFunc);
					int ctagCFunc = MCWeight->ctagCFunc; btagcorr.SetCtagCFunc(ctagCFunc);
					int mistagCFunc = MCWeight->mistagCFunc;  btagcorr.SetMistagCFunc(mistagCFunc);
				}
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
				KParser::printvec(prob,cout," ");
				cout << endl;
			}
			return true;
		}
		
		//member variables
		int debug;
		string calib, calibfast;
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
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"GenElectrons",
				"GenMuons",
				"GenTaus",
			});
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
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"JetsAK8",
				"GenParticles",
				"GenParticles_PdgId",
				"GenParticles_ParentId",
			});
			if(useSubjets) branches.push_back("JetsAK8_subjets");
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
//get gen-level MET quantities from dark hadrons
class KDarkHadronMETSelector : public KSelector {
	public:
		//constructor
		KDarkHadronMETSelector() : KSelector() { }
		KDarkHadronMETSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(2) {
			canfail = false;
			//check options
			localOpt->Get("njet",njet);
		}
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"JetsAK8",
				"GenParticles",
				"GenParticles_PdgId",
				"GenParticles_ParentIdx",
			});
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			unsigned njet_ = min(njet,(unsigned)looper->JetsAK8->size());
			//clear
			JetsAK8_dark.clear(); JetsAK8_dark.reserve(njet_);
			DarkMass = 0.; DarkPt = 0.;

			//make list of stable dark hadrons
			unordered_set<int> dark_indices;
			for(unsigned g = 0; g < looper->GenParticles->size(); ++g){
				int pdgid = abs(looper->GenParticles_PdgId->at(g));
				//check only DM particles
				if(pdgid != 51 and pdgid != 52 and pdgid != 53) continue;
				//store parent index
				dark_indices.insert(looper->GenParticles_ParentIdx->at(g));
			}

			//associate to jets
			TLorentzVector vall;
			for(unsigned j = 0; j < njet_; ++j){
				TLorentzVector vgen;
				for(auto g : dark_indices){
					if(g<0) continue;
					int pdgid = abs(looper->GenParticles_PdgId->at(g));
					if(abs(pdgid-4900000)>1000) continue;
					double mindR = 0.8;
					double dR = KMath::DeltaR(looper->JetsAK8->at(j).Phi(),looper->JetsAK8->at(j).Eta(),
											  looper->GenParticles->at(g).Phi(),looper->GenParticles->at(g).Eta());
					if(dR<mindR){
						vgen += looper->GenParticles->at(g);
					}
				}
				JetsAK8_dark.push_back(vgen);
				vall += vgen;
			}
			DarkMass = vall.M();
			DarkPt = vall.Pt();
			return true;
		}
		
		//member variables
		unsigned njet;
		vector<TLorentzVector> JetsAK8_dark;
		double DarkMass, DarkPt;
};
REGISTER_SELECTOR(DarkHadronMET);

//---------------------------------------------------------------
//associate gen-level info with reco jets
class KGenMatchSelector : public KSelector {
	public:
		//constructor
		KGenMatchSelector() : KSelector() { }
		KGenMatchSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), njet(2), sigversion(3) {
			canfail = false;
			//check options
			localOpt->Get("njet",njet);
		}
		virtual void CheckBase(){
			base->GetLocalOpt()->Get("sigversion",sigversion);
		}
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"JetsAK8",
				"GenJetsAK8",
				"GenParticles",
				"GenParticles_PdgId",
				"GenParticles_ParentId",
				"GenParticles_ParentIdx",
				"GenParticles_Status",
			});
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			unsigned njet_ = min(njet,(unsigned)looper->JetsAK8->size());
			//clear
			GenJetsAK8.clear(); GenJetsAK8.resize(njet_);
			GenNuAK8.clear(); GenNuAK8.resize(njet_);
			GenHVAK8.clear(); GenHVAK8.resize(njet_);
			GenQuarkAK8.clear(); GenQuarkAK8.resize(njet_);

			//make lists of indices
			unordered_set<int> nu_indices, meson_indices, quark_indices;
			for(unsigned g = 0; g < looper->GenParticles->size(); ++g){
				const auto& GenPart = looper->GenParticles->at(g);
				int absid = abs(looper->GenParticles_PdgId->at(g));
				if(absid==12 or absid==14 or absid==16) {
					nu_indices.insert(g);
				}
				//handle both versions of invisible HV particles
				else if( (sigversion==3 and (absid==51 or absid==52 or absid==53)) or (sigversion==2 and absid==4900211)){
					//save the parent
					meson_indices.insert(looper->GenParticles_ParentIdx->at(g));
				}
				else if((absid==4900101 or absid==4900102) and looper->GenParticles_Status->at(g)==23){
					quark_indices.insert(g);
				}
			}
			

			for(unsigned j = 0; j < njet_; ++j){
				const auto& Jet = looper->JetsAK8->at(j);
				//match gen jets to reco jets
				double mindR = 100.0;
				for(const auto& GenJet: *looper->GenJetsAK8){
					double dR = GenJet.DeltaR(Jet);
					if(dR<mindR){
						mindR = dR;
						GenJetsAK8[j] = GenJet;
					}
				}

				//match neutrinos to GenJets
				for(auto g : nu_indices){
					const auto& GenPart = looper->GenParticles->at(g);
					if(GenJetsAK8[j].DeltaR(GenPart)<0.8) GenNuAK8[j] += GenPart;
				}

				//match HVs to GenJets
				for(auto g : meson_indices){
					const auto& GenPart = looper->GenParticles->at(g);
					if(GenJetsAK8[j].DeltaR(GenPart)<0.8) GenHVAK8[j] += GenPart;
				}

				//match quarks to GenJets
				mindR = 100.0;
				for(auto g : quark_indices){
					const auto& GenPart = looper->GenParticles->at(g);
					double dR = GenJetsAK8[j].DeltaR(GenPart);
					if(dR < mindR){
						mindR = dR;
						GenQuarkAK8[j] = GenPart;
					}
				}
			}
			return true;
		}
		
		//member variables
		unsigned njet;
		int sigversion;
		vector<TLorentzVector> GenJetsAK8, GenNuAK8, GenHVAK8, GenQuarkAK8;
};
REGISTER_SELECTOR(GenMatch);

//----------------------------------------------------
//selects events based on leading jet pt
class KMTRegressionSelector : public KSelector {
	public:
		//constructor
		KMTRegressionSelector() : KSelector() { }
		KMTRegressionSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), version(1), MTAK8(0.) { 
			canfail = false;
			//check for option
			localOpt->Get("version",version);
		}
		virtual void CheckDeps(){
			JetMatch = sel->Get<KJetMatchSelector*>("JetMatch");
			if(version==1 and !JetMatch) depfailed = true;			
		}
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"JetsAK8",
				"MET",
				"METPhi",
			});
			if(version==1) branches.push_back("Jets_bJetTagDeepCSVBvsAll");
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			MTAK8 = 0.;

			//apply regression to jet 4vecs, recalculate MT
			if(looper->JetsAK8->size()>1){
				array<TLorentzVector,2> vj;
				array<double,2> params;
				if(version==1) params = {0.0904739,0.125758};

				for(unsigned j = 0; j < 2; ++j){
					vj[j] = looper->JetsAK8->at(j);
					if(version==1){
						vector<double> discrs; discrs.reserve(JetMatch->JetIndices[j].size());
						for(auto jj : JetMatch->JetIndices[j]) discrs.push_back(looper->Jets_bJetTagDeepCSVBvsAll->at(jj));
						vj[j] *= max(1.0, params[j]*sqrt(*(TMath::LocMax(discrs.begin(),discrs.end())))+1.0);
					}
				}

				TLorentzVector vjj = vj[0] + vj[1];
				MTAK8 = KMath::TransverseMass(vjj.Px(),vjj.Py(),vjj.M(),looper->MET*cos(looper->METPhi),looper->MET*sin(looper->METPhi),0);
			}

			return true;
		}
		
		//member variables
		int version;
		KJetMatchSelector* JetMatch;
		double MTAK8;
};
REGISTER_SELECTOR(MTRegression);

//----------------------------------------------------
//compute SVJ tags using BDT values
class KSVJTagSelector : public KSelector {
	public:
		enum svjbranches { bdt = 0, seltor = 2 };
		//constructor
		KSVJTagSelector() : KSelector() { }
		KSVJTagSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), branch(""), cut(0.), BDT(NULL) { 
			//check for option
			localOpt->Get("branch",branch);
			if(branch=="bdtSVJtag") ebranch = bdt;
			else if(branch.empty()) ebranch = seltor;
			else throw runtime_error("Unknown bdt branch: "+branch);
			localOpt->Get("cut",cut);
			canfail = false;
		}
		virtual void ListBranches(){
			if(!branch.empty()) branches.push_back("JetsAK8_"+branch);
		}
		virtual void CheckDeps(){
			if(ebranch==seltor){
				BDT = sel->Get<KBDTSelector*>("BDT");
				if(!BDT) depfailed = true;
			}
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			//reset
			ntags = 0;
			JetsAK8_tagged.clear();
			const auto& SVJTag = ebranch==seltor ? BDT->JetsAK8_bdt : *looper->JetsAK8_bdtSVJtag;
			JetsAK8_tagged.reserve(SVJTag.size());
			for(unsigned j = 0; j < min(SVJTag.size(),2ul); ++j){
				JetsAK8_tagged.push_back(SVJTag[j] > cut);
				if(JetsAK8_tagged.back()) ++ntags;
			}

			return true;
		}
		
		//member variables
		string branch;
		double cut;
		unsigned ntags;
		vector<bool> JetsAK8_tagged;
		svjbranches ebranch;
		KBDTSelector* BDT;
};
REGISTER_SELECTOR(SVJTag);

//----------------------------------------------------
//filter jets based on svj tag for per-jet histos
class KSVJFilterSelector : public KSelector {
	public:
		//constructor
		KSVJFilterSelector() : KSelector() { }
		KSVJFilterSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), filter(0), SVJTag(NULL) { 
			//check for option
			//1 selects tagged, -1 selects anti-tagged
			localOpt->Get("filter",filter);
			canfail = false;
		}
		virtual void CheckDeps(){
			SVJTag = sel->Get<KSVJTagSelector*>("SVJTag");
			if(!SVJTag) depfailed = true;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return true;
		}
		
		//member variables
		int filter;
		KSVJTagSelector* SVJTag;
};
REGISTER_SELECTOR(SVJFilter);

//----------------------------------------------------
//select number of SVJ tags
class KNSVJSelector : public KSelector {
	public:
		//constructor
		KNSVJSelector() : KSelector() { }
		KNSVJSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), source(""), num(0), SVJTag(NULL) { 
			//check for option
			localOpt->Get("source",source);
			localOpt->Get("num",num);
			canfail = false;
		}
		virtual void CheckDeps(){
			SVJTag = sel->Get<KSVJTagSelector*>(source);
			if(!SVJTag) depfailed = true;
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			return (SVJTag->ntags==num);
		}
		
		//member variables
		string source;
		unsigned num;
		KSVJTagSelector* SVJTag;
};
REGISTER_SELECTOR(NSVJ);

//----------------------------------------------------
//filter jets based on lepton fractions for per-jet histos
class KLepFracFilterSelector : public KSelector {
	public:
		//constructor
		KLepFracFilterSelector() : KSelector() { }
		KLepFracFilterSelector(string name_, OptionMap* localOpt_) : KSelector(name_,localOpt_), filter(0), cut(0.2) { 
			//check for option
			//1 selects tagged, -1 selects anti-tagged
			localOpt->Get("filter",filter);
			localOpt->Get("cut",cut);
			canfail = false;
		}
		virtual void ListBranches(){
			branches.insert(branches.end(),{
				"JetsAK8_muonEnergyFraction",
				"JetsAK8_electronEnergyFraction",
			});
		}
		
		//this selector doesn't add anything to tree
		
		//used for non-dummy selectors
		virtual bool Cut() {
			JetsAK8_passCut.clear();
			JetsAK8_passCut.reserve(looper->JetsAK8_muonEnergyFraction->size());
			for(unsigned j = 0; j < looper->JetsAK8_muonEnergyFraction->size(); ++j){
				JetsAK8_passCut.push_back(looper->JetsAK8_muonEnergyFraction->at(j)<cut and looper->JetsAK8_electronEnergyFraction->at(j)<cut);
			}
			return true;
		}
		
		//member variables
		int filter;
		double cut;
		vector<bool> JetsAK8_passCut;
};
REGISTER_SELECTOR(LepFracFilter);

//avoid unwanted dependency
void KRA2BinSelector::CheckDeps(){
	CheckLabels();
	//check other options
	MCWeight = sel->Get<KMCWeightSelector*>("MCWeight");
	SVJTag = sel->Get<KSVJTagSelector*>("SVJTag");
}
void KRA2BinSelector::CheckBase(){
	DoBTagSF = MCWeight ? MCWeight->btagcorr : false;
}
unsigned KRA2BinSelector::GetNSVJ(){ return SVJTag ? SVJTag->ntags : 0; }

//avoid unwanted dependency
double KHisto::GetWeight(){
	double w = 1.0;
	if(MCWeight) w = MCWeight->GetWeight();
	return w;
}
double KHisto::GetWeightPerJet(unsigned index){
	double w = 1.0;
	//range of flatqty for AK8 jet pt
	if(MCWeight and MCWeight->flatten and MCWeight->flatqty>KMCWeightSelector::noflatqty and MCWeight->flatqty<=KMCWeightSelector::fourthjetAK8pt) {
		if( (MCWeight->flatqty==KMCWeightSelector::leadjetAK8pt and index!=0) or
			(MCWeight->flatqty==KMCWeightSelector::subleadjetAK8pt and index!=1) or
			(MCWeight->flatqty==KMCWeightSelector::bothjetAK8pt and (index!=0 and index!=1)) or
			(MCWeight->flatqty==KMCWeightSelector::thirdjetAK8pt and index!=2) or
			(MCWeight->flatqty==KMCWeightSelector::fourthjetAK8pt and index!=3) )
		{
			throw runtime_error("flattening qty is "+MCWeight->sflatqty+" but jet index is "+to_string(index));
		}
		w *= MCWeight->flattener.GetWeight(looper->JetsAK8->at(index).Pt());
	}
	//requiring svj tag for a jet
	if(SVJFilter and SVJFilter->filter!=0 and !SVJFilter->FailedDependency()){
		w *= (SVJFilter->filter==1 and SVJFilter->SVJTag->JetsAK8_tagged[index]) or
			 (SVJFilter->filter==-1 and !SVJFilter->SVJTag->JetsAK8_tagged[index]);
	}
	//requiring lep frac cut for a jet
	if(LepFracFilter and LepFracFilter->filter!=0){
		w *= (LepFracFilter->filter==1 and LepFracFilter->JetsAK8_passCut[index]) or
			 (LepFracFilter->filter==-1 and !LepFracFilter->JetsAK8_passCut[index]);
	}
	return w;
}

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
		
		virtual void ListBranches(){
			for(auto& hit : base->GetTable()){
				if(hit.second->khtmp) hit.second->khtmp->CheckBranches();
			}
		}
		//used for non-dummy selectors
		virtual bool Cut() {
			for(auto& hit : base->GetTable()){
				if(hit.second->khtmp) hit.second->khtmp->Fill();
			}

			return true;
		}
		virtual void Finalize(TFile* file){
			for(auto& hit : base->GetTable()){
				if(hit.second->khtmp) hit.second->khtmp->Finalize();
			}
		}
};
REGISTER_SELECTOR(Histo);

#endif
