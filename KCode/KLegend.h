#ifndef KLEGEND_H
#define KLEGEND_H

//custom headers
#include "KMap.h"

//ROOT headers
#include <TROOT.h>
#include <TLegend.h>
#include <TH1.h>
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

//----------------------------------------------------------------------------------------------
//little class to store legend entry info before legend construction
class KLegendEntry {
	public:
		//constructor
		KLegendEntry() : obj(NULL), label(""), option("") {}
		KLegendEntry(TObject* obj_, string label_, string option_) : 
					obj(obj_), label(label_), option(option_) {}
		
		//accessors
		TObject* GetObj() { return obj; }
		string GetLabel() { return label; }
		string GetOption() { return option; }
		TH1* GetHist() {
			if(obj && obj->InheritsFrom("TH1")) return (TH1*)obj;
			else return NULL;
		}
		
	protected:
		//member variables
		TObject* obj;
		string label, option;
};

//----------------------------------------------------------------------------------------------
//little class to store groups of legend entries (to keep a set's extra_text associated with it)
class KLegendMultiEntry {
	public:
		//constructor
		KLegendMultiEntry() : entries(), legwidth(0.), legheight(0.) {}
		
		//accessors
		void push_back(KLegendEntry ent) { entries.push_back(ent); }
		void SetSize(double legw, double legh) { legwidth = legw; legheight = legh; }
		vector<KLegendEntry>& GetEntries() { return entries; }
		double GetWidth() { return legwidth; }
		double GetHeight() { return legheight; }
		int GetLines() { return entries.size(); }
		
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
			leg(0), ymin(0), ymax(0), userange(false), manual_ymin(false) 
		{
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
			
			debug = globalOpt->Get("debug_legend",false);
			
			padH = pad->GetWh()*pad->GetAbsHNDC();
			sizeLeg = 26; globalOpt->Get("sizeLeg",sizeLeg);
			legentry = sizeLeg/padH; //line height for each entry
			//todo: configurable font type
			
			//get panel setting (global overrides local)
			localOpt->Get("npanel",npanel);
			globalOpt->Get("npanel",npanel);
			entries.resize(npanel,vector<KLegendEntry>());
			legheights.resize(npanel,0.);
			legwidths.resize(npanel,0.);
			
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
			
			//balancing algorithm, puts MultiEntry entries into simple entries vector(s)
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
				vector<int> min_index_legheight(npanel,-1);
				for(unsigned p = 0; p < npanel; p++){
					if(debug) cout << "panel " << p << ": ";
					for(unsigned e = 0; e < multi_entries.size(); e++){
						double delta_tmp = fabs(multi_legheights[e]/total_legheight - ((double)(p+1)/(double)npanel));
						if(debug) cout << delta_tmp << ", ";
						if(delta_tmp < min_delta_legheight[p]) { min_delta_legheight[p] = delta_tmp; min_index_legheight[p] = e; }
					}
					if(debug) cout << endl;
				}
				
				//fill in entries vector with balanced panel assignments
				for(unsigned p = 0; p < npanel; p++){
					unsigned e_min = 0;
					if(p>0) e_min = min_index_legheight[p-1]+1;
					if(debug) cout << "panel " << p << ": " << e_min << ", " << min_index_legheight[p] << endl;
					for(unsigned e = e_min; e <= min_index_legheight[p]; e++){
						legheights[p] += multi_entries[e].GetHeight();
						if(multi_entries[e].GetWidth()>legwidths[p]) legwidths[p] = multi_entries[e].GetWidth();
						entries[p].insert(entries[p].end(),multi_entries[e].GetEntries().begin(),multi_entries[e].GetEntries().end());
					}
				}
				
			}
			
			//account for npanel
			unsigned max_panel_entries = 0;
			legwidth = legheight = 0;
			for(unsigned p = 0; p < npanel; p++){
				if(entries[p].size()>max_panel_entries) max_panel_entries = entries[p].size();
				if(legheights[p]>legheight) legheight = legheights[p];
				legwidth += legwidths[p];
			}
			
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
			leg->SetNColumns(npanel);
			
			for(unsigned e = 0; e < max_panel_entries; e++){
				for(unsigned p = 0; p < npanel; p++){
					if(entries[p].size()<=e) leg->AddEntry((TObject*)NULL,"",""); //dummy entry to keep things aligned
					else leg->AddEntry(entries[p][e].GetObj(),entries[p][e].GetLabel().c_str(),entries[p][e].GetOption().c_str());
				}
			}
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
			userange = true;
		}
		void Draw(){
			pad->cd();
			if(leg) leg->Draw("same");
		}
		void AddEntry(TObject* obj, string label, string option, unsigned panel=0, const vector<string>& extra_text=vector<string>()){
			if(panel>=npanel) panel = npanel - 1;
		
			KLegendMultiEntry multi;
		
			//check size of each entry, add to multi
			pair<double,double> e_legsize = CheckSize(label);
			TH1* htest = NULL;
			if(balance_panels) {
				multi.push_back(KLegendEntry(obj,label,option));
				htest = multi.GetEntries().back().GetHist();
			}
			else {
				entries[panel].push_back(KLegendEntry(obj,label,option));
				htest = entries[panel].back().GetHist();
			}
			if(htest) hists.push_back(htest);
			for(unsigned t = 0; t < extra_text.size(); t++){
				pair<double,double> e_tmp = CheckSize(extra_text[t]);
				if(e_tmp.first > e_legsize.first) e_legsize.first = e_tmp.first;
				e_legsize.second += e_tmp.second;
				
				if(balance_panels) multi.push_back(KLegendEntry((TObject*)NULL,extra_text[t],""));
				else entries[panel].push_back(KLegendEntry((TObject*)NULL,extra_text[t],""));
			}

			//store multi
			if(balance_panels){
				multi.SetSize(e_legsize.first,e_legsize.second);
				multi_entries.push_back(multi);
			}
			//set size
			else {
				legheights[panel] += e_legsize.second;
				if(e_legsize.first>legwidths[panel]) legwidths[panel] = e_legsize.first;
			}
			
		}
		pair<double,double> CheckSize(string text){
			pad->cd();
			TLatex size_test(0,0,text.c_str());
			size_test.SetTextSize(legentry);
			return make_pair(size_test.GetXsize(), size_test.GetYsize());
		}
		
		//accessors
		void AddHist(TH1* h) { hists.push_back(h); }
		TLegend* GetLegend() { return leg; }
		bool UseRange() { return userange; }
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
		bool debug;
		int npanel;
		bool balance_panels;
		double legwidth, legheight;
		vector<double> legwidths, legheights;
		double lbound, rbound, tbound, bbound;
		double umin, umax, vmin, vmax;
		double padH, sizeLeg, legentry;
		vector<KLegendMultiEntry> multi_entries;
		vector<vector<KLegendEntry> > entries;
		vector<TH1*> hists;
		TLegend* leg;
		double ymin, ymax;
		bool userange;
		bool manual_ymin;
};

#endif