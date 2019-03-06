//analysis headers
#include "Analysis/KCode/KMap.h"
#include "Analysis/KCode/KLegend.h"
#include "Analysis/KCode/KPlot.h"
#include "Analysis/KCode/KMath.h"
//#include "Analysis/KCode/KStyle.h"

//ROOT headers
#include <TFile.h>
#include <TH1.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TStyle.h>
#include <TF1.h>
#include <TMath.h>
#include <TGaxis.h>

//STL headers
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

static const double sqrt2 = sqrt(2.);
double trigErf(double *x, double *par) {
	return 0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt2*par[2]) ));
}


string histName(string region, string year, string sel, string qty){
	stringstream ss;
	ss << qty << "_" << region << "_" << year << "_" << sel;
	return ss.str();
}

void print(TCanvas* can, const string& oname, const vector<string>& pformats){
	for(const auto& pformat : pformats){
		can->Print((oname+"."+pformat).c_str(),pformat.c_str());
		//if(pformat=="eps") system(("epstopdf "+oname+".eps").c_str());
	}
}

//root -b -l -q 'plotTrigEff.C+("test/hist_trig_2016.root","SingleMuon","2016","trigDenom",{"trigNumerJet","trigNumerHT","trigNumerJetHT"},{"MTAK8","ht","leadjetAK8pt","met","subleadjetAK8pt"})'
void plotTrigEff(string filename, string region, string year, string denom, vector<string> numers, vector<string> quantities, bool fit=false, bool showhist=false, int rebin=0, string cutname="", vector<string> pformats={"png"}){
	TFile* file = TFile::Open(filename.c_str());
	if(!file) return;

	//qty names for legend
	map<string,string> qtitles{
		{"MTAK8","M_{T}"},
		{"leadjetAK8pt","p_{T}(j_{1})"},
	};
	//units
	map<string,string> utitles{
		{"MTAK8","GeV"},
		{"leadjetAK8pt","GeV"},
	};
	
	//y axis titles
	map<string,string> ytitles{
		{"trigNumerJet","HLT AK8PFJet efficiency"},
		{"trigNumerHT","HLT PFHT efficiency"},
		{"trigNumerJetHT","HLT JetHT efficiency"},
		{"trigNumer","HLT JetHT efficiency"}
	};

	map<string,string> ctitles{
		{"Deta1","#Delta#eta(j_{1},j_{2}) < 1.0"},
		{"Deta15","#Delta#eta(j_{1},j_{2}) < 1.5"},
		{"Deta","#Delta#eta(j_{1},j_{2}) < 1.5"},
		{"DetaHLTmatch","#Delta#eta(j_{1},j_{2}) < 1.5, HLT match"},
		{"Deta2","#Delta#eta(j_{1},j_{2}) < 2.0"},
		{"HT2016","H_{T} > 1100 GeV"},
		{"HT2017","H_{T} > 1500 GeV"},
		{"HTMT","H_{T}/M_{T} < 0.7"},
//		{"PT2016","p_{T}(j_{1})+p_{T}(j_{2}) > 1000 GeV"},
//		{"PT2017","p_{T}(j_{1})+p_{T}(j_{2}) > 1200 GeV"},
		{"PT2016","p_{T}(j_{1}) > 600 GeV"},
		{"PT2017","p_{T}(j_{1}) > 700 GeV"},
		{"PTMT","(p_{T}(j_{1})+p_{T}(j_{2}) > 1000 GeV)/M_{T} < 0.7"},
		{"Deta0.5HLTmatch","#Delta#eta < 0.5, HLT match"},
		{"Deta0.6HLTmatch","#Delta#eta < 0.6, HLT match"},
		{"Deta0.7HLTmatch","#Delta#eta < 0.7, HLT match"},
		{"Deta0.8HLTmatch","#Delta#eta < 0.8, HLT match"},
		{"Deta0.9HLTmatch","#Delta#eta < 0.9, HLT match"},
		{"Deta1.0HLTmatch","#Delta#eta < 1.0, HLT match"},
		{"Deta1.1HLTmatch","#Delta#eta < 1.1, HLT match"},
		{"Deta1.2HLTmatch","#Delta#eta < 1.2, HLT match"},
		{"Deta1.3HLTmatch","#Delta#eta < 1.3, HLT match"},
		{"Deta1.4HLTmatch","#Delta#eta < 1.4, HLT match"},
		{"Deta1.5HLTmatch","#Delta#eta < 1.5, HLT match"},
		{"Deta1.6HLTmatch","#Delta#eta < 1.6, HLT match"},
		{"Deta1.7HLTmatch","#Delta#eta < 1.7, HLT match"},
		{"Deta1.8HLTmatch","#Delta#eta < 1.8, HLT match"},
		{"Deta1.9HLTmatch","#Delta#eta < 1.9, HLT match"},
		{"Deta2.0HLTmatch","#Delta#eta < 2.0, HLT match"},
	};

	map<string,string> ltitles{
		{"2016","35.9 fb^{-1} (13 TeV)"},
		{"2017","41.5 fb^{-1} (13 TeV)"},
		{"2018","59.5 fb^{-1} (13 TeV)"},
		{"2018NoHEM","21.0 fb^{-1} (13 TeV)"},
		{"2018HEM","38.4 fb^{-1} (13 TeV)"},
	};
	
	//make plot options
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<string>("prelim_text","Work-in-progress");
	globalOpt->Set<string>("lumi_text",ltitles[year]);
	globalOpt->Set<bool>("checkerr",false);
	globalOpt->Set<int>("npanel",1);
	if(showhist) globalOpt->Set<double>("marginR",95);
	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logy",false);
	localOpt->Set<bool>("logz",false);
	double zmin = 0.;
	string drawopt = "colz";
	string prefix = "trigEff";
	if(fit) prefix += "Fit";
	if(filename.find("zoom")!=string::npos){
		zmin = 0.8;
		drawopt = "colz text";
		gStyle->SetPaintTextFormat(".2f");
		prefix += "Zoom";
	}

	//get denom
	if(!cutname.empty()) denom += cutname;
	for(const auto& qty: quantities){
		const auto& hname = histName(region,year,denom,qty);

		//1D hist
		if(qty.find("_")==string::npos){
			TH1F* hdenom = (TH1F*)file->Get(hname.c_str());

			vector<double> newbins;
			if(rebin>0){
				int sum = 0;
				newbins.push_back(hdenom->GetBinLowEdge(1));
				for(unsigned b = 1; b <= hdenom->GetNbinsX(); ++b){
					sum += hdenom->GetBinContent(b);
					if(sum>rebin or b==hdenom->GetNbinsX()){
						newbins.push_back(hdenom->GetBinLowEdge(b+1));
						sum = 0;
					}
				}
				hdenom = (TH1F*)hdenom->Rebin(newbins.size()-1,(hname+"_rebin").c_str(),newbins.data());
			}
	
			//compare to numers
			for(auto numer: numers){
				string ytitle = ytitles[numer].c_str();
				if(!cutname.empty()) numer += cutname;
				const auto& hname2 = histName(region,year,numer,qty);
				TH1F* hnumer = (TH1F*)file->Get(hname2.c_str());
				if(rebin>0) hnumer = (TH1F*)hnumer->Rebin(newbins.size()-1,(hname2+"_rebin").c_str(),newbins.data());
				TGraphAsymmErrors* btmp = new TGraphAsymmErrors(hnumer,hdenom);

				TF1* fn = nullptr;
				if(fit){
					double xmin = 0;
					double xmax = hnumer->GetXaxis()->GetXmax()*0.75;
					fn = new TF1("trigErf",trigErf,xmin,xmax,3);
					double params[] = {0.99,xmin+(xmin-xmax)/4.,50.};
					fn->SetParameters(params);
					fn->SetLineColor(kRed);
					fn->SetLineWidth(2);
					gStyle->SetOptFit(0);
					btmp->Fit("trigErf","0QEM+","",xmin,xmax);
				}
			
				//clone for axis
				TH1F* haxis = (TH1F*)hnumer->Clone("axis");
				haxis->Reset("MICES");
				haxis->GetYaxis()->SetRangeUser(0,1);
				haxis->GetYaxis()->SetTitle(ytitle.c_str());
			
				stringstream oname;
				oname << prefix << "_" << hname2;
				if(rebin>0) oname << "_rebin" << rebin;
			
				//make plot
				KPlot* plot = new KPlot(oname.str(),localOpt,globalOpt);
				plot->Initialize(haxis);
				KLegend* kleg = plot->GetLegend();
				TCanvas* can = plot->GetCanvas();
				TPad* pad1 = plot->GetPad1();
				if(showhist) pad1->SetTicks(1,0);
				pad1->cd();
			
				//make legend
				kleg->AddHist(plot->GetHisto()); //for tick sizes
				TH1F* leghist = (TH1F*)plot->GetHisto()->Clone("leghist");
				for(unsigned b = 0; b <= leghist->GetNbinsX(); ++b) { leghist->SetBinContent(b,1.0); }
				leghist->SetBinContent(1,1.4);
				kleg->AddHist(leghist); //for placement
				if(!cutname.empty()) kleg->AddEntry(btmp,ctitles[cutname],"pel");
				if(fn){
					double mu = fn->GetParameter(1);
					double sigma = fn->GetParameter(2);
					double plateau = mu+3*sigma;
					double p98 = mu+2.05*sigma;

					//calculate plateau eff and err from data
					unsigned bplateau = hdenom->FindBin(plateau);
					double nplateau = hnumer->Integral(bplateau,-1);
					double dplateau = hdenom->Integral(bplateau,-1);
					double eupplateau = KMath::EffError(nplateau,dplateau,1);
					double ednplateau = KMath::EffError(nplateau,dplateau,0);
					double effplateau = nplateau/dplateau;

					string eps("#scale[1.3]{#font[122]{e}}");
					stringstream ss;
					ss << fixed << setprecision(0);
					ss << eps << "[^{}" << qtitles[qty] << " #geq " << plateau << " " << utitles[qty] << "] = ";
					ss << fixed << setprecision(1);
					ss << "^{}" << effplateau*100 << "^{+" << eupplateau*100 << "}_{-" << ednplateau*100 << "} %";
					kleg->AddEntry((TObject*)NULL,ss.str(),"");
					
					stringstream ss2;
					ss2 << fixed << setprecision(0);
					ss2 << "98% of plateau at " << p98 << " " << utitles[qty];
					kleg->AddEntry((TObject*)NULL,ss2.str(),"");
				}
				kleg->Build();//KLegend::right,KLegend::bottom);

				//draw blank histo for axes
				plot->DrawHist();

				if(showhist){
					TH1F* hnumer2 = (TH1F*)hnumer->Clone();
					TH1F* hdenom2 = (TH1F*)hdenom->Clone();
					
					//scale so numer goes up to 0.5
					double scale = 0.5/hnumer2->GetMaximum();
					double hymax = kleg->GetRange().second/scale;

					//denom settings
					Color_t shcol = kBlue + 2;
					hdenom2->Scale(scale);
					hdenom2->SetLineColor(shcol);
					hdenom2->SetLineStyle(2);
					hdenom2->SetFillColor(0);
					hdenom2->SetLineWidth(2);
					hdenom2->Draw("hist same");

					//numer settings
					hnumer2->Scale(scale);
					hnumer2->SetFillColor(kBlue - 9);
					hnumer2->SetLineStyle(0);
					hnumer2->Draw("hist same");

					//extra y axis
					double xmax = hnumer->GetXaxis()->GetXmax();
					TAxis* yaxis = plot->GetHisto()->GetYaxis();
					TGaxis* hyaxis = new TGaxis(xmax, 0, xmax, kleg->GetRange().second, 0, hymax, 507, "+L");
					hyaxis->SetTitleFont(42);
					hyaxis->SetLabelFont(42);
					hyaxis->SetLineColor(shcol);
					hyaxis->SetTextColor(shcol);
					hyaxis->SetLabelColor(shcol);
					hyaxis->SetTitleOffset(yaxis->GetTitleOffset());
					hyaxis->SetTitleSize(yaxis->GetTitleSize());
					hyaxis->SetLabelSize(yaxis->GetLabelSize());
					hyaxis->SetLabelOffset(yaxis->GetLabelOffset());
					hyaxis->SetTickLength(yaxis->GetTickLength());
					hyaxis->SetTitle("number of events");
					hyaxis->Draw();

					TLine* unitline = new TLine(hnumer->GetXaxis()->GetXmin(),1,xmax,1);
					unitline->SetLineColor(kBlack);
					unitline->SetLineStyle(7);
					unitline->SetLineWidth(2);
					unitline->Draw("same");
				}
			
				//draw eff
				btmp->Draw("PZ same");
				if(fn) fn->Draw("same");

				plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
				plot->DrawText();
			
				//print image
				print(can,oname.str(),pformats);
			}
		}

		//2D hist
		else {
			TH2F* hdenom = (TH2F*)file->Get(hname.c_str());

			//compare to numers
			for(auto numer: numers){
			if(!cutname.empty()) numer += cutname;
				const auto& hname2 = histName(region,year,numer,qty);
				TH2F* hnumer = (TH2F*)file->Get(hname2.c_str());
				TH2F* heff = (TH2F*)hnumer->Clone();
				heff->Divide(hnumer,hdenom);
			
				//axis settings
				heff->GetZaxis()->SetRangeUser(zmin,1);
				heff->GetZaxis()->SetTitle(ytitles[numer].c_str());
			
				stringstream oname;
				oname << prefix << "_" << hname2;
				if(rebin>0) oname << "_rebin" << rebin;
			
				//make plot
				KPlot2D* plot = new KPlot2D(oname.str(),numer,localOpt,globalOpt);
				plot->Initialize(heff);
				TCanvas* can = plot->GetCanvas();
				TPad* pad1 = plot->GetPad1();
				pad1->cd();
			
				//draw histo
				heff->Draw(drawopt.c_str());
				plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
				plot->DrawText();
			
				//print image
				print(can,oname.str(),pformats);
			}
		}
	}
}
