#ifndef KMANAGER_H
#define KMANAGER_H

//custom headers
#include "KSet.h"
#include "KPlot.h"
#include "KLegend.h"

//ROOT headers
#include <TROOT.h>

//STL headers
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

//forward declaration of parser
class KParser;

//------------------------------------------
//class to manage all objects and draw plots
class KManager {
	public:
		friend class KParser;
		//constructor
		KManager() : input(""),treedir("tree"),MyParser(0),MyRatio(0),globalOpt(0),s_numer(""),s_denom(""),s_yieldref(""),numer(0),denom(0),yieldref(0),doPrint(false),varbins(0),nbins(0),parsed(false) {}
		KManager(string in, string dir="tree"); //implemented in KParser.h to avoid circular dependency
		//destructor
		virtual ~KManager() {}
		virtual void FakeTauEstimationInit(){
			string tfr_name = "";
			TFile* tfr_file = 0;
			if(globalOpt->Get("tfr_file",tfr_name)) tfr_file = TFile::Open(tfr_name.c_str(),"READ");
			if(tfr_file) {
				TGraphAsymmErrors* tfr_data = (TGraphAsymmErrors*)tfr_file->Get("data");
				TGraphAsymmErrors* tfr_mc = 0;
				if(globalOpt->Get("wjet_tfr",false)) tfr_mc = (TGraphAsymmErrors*)tfr_file->Get("W + jets");
				else if(globalOpt->Get("ttbar_tfr",false)) tfr_mc = (TGraphAsymmErrors*)tfr_file->Get("ttbar");
				else tfr_mc = (TGraphAsymmErrors*)tfr_file->Get("Z + jets");
				globalOpt->Set("tfr_data",tfr_data);
				globalOpt->Set("tfr_mc",tfr_mc);
			}
		}
		//where the magic happens
		void DrawPlots(){
			//do everything
			if(!parsed) return;
			
			//setup root output file if requested
			TFile* out_file = 0;
			string rootfilename = "";
			if(globalOpt->Get<string>("rootfile",rootfilename)) out_file = TFile::Open((rootfilename+".root").c_str(),"RECREATE");
			
			//check for intlumi
			//if it has not been set by the user, get it from data
			double intlumi = 0;
			if(!(globalOpt->Get<double>("luminorm",intlumi))) {
				for(unsigned s = 0; s < MySets.size(); s++){
					double tmp = 0;
					MySets[s]->GetLocalOpt()->Get("intlumi",tmp);
					if(tmp>0) { intlumi = tmp; }
				}
				if(intlumi==0) intlumi = 1; //default value
				globalOpt->Set("luminorm",intlumi);
			}
			
			//get special status options
			globalOpt->Get("numer",s_numer);
			globalOpt->Get("denom",s_denom);
			globalOpt->Get("yieldref",s_yieldref);
			
			//check for special status sets
			for(unsigned s = 0; s < MySets.size(); s++){
				if(s_numer.size()>0 && numer==0 && MySets[s]->GetName()==s_numer) numer = MySets[s];
				if(s_denom.size()>0 && denom==0 && MySets[s]->GetName()==s_denom) denom = MySets[s];
				if(s_yieldref.size()>0 && yieldref==0 && MySets[s]->GetName()==s_yieldref) yieldref = MySets[s];
			}
			bool ratio_allowed = numer && denom;
			if(ratio_allowed){
				//add children to ratio
				MyRatio->AddNumerator(numer);
				MyRatio->AddDenominator(denom);
				//set ratio name for 2D histos
				string rationame2D = "[" + numer->GetName() + " - " + denom->GetName() + "]/#sigma";
				globalOpt->Set("rationame2D",rationame2D);
			}			
			
			//create plots from local options
			//in 2D case, one plot for each top-level set
			OMMit om;			
			for(om = MyPlotOptions.GetTable().begin(); om != MyPlotOptions.GetTable().end(); om++){
				if(om->second->Get("ratio",true) && !ratio_allowed){ //ratios turned on by default
					om->second->Set("ratio",false); //disable ratios if components not available
					cout << "Input error: ratio requested for histo " << om->first << ", but ";
					if(!numer && !denom) cout << "numer and denom ";
					else if(!numer) cout << "numer ";
					else if(!denom) cout << "denom ";
					cout << "not set. Ratio will not be drawn." << endl;
				}
				int dim = 0;
				om->second->Get("dimension",dim);
				if(dim==1){
					KPlot* ptmp = new KPlot(om->first,om->second,globalOpt);
					if(ptmp->Initialize()) MyPlots.Add(om->first,ptmp);
					else {
						cout << "Input error: unable to build histo " << om->first << ". Check binning options." << endl;
						delete ptmp;
					}
				}
				else if(dim==2){
					PlotMap* p2map = new PlotMap();
					for(unsigned s = 0; s < MySets.size()+1; s++){
						KBase* theSet;
						if(s>=MySets.size()){ //add ratio set at the end of loop if enabled
							if(om->second->Get("ratio",true)) theSet = MyRatio;
							else continue;
						}
						else theSet = MySets[s];
						
						KPlot* ptmp = new KPlot2D(om->first,theSet,om->second,globalOpt);
						if(ptmp->Initialize()) p2map->Add(theSet->GetName(),ptmp);
						else {
							cout << "Input error: unable to build 2D histo " << om->first << " for set " << theSet->GetName() << ". Check binning options." << endl;
							delete ptmp;
						}
					}
					
					if(p2map->GetTable().begin() != p2map->GetTable().end()) MyPlots2D.Add(om->first,p2map);
					else {
						cout << "Input error: unable to build any 2D histos " << om->first << ". Check binning options." << endl;
						delete p2map;
					}
				}
			}
		
			//load histos into sets
			PMit p;
			for(p = MyPlots.GetTable().begin(); p != MyPlots.GetTable().end(); p++){
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->AddHisto(p->first,p->second->GetHisto());
					//cout << p->first << " " << MySets[s]->GetName() << endl;
				}
			}
			PMMit pm;
			for(pm = MyPlots2D.GetTable().begin(); pm != MyPlots2D.GetTable().end(); pm++){
				PlotMap* p2map = pm->second;
				for(unsigned s = 0; s < MySets.size(); s++){
					KPlot* ptmp = p2map->Get(MySets[s]->GetName());
					if(ptmp) MySets[s]->AddHisto(pm->first,ptmp->GetHisto());
					//cout << pm->first << " " << MySets[s]->GetName() << endl;
				}
			}
			
			//build everything
			for(unsigned s = 0; s < MySets.size(); s++){
				MySets[s]->Build();
			}
			
			//fake tau estimation is calculated during build
			if(globalOpt->Get("calcfaketau",false)){
				double ft_norm = 0;
				globalOpt->Get("ft_norm",ft_norm);
				double ft_err = 0;
				globalOpt->Get("ft_err",ft_err);
				//finish error calc
				ft_err = sqrt(ft_err);
				
				int prcsn;
				if(globalOpt->Get("yieldprecision",prcsn)) cout << fixed << setprecision(prcsn);
				cout << "fake tau norm = " << ft_norm << " +/- " << ft_err << endl;
			}
			
			//draw each plot - normalization, legend, ratio
			for(p = MyPlots.GetTable().begin(); p != MyPlots.GetTable().end(); p++){
				//get drawing objects from KPlot
				TCanvas* can = p->second->GetCanvas();
				TPad* pad1 = p->second->GetPad1();
				
				//create legend
				string chan_label = "";
				globalOpt->Get("chan_label",chan_label);
				KLegend* kleg = new KLegend(pad1,chan_label);
				double ymin = 1;
				if(globalOpt->Get("ymin",ymin)) kleg->SetManualYmin(ymin);
				p->second->SetLegend(kleg);
				
				//select current histogram in sets
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->GetHisto(p->first);
				}
				
				//check if normalization to yield is desired (disabled by default)
				//BEFORE printing yields
				//needs to be separate loop because yieldref must have current histo selected to get appropriate yield
				//then print yield if enabled
				//BEFORE division by bin width if requested
				//then pass current histogram to legend
				//AFTER bindivide if requested
				//and calculate legend size
				double yield = 0;
				if(yieldref) yield = yieldref->GetYield();
				if(globalOpt->Get("printyield",false)) cout << p->first << " yield:" << endl;
				for(unsigned s = 0; s < MySets.size(); s++){
					if(yieldref && p->second->GetLocalOpt()->Get("yieldnorm",false) && yield>0 && MySets[s] != yieldref) MySets[s]->Normalize(yield);
					if(globalOpt->Get("printyield",false)) MySets[s]->PrintYield();
					if(p->second->GetLocalOpt()->Get("bindivide",false)) MySets[s]->BinDivide();
					kleg->AddHist((TH1F*)MySets[s]->GetHisto());
					MySets[s]->GetLegendInfo(kleg);
				}
				if(globalOpt->Get("printyield",false)) cout << endl;
				if(p->second->GetLocalOpt()->Get("yieldnorm",false) && !yieldref){
					cout << "Input error: normalization to yield requested for " << p->first << ", but yieldref not set. Normalization will not be performed." << endl;
				}
				
				//build legend
				kleg->Build();
				
				//add sets to legend, after building
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->AddToLegend(kleg->GetLegend());
				}
				
				//draw blank histo for axes
				p->second->DrawHist();
				//draw sets (reverse order, so first set is on top)
				for(int s = MySets.size()-1; s >= 0; s--){
					MySets[s]->Draw(pad1);
					
					//save histos in root file if requested
					if(out_file){
						out_file->cd();
						string oname = p->first + "_" + MySets[s]->GetName();
						MySets[s]->GetHisto()->Write(oname.c_str());
					}
				}
				p->second->GetHisto()->Draw("sameaxis"); //draw again so axes on top
				p->second->DrawText();
				
				//ratio (enabled by default, auto-disabled above if components not set)
				if(p->second->GetLocalOpt()->Get("ratio",true)){
					TPad* pad2 = p->second->GetPad2();

					p->second->DrawRatio();
				
					MyRatio->Build(p->second->GetHisto());
					
					MyRatio->Draw(pad2);
					p->second->DrawLine();
				}
				
				//if printing not enabled, does nothing
				PrintCanvas(p->first,can);
			}

						
			//draw each 2D plot - normalization, etc.
			for(pm = MyPlots2D.GetTable().begin(); pm != MyPlots2D.GetTable().end(); pm++){
				PlotMap* p2map = pm->second;
				
				//select current histogram in sets
				//needs to happen first in case of normalization to yield
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->GetHisto(pm->first);
				}
				double yield = 0;
				if(p2map->GetTable().begin()->second->GetLocalOpt()->Get("yieldnorm",false) && yieldref){
						yield = yieldref->GetYield();
				}
				else if(p2map->GetTable().begin()->second->GetLocalOpt()->Get("yieldnorm",false) && !yieldref){
					cout << "Input error: normalization to yield requested for " << pm->first << ", but yieldref not set. Normalization will not be performed." << endl;
				}
				
				double zmin = 1e100;
				double zmax = 0;
				//check if normalization to yield is desired (disabled by default)
				//BEFORE printing yields
				//then print yield if enabled
				//BEFORE division by bin width if requested
				if(globalOpt->Get("printyield",false)) cout << pm->first << " yield:" << endl;
				for(unsigned s = 0; s < MySets.size(); s++){
					KBase* theSet = MySets[s];
					KPlot* ptmp = p2map->Get(theSet->GetName());
					
					if(yield>0 && theSet != yieldref) theSet->Normalize(yield);
					if(globalOpt->Get("printyield",false)) theSet->PrintYield();
					if(ptmp->GetLocalOpt()->Get("bindivide",false)) theSet->BinDivide();
					
					//check z-axis limits after all potential normalizations are done
					if(theSet->GetHisto()->GetMinimum() < zmin) zmin = theSet->GetHisto()->GetMinimum();
					if(theSet->GetHisto()->GetMaximum() > zmax) zmax = theSet->GetHisto()->GetMaximum();
				}
				if(globalOpt->Get("printyield",false)) cout << endl;
				
				//one plot for each set
				for(unsigned s = 0; s < MySets.size()+1; s++){
					KBase* theSet;
					KPlot* ptmp;
					if(s>=MySets.size()){ //add ratio set at the end of loop if enabled
						ptmp = p2map->Get("ratio");
						if(ptmp) theSet = MyRatio;
						else continue;
						//build ratio histo
						theSet->Build(ptmp->GetHisto());
					}
					else {
						theSet = MySets[s];
						ptmp = p2map->Get(theSet->GetName());
						if(!ptmp) continue;
					}
					
					//get drawing objects from KPlot
					TCanvas* can = ptmp->GetCanvas();
					TPad* pad1 = ptmp->GetPad1();
					
					//reset histo z-axes
					if(theSet==MyRatio) {
						//symmetric axis for ratio
						//take min above -999 because empty bins set to -1000 (drawing hack)
						double zmax_ratio = fmax(fabs(theSet->GetHisto()->GetMinimum(-999)),fabs(theSet->GetHisto()->GetMaximum()));
						ptmp->GetHisto()->GetZaxis()->SetRangeUser(-zmax_ratio,zmax_ratio);
						theSet->GetHisto()->GetZaxis()->SetRangeUser(-zmax_ratio,zmax_ratio);
					}
					else {
						ptmp->GetHisto()->GetZaxis()->SetRangeUser(zmin,zmax);
						theSet->GetHisto()->GetZaxis()->SetRangeUser(zmin,zmax);
					}
					//draw blank histo for axes
					ptmp->DrawHist();
					//draw set
					theSet->Draw(pad1);
					//save histo in root file if requested
					if(out_file){
						out_file->cd();
						string oname = pm->first + "_" + theSet->GetName();
						theSet->GetHisto()->Write(oname.c_str());
					}
					ptmp->GetHisto()->Draw("sameaxis"); //draw again so axes on top
					ptmp->DrawText();
					
					//if printing not enabled, does nothing
					PrintCanvas(pm->first,can);
				}
			}
			
			//close all root files
			if(globalOpt->Get("closefiles",false)){
				out_file->Close();
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->CloseFile();
				}
			}
		}
		
		//accessors
		bool GetPrint() { return doPrint; }
		void SetPrint(bool p) { doPrint = p; }
		void PrintCanvas(string oname, TCanvas* can){
			//print formats given as a vector option
			vector<string> printformat;
			if(doPrint && globalOpt->Get<vector<string> >("printformat",printformat)){
				for(int j = 0; j < printformat.size(); j++){
					string pformat = printformat[j];
					string suff = "";
					if(globalOpt->Get("printsuffix",suff)) oname += "_" + suff;
					oname += "." + pformat;
					can->Print(oname.c_str(),pformat.c_str());
				}
			}
		}
		OptionMap* GetGlobalOpt() { return globalOpt; }
		void ListOptions() {
			OMit it;
			for(it = globalOpt->GetTable().begin(); it != globalOpt->GetTable().end(); it++){
				cout << it->first /*<< ": " << it->second->value*/ << endl;
			}
		}
		KParser* GetParser() { return MyParser; }

		
	private:
		//member variables
		string input, treedir;
		KParser* MyParser;
		PlotMap MyPlots;
		PlotMapMap MyPlots2D;
		OptionMapMap MyPlotOptions;
		vector<KBase*> MySets;
		KSetRatio* MyRatio;
		OptionMap* globalOpt;
		string s_numer, s_denom, s_yieldref; //names for special sets
		KBase *numer, *denom, *yieldref; //pointers to special sets
		bool doPrint;
		map<int,KBase*> curr_sets;
		double* varbins;
		int nbins;
		bool parsed;
};


#endif