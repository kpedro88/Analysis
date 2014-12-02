#ifndef KPLOT_H
#define KPLOT_H

//custom headers
#include "KMap.h"
#include "KLegend.h"

//ROOT headers
#include <TROOT.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>

//STL headers
#include <string>
#include <sstream>

using namespace std;

//-----------------------------------------------------------
//class to store root drawing objects and do histo formatting
//also contains histo-specific options
class KPlot{
	public:
		//constructor
		KPlot() : option(0), histo(0), ratio(0), can(0), pad1(0), pad2(0), leg(0), paveCMS(0), paveExtra(0), paveLumi(0), line(0), intlumi(0),
				  canvasW(0), canvasH(0), marginL(0), marginR(0), marginB(0), marginT(0), marginM(0), sizeT(0), sizeL(0), sizeP(0), posP(0),
				  pad1W(0), pad1H(0), pad2W(0), pad2H(0)
		{
			//must always have an option map
			if(option==0) option = new OptionMap();
		}
		KPlot(TH1F* h_, OptionMap* option_) : option(option_), histo(h_), ratio(0), can(0), pad1(0), pad2(0), leg(0), paveCMS(0), paveExtra(0), paveLumi(0), line(0), intlumi(0),
											  canvasW(0), canvasH(0), marginL(0), marginR(0), marginB(0), marginT(0), marginM(0), sizeT(0), sizeL(0), sizeP(0), posP(0),
											  pad1W(0), pad1H(0), pad2W(0), pad2H(0)
		{
			//must always have an option map
			if(option==0) option = new OptionMap();
			
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
			//plotting without ratio disabled by default
			//(i.e. ratio enabled by default)
			if(option->Get("noratio",false)) {
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
				if(!option->Get("liny",false)) pad1->SetLogy(); //liny off by default (i.e. logy on by default)
				if(option->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)

				histo->GetYaxis()->SetTitleOffset(1.25);
				histo->GetXaxis()->SetTitleOffset(0.95);
				histo->GetXaxis()->SetLabelOffset(5/pad1H);
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
				if(!option->Get("liny",false)) pad1->SetLogy(); //liny off by default (i.e. logy on by default)
				if(option->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)
				pad2 = new TPad("dmc","",0,0,1.0,2./7.);
				pad2W = pad2->GetWw()*pad2->GetAbsWNDC();
				pad2H = pad2->GetWh()*pad2->GetAbsHNDC();
				//L,R,B,T: 85,35,70,- * m=1.4
				pad2->SetMargin(marginL/pad2W,marginR/pad2W,marginB/pad2H,marginM/pad2H);
				pad2->SetTicks(1,1);
			
				//format ratio histo
				ratio = (TH1F*)histo->Clone();
				ratio->SetMarkerStyle(20);
				ratio->SetMarkerColor(kBlack);
				ratio->SetLineColor(kBlack);
				ratio->GetYaxis()->SetTitleOffset(0.425);
				ratio->GetXaxis()->SetTitleOffset(0.95);
				ratio->GetXaxis()->SetTitleSize(sizeT/pad2H);
				ratio->GetXaxis()->SetLabelSize(sizeL/pad2H);
				ratio->GetXaxis()->SetLabelOffset(5/pad2H);
				ratio->GetYaxis()->SetTitleSize(sizeT/pad2H);
				ratio->GetYaxis()->SetLabelSize(sizeL/pad2H);
				ratio->GetYaxis()->SetLabelOffset(5/pad2H);
				//ratio->GetYaxis()->SetTickLength(6/pad2H);
				ratio->GetYaxis()->SetTickLength(9/pad2H);
				ratio->GetXaxis()->SetTickLength(12/pad2H);
				ratio->GetYaxis()->SetNdivisions(503);
				ratio->GetXaxis()->SetNdivisions(507);
				ratio->GetYaxis()->SetRangeUser(0.45,1.55);
				
				ratio->GetYaxis()->SetTitle("Data/MC"); //default title
				
				//special formatting for blank histo
				histo->GetYaxis()->SetTitleOffset(1.1);
				histo->GetXaxis()->SetLabelOffset(999);
				histo->GetXaxis()->SetTitle("");
				
				//make line
				line = new TLine(ratio->GetXaxis()->GetXmin(),1,ratio->GetXaxis()->GetXmax(),1);
				line->SetLineStyle(2);
				line->SetLineWidth(2);
				line->SetLineColor(kRed);
			}

			//setup CMS text
			posP = 1-(marginT-1)/pad1H;
			double uminCMS = marginL/pad1W;
			double umaxCMS = marginL/pad1W + sizeP/pad1W*3*0.6;
			paveCMS = new TPaveText(uminCMS,posP,umaxCMS,1.0,"NDC");
			paveCMS->SetFillColor(0);
			paveCMS->SetBorderSize(0);
			paveCMS->SetTextFont(61);
			paveCMS->SetTextSize(sizeP/pad1H);
			paveCMS->AddText("CMS");
			
			//setup prelim text
			//todo: add option to enable/disable/change
			double sizePextra = sizeP - 3; //smaller
			double uminExtra = umaxCMS + sizeP/pad1W*0.4;
			double umaxExtra = uminExtra + sizePextra/pad1W*8*0.6;
			paveExtra = new TPaveText(uminExtra,posP,umaxExtra,1.0,"NDC");
			paveExtra->SetFillColor(0);
			paveExtra->SetBorderSize(0);
			paveExtra->SetTextFont(52);
			paveExtra->SetTextSize(sizePextra/pad1H);
			paveExtra->AddText("Preliminary");

			//common formatting for blank histo
			histo->GetYaxis()->SetTitleSize(sizeT/pad1H);
			histo->GetYaxis()->SetLabelSize(sizeL/pad1H);
			histo->GetYaxis()->SetLabelOffset(5/pad1H);
			histo->GetXaxis()->SetTitleSize(sizeT/pad1H);
			histo->GetXaxis()->SetLabelSize(sizeL/pad1H);
			histo->GetYaxis()->SetTickLength(12/pad1H);
			histo->GetXaxis()->SetTickLength(12/pad1H);
			histo->GetXaxis()->SetNdivisions(507);
		}
		
		//drawing
		void DrawInit(){
			can->cd();
			pad1->Draw();
			pad1->cd();
		}
		void DrawHist(){
			//get y axis range of histo from KLegend
			histo->GetYaxis()->SetRangeUser(leg->GetRange().first,leg->GetRange().second);
			histo->Draw("hist");
		}
		void DrawText(){
			pad1->cd();
			leg->Draw(pad1);
			paveCMS->Draw("same");
			paveExtra->Draw("same");
			paveLumi->Draw("same");
		}
		void DrawRatio(){
			can->cd();
			pad2->Draw();
			pad2->cd();
			ratio->Draw("hist");
		}
		void DrawLine(){
			pad2->cd();
			line->Draw("same");
		}
		
		void SetLumi(double intlumi_){
			intlumi = intlumi_;
			if(paveLumi) delete paveLumi;
			
			stringstream fbname_;
			fbname_ << fixed << setprecision(1) << intlumi/1000 << " fb^{-1} (8 TeV)";
			string fbname = fbname_.str();
			
			//setup lumi text
			double umaxLumi = 1-marginR/pad1W;
			double uminLumi = umaxLumi - sizeP/pad1W*11*0.6;
			paveLumi = new TPaveText(uminLumi,posP,umaxLumi,1.0,"NDC");
			paveLumi->SetFillColor(0);
			paveLumi->SetBorderSize(0);
			paveLumi->SetTextFont(42);
			paveLumi->SetTextSize(sizeP/pad1H);
			paveLumi->AddText(fbname.c_str());
		}
		
		//accessors
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
		OptionMap* GetOption() { return option; }
		void SetOption(OptionMap* opt) { option = opt; if(option==0) option = new OptionMap(); } //must always have an option map

	protected:
		//member variables
		OptionMap* option;
		TH1F *histo, *ratio;
		TCanvas *can;
		TPad *pad1, *pad2;
		KLegend* leg;
		TPaveText* paveCMS;
		TPaveText* paveExtra;
		TPaveText* paveLumi;
		TLine* line;
		double intlumi;
		double canvasW, canvasH, marginL, marginR, marginB, marginT, marginM, sizeT, sizeL, sizeP, posP;
		double pad1W, pad1H, pad2W, pad2H;
};

#endif