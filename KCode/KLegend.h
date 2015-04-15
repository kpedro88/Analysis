#ifndef KLEGEND_H
#define KLEGEND_H

//custom headers
#include "KMap.h"

//ROOT headers
#include <TROOT.h>
#include <TLegend.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TPad.h>

//STL headers
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <utility>
#include <iomanip>
#include <cmath>

using namespace std;

class KLegend{
	public:
		//position enums
		enum Horz { hdefault, left, center, right };
		enum Vert { vdefault, top, middle, bottom };
	
		//constructor
		KLegend(TPad* pad_, OptionMap* localOpt_, OptionMap* globalOpt_) : 
			pad(pad_), localOpt(localOpt_), globalOpt(globalOpt_), legwidth(0), legheight(0), 
			lbound(0), rbound(0), tbound(0), bbound(0), umin(0), umax(0), vmin(0), vmax(0),
			leg(0), ymin(0), ymax(0), manual_ymin(false) 
		{
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
			
			padH = pad->GetWh()*pad->GetAbsHNDC();
			legentry = 26/padH; //line height for each entry
			
			//allow multiple lines of text at top of legend
			globalOpt->Get("extra_text",extra_text);
			//will be added to top of legend
			for(unsigned t = 0; t < extra_text.size(); t++){
				CheckSize(extra_text[t]); 
			}
			
			double ymin_ = 1;
			if(globalOpt->Get("ymin",ymin_)) SetManualYmin(ymin_);
		}
		//destructor
		virtual ~KLegend() {}
		
		//functions
		void Build(Horz hdir, Vert vdir){
			//boundaries of plot area
			double ytick = (hists.size()>0) ? hists[0]->GetYaxis()->GetTickLength() : 0;
			double xtick = (hists.size()>0) ? hists[0]->GetXaxis()->GetTickLength() : 0;
			lbound = pad->GetLeftMargin() + ytick;
			rbound = 1 - (pad->GetRightMargin() + ytick);
			tbound = 1 - (pad->GetTopMargin() + xtick);
			bbound = 1 - (pad->GetTopMargin() + xtick);
			
			//symbol box takes up fMargin = 0.25 by default
			legwidth /= 0.75;
			//add a little padding for each line
			legheight *= 1.2;
			
			if(hdir==left) {
				umin = lbound;
				umax = umin + legwidth;
			}
			else if (hdir==right){
				umax = rbound;
				umin = rbound - legwidth;
			}
			else if(hdir==center){
				umin = (lbound + rbound)/2. - legwidth/2.;
				umax = (lbound + rbound)/2. + legwidth/2.;
			}
			
			if(vdir==top){
				vmax = tbound;
				vmin = vmax - legheight;
			}
			else if(vdir==bottom){
				vmin = bbound;
				vmax = vmin + legheight;
			}
			else if(vdir==middle){
				vmin = (tbound + bbound)/2. - legheight/2.;
				vmax = (tbound + bbound)/2. + legheight/2.;
			}
			
			//initialize legend with determined coords
			leg = new TLegend(umin,vmin,umax,vmax);
			leg->SetFillColor(0);
			leg->SetBorderSize(0);
			leg->SetTextSize(legentry);
			leg->SetTextFont(42);
		}
		void Build(Horz hdir=hdefault){
			bool logy = pad->GetLogy();
			bool logx = pad->GetLogx();
		
			//step 1: determine the highest peak
			int p = 1; //bin number of peak
			int nbins = 1; //number of bins of histo
			double ph = 0; //height of peak + error
			//(could have to modify this later to use error band separately, in case of systematics)
			//loop over sets
			for(unsigned s = 0; s < hists.size(); s++){
				int p_ = hists[s]->GetMaximumBin();
				int nbins_ = hists[s]->GetNbinsX();
				double ph_ = hists[s]->GetBinContent(p_) + hists[s]->GetBinError(p_);
				//check peak height
				if(ph_ > ph) { p = p_; nbins = nbins_; ph = ph_; }
			}
			
			//step 2: determine appropriate legend coords
			if(hdir==hdefault){
				if(p > nbins/2) hdir = left;
				else hdir = right;
			}
			Build(hdir,top);
			
			//step 3: determine ymin (to show low-statistics bins if logy)
			//loop over histos
			if(logy){
				if(manual_ymin) { 
					//ymin is already set
					//make sure it is not zero
					if(ymin==0) ymin = 0.001;
				}
				else{
					ymin = 1; //default value
					double ymin_max = 1;
					double ymin_min = 0.001; //based on user preference
					for(unsigned s = 0; s < hists.size(); s++){
						double ymin_s = hists[s]->GetMinimum(0); //exclude 0
						if(ymin > ymin_s) ymin = ymin_s;
					}
					//final checks
					if(ymin > ymin_max) ymin = ymin_max;
					else if (ymin < ymin_min) ymin = ymin_min;
				}
			}
			else ymin = 0; //just set to 0 for liny
			
			//step 3: find highest bin that could overlap with legend, and set ymax to prevent overlap
			double gy = 1 - (pad->GetBottomMargin() + pad->GetTopMargin());
			double gx = 1 - (pad->GetLeftMargin() + pad->GetRightMargin());
			double eps = 0.05; //small separation between legend and histos

			//bounds to check
			double ucmin[2], ucmax[2], vcmin[2]; //[0] is legend side, [1] is other side
			ucmin[0] = umin;
			ucmax[0] = umax;
			if(hdir==left) {
				ucmin[1] = ucmax[0];
				ucmax[1] = rbound;
			}
			else {
				ucmin[1] = lbound;
				ucmax[1] = ucmin[0];
			}
			vcmin[0] = vmin; //legend always at the bottom
			vcmin[1] = vmax; //just compare to top of plot (margin + ticks) on the other side
			
			//loop over histos
			double bh[2]; //height of highest bin + error (legend)
			bool checkerr = globalOpt->Get("checkerr",true);
			bh[0] = bh[1] = 0;
			for(unsigned s = 0; s < hists.size(); s++){
				TAxis* x1 = hists[s]->GetXaxis();
				int xomin, xomax; //original xmin and xmax bin #s, to reset range at the end
				xomin = x1->GetFirst();
				xomax = x1->GetLast();
				
				for(int i = 0; i < 2; i++){ //check each side of plot
					//new bin #s for limited range
					int xbmin, xbmax;
					
					xbmin = logx
							? x1->FindBin(x1->GetXmin()*pow(x1->GetXmax()/x1->GetXmin(), (ucmin[i] - pad->GetLeftMargin())/gx))
							: x1->FindBin((ucmin[i] - pad->GetLeftMargin())*(x1->GetXmax() - x1->GetXmin())/gx + x1->GetXmin());
					if(xbmin > 1) xbmin -= 1; //include partial overlap
					xbmax = logx
							? x1->FindBin(x1->GetXmin()*pow(x1->GetXmax()/x1->GetXmin(), (ucmax[i] - pad->GetLeftMargin())/gx))
							: x1->FindBin((ucmax[i] - pad->GetLeftMargin())*(x1->GetXmax() - x1->GetXmin())/gx + x1->GetXmin());
					if(xbmax < hists[s]->GetNbinsX()) xbmax += 1; //include partial overlap
					
					//set range for search
					x1->SetRange(xbmin,xbmax);
					int b_ = hists[s]->GetMaximumBin();
					double bh_ = hists[s]->GetBinContent(b_);
					if(checkerr) bh_ += hists[s]->GetBinError(b_);
					//check height
					if(bh_ > bh[i]) bh[i] = bh_;
					
					//reset to original range
					x1->SetRange(xomin,xomax);
				}
			}
			
			double ymax_[2];
			for(int i = 0; i < 2; i++){
				//account for log scale if enabled
				ymax_[i] = logy ? ymin*pow(bh[i]/ymin, gy/(vcmin[i] - pad->GetBottomMargin() - eps)) : ymin + gy*(bh[i] - ymin)/(vcmin[i] - pad->GetBottomMargin() - eps);
			}
			ymax = max(ymax_[0],ymax_[1]);
			
			for(unsigned t = 0; t < extra_text.size(); t++){
				leg->AddEntry((TObject*)NULL,extra_text[t].c_str(),"");
			}
		}
		void Draw(){
			pad->cd();
			if(leg) leg->Draw("same");
		}
		void CheckSize(string line){
			pad->cd();
			TLatex size_test(0,0,line.c_str());
			size_test.SetTextSize(legentry);
			legheight += size_test.GetYsize();
			if(size_test.GetXsize() > legwidth) legwidth = size_test.GetXsize();
		}
		
		//accessors
		void AddHist(TH1F* h) { hists.push_back(h); }
		TLegend* GetLegend() { return leg; }
		pair<double,double> GetRange(){ return make_pair(ymin,ymax); }
		void SetManualYmin(double ym) { ymin = ym; manual_ymin = true; }
		OptionMap* GetLocalOpt() { return localOpt; }
		void SetLocalOpt(OptionMap* opt) { localOpt = opt; if(localOpt==0) localOpt = new OptionMap(); } //must always have an option map
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetGlobalOpt(OptionMap* opt) { globalOpt = opt; if(globalOpt==0) globalOpt = new OptionMap(); } //must always have an option map

	protected:
		//member variables
		TPad* pad;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		double legwidth, legheight;
		double lbound, rbound, tbound, bbound;
		double umin, umax, vmin, vmax;
		double padH, legentry;
		vector<TH1F*> hists;
		TLegend* leg;
		double ymin, ymax;
		bool manual_ymin;
		vector<string> extra_text;
};

#endif