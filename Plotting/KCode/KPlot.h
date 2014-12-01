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
		KPlot() : option(0), histo(0), ratio(0), can(0), pad1(0), pad2(0), leg(0), pave(0), line(0) {
			//must always have an option map
			if(option==0) option = new OptionMap();
		}
		KPlot(TH1F* h_, OptionMap* option_) : option(option_), histo(h_), ratio(0), can(0), pad1(0), pad2(0), leg(0), pave(0), line(0) {
			//must always have an option map
			if(option==0) option = new OptionMap();
			
			//plotting without ratio disabled by default
			//(i.e. ratio enabled by default)
			if(option->Get("noratio",false)) {
				//can = new TCanvas(histo->GetName(),histo->GetName(),700,500);
				//can = new TCanvas(histo->GetName(),histo->GetName(),700,700);
				//495x475 * m=1.4
				can = new TCanvas(histo->GetName(),histo->GetName(),693,665);
			
				pad1 = new TPad("graph","",0,0,1,1);
				//pad1->SetMargin(0.135,0.05,0.135,0.05);
				//pad1->SetMargin(0.125,0.05,0.1,0.05);
				//L,R,B,T: 85,35,70,30 * m=1.4
				pad1->SetMargin(119/(pad1->GetWw()*pad1->GetAbsWNDC()),49/(pad1->GetWw()*pad1->GetAbsWNDC()),98/(pad1->GetWh()*pad1->GetAbsHNDC()),42/(pad1->GetWh()*pad1->GetAbsHNDC()));
				pad1->SetTicks(1,1);
				if(!option->Get("liny",false)) pad1->SetLogy(); //liny off by default (i.e. logy on by default)
				if(option->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)
				
				histo->GetYaxis()->SetTitleOffset(1.50);
				histo->GetXaxis()->SetTitleOffset(1.05);
				histo->GetXaxis()->SetLabelOffset(5/(pad1->GetWh()*pad1->GetAbsHNDC()));
			}
			else {
				//can = new TCanvas(histo->GetName(),histo->GetName(),700,750);
				//580 = (665 - 70*1.4 + 13) -> 812 = 580/(5/7)
				can = new TCanvas(histo->GetName(),histo->GetName(),693,812);
			
				//setup histo and ratio areas for canvas
				//pad1 = new TPad("graph","",0,0.27,1.0,1.0);
				//pad1->SetMargin(0.125,0.05,0.02,0.05);
				pad1 = new TPad("graph","",0,0.2857,1.0,1.0);
				//L,R,B,T: 85,35,-,30 * m=1.4
				pad1->SetMargin(119/(pad1->GetWw()*pad1->GetAbsWNDC()),49/(pad1->GetWw()*pad1->GetAbsWNDC()),13/(pad1->GetWh()*pad1->GetAbsHNDC()),42/(pad1->GetWh()*pad1->GetAbsHNDC()));
				pad1->SetTicks(1,1);
				if(!option->Get("liny",false)) pad1->SetLogy(); //liny off by default (i.e. logy on by default)
				if(option->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)
				//pad2 = new TPad("dmc","",0,0,1.0,0.25);
				//pad2->SetMargin(0.125,0.05,0.35,0.05);
				pad2 = new TPad("dmc","",0,0,1.0,0.2857);
				//L,R,B,T: 85,35,70,- * m=1.4
				pad2->SetMargin(119/(pad2->GetWw()*pad2->GetAbsWNDC()),49/(pad2->GetWw()*pad2->GetAbsWNDC()),98/(pad2->GetWh()*pad2->GetAbsHNDC()),13/(pad2->GetWh()*pad2->GetAbsHNDC()));
				pad2->SetTicks(1,1);
			
				//format ratio histo
				ratio = (TH1F*)histo->Clone();
				ratio->SetMarkerStyle(20);
				ratio->SetMarkerColor(kBlack);
				ratio->SetLineColor(kBlack);
				ratio->GetYaxis()->SetTitleOffset(0.51);
				ratio->GetXaxis()->SetTitleOffset(1.05);
				ratio->GetXaxis()->SetTitleSize(38/(pad2->GetWh()*pad2->GetAbsHNDC()));
				ratio->GetXaxis()->SetLabelSize(32/(pad2->GetWh()*pad2->GetAbsHNDC()));
				ratio->GetXaxis()->SetLabelOffset(5/(pad2->GetWh()*pad2->GetAbsHNDC()));
				ratio->GetYaxis()->SetTitleSize(38/(pad2->GetWh()*pad2->GetAbsHNDC()));
				ratio->GetYaxis()->SetLabelSize(32/(pad2->GetWh()*pad2->GetAbsHNDC()));
				ratio->GetYaxis()->SetLabelOffset(5/(pad2->GetWh()*pad2->GetAbsHNDC()));
				//ratio->GetYaxis()->SetTickLength(6/(pad2->GetWh()*pad2->GetAbsHNDC()));
				ratio->GetYaxis()->SetTickLength(9/(pad2->GetWh()*pad2->GetAbsHNDC()));
				ratio->GetXaxis()->SetTickLength(12/(pad2->GetWh()*pad2->GetAbsHNDC()));
				ratio->GetYaxis()->SetNdivisions(503);
				ratio->GetXaxis()->SetNdivisions(507);
				ratio->GetYaxis()->SetRangeUser(0.45,1.55);
				
				ratio->GetYaxis()->SetTitle("Data/MC"); //default title
				
				//special formatting for blank histo
				histo->GetYaxis()->SetTitleOffset(1.315);
				histo->GetXaxis()->SetLabelOffset(999);
				histo->GetXaxis()->SetTitle("");
				
				//make line
				line = new TLine(ratio->GetXaxis()->GetXmin(),1,ratio->GetXaxis()->GetXmax(),1);
				line->SetLineStyle(2);
				line->SetLineWidth(2);
				line->SetLineColor(kRed);
			}

			//setup prelim text
			double umax3 = .925;
			pave = new TPaveText(umax3-0.3,0.94,umax3,1.0,"NDC");
			pave->SetFillColor(0);
			pave->SetBorderSize(0);
			//pave->SetTextFont(42);
			pave->SetTextSize(26/(pad1->GetWh()*pad1->GetAbsHNDC()));
			pave->AddText("CMS preliminary 2012");
			
			//common formatting for blank histo
			//histo->GetYaxis()->SetTitleSize(32/(pad1->GetWh()*pad1->GetAbsHNDC()));
			//histo->GetYaxis()->SetLabelSize(28/(pad1->GetWh()*pad1->GetAbsHNDC()));
			//histo->GetXaxis()->SetTitleSize(32/(pad1->GetWh()*pad1->GetAbsHNDC()));
			//histo->GetXaxis()->SetLabelSize(28/(pad1->GetWh()*pad1->GetAbsHNDC()));
			histo->GetYaxis()->SetTitleSize(38/(pad1->GetWh()*pad1->GetAbsHNDC()));
			histo->GetYaxis()->SetLabelSize(32/(pad1->GetWh()*pad1->GetAbsHNDC()));
			histo->GetYaxis()->SetLabelOffset(5/(pad1->GetWh()*pad1->GetAbsHNDC()));
			histo->GetXaxis()->SetTitleSize(38/(pad1->GetWh()*pad1->GetAbsHNDC()));
			histo->GetXaxis()->SetLabelSize(32/(pad1->GetWh()*pad1->GetAbsHNDC()));
			
			histo->GetYaxis()->SetTickLength(12/(pad1->GetWh()*pad1->GetAbsHNDC()));
			histo->GetXaxis()->SetTickLength(12/(pad1->GetWh()*pad1->GetAbsHNDC()));
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
			pave->Draw("same");
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
		
		//accessors
		TH1F* GetHisto() { return histo; }
		TH1F* GetRatio() { return ratio; }
		TCanvas* GetCanvas() { return can; }
		TPad* GetPad1() { return pad1; }
		TPad* GetPad2() { return pad2; }
		KLegend* GetLegend() { return leg; }
		void SetLegend(KLegend* leg_) { leg = leg_; }
		TPaveText* GetText() { return pave; }
		OptionMap* GetOption() { return option; }
		void SetOption(OptionMap* opt) { option = opt; if(option==0) option = new OptionMap(); } //must always have an option map

	protected:
		//member variables
		OptionMap* option;
		TH1F *histo, *ratio;
		TCanvas *can;
		TPad *pad1, *pad2;
		KLegend* leg;
		TPaveText* pave;
		TLine* line;

};

#endif