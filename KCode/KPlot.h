#ifndef KPLOT_H
#define KPLOT_H

//custom headers
#include "THN.h"
#include "KMap.h"
#include "KLegend.h"
#include "KParser.h"

//ROOT headers
#include <TROOT.h>
#include <TLegend.h>
#include <TH1.h>
#include <TH1F.h>
#include <TProfile.h>
#include <TH2.h>
#include <TH2F.h>
#include <THnSparse.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLine.h>
#include <TArrow.h>
#include <TLatex.h>
#include <TAxis.h>
#include <TExec.h>

//STL headers
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <exception>

using namespace std;

//-----------------------------------------------------------
//class to store root drawing objects and do histo formatting
//also contains histo-specific options
class KPlot{
	public:
		//constructor
		KPlot() : 
			name(""), localOpt(0), globalOpt(0), histo(0), ratio(0), exec(0), isInit(false), 
			can(0), pad1(0), pad2(0), leg(0), rleg(0), textCMS(0), textExtra(0), textLumi(0), line(0),
			pad1size(0), pad1W(0), pad1H(0), pad2size(0), pad2W(0), pad2H(0), debugarrow(false)
		{
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
			
			SetStyle();
		}
		//universal size values set in initialization list
		KPlot(string name_, OptionMap* localOpt_, OptionMap* globalOpt_) : 
			name(name_), localOpt(localOpt_), globalOpt(globalOpt_), histo(0), ratio(0), exec(0), isInit(false),
			can(0), pad1(0), pad2(0), leg(0), rleg(0), textCMS(0), textExtra(0), textLumi(0), line(0),
			pad1size(0), pad1W(0), pad1H(0), pad2size(0), pad2W(0), pad2H(0)
		{
			//must always have local & global option maps
			if(localOpt==0) localOpt = new OptionMap();
			if(globalOpt==0) globalOpt = new OptionMap();
			
			//enable histo errors
			TH1::SetDefaultSumw2(kTRUE);

			//for debugging
			debugarrow = globalOpt->Get("debugarrow",false);
			
			SetStyle();
		}
		//destructor
		virtual ~KPlot() {}
		//initialization
		void SetStyle(){
			//canvas sizes
			canvasW = 700; globalOpt->Get("canvasW",canvasW);
			canvasH = 550; globalOpt->Get("canvasH",canvasH);
			canvasWextra = 4;
			canvasHextra = 28;
			
			//margins
			marginL = 95; globalOpt->Get("marginL",marginL);
			marginR = 35; globalOpt->Get("marginR",marginR);
			marginB = 75; globalOpt->Get("marginB",marginB);
			marginT = 35; globalOpt->Get("marginT",marginT);
			marginM1 = 15; globalOpt->Get("marginM1",marginM1);
			marginM2 = 10; globalOpt->Get("marginM2",marginM2);
			marginPal = 0; //only used in KPlot2D for palette axis
			
			//automatic calculation of ratio extension height
			//to ensure plot area on pad1 equal to non-ratio case, given 5/7 for pad1, 2/7 for pad2
			pad1size = 5.; globalOpt->Get("pad1size",pad1size);
			pad2size = 2.; globalOpt->Get("pad2size",pad2size);
			ratioH = canvasH*pad2size/pad1size+(marginM1-marginB)*(pad1size+pad2size)/pad1size;
			
			//todo: make relative size of pad2 vs pad1 configurable
			
			//font sizes
			sizeT = 32; globalOpt->Get("sizeT",sizeT);
			sizeL = 28; globalOpt->Get("sizeL",sizeL);
			sizeP = 26; globalOpt->Get("sizeP",sizeP);
			sizePprelim = 23; globalOpt->Get("sizePprelim",sizePprelim);
			sizePlumi = 26; globalOpt->Get("sizePlumi",sizePlumi);
			sizeTick = 12; globalOpt->Get("sizeTick",sizeTick);
			sizeLoff = 5; globalOpt->Get("sizeLoff",sizeLoff);
			epsilon = 2; globalOpt->Get("epsilon",epsilon);
			sizeArrow = 0.075; globalOpt->Get("sizeArrow",sizeArrow); //fractional like sizeLeg
			sizeArrowhead = 8; globalOpt->Get("sizeArrowhead",sizeArrowhead);
			
			//todo: font types
			
			//axis divisions
			NdivX = 507; globalOpt->Get("NdivX",NdivX);
			NdivYhisto = 510; globalOpt->Get("NdivYhisto",NdivYhisto);
			NdivYratio = 503; globalOpt->Get("NdivYratio",NdivYratio);
			
			//ratio range
			ratiomin = 0.45; globalOpt->Get("ratiomin",ratiomin);
			ratiomax = 1.55; globalOpt->Get("ratiomax",ratiomax);
			ratiology = globalOpt->Get("ratiology",false);
			
			//ratio line
			ratiolineval = 1.; globalOpt->Get("ratiolineval",ratiolineval);
			ratiolinewidth = 2; globalOpt->Get("ratiolinewidth",ratiolinewidth);
			ratiolinestyle = 2; globalOpt->Get("ratiolinestyle",ratiolinestyle);
			ratiolinecolor = kRed; globalOpt->Get("ratiolinecolor",ratiolinecolor);
		}
		virtual void CreateHist(){
			//construct histogram		
			vector<double> xbins; localOpt->Get("xbins",xbins);
			int xnum = 0; localOpt->Get("xnum",xnum);
			double xmin = 0; localOpt->Get("xmin",xmin);
			double xmax = 0; localOpt->Get("xmax",xmax);
			
			//make evenly-spaced bins for log scale
			if(localOpt->Get("logxbin",false) and xnum>0 and xmax>xmin){
				xmin = log10(xmin);
				xmax = log10(xmax);
				double width = (xmax-xmin)/xnum;
				xbins.reserve(xnum+1);
				for(int i = 0; i <= xnum; ++i){
					xbins.push_back(pow(10.,xmin+i*width));
				}
			}

			if(!xbins.empty()){ //variable binning case
				if(vars.size()==2) histo = new THN1(new TProfile(name.c_str(),"",xbins.size()-1,&xbins[0]));
				else histo = new THN1(new TH1F(name.c_str(),"",xbins.size()-1,&xbins[0]));
				isInit = true;
			}
			else if(xnum>0 and xmax>xmin){ //standard binning case
				if(vars.size()==2) histo = new THN1(new TProfile(name.c_str(),"",xnum,xmin,xmax));
				else histo = new THN1(new TH1F(name.c_str(),"",xnum,xmin,xmax));
				isInit = true;
			}
			else { //no/incomplete binning information, build failed
				isInit = false;
				return;
			}

			//option to change displayed axis
			double dxmin = 0; bool has_dxmin = localOpt->Get("dxmin",dxmin);
			double dxmax = 0; bool has_dxmax = localOpt->Get("dxmax",dxmax);
			if(has_dxmin and has_dxmax){
				histo->SetAxisRange(dxmin,dxmax);
			}
		}
		virtual void CreateSparseHist(){
			vector<int> nums; localOpt->Get("nums",nums);
			vector<double> mins; localOpt->Get("mins",mins);
			vector<double> maxs; localOpt->Get("maxs",maxs);

			//safety checks
			if(vars.size() != nums.size() || nums.size() != mins.size() || nums.size() != maxs.size()){
				throw runtime_error("Inconsistent sizes for vars, nums, mins, maxs: "+to_string(vars.size())+", "+to_string(nums.size())+", "+to_string(mins.size())+", "+to_string(maxs.size()));
			}

			//create sparse histogram
			THnSparse* htmp = new THnSparseF(name.c_str(),"",vars.size(),nums.data(),mins.data(),maxs.data());

			//check for variable binning (indicated by num=0)
			vector<vector<double>> bins(nums.size());
			for(int i = 0; i < nums.size(); ++i){
				bool variable_bins = false;
				if(nums[i]==0){
					bool found_bins = localOpt->Get("bins"+to_string(i),bins[i]);
					if(!found_bins) throw runtime_error("Indicated variable bins for dimension "+to_string(i)+" but bins"+to_string(i)+" not found");
					variable_bins = true;
				}
				//evenly-spaced bins for log scale
				else if(localOpt->Get("logbin"+to_string(i),false)){
					double bmin = log10(mins[i]);
					double bmax = log10(maxs[i]);
					double width = (bmax-bmin)/nums[i];
					bins[i].reserve(nums[i]+1);
					for(int b = 0; b <= nums[i]; ++b){
						bins[i].push_back(pow(10.,bmin+b*width));
					}
					variable_bins = true;
				}

				if(variable_bins){
					htmp->SetBinEdges(i, bins[i].data());
				}
			}

			histo = new THNn(htmp);
			isInit = true;
		}
		virtual bool Initialize(THN* histo_=NULL){
			if(isInit) return isInit;
			
			//check for TProfile case
			if(!localOpt->Get("vars",vars)){
				//split up histo variables from name (if not otherwise specified)
				KParser::process(name,'_',vars);
			}
			histo = histo_;
			if(!histo){
				int dim = -1;
				localOpt->Get("dimension",dim);
				if(dim==0) CreateSparseHist();
				else CreateHist();
			}
			if(!histo) return isInit; //histo creation failed
			else isInit = true;
			
			//assign bin labels if requested
			//we might want to relabel an existing histo
			if(localOpt->Get("xbinlabel",false)){
				vector<string> labels;
				//check both option maps
				bool has_labels = localOpt->Get("xlabels",labels) || globalOpt->Get(vars[0]+"_labels",labels);
				if(has_labels){
					histo->LabelsOption("v","X");
					for(unsigned b = 0; b < labels.size(); b++){
						histo->GetXaxis()->SetBinLabel(b+1,labels[b].c_str());
					}
					histo->GetXaxis()->SetNoAlphanumeric(); //todo: make optional?
				}
			}
			string xtitle, ytitle;
			if(localOpt->Get("xtitle",xtitle)) histo->GetXaxis()->SetTitle(xtitle.c_str());
			if(localOpt->Get("ytitle",ytitle)) histo->GetYaxis()->SetTitle(ytitle.c_str());
			//allow global override of y title
			if(globalOpt->Get("ytitle",ytitle)) histo->GetYaxis()->SetTitle(ytitle.c_str());
			
			//plotting with ratio enabled by default
			if(localOpt->Get("ratio",true)) {
				
				canvasH += ratioH;
				//can = new TCanvas(histo->GetName(),histo->GetName(),700,700);
				//account for window frame: 2+2px width, 2+26px height
				can = new TCanvas(name.c_str(),name.c_str(),canvasW+canvasWextra,canvasH+canvasHextra);
				//500/(5/7) = 700

				//setup histo and ratio areas for canvas
				pad1 = new TPad("graph","",0,pad2size/(pad1size+pad2size),1.0,1.0);
				pad1W = pad1->GetWw()*pad1->GetAbsWNDC();
				pad1H = pad1->GetWh()*pad1->GetAbsHNDC();
				pad1->SetMargin(marginL/pad1W,marginR/pad1W,marginM1/pad1H,marginT/pad1H);
				pad1->SetTicks(1,1);
				if(localOpt->Get("logy",true)) pad1->SetLogy(); //logy on by default (i.e. liny off by default)
				if(localOpt->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)
				pad1->Draw();
			
				pad2 = new TPad("dmc","",0,0,1.0,pad2size/(pad1size+pad2size));
				pad2W = pad2->GetWw()*pad2->GetAbsWNDC();
				pad2H = pad2->GetWh()*pad2->GetAbsHNDC();
				can->cd();
				pad2->SetMargin(marginL/pad2W,marginR/pad2W,marginB/pad2H,marginM2/pad2H);
				pad2->SetTicks(1,1);
				if(localOpt->Get("logx",false)) pad2->SetLogx(); //logx off by default (i.e. linx on by default)
				if(ratiology) pad2->SetLogy(); //separate logy option for ratio
				pad2->Draw();
			
				//format ratio histo
				ratio = histo->Clone();
				ratio->SetMarkerStyle(20);
				ratio->SetMarkerColor(kBlack);
				ratio->SetLineColor(kBlack);
				FormatHist(pad2,ratio);
				ratio->GetYaxis()->SetRangeUser(ratiomin,ratiomax);
				
				string rationame = "Data/MC";
				globalOpt->Get<string>("rationame",rationame); //ratio name could be provided by user
				ratio->GetYaxis()->SetTitle(rationame.c_str());
				
				//note: axis titles are middle-aligned
				SetTitleOffset(pad2,ratio->GetXaxis());
				SetTitleOffset(pad2,ratio->GetYaxis());
				
				//special formatting for blank histo
				SetTitleOffset(pad1,histo->GetYaxis());
				FormatHist(pad1,histo);
				histo->GetXaxis()->SetLabelOffset(999);
				histo->GetXaxis()->SetTitle("");
				
				//make line
				pad2->cd();
				line = new TLine(
					ratio->GetXaxis()->GetBinLowEdge(ratio->GetXaxis()->GetFirst()),
					ratiolineval,
					ratio->GetXaxis()->GetBinUpEdge(ratio->GetXaxis()->GetLast()),
					ratiolineval
				);
				line->SetLineStyle(ratiolinestyle);
				line->SetLineWidth(ratiolinewidth);
				line->SetLineColor(ratiolinecolor);
			}
			else {
				//can = new TCanvas(histo->GetName(),histo->GetName(),700,550);
				//account for window frame: 2+2px width, 2+26px height
				can = new TCanvas(name.c_str(),name.c_str(),canvasW+canvasWextra,canvasH+canvasHextra);
			
				pad1 = new TPad("graph","",0,0,1,1);
				pad1W = pad1->GetWw()*pad1->GetAbsWNDC();
				pad1H = pad1->GetWh()*pad1->GetAbsHNDC();
				pad1->SetMargin(marginL/pad1W,marginR/pad1W,marginB/pad1H,marginT/pad1H);
				pad1->SetTicks(1,1);
				if(localOpt->Get("logy",true)) pad1->SetLogy(); //logy on by default (i.e. liny off by default)
				if(localOpt->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)
				pad1->Draw();

				FormatHist(pad1,histo);
				SetTitleOffset(pad1,histo->GetXaxis());
				SetTitleOffset(pad1,histo->GetYaxis());
			}
			
			InitializeText();
			InitializeLegend();
			
			return isInit;
		}
		void InitializeLegend(){
			//create legend
			leg = new KLegend(pad1,localOpt,globalOpt);
			if(pad2 and globalOpt->Get("ratioleg",false)) rleg = new KLegend(pad2,localOpt,globalOpt);
		}
		void InitializeText(){
			pad1->cd();
			
			//setup CMS text
			const int cmsFont = 61;
			string cms_text = "CMS";
			globalOpt->Get("cms_text",cms_text);
			TLatex width_test_cms(0,0,cms_text.c_str());
			width_test_cms.SetTextSize(sizeP/pad1H);
			width_test_cms.SetTextFont(cmsFont);
			double posP = 1-(marginT-8)/pad1H;
			double uminCMS = marginL/pad1W;
			double umaxCMS = marginL/pad1W + width_test_cms.GetXsize();
			textCMS = new TLatex(uminCMS,posP,cms_text.c_str());
			textCMS->SetTextSize(sizeP/pad1H);
			textCMS->SetTextFont(cmsFont);
			textCMS->SetNDC();
			
			//setup prelim text
			const int prelimFont = 52;
			string prelim_text = "Preliminary";
			globalOpt->Get("prelim_text",prelim_text);
			if(!prelim_text.empty() and prelim_text[0]!=' ') prelim_text = " "+prelim_text;
			TLatex width_test_extra(0,0,prelim_text.c_str());
			width_test_extra.SetTextSize(sizePprelim/pad1H);
			width_test_extra.SetTextFont(prelimFont);
			double uminExtra = umaxCMS;
			double umaxExtra = uminExtra + width_test_extra.GetXsize();
			textExtra = new TLatex(uminExtra,posP,prelim_text.c_str());
			textExtra->SetTextSize(sizePprelim/pad1H);
			textExtra->SetTextFont(prelimFont);
			textExtra->SetNDC();

			//setup lumi text
			const int lumiFont = 42;
			double intlumi = 0;
			globalOpt->Get<double>("luminorm",intlumi);
			string luminormunit = "fbinv";
			globalOpt->Get<string>("luminormunit",luminormunit);
			int lumiprec = 1; globalOpt->Get("lumiprec",lumiprec);
			stringstream fbname_;
			if(luminormunit=="fbinv") fbname_ << fixed << setprecision(lumiprec) << intlumi/1000 << " fb^{-1} (13 TeV)";
			else if(luminormunit=="pbinv") fbname_ << fixed << setprecision(lumiprec) << intlumi << " pb^{-1} (13 TeV)";
			string fbname = fbname_.str();
			globalOpt->Get("lumi_text",fbname);
			TLatex width_test_lumi(0,0,fbname.c_str());
			width_test_lumi.SetTextSize(sizePlumi/pad1H);
			width_test_lumi.SetTextFont(lumiFont);
			double umaxLumi = 1-marginR/pad1W;
			double uminLumi = umaxLumi - width_test_lumi.GetXsize();
			textLumi = new TLatex(uminLumi,posP,fbname.c_str());
			textLumi->SetTextSize(sizePlumi/pad1H);
			textLumi->SetTextFont(lumiFont);
			textLumi->SetNDC();
		}
		
		//drawing
		virtual void DrawHist(){
			pad1->cd();
			//get y axis range of histo from KLegend
			if(leg->UseRange()) histo->GetYaxis()->SetRangeUser(leg->GetRange().first,leg->GetRange().second);
			histo->Draw("hist");
			pad1->Update();
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
			pad1->Update();
			
			//draw cut lines BEFORE legend/text
			vector<double> xcuts, ycuts;
			vector<Color_t> xcut_colors, ycut_colors;
			vector<int> xcut_arrows, ycut_arrows;

			//pad ranges
			double x1 = pad1->GetLogx() ? pow(10,pad1->GetUxmin()) : pad1->GetUxmin();
			double x2 = pad1->GetLogx() ? pow(10,pad1->GetUxmax()) : pad1->GetUxmax();
			double y1 = pad1->GetLogy() ? pow(10,pad1->GetUymin()) : pad1->GetUymin();
			double y2 = pad1->GetLogy() ? pow(10,pad1->GetUymax()) : pad1->GetUymax();

			//x cuts, use pad y-range
			localOpt->Get("xcuts",xcuts);
			localOpt->Get("xcut_colors",xcut_colors);
			localOpt->Get("xcut_arrows",xcut_arrows);
			bool use_xcut_colors = (xcuts.size()==xcut_colors.size());
			if(!use_xcut_colors && xcut_colors.size()>0){
				throw runtime_error("in histo "+name+", vector lengths of xcuts and xcut_colors do not match!");
			}
			bool use_xcut_arrows = (xcuts.size()==xcut_arrows.size());
			if(!use_xcut_arrows && xcut_arrows.size()>0){
				throw runtime_error("in histo "+name+", vector lengths of xcuts and xcut_arrows do not match!");
			}
			for(unsigned c = 0; c < xcuts.size(); c++){
				TLine* tmp = new TLine(xcuts[c],y1,xcuts[c],y2);
				tmp->SetLineStyle(7);
				tmp->SetLineWidth(2);
				if(use_xcut_colors) tmp->SetLineColor(xcut_colors[c]);
				else tmp->SetLineColor(kBlack);
				tmp->Draw("same");
				xcut_lines.push_back(tmp);
				if(use_xcut_arrows){
					if(xcut_arrows[c]==0) continue;
					//sign of xcut_arrows determines direction: 1 = toward positive x, -1 = toward negative x
					double xarrow = xcuts[c]+xcut_arrows[c]*sizeArrow*(x2-x1);
					//put just below legend (or above if legend on bottom) if arrow would overlap w/ legend
					//otherwise (or if no legend) Uymax (or Uymin if legend on bottom) (need bigger shift in this case)
					bool above = leg ? leg->GetLoc().second==KLegend::bottom : false;
					//include a small offset
					double yshift = (above ? 1 : -1)*histo->GetYaxis()->GetTickLength();
					double yarrow = convertNDCtoUser(convertUsertoNDC(y2, pad1, true) + 2*yshift, pad1, true);
					if(debugarrow) cout << "default: y2 = " << y2 << ", y2NDC = " << convertUsertoNDC(y2,pad1,true) << ", yshift = " << 2*yshift << ", yarrow = " << yarrow << endl;
					if(leg){
						auto legx = leg->GetXcoords();
						legx.first = convertNDCtoUser(legx.first,pad1);
						legx.second = convertNDCtoUser(legx.second,pad1);
						//overlap check
						bool overlap1 = legx.first < xcuts[c] and xcuts[c] < legx.second;
						bool overlap2 = legx.first < xarrow and xarrow < legx.second;
						if(debugarrow) cout << "overlap: legx1 = " << legx.first << ", legx2 = " << legx.second << ", xcut = " << xcuts[c] << " (" << overlap1 << "), xarrow = " << xarrow << " (" << overlap2 << ")" << endl;
						if(overlap1 or overlap2){
							yarrow = convertNDCtoUser(yshift + (above ? leg->GetYcoords().second : leg->GetYcoords().first), pad1, true);
						}
						else if(above){
							yarrow = convertNDCtoUser(convertUsertoNDC(y1, pad1, true) + 2*yshift, pad1, true);
						}
					}
					if(debugarrow) cout << "final: " << yarrow << endl;
					TArrow* atmp = new TArrow(xcuts[c],yarrow,xarrow,yarrow,sizeArrowhead/pad1H);
					atmp->SetLineWidth(2);
					if(use_xcut_colors){
						atmp->SetLineColor(xcut_colors[c]);
						atmp->SetFillColor(xcut_colors[c]);
					}
					atmp->Draw();
					xcut_arrow_lines.push_back(atmp);
				}
			}
			
			//y cuts, use pad x-range
			localOpt->Get("ycuts",ycuts);
			localOpt->Get("ycut_colors",ycut_colors);
			localOpt->Get("ycut_arrows",ycut_arrows);
			bool use_ycut_colors = (ycuts.size()==ycut_colors.size());
			if(!use_ycut_colors && ycut_colors.size()>0){
				throw runtime_error("in histo "+name+", vector lengths of ycuts and ycut_colors do not match!");
			}
			bool use_ycut_arrows = (ycuts.size()==ycut_arrows.size());
			if(!use_ycut_arrows && ycut_arrows.size()>0){
				throw runtime_error("in histo "+name+", vector lengths of ycuts and ycut_arrows do not match!");
			}
			for(unsigned c = 0; c < ycuts.size(); c++){
				TLine* tmp = new TLine(x1,ycuts[c],x2,ycuts[c]);
				tmp->SetLineStyle(7);
				tmp->SetLineWidth(2);
				if(use_ycut_colors) tmp->SetLineColor(ycut_colors[c]);
				else tmp->SetLineColor(kBlack);
				tmp->Draw("same");
				ycut_lines.push_back(tmp);
				if(use_ycut_arrows){
					if(ycut_arrows[c]==0) continue;
					//sign of ycut_arrows determines direction: 1 = toward positive y, -1 = toward negative y
					double yarrow = ycuts[c]+ycut_arrows[c]*sizeArrow*(y2-y1);
					//put just left of legend (or right if legend on left) if arrow would overlap w/ legend
					//otherwise (or if no legend) Uxmax (or Uxmin if legend on right)
					bool right = leg ? leg->GetLoc().first==KLegend::left : false;
					//include a small offset
					double xshift = (right ? 1 : -1)*histo->GetXaxis()->GetTickLength();
					double xarrow = convertNDCtoUser(convertUsertoNDC(x2, pad1) + 2*xshift, pad1);
					if(debugarrow) cout << "default: x2 = " << x2 << ", x2NDC = " << convertUsertoNDC(x2,pad1) << ", xshift = " << 2*xshift << ", xarrow = " << xarrow << endl;
					if(leg){
						auto legy = leg->GetYcoords();
						legy.first = convertNDCtoUser(legy.first,pad1,true);
						legy.second = convertNDCtoUser(legy.second,pad1,true);
						//overlap check
						bool overlap1 = legy.first < ycuts[c] and ycuts[c] < legy.second;
						bool overlap2 = legy.first < yarrow and yarrow < legy.second;
						if(debugarrow) cout << "overlap: legy1 = " << legy.first << ", legy2 = " << legy.second << ", ycut = " << ycuts[c] << " (" << overlap1 << "), yarrow = " << yarrow << " (" << overlap2 << ")" << endl;
						if(overlap1 or overlap2){
							xarrow = convertNDCtoUser(right ? leg->GetXcoords().second : leg->GetXcoords().first, pad1);
						}
						else if(right){
							xarrow = convertNDCtoUser(convertUsertoNDC(x1, pad1) + 2*xshift, pad1);
						}
					}
					if(debugarrow) cout << "final: " << xarrow << endl;
					TArrow* atmp = new TArrow(xarrow,ycuts[c],xarrow,yarrow,sizeArrowhead/pad1H);
					atmp->SetLineWidth(2);
					if(use_ycut_colors){
						atmp->SetLineColor(ycut_colors[c]);
						atmp->SetFillColor(ycut_colors[c]);
					}
					atmp->Draw();
					ycut_arrow_lines.push_back(atmp);
				}
			}

			if(leg) leg->Draw();
			textCMS->Draw("same");
			textExtra->Draw("same");
			textLumi->Draw("same");
		}
		void DrawRatio(){
			pad2->cd();
			ratio->Draw("hist");
			pad2->Update();
			exec->Draw();
		}
		void DrawLine(){
			if(!pad2) return;
			pad2->cd();
			pad2->Update();
			
			//copy x cuts, use pad y-range
			double y1 = pad2->GetLogy() ? pow(10,pad2->GetUymin()) : pad2->GetUymin();
			double y2 = pad2->GetLogy() ? pow(10,pad2->GetUymax()) : pad2->GetUymax();
			for(unsigned c = 0; c < xcut_lines.size(); c++){
				TLine* tmp = (TLine*)xcut_lines[c]->Clone();
				tmp->SetY1(y1);
				tmp->SetY2(y2);
				tmp->Draw("same");
				xcut_ratio_lines.push_back(tmp);
			}
			
			if(line and globalOpt->Get("ratioline",true)) line->Draw("same");
			if(rleg) rleg->Draw();
		}

		//helpers
		void SetTitleOffset(TPad* pad, TAxis* axis){
			double padW, padH;
			pad->cd();
			if(pad==pad1) { padW = pad1W; padH = pad1H; }
			else if(pad==pad2) { padW = pad2W; padH = pad2H; }
			else return;
			
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
			else if(strcmp(axis->GetName(),"zaxis")==0){
				//need to scale title height value from pad height to pad width for z palette axis
				//(acts like y axis)
				Theight *= padH/padW;
				Toff = ((marginR-marginPal)/padW - epsilon/padW - Theight/2.)/(1.6*sizeT/padH);
			}
			
			axis->SetTitleOffset(Toff);
		}
		virtual void FormatHist(TPad* pad, THN* hist){
			double padW, padH;
			double tickScaleX, tickScaleY;
			pad->cd();
			if(pad==pad1) {
				padW = pad1W; padH = pad1H;
				if(pad2) tickScaleY = (padH - marginM1 - marginT)/padH*padW;
				else tickScaleY = (padH - marginB - marginT)/padH*padW;
			}
			else if(pad==pad2) { 
				padW = pad2W; padH = pad2H;
				tickScaleY = (padH - marginB - marginM2)/padH*padW;
			}
			else return;
			tickScaleX = (padW - marginL - marginR)/padW*padH;

			//fix log-scale label offsets (not final)
			double offX, offY, offZ;
			offX = offY = offZ = 1.0;
			if(pad1->GetLogx()) {
				offX = 0.5;
				if(localOpt->Get("morelogxlabels",false)) hist->GetXaxis()->SetMoreLogLabels();
			}
			if(pad1->GetLogy()) {
				offY = 0.5;
				if(localOpt->Get("morelogylabels",false)) hist->GetYaxis()->SetMoreLogLabels();
			}
			if(pad1->GetLogz()) {
				offZ = 0.5;
				if(localOpt->Get("morelogzlabels",false)) hist->GetZaxis()->SetMoreLogLabels();
			}
			
			hist->GetYaxis()->SetTitleSize(sizeT/padH);
			hist->GetYaxis()->SetLabelSize(sizeL/padH);
			hist->GetYaxis()->SetLabelOffset(offY*sizeLoff/padW);
			hist->GetYaxis()->SetTickLength(sizeTick/tickScaleY);
			hist->GetXaxis()->SetTitleSize(sizeT/padH);
			hist->GetXaxis()->SetLabelSize(sizeL/padH);
			hist->GetXaxis()->SetLabelOffset(offX*sizeLoff/padH);
			hist->GetXaxis()->SetTickLength(sizeTick/tickScaleX);
			hist->GetXaxis()->SetNdivisions(NdivX);
			if(hist==ratio) hist->GetYaxis()->SetNdivisions(NdivYratio);
			else if(hist==histo) hist->GetYaxis()->SetNdivisions(NdivYhisto);
			
			//format z axis just in case
			hist->GetZaxis()->SetTitleSize(sizeT/padH);
			hist->GetZaxis()->SetLabelSize(sizeL/padH);
			hist->GetZaxis()->SetLabelOffset(offZ*sizeLoff/padW);
			hist->GetZaxis()->SetTickLength(sizeTick/tickScaleY);
		}
		double convertUsertoNDC(double val, TPad* pad, bool do_y=false){
			if(!do_y) {
				if(pad->GetLogx()) val = log10(val);
				double x1 = pad->GetX1();
				double x2 = pad->GetX2();
				double u = (val-x1)/(x2-x1);
				return u;
			}
			else {
				if(pad->GetLogy()) val = log10(val);
				double y1 = pad->GetY1();
				double y2 = pad->GetY2();
				double v = (val-y1)/(y2-y1);
				return v;
			}
		}
		double convertNDCtoUser(double val, TPad* pad, bool do_y=false){
			if(!do_y) {
				double x1 = pad->GetX1();
				double x2 = pad->GetX2();
				double x = x1 + val*(x2-x1);
				return pad->GetLogx() ? pow(10,x) : x;
			}
			else {
				double y1 = pad->GetY1();
				double y2 = pad->GetY2();
				double y = y1 + val*(y2-y1);
				return pad->GetLogy() ? pow(10,y) : y;
			}
		}
		
		//accessors
		string GetName() { return name; }
		virtual string GetNameX() { return name; }
		virtual string GetNameY() { return ""; }
		bool IsInit() { return isInit; }
		void SetName(string name_) { name = name_; }
		THN* GetHisto() { return histo; }
		THN* GetRatio() { return ratio; }
		TCanvas* GetCanvas() { return can; }
		TPad* GetPad1() { return pad1; }
		TPad* GetPad2() { return pad2; }
		KLegend* GetLegend() { return leg; }
		void SetLegend(KLegend* leg_) { leg = leg_; }
		KLegend* GetLegendRatio() { return rleg; }
		void SetLegendRatio(KLegend* leg_) { rleg = leg_; }
		TLatex* GetCMSText() { return textCMS; }
		TLatex* GetExtraText() { return textExtra; }
		TLatex* GetLumiText() { return textLumi; }
		OptionMap* GetLocalOpt() { return localOpt; }
		void SetLocalOpt(OptionMap* opt) { localOpt = opt; if(localOpt==0) localOpt = new OptionMap(); } //must always have an option map
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void SetGlobalOpt(OptionMap* opt) { globalOpt = opt; if(globalOpt==0) globalOpt = new OptionMap(); } //must always have an option map

	protected:
		//member variables
		string name;
		OptionMap* localOpt;
		OptionMap* globalOpt;
		THN *histo, *ratio;
		TExec *exec;
		bool isInit;
		TCanvas *can;
		TPad *pad1, *pad2;
		KLegend* leg;
		KLegend* rleg;
		TLatex* textCMS;
		TLatex* textExtra;
		TLatex* textLumi;
		TLine* line;
		vector<TLine*> xcut_lines, ycut_lines, xcut_ratio_lines;
		vector<TArrow*> xcut_arrow_lines, ycut_arrow_lines;
		double canvasW, canvasH, canvasWextra, canvasHextra, ratioH;
		double marginL, marginR, marginB, marginT, marginM1, marginM2, marginPal;
		double sizeT, sizeL, sizeP, sizePprelim, sizePlumi, sizeTick, sizeLoff, epsilon, sizeArrow, sizeArrowhead;
		double NdivX, NdivYhisto, NdivYratio;
		double pad1size, pad1W, pad1H, pad2size, pad2W, pad2H;
		double ratiomin, ratiomax;
		bool ratiology;
		double ratiolineval;
		int ratiolinewidth, ratiolinestyle;
		Color_t ratiolinecolor;
		vector<string> vars;
		bool debugarrow;
};

//-----------------------------------------------------------
//extension of KPlot for 2D histograms
//drawn with colz option -> one canvas for each set
class KPlot2D: public KPlot {
	public:
		//constructor
		KPlot2D() : KPlot(), setname("") {}
		KPlot2D(string name_, string setname_, OptionMap* localOpt_, OptionMap* globalOpt_) : KPlot(name_,localOpt_,globalOpt_), setname(setname_) {
			SetStyle();
		}
		//destructor
		virtual ~KPlot2D() {}
		//initialization
		void SetStyle(){
			//2D changes
			//extra space for z axis palette
			//palette width calc from THistPainter::PaintPalette(): mPal = 0.045*padW
			canvasW = 1./0.95*(canvasW + marginL - marginR);
			marginPal = 0.05*canvasW;
			marginR = marginL + marginPal;
		}
		virtual void CreateHist(){
			//construct histogram
			vector<double> xbins; localOpt->Get("xbins",xbins);
			vector<double> ybins; localOpt->Get("ybins",ybins);
			int xnum = 0; localOpt->Get("xnum",xnum);
			int ynum = 0; localOpt->Get("ynum",ynum);
			double xmin = 0; localOpt->Get("xmin",xmin);
			double xmax = 0; localOpt->Get("xmax",xmax);
			double ymin = 0; localOpt->Get("ymin",ymin);
			double ymax = 0; localOpt->Get("ymax",ymax);

			//make evenly-spaced bins for log scale
			if(localOpt->Get("logxbin",false) and xnum>0 and xmax>xmin){
				xmin = log10(xmin);
				xmax = log10(xmax);
				double width = (xmax-xmin)/xnum;
				xbins.reserve(xnum+1);
				for(int i = 0; i <= xnum; ++i){
					xbins.push_back(pow(10.,xmin+i*width));
				}
			}
			//make evenly-spaced bins for log scale
			if(localOpt->Get("logybin",false) and ynum>0 and ymax>ymin){
				ymin = log10(ymin);
				ymax = log10(ymax);
				double width = (ymax-ymin)/ynum;
				ybins.reserve(ynum+1);
				for(int i = 0; i <= ynum; ++i){
					ybins.push_back(pow(10.,ymin+i*width));
				}
			}

			if(!xbins.empty()){ //variable x-binning case
				if(!ybins.empty()) { //variable y-binning case
					histo = new THN1(new TH2F(name.c_str(),"",xbins.size()-1,&xbins[0],ybins.size()-1,&ybins[0]));
					isInit = true;
				}
				else if(ynum>0 and ymax>ymin){ //standard y-binning case
					histo = new THN1(new TH2F(name.c_str(),"",xbins.size()-1,&xbins[0],ynum,ymin,ymax));
					isInit = true;
				}
				else { //no/incomplete binning information, build failed
					isInit = false;
					return;
				}
			}
			else if(xnum>0 and xmax>xmin){ //standard x-binning case
				if(!ybins.empty()) { //variable y-binning case
					histo = new THN1(new TH2F(name.c_str(),"",xnum,xmin,xmax,ybins.size()-1,&ybins[0]));
					isInit = true;
				}
				else if(ynum>0 and ymax>ymin){ //standard y-binning case
					histo = new THN1(new TH2F(name.c_str(),"",xnum,xmin,xmax,ynum,ymin,ymax));
					isInit = true;
				}
				else { //no/incomplete binning information, build failed
					isInit = false;
					return;
				}
			}
			else { //no/incomplete binning information, build failed
				isInit = false;
				return;
			}

			//option to change displayed axis
			double dxmin = 0; bool has_dxmin = localOpt->Get("dxmin",dxmin);
			double dxmax = 0; bool has_dxmax = localOpt->Get("dxmax",dxmax);
			if(has_dxmin and has_dxmax){
				histo->SetAxisRange(dxmin,dxmax);
			}
			double dymin = 0; bool has_dymin = localOpt->Get("dymin",dymin);
			double dymax = 0; bool has_dymax = localOpt->Get("dymax",dymax);
			if(has_dymin and has_dymax){
				histo->SetAxisRange(dymin,dymax,"Y");
			}

			//get vars for labels
			if(!localOpt->Get("vars",vars)){
				//split up histo variables from name (if not otherwise specified)
				KParser::process(name,'_',vars);
			}
			
			//assign bin labels if requested
			//we might want to relabel an existing histo
			if(localOpt->Get("xbinlabel",false)){
				vector<string> labels;
				//check both option maps
				bool has_labels = localOpt->Get("xlabels",labels) || globalOpt->Get(vars[0]+"_labels",labels);
				if(has_labels){
					histo->LabelsOption("v","X");
					for(unsigned b = 0; b < labels.size(); b++){
						histo->GetXaxis()->SetBinLabel(b+1,labels[b].c_str());
					}
					histo->GetXaxis()->SetNoAlphanumeric(); //todo: make optional?
				}
			}
			if(localOpt->Get("ybinlabel",false)){
				vector<string> labels;
				//check both option maps
				bool has_labels = localOpt->Get("ylabels",labels) || globalOpt->Get(vars[1]+"_labels",labels);
				if(has_labels){
					histo->LabelsOption("h","Y");
					for(unsigned b = 0; b < labels.size(); b++){
						histo->GetYaxis()->SetBinLabel(b+1,labels[b].c_str());
					}
					histo->GetYaxis()->SetNoAlphanumeric(); //todo: make optional?
				}
			}

			string xtitle, ytitle, ztitle;
			localOpt->Get("xtitle",xtitle);
			localOpt->Get("ytitle",ytitle);
			localOpt->Get("ztitle",ztitle);
			histo->GetXaxis()->SetTitle(xtitle.c_str());
			histo->GetYaxis()->SetTitle(ytitle.c_str());
			//include name of ratio in z title
			if(setname.find("ratio")!=string::npos){
				string rationame2D = "";
				localOpt->Get(setname + "_name2D",rationame2D); //set in KPlotManager
				histo->GetZaxis()->SetTitle(rationame2D.c_str());
			}
			else { //include legname of set in z title
				string legname = "";
				localOpt->Get(setname + "_legname",legname); //set in KPlotManager
				histo->GetZaxis()->SetTitle(((legname.size()>0 ? legname : setname) + ": " + ztitle).c_str());
			}
		}
		virtual bool Initialize(THN* histo_=NULL){
			if(isInit) return isInit;
			
			histo = histo_;
			if(!histo) CreateHist();
			if(!histo) return isInit; //histo creation failed
			
			string cname = name;
			cname = cname + "_" + setname;
			can = new TCanvas(cname.c_str(),cname.c_str(),canvasW+canvasWextra,canvasH+canvasHextra);
		
			pad1 = new TPad("graph","",0,0,1,1);
			pad1W = pad1->GetWw()*pad1->GetAbsWNDC();
			pad1H = pad1->GetWh()*pad1->GetAbsHNDC();
			pad1->SetMargin(marginL/pad1W,marginR/pad1W,marginB/pad1H,marginT/pad1H);
			pad1->SetTicks(1,1);
			if(localOpt->Get("logz",true) && setname!="ratio") pad1->SetLogz(); //logz on by default (i.e. linz off by default); except for ratio, which never has logz
			if(localOpt->Get("logy",false)) pad1->SetLogy(); //logy off by default (i.e. liny on by default)
			if(localOpt->Get("logx",false)) pad1->SetLogx(); //logx off by default (i.e. linx on by default)
			pad1->Draw();

			FormatHist(pad1,histo);
			SetTitleOffset(pad1,histo->GetXaxis());
			SetTitleOffset(pad1,histo->GetYaxis());
			SetTitleOffset(pad1,histo->GetZaxis());
			
			InitializeText();
			if(globalOpt->Get("leg2d",false)) InitializeLegend();
			
			return isInit;
		}
		//drawing
		void DrawHist(){
			pad1->cd();
			histo->Draw("colz");
			pad1->Update();
		}
		
		//accessors
		string GetSetName() { return setname; }
		
	protected:
		//member variables
		string setname;
};

#endif
