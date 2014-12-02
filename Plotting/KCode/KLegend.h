#ifndef KLEGEND_H
#define KLEGEND_H

//ROOT headers
#include <TROOT.h>
#include <TLegend.h>
#include <TH1F.h>

//STL headers
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iomanip>
#include <cmath>

using namespace std;

class KLegend{
	public:
		//constructor
		KLegend(int nentries_, bool chan_=true) : nentries(nentries_), leg(0), ymin(0), ymax(0), manual_ymin(false), do_chan(chan_), chan_label("") {
			if(do_chan) {
				//todo: change option from bool to string, to vary label
				chan_label = "#mu#tau channel";
				++nentries; //will be added to top of legend
			}
		}
		//destructor
		virtual ~KLegend() {}
		
		//functions
		void Build(TPad* pad){
			bool logy = pad->GetLogy();
			bool logx = pad->GetLogx();
			double padH = pad->GetWh()*pad->GetAbsHNDC();
			double ytick = (hists.size()>0) ? hists[0]->GetYaxis()->GetTickLength() : 0;
			double xtick = (hists.size()>0) ? hists[0]->GetXaxis()->GetTickLength() : 0;
		
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
			double umin, umax, vmin, vmax;
			double lbound = pad->GetLeftMargin() + ytick;
			double rbound = 1 - (pad->GetRightMargin() + ytick);
			
			//how to determine width?
			double legwidth = 0.33;
			
			if(p > nbins/2) {
				umin = lbound;
				umax = umin + legwidth;
			}
			else {
				umax = rbound;
				umin = umax - legwidth;
			}

			//double legentry = 0.05; //line height for each entry
			double legentry = 26/padH; //line height for each entry
			double legheight = legentry*nentries; //total height of all entries
			
			vmax = 1 - (pad->GetTopMargin() + xtick);
			vmin = vmax - legheight;		

			//initialize legend with determined coords
			leg = new TLegend(umin,vmin,umax,vmax);
			leg->SetFillColor(0);
			leg->SetBorderSize(0);
			leg->SetTextSize(legentry);
			leg->SetTextFont(42);
			
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
			if(p > nbins/2) {
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
					double bh_ = hists[s]->GetBinContent(b_) + hists[s]->GetBinError(b_);
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
			
			if(do_chan) leg->AddEntry((TObject*)NULL,chan_label.c_str(),"");
		}
		void Draw(TPad* pad){
			pad->cd();
			if(leg) leg->Draw("same");
		}
		
		//accessors
		void AddHist(TH1F* h) { hists.push_back(h); }
		TLegend* GetLegend() { return leg; }
		pair<double,double> GetRange(){ return make_pair(ymin,ymax); }
		void SetManualYmin(double ym) { ymin = ym; manual_ymin = true; }

	protected:
		//member variables
		int nentries;
		string fbname;
		vector<TH1F*> hists;
		TLegend* leg;
		double ymin, ymax;
		bool manual_ymin;
		bool do_chan;
		string chan_label;
};

#endif