#ifndef KLEGEND_H
#define KLEGEND_H

//custom headers
#include "KMap.h"

//ROOT headers
#include <TROOT.h>
#include <TLegend.h>
#include <TH1.h>
#include <TGraph.h>
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
#include <exception>

using namespace std;

//----------------------------------------------------------------------------------------------
//little class to store legend entry info before legend construction, automatically checks size
class KLegendEntry {
	public:
		//constructor
		KLegendEntry() : obj(NULL), label(""), option(""), legwidth(0.), legheight(0.) {}
		KLegendEntry(TObject* obj_, string label_, string option_, TPad* pad_=NULL, double legentry_=0.) : 
					obj(obj_), label(label_), option(option_), legwidth(0.), legheight(0.)
		{
			if(pad_) CheckSize(pad_,legentry_);
		}
		
		//accessors
		TObject* GetObj() { return obj; }
		string GetLabel() { return label; }
		string GetOption() { return option; }
		TH1* GetHist() {
			if(obj && obj->InheritsFrom("TH1")) return (TH1*)obj;
			else return NULL;
		}
		TGraph* GetGraph() {
			if(obj && obj->InheritsFrom("TGraph")) return (TGraph*)obj;
			else return NULL;
		}
		void CheckSize(TPad* pad, double legentry){
			pad->cd();
			TLatex size_test(0,0,label.c_str());
			size_test.SetTextSize(legentry);
			legwidth = size_test.GetXsize();
			legheight = size_test.GetYsize();
		}
		double GetWidth() { return legwidth; }
		double GetHeight() { return legheight; }
		
	protected:
		//member variables
		TObject* obj;
		string label, option;
		double legwidth, legheight;
};

//----------------------------------------------------------------------------------------------
//little class to store groups of legend entries (to keep a set's extra_text associated with it)
class KLegendMultiEntry {
	public:
		//constructor
		KLegendMultiEntry() : entries(), legwidth(0.), legheight(0.) {}
		
		//accessors
		void push_back(KLegendEntry ent) { 
			entries.push_back(ent);
			//check width and height
			if(ent.GetWidth() > legwidth) legwidth = ent.GetWidth();
			legheight += ent.GetHeight();
		}
		void push_back(KLegendMultiEntry& multi){
			for(unsigned e = 0; e < multi.entries.size(); ++e){
				push_back(multi.entries[e]);
			}
		}
		unsigned size() { return entries.size(); }
		KLegendEntry& back() { return entries.back(); }
		vector<KLegendEntry>& GetEntries() { return entries; }
		KLegendEntry& operator[](unsigned e) { return entries[e]; }
		double GetWidth() { return legwidth; }
		double GetHeight() { return legheight; }
		
	protected:
		//member variables
		vector<KLegendEntry> entries;
		double legwidth, legheight;
};

//----------------------------------------------------------------------------------------------
//class for automatic legend construction and placement
class KLegend{
	public:
		//position enums
		enum Horz { hdefault, left, center, right };
		enum Vert { vdefault, top, middle, bottom };
	
		//constructor
		KLegend(TPad* pad_, OptionMap* localOpt_, OptionMap* globalOpt_) : 
			pad(pad_), localOpt(localOpt_), globalOpt(globalOpt_), debug(false), npanel(1), balance_panels(false), legwidth(0), legheight(0), 
			lbound(0), rbound(0), tbound(0), bbound(0), umin(0), umax(0), vmin(0), vmax(0),
			leg(0), ymin(0), ymax(0), ymin_min(0.001), ymin_max(1), userange(false), manual_ymin(false), manual_ymax(false), hdirFinal(hdefault), vdirFinal(vdefault)
		{
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
			
			debug = globalOpt->Get("debug_legend",false);
			auto_g = globalOpt->Get("auto_g",true);
			
			padH = pad->GetWh()*pad->GetAbsHNDC();
			padW = pad->GetWw()*pad->GetAbsWNDC();
			sizeLeg = 26; globalOpt->Get("sizeLeg",sizeLeg);
			sizeSymb = 25; globalOpt->Get("sizeSymb",sizeSymb);
			legentry = sizeLeg/padH; //line height for each entry
			sizeSymb /= padW;
			//todo: configurable font type
			
			//get panel setting (global overrides local)
			localOpt->Get("npanel",npanel);
			globalOpt->Get("npanel",npanel);
			entries.resize(npanel,KLegendMultiEntry());
			
			//allow multiple lines of text at top of legend
			vector<string> extra_text;
			globalOpt->Get("extra_text",extra_text);
			vector<int> extra_text_panels;
			bool set_panels = globalOpt->Get("extra_text_panels",extra_text_panels);
			unsigned panel_ctr = 0;
			//will be added to top of legend
			for(unsigned t = 0; t < extra_text.size(); t++){
				unsigned panel_tmp = 0;
				if(panel_ctr%npanel==0) panel_ctr = 0;
				
				if(extra_text_panels.size()>t) panel_tmp = extra_text_panels[t];
				else panel_tmp = panel_ctr;
				AddEntry((TObject*)NULL,extra_text[t],"",panel_tmp);
				
				panel_ctr++;
			}
			//keep extra text and real legend entries separate
			//only when entries are being automatically split between panels
			if(!set_panels){
				for(unsigned t = 0; t < (npanel - extra_text.size()%npanel)%npanel; t++){
					if(panel_ctr%npanel==0) panel_ctr = 0;
					AddEntry((TObject*)NULL,"","",panel_ctr);
					panel_ctr++;
				}
			}
			
			//check for automatic panel balancing *after* adding extra_text
			//(not needed if only 1 panel)
			if(npanel>1){
				balance_panels = localOpt->Get("balance_panels",true);
				balance_panels = globalOpt->Get("balance_panels",true);
			}
			
			double ymin_ = 1;
			if(globalOpt->Get("ymin",ymin_) and !globalOpt->Get("disable_ymin",false)) SetManualYmin(ymin_);
			globalOpt->Get("ymin_min",ymin_min);
			globalOpt->Get("ymin_max",ymin_max);
			double ymax_ = 1;
			if(globalOpt->Get("ymax",ymax_) and !globalOpt->Get("disable_ymax",false)) SetManualYmax(ymax_);
			qdefault = 0; globalOpt->Get("qdefault",qdefault);
		}
		//destructor
		virtual ~KLegend() {}
		
		//functions
		void CheckQuadrants(vector<unsigned>& quadrants, const vector<double>& bounds, int n, double* x, double* y){
			double x0 = bounds[0], x1 = bounds[1], x2 = bounds[2];
			double y0 = bounds[3], y1 = bounds[4], y2 = bounds[5];
			for(int i = 0; i < n; ++i){
				Horz hdir = hdefault;
				Vert vdir = vdefault;
				//ignore off-scale points
				if(x0 < x[i] and x[i] < x1) hdir = left;
				else if(x1 < x[i] and x[i] < x2) hdir = right;
				if(y0 < y[i] and y[i] < y1) vdir = bottom;
				else if(y1 < y[i] and y[i] < y2) vdir = top;

				//assign to quadrant
				if(hdir==left and vdir==top) ++quadrants[0];
				else if(hdir==right and vdir==top) ++quadrants[1];
				else if(hdir==right and vdir==bottom) ++quadrants[2];
				else if(hdir==left and vdir==bottom) ++quadrants[3];
			}
		}
		pair<Horz,Vert> BuildG(){
			//first attempt at "best" placement of legend around graphs:
			//find # of points in each quadrant
			//todo: could also include histograms (treat each bin as a point)
			//this doesn't handle a sparse graph with a line through a quadrant connecting points in other quadrants
			//but it seems one would need a full-fledged graphics engine to handle that
			
			//quadrants: 0 = top left, 1 = top right, 2 = bottom right, 3 = bottom left
			vector<unsigned> quadrants(4,0);
			
			//get display bounds; assumption: axis histo has already been drawn so user coords are known
			vector<double> bounds(6,0);
			bounds[0] = pad->GetLogx() ? pow(10,pad->GetUxmin()) : pad->GetUxmin(); //x0
			bounds[2] = pad->GetLogx() ? pow(10,pad->GetUxmax()) : pad->GetUxmax(); //x2
			bounds[1] = pad->GetLogx() ? pow(10,(pad->GetUxmin()+pad->GetUxmax())/2) : (bounds[0]+bounds[2])/2; //x1 midpoint
			bounds[3] = pad->GetLogy() ? pow(10,pad->GetUymin()) : pad->GetUymin(); //y0
			bounds[5] = pad->GetLogy() ? pow(10,pad->GetUymax()) : pad->GetUymax(); //y2
			bounds[4] = pad->GetLogy() ? pow(10,(pad->GetUymin()+pad->GetUymax())/2) : (bounds[3]+bounds[5])/2; //y1 midpoint

			int ngraphpts = 0;
			for(const auto graph : graphs){
				double* x = graph->GetX();
				double* y = graph->GetY();
				int n = graph->GetN();
				CheckQuadrants(quadrants, bounds, n, x, y);
				ngraphpts += n;
			}

			//also check histograms!
			int nhistpts = 0;
			for(const auto hist : hists){
				vector<double> x, y;
				for(int b = 1; b <= hist->GetNbinsX(); ++b){
					x.push_back(hist->GetBinCenter(b));
					y.push_back(hist->GetBinContent(b));
				}
				CheckQuadrants(quadrants, bounds, hist->GetNbinsX(), x.data(), y.data());
				nhistpts += x.size();
			}
			
			//find the least populated quadrant
			unsigned bestq = qdefault; //default
			unsigned bestqval = quadrants[qdefault];
			for(unsigned q = 1; q < quadrants.size(); ++q){
				if(quadrants[q] < bestqval){
					bestqval = quadrants[q];
					bestq = q;
				}
			}
			
			if(debug){
				cout << scientific;
				cout << "g: " << graphs.size() << " w/ " << ngraphpts << " total points" << endl;
				cout << "h: " << hists.size() << " w/ " << nhistpts << " total points" << endl;
				cout << "x: " << bounds[0] << " " << bounds[1] << " " << bounds[2] << endl;
				cout << "y: " << bounds[3] << " " << bounds[4] << " " << bounds[5] << endl;
				cout << "q: " << quadrants[0] << " " << quadrants[1] << " " << quadrants[2] << " " << quadrants[3] << endl;
				cout << "b: " << bestq << endl;
				cout << fixed;
			}
			
			if(bestq==1) return make_pair(right,top);
			else if(bestq==2) return make_pair(right,bottom);
			else if(bestq==3) return make_pair(left,bottom);
			else return make_pair(left,top); //default
		}
		void Build(Horz hdir, Vert vdir){
			//boundaries of plot area
			double ytick = (hists.size()>0) ? hists[0]->GetYaxis()->GetTickLength() : 0;
			double xtick = (hists.size()>0) ? hists[0]->GetXaxis()->GetTickLength() : 0;
			lbound = pad->GetLeftMargin() + ytick;
			rbound = 1 - (pad->GetRightMargin() + ytick);
			tbound = 1 - (pad->GetTopMargin() + xtick);
			bbound = pad->GetBottomMargin() + xtick;
			if(debug) cout << "bounds: " << lbound << " " << rbound << " " << tbound << " " << bbound << endl;
			
			//use graph-based algo
			if((auto_g and !graphs.empty()) or (hdir==hdefault and vdir==vdefault)){
				auto dirs = BuildG();
				hdir = dirs.first;
				vdir = dirs.second;
			}
			
			//balancing algorithm, puts MultiEntry entries into panel entries vector(s)
			if(balance_panels){
				//sum up heights
				vector<double> multi_legheights(multi_entries.size(),0.);
				multi_legheights[0] = multi_entries[0].GetHeight();
				if(debug) cout << multi_legheights[0] << ", ";
				for(unsigned e = 1; e < multi_entries.size(); e++){
					multi_legheights[e] = multi_legheights[e-1] + multi_entries[e].GetHeight();
					if(debug) cout << multi_legheights[e] << ", ";
				}
				if(debug) cout << endl;
				
				//check for closest height to total_legheight/npanel, ... etc.
				double total_legheight = multi_legheights.back();
				vector<double> min_delta_legheight(npanel,1e10);
				vector<unsigned> min_index_legheight(npanel,-1);
				for(int p = 0; p < npanel; p++){
					if(debug) cout << "panel " << p << ": ";
					for(unsigned e = 0; e < multi_entries.size(); e++){
						double delta_tmp = fabs(multi_legheights[e]/total_legheight - ((double)(p+1)/(double)npanel));
						if(debug) cout << delta_tmp << ", ";
						if(delta_tmp < min_delta_legheight[p]) { min_delta_legheight[p] = delta_tmp; min_index_legheight[p] = e; }
					}
					if(debug) cout << endl;
				}
				
				//fill in entries vector with balanced panel assignments
				for(int p = 0; p < npanel; p++){
					unsigned e_min = 0;
					if(p>0) e_min = min_index_legheight[p-1]+1;
					if(debug) cout << "panel " << p << ": " << e_min << ", " << min_index_legheight[p] << endl;
					for(unsigned e = e_min; e <= min_index_legheight[p]; e++){
						entries[p].push_back(multi_entries[e]);
					}
				}
				
			}
			
			//account for npanel
			unsigned max_panel_entries = 0;
			legwidth = legheight = 0;
			for(int p = 0; p < npanel; p++){
				if(entries[p].size()>max_panel_entries) max_panel_entries = entries[p].size();
				if(entries[p].GetHeight()>legheight) legheight = entries[p].GetHeight();
				legwidth += entries[p].GetWidth();
			}
			
			//symbol box takes up fMargin = 0.25 by default (now configurable in pixels)
			legwidth += sizeSymb*npanel;
			double sizeSymbRel = sizeSymb*npanel/legwidth;
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
			
			if(debug) cout << "coords: " << umin << " " << vmin << " " << umax << " " << vmax << endl;
			//initialize legend with determined coords
			leg = new TLegend(umin,vmin,umax,vmax);
			leg->SetFillColor(0);
			leg->SetBorderSize(0);
			leg->SetTextSize(legentry);
			leg->SetTextFont(42);
			leg->SetNColumns(npanel);
			leg->SetMargin(sizeSymbRel);
			
			for(unsigned e = 0; e < max_panel_entries; e++){
				for(int p = 0; p < npanel; p++){
					if(entries[p].size()<=e) leg->AddEntry((TObject*)NULL,"",""); //dummy entry to keep things aligned
					else leg->AddEntry(entries[p][e].GetObj(),entries[p][e].GetLabel().c_str(),entries[p][e].GetOption().c_str());
				}
			}

			hdirFinal = hdir;
			vdirFinal = vdir;
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
					for(unsigned s = 0; s < hists.size(); s++){
						double ymin_s = hists[s]->GetMinimum(0); //exclude 0
						if(ymin > ymin_s) ymin = ymin_s;
					}
					//final checks
					if(ymin_max > 0 && ymin > ymin_max) ymin = ymin_max;
					else if (ymin_min > 0 && ymin < ymin_min) ymin = ymin_min;
				}
			}
			else {
				if(!manual_ymin) ymin = 0; //just set to 0 for liny
			}

			//step 3.5: stop if manual ymax specified
			if(manual_ymax){
				userange = true;
				return;
			}
			
			//step 4: find highest bin that could overlap with legend, and set ymax to prevent overlap
			double gy = 1 - (pad->GetBottomMargin() + pad->GetTopMargin());
			double gx = 1 - (pad->GetLeftMargin() + pad->GetRightMargin());
			double eps = 0.05; //small separation between legend and histos
			if(debug){
				cout << "Pad margins: " << pad->GetLeftMargin() << " " << pad->GetRightMargin() << " " << pad->GetBottomMargin() << " " << pad->GetTopMargin() << endl;
				cout << "Misc: " << gx << " " << gy << " " << eps << endl;
			}

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

			if(debug){
				cout << "Bounds to check (legend side, 0): " << ucmin[0] << " " << ucmax[0] << " " << vcmin[0] << endl;
				cout << "Bounds to check  (other side, 1): " << ucmin[1] << " " << ucmax[1] << " " << vcmin[1] << endl;
			}

			//loop over histos
			double bh[2]; //height of highest bin + error (legend)
			bool checkerr = globalOpt->Get("checkerr",true);
			bh[0] = bh[1] = 0;
			for(unsigned s = 0; s < hists.size(); s++){
				TAxis* x1 = hists[s]->GetXaxis();
				//original xmin and xmax bin #s, to reset range at the end
				int xomin = x1->GetFirst();
				int xomax = x1->GetLast();
				//get min & max accounting for possible range change
				double x1min = x1->GetBinLowEdge(xomin);
				double x1max = x1->GetBinUpEdge(xomax);

				for(int i = 0; i < 2; i++){ //check each side of plot
					//new bin #s for limited range
					int xbmin, xbmax;
					
					xbmin = logx
							? x1->FindBin(x1min*pow(x1max/x1min, (ucmin[i] - pad->GetLeftMargin())/gx))
							: x1->FindBin((ucmin[i] - pad->GetLeftMargin())*(x1max - x1min)/gx + x1min);
					if(i==1 and xbmin > 1) xbmin -= 1; //include extra bin for non-legend side (in case histo taller than legend)
					xbmax = logx
							? x1->FindBin(x1min*pow(x1max/x1min, (ucmax[i] - pad->GetLeftMargin())/gx))
							: x1->FindBin((ucmax[i] - pad->GetLeftMargin())*(x1max - x1min)/gx + x1min);
					if(i==1 and xbmax < hists[s]->GetNbinsX()) xbmax += 1; //include extra bin for non-legend side (in case histo taller than legend)

					//set range for search
					x1->SetRange(xbmin,xbmax);
					int b_ = hists[s]->GetMaximumBin();
					double bh_ = hists[s]->GetBinContent(b_);
					if(checkerr) bh_ += hists[s]->GetBinError(b_);
					//check height
					if(bh_ > bh[i]) bh[i] = bh_;

					if(debug) cout << "Overlap check for " << hists[s]->GetName() << " (" << i << "): bin range [" << xbmin << "," << xbmax << "], max bin " << b_ << ", height " << bh_ << (checkerr ? " (includes err)" : "") << endl;
					
					//reset to original range
					x1->SetRange(xomin,xomax);
				}
			}
			
			double ymax_[2];
			for(int i = 0; i < 2; i++){
				//account for log scale if enabled
				ymax_[i] = logy ? ymin*pow(bh[i]/ymin, gy/(vcmin[i] - pad->GetBottomMargin() - eps)) : ymin + gy*(bh[i] - ymin)/(vcmin[i] - pad->GetBottomMargin() - eps);
			}
			if(debug){
				cout << "Results (" << 0 << "): bh = " << bh[0] << ", ymax = " << ymax_[0] << endl;
				cout << "Results (" << 1 << "): bh = " << bh[1] << ", ymax = " << ymax_[1] << endl;
			}
			ymax = max(ymax_[0],ymax_[1]);
			userange = true;
		}
		void Draw(){
			pad->cd();
			if(leg) leg->Draw("same");
		}
		void AddEntry(TObject* obj, string label, string option, int panel=0, const vector<string>& extra_text=vector<string>(), bool addToPrev=false){
			if(panel>=npanel) panel = npanel - 1;
		
			if(balance_panels && (!addToPrev || multi_entries.size()==0)) multi_entries.push_back(KLegendMultiEntry());
			
			KLegendMultiEntry& multi = balance_panels ? multi_entries.back() : entries[panel];
		
			//add entry to multi & check histo
			multi.push_back(KLegendEntry(obj,label,option,pad,legentry));
			TH1* htest = multi.back().GetHist();
			if(htest) hists.push_back(htest);
			else {
				TGraph* gtest = multi.back().GetGraph();
				if(gtest) graphs.push_back(gtest);
			}
			//add extra text to multi
			for(unsigned t = 0; t < extra_text.size(); t++){				
				multi.push_back(KLegendEntry((TObject*)NULL,extra_text[t],"",pad,legentry));
			}
		}
		
		//accessors
		void AddHist(TH1* h) { hists.push_back(h); }
		void AddGraph(TGraph* g) { graphs.push_back(g); }
		TLegend* GetLegend() { return leg; }
		bool UseRange() { return userange; }
		pair<double,double> GetRange(){ return make_pair(ymin,ymax); }
		pair<double,double> GetXcoords(){ return make_pair(umin,umax); }
		pair<double,double> GetYcoords(){ return make_pair(vmin,vmax); }
		void SetManualYmin(double ym) { ymin = ym; manual_ymin = true; }
		void SetManualYmax(double ym) { ymax = ym; manual_ymax = true; }
		OptionMap* GetLocalOpt() { return localOpt; }
		void SetLocalOpt(OptionMap* opt) { localOpt = opt; if(localOpt==0) localOpt = new OptionMap(); } //must always have an option map
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetGlobalOpt(OptionMap* opt) { globalOpt = opt; if(globalOpt==0) globalOpt = new OptionMap(); } //must always have an option map
		void SetDefaultQuadrant(int q) { qdefault = q; }
		int GetDefaultQuadrant() { return qdefault; }
		pair<Horz,Vert> GetLoc() { return make_pair(hdirFinal,vdirFinal); }

	protected:
		//member variables
		TPad* pad;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		bool debug, auto_g;
		int npanel;
		bool balance_panels;
		double legwidth, legheight;
		double lbound, rbound, tbound, bbound;
		double umin, umax, vmin, vmax;
		double padH, padW, sizeLeg, legentry, sizeSymb;
		vector<KLegendMultiEntry> multi_entries;
		vector<KLegendMultiEntry> entries;
		vector<TH1*> hists;
		vector<TGraph*> graphs;
		TLegend* leg;
		double ymin, ymax;
		double ymin_min, ymin_max;
		bool userange;
		bool manual_ymin, manual_ymax;
		int qdefault;
		Horz hdirFinal;
		Vert vdirFinal;
};

#endif
