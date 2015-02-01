#ifndef KPLOT_H
#define KPLOT_H

//custom headers
#include "KMap.h"
#include "KLegend.h"

//ROOT headers
#include <TROOT.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TH1.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>
#include <TLatex.h>
#include <TAxis.h>
#include <TExec.h>

//STL headers
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

//-----------------------------------------------------------
//class to store root drawing objects and do histo formatting
//also contains histo-specific options
class KPlot{
	public:
		//constructor
		KPlot() : name(""), localOpt(0), globalOpt(0), histo(0), ratio(0), exec(0), isBuilt(false), can(0), pad1(0), pad2(0), leg(0), paveCMS(0), paveExtra(0), paveLumi(0), line(0), intlumi(0),
				  canvasW(0), canvasH(0), marginL(0), marginR(0), marginB(0), marginT(0), marginM(0), sizeT(0), sizeL(0), sizeP(0), sizeTick(0), sizeLoff(0), 
				  posP(0), epsilon(0), pad1W(0), pad1H(0), pad2W(0), pad2H(0)
		{
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
		}
		KPlot(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : name(name_), localOpt(localOpt_), globalOpt(globalOpt_), histo(0), ratio(0), exec(0), isBuilt(false),
																	   can(0), pad1(0), pad2(0), leg(0), paveCMS(0), paveExtra(0), paveLumi(0), line(0), intlumi(0)
		{
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
			
			//enable histo errors
			TH1::SetDefaultSumw2(kTRUE);
			//construct histogram		
			vector<double> xbins;
			int xnum;
			double xmin, xmax;
			if(localOpt->Get("xbins",xbins)){ //variable binning case
				histo = new TH1F(name.c_str(),"",xbins.size()-1,&xbins[0]);
				isBuilt = true;
			}
			else if(localOpt->Get("xnum",xnum) && localOpt->Get("xmin",xmin) && localOpt->Get("xmax",xmax)){ //standard binning case
				histo = new TH1F(name.c_str(),"",xnum,xmin,xmax);
				isBuilt = true;
			}
			else { //no/incomplete binning information, build failed
				isBuilt = false;
				return;
			}
			string xtitle, ytitle;
			localOpt->Get("xtitle",xtitle);
			localOpt->Get("ytitle",ytitle);
			histo->GetXaxis()->SetTitle(xtitle.c_str());
			histo->GetYaxis()->SetTitle(ytitle.c_str());
			
			//universal size values
			canvasW = 704;
			marginL = 95;
			marginR = 35;
			marginB = 65;
			marginT = 35;
			marginM = 13;
			sizeT = 32;
			sizeL = 28;
			sizeP = 26;
			sizeTick = 12;
			sizeLoff = 5;
			epsilon = 2;
			//plotting with ratio enabled by default
			if(!localOpt->Get("ratio",true)) {
				canvasH = 583;
				//can = new TCanvas(histo->GetName(),histo->GetName(),700,555);
				//account for window frame: 2+2px width, 2+26px height
				can = new TCanvas(histo->GetName(),histo->GetName(),canvasW,canvasH);
			
				pad1 = new TPad("graph","",0,0,1,1);
				pad1W = pad1->GetWw()*pad1->GetAbsWNDC();
				pad1H = pad1->GetWh()*pad1->GetAbsHNDC();
				//pad1->SetMargin(0.135,0.05,0.135,0.05);
				//pad1->SetMargin(0.125,0.05,0.1,0.05);
				//L,R,B,T: 85,35,70,30 * m=1.4
				pad1->SetMargin(marginL/pad1W,marginR/pad1W,marginB/pad1H,marginT/pad1H);
				pad1->SetTicks(1,1);
				if(!localOpt->Get("liny",false)) pad1->SetLogy(); //liny off by default (i.e. logy on by default)
				if(localOpt->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)
				pad1->Draw();

				histo->GetXaxis()->SetLabelOffset(sizeLoff/pad1H);
				SetTitleOffset(pad1,histo->GetXaxis());
				SetTitleOffset(pad1,histo->GetYaxis());
			}
			else {
				canvasH = 728;
				//can = new TCanvas(histo->GetName(),histo->GetName(),700,700);
				//account for window frame: 2+2px width, 2+26px height
				can = new TCanvas(histo->GetName(),histo->GetName(),canvasW,canvasH);
				//500/(5/7) = 700

				//setup histo and ratio areas for canvas
				pad1 = new TPad("graph","",0,2./7.,1.0,1.0);
				pad1W = pad1->GetWw()*pad1->GetAbsWNDC();
				pad1H = pad1->GetWh()*pad1->GetAbsHNDC();
				pad1->SetMargin(marginL/pad1W,marginR/pad1W,marginM/pad1H,marginT/pad1H);
				pad1->SetTicks(1,1);
				if(!localOpt->Get("liny",false)) pad1->SetLogy(); //liny off by default (i.e. logy on by default)
				if(localOpt->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)
				pad1->Draw();
				pad2 = new TPad("dmc","",0,0,1.0,2./7.);
				pad2W = pad2->GetWw()*pad2->GetAbsWNDC();
				pad2H = pad2->GetWh()*pad2->GetAbsHNDC();
				can->cd();
				//L,R,B,T: 85,35,70,- * m=1.4
				pad2->SetMargin(marginL/pad2W,marginR/pad2W,marginB/pad2H,marginM/pad2H);
				pad2->SetTicks(1,1);
				pad2->Draw();
				double tickScaleX2 = (pad2W - marginL - marginR)/pad2W*pad2H;
				double tickScaleY2 = (pad2H - marginB - marginM)/pad2H*pad2W;
			
				//format ratio histo
				ratio = (TH1F*)histo->Clone();
				ratio->SetMarkerStyle(20);
				ratio->SetMarkerColor(kBlack);
				ratio->SetLineColor(kBlack);
				ratio->GetXaxis()->SetTitleSize(sizeT/pad2H);
				ratio->GetXaxis()->SetLabelSize(sizeL/pad2H);
				ratio->GetXaxis()->SetLabelOffset(sizeLoff/pad2H);
				ratio->GetYaxis()->SetTitleSize(sizeT/pad2H);
				ratio->GetYaxis()->SetLabelSize(sizeL/pad2H);
				ratio->GetYaxis()->SetLabelOffset(sizeLoff/pad2W);
				//ratio->GetYaxis()->SetTickLength(6/pad2H);
				ratio->GetYaxis()->SetTickLength(sizeTick/tickScaleY2);
				ratio->GetXaxis()->SetTickLength(sizeTick/tickScaleX2);
				ratio->GetYaxis()->SetNdivisions(503);
				ratio->GetXaxis()->SetNdivisions(507);
				ratio->GetYaxis()->SetRangeUser(0.45,1.55);
				
				ratio->GetYaxis()->SetTitle("Data/MC"); //default title
				
				//note: axis titles are middle-aligned
				SetTitleOffset(pad2,ratio->GetXaxis());
				SetTitleOffset(pad2,ratio->GetYaxis());
				
				//special formatting for blank histo
				SetTitleOffset(pad1,histo->GetYaxis());
				histo->GetXaxis()->SetLabelOffset(999);
				histo->GetXaxis()->SetTitle("");
				
				//make line
				pad2->cd();
				line = new TLine(ratio->GetXaxis()->GetXmin(),1,ratio->GetXaxis()->GetXmax(),1);
				line->SetLineStyle(2);
				line->SetLineWidth(2);
				line->SetLineColor(kRed);
			}
			pad1->cd();
			
			//setup CMS text
			TLatex width_test_cms(0,0,"CMS");
			width_test_cms.SetTextSize(sizeP/pad1H);
			posP = 1-(marginT-1)/pad1H;
			double uminCMS = marginL/pad1W;
			double umaxCMS = marginL/pad1W + width_test_cms.GetXsize();
			paveCMS = new TPaveText(uminCMS,posP,umaxCMS,1.0,"NDC");
			paveCMS->SetFillColor(0);
			paveCMS->SetBorderSize(0);
			paveCMS->SetTextFont(61);
			paveCMS->SetTextSize(sizeP/pad1H);
			paveCMS->AddText("CMS");
			
			//setup prelim text
			//todo: add option to enable/disable/change
			double sizePextra = sizeP - 3; //smaller
			TLatex width_test_extra(0,0," Preliminary");
			width_test_extra.SetTextSize(sizePextra/pad1H);
			double uminExtra = umaxCMS;
			double umaxExtra = uminExtra + width_test_extra.GetXsize();
			paveExtra = new TPaveText(uminExtra,posP,umaxExtra,1.0,"NDC");
			paveExtra->SetFillColor(0);
			paveExtra->SetBorderSize(0);
			paveExtra->SetTextFont(52);
			paveExtra->SetTextSize(sizePextra/pad1H);
			paveExtra->AddText(" Preliminary");
			
			//setup lumi text
			globalOpt->Get<double>("luminorm",intlumi);
			stringstream fbname_;
			fbname_ << fixed << setprecision(1) << intlumi/1000 << " fb^{-1} (8 TeV)";
			string fbname = fbname_.str();
			TLatex width_test_lumi(0,0,fbname.c_str());
			width_test_lumi.SetTextSize(sizeP/pad1H);
			double umaxLumi = 1-marginR/pad1W;
			double uminLumi = umaxLumi - width_test_lumi.GetXsize();
			paveLumi = new TPaveText(uminLumi,posP,umaxLumi,1.0,"NDC");
			paveLumi->SetFillColor(0);
			paveLumi->SetBorderSize(0);
			paveLumi->SetTextFont(42);
			paveLumi->SetTextSize(sizeP/pad1H);
			paveLumi->AddText(fbname.c_str());

			//common formatting for blank histo
			double tickScaleX1 = (pad1W - marginL - marginR)/pad1W*pad1H;
			double tickScaleY1 = (pad1H - marginM - marginT)/pad1H*pad1W;
			histo->GetYaxis()->SetTitleSize(sizeT/pad1H);
			histo->GetYaxis()->SetLabelSize(sizeL/pad1H);
			histo->GetYaxis()->SetLabelOffset(sizeLoff/pad1W);
			histo->GetXaxis()->SetTitleSize(sizeT/pad1H);
			histo->GetXaxis()->SetLabelSize(sizeL/pad1H);
			histo->GetYaxis()->SetTickLength(sizeTick/tickScaleY1);
			histo->GetXaxis()->SetTickLength(sizeTick/tickScaleX1);
			histo->GetXaxis()->SetNdivisions(507);
		}
		
		//drawing
		void DrawHist(){
			pad1->cd();
			//get y axis range of histo from KLegend
			histo->GetYaxis()->SetRangeUser(leg->GetRange().first,leg->GetRange().second);
			histo->Draw("hist");
			//horizontal error bars for histograms are DISABLED by default
			//but auto-enabled for variable-binned histograms (per CMS style guidelines)
			//(note: TExec cannot be the first thing drawn on a pad)
			string execname = "exec_" + name;
			if(globalOpt->Get("horizerrbars",false) || histo->GetXaxis()->IsVariableBinSize()){
				exec = new TExec(execname.c_str(),"gStyle->SetErrorX(0.5);");
			}
			else exec = new TExec(execname.c_str(),"gStyle->SetErrorX(0);");
			exec->Draw();
		}
		void DrawText(){
			pad1->cd();
			leg->Draw();
			paveCMS->Draw("same");
			paveExtra->Draw("same");
			paveLumi->Draw("same");
		}
		void DrawRatio(){
			pad2->cd();
			ratio->Draw("hist");
			exec->Draw();
		}
		void DrawLine(){
			pad2->cd();
			line->Draw("same");
		}

		//helper
		void SetTitleOffset(TPad* pad, TAxis* axis){
			double padW, padH;
			pad->cd();
			if(pad==pad1) { padW = pad1W; padH = pad1H; }
			else if(pad==pad2) { padW = pad2W; padH = pad2H; }
			
			TLatex height_test(0,0,axis->GetTitle());
			height_test.SetTextSize(sizeT/padH);
			double Theight = height_test.GetYsize();
			double Toff = 1;
			
			//note: axis titles are middle-aligned
			if(strcmp(axis->GetName(),"xaxis")==0){
				Toff = (marginB/padH - epsilon/padH - Theight/2.)/(1.6*sizeT/padH);
			}
			else if(strcmp(axis->GetName(),"yaxis")==0){
				//need to scale title height value from pad height to pad width for y axis
				Theight *= padH/padW;
				Toff = (marginL/padW - epsilon/padW - Theight/2.)/(1.6*sizeT/padH);
			}
			
			axis->SetTitleOffset(Toff);
			
		}
		
		//accessors
		string GetName() { return name; }
		bool GetBuilt() { return isBuilt; }
		void SetName(string name_) { name = name_; }
		TH1F* GetHisto() { return histo; }
		TH1F* GetRatio() { return ratio; }
		TCanvas* GetCanvas() { return can; }
		TPad* GetPad1() { return pad1; }
		TPad* GetPad2() { return pad2; }
		KLegend* GetLegend() { return leg; }
		void SetLegend(KLegend* leg_) { leg = leg_; }
		TPaveText* GetCMSText() { return paveCMS; }
		TPaveText* GetExtraText() { return paveExtra; }
		TPaveText* GetLumiText() { return paveLumi; }
		OptionMap* GetLocalOpt() { return localOpt; }
		void SetLocalOpt(OptionMap* opt) { localOpt = opt; if(localOpt==0) localOpt = new OptionMap(); } //must always have an option map
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetGlobalOpt(OptionMap* opt) { globalOpt = opt; if(globalOpt==0) globalOpt = new OptionMap(); } //must always have an option map

	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		TH1F *histo, *ratio;
		TExec *exec;
		bool isBuilt;
		TCanvas *can;
		TPad *pad1, *pad2;
		KLegend* leg;
		TPaveText* paveCMS;
		TPaveText* paveExtra;
		TPaveText* paveLumi;
		TLine* line;
		double intlumi;
		double canvasW, canvasH, marginL, marginR, marginB, marginT, marginM, sizeT, sizeL, sizeP, sizeTick, sizeLoff, posP, epsilon;
		double pad1W, pad1H, pad2W, pad2H;
};

#endif