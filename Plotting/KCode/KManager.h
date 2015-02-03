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
				if(s_numer.size()>0 && numer==0) numer = MySets[s]->CheckSpecial(s_numer);
				if(s_denom.size()>0 && denom==0) denom = MySets[s]->CheckSpecial(s_denom);
				if(s_yieldref.size()>0 && yieldref==0) yieldref = MySets[s]->CheckSpecial(s_yieldref);
			}
			bool ratio_allowed = numer && denom;
			if(ratio_allowed){
				//add children to ratio
				MyRatio->AddNumerator(numer);
				MyRatio->AddDenominator(denom);
			}
			
			//create plots from local options
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
				KPlot* ptmp = new KPlot(om->first,om->second,globalOpt);
				if(ptmp->Initialize()) MyPlots.Add(om->first,ptmp);
				else {
					cout << "Input error: unable to build histo " << om->first << ". Check binning options." << endl;
					delete ptmp;
				}
			}
		
			//load histos into sets
			PMit p;
			for(p = MyPlots.GetTable().begin(); p != MyPlots.GetTable().end(); p++){
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->AddHisto(p->first,p->second->GetHisto());
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
				if(p->second->GetLocalOpt()->Get("yieldnorm",false) && yieldref){
					double yield = yieldref->GetYield();
					if(yield>0){
						for(unsigned s = 0; s < MySets.size(); s++){
							if(MySets[s] != yieldref) MySets[s]->Normalize(yield);
						}
					}
				}
				else if(p->second->GetLocalOpt()->Get("yieldnorm",false) && !yieldref){
					cout << "Input error: normalization to yield requested, but yieldref not set. Normalization will not be performed." << endl;
				}
				
				//print yield if enabled
				//BEFORE bindivide if requested
				if(globalOpt->Get("printyield",false)) cout << p->first << " yield:" << endl;
				for(unsigned s = 0; s < MySets.size(); s++){
					if(globalOpt->Get("printyield",false)) {
						int prcsn;
						if(globalOpt->Get("yieldprecision",prcsn)) cout << fixed << setprecision(prcsn);
						MySets[s]->PrintYield();
					}
				}
				if(globalOpt->Get("printyield",false)) cout << endl;
				
				//check if division by bin width is desired (disabled by default)
				//AFTER printing yields
				if(p->second->GetLocalOpt()->Get("bindivide",false)){
					for(unsigned s = 0; s < MySets.size(); s++){
						MySets[s]->BinDivide();
					}
				}
			
				//pass current histogram to legend
				//AFTER bindivide if requested
				//and calculate legend size
				for(unsigned s = 0; s < MySets.size(); s++){
					kleg->AddHist(MySets[s]->GetHisto());
					MySets[s]->GetLegendInfo(kleg);
				}
				
				//build legend
				kleg->Build();
				
				//add sets to legend
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
					
					//reset ratio name if provided by user
					//default, data/mc, is set in KPlot
					string rationame;
					if(globalOpt->Get<string>("rationame",rationame)) p->second->GetRatio()->GetYaxis()->SetTitle(rationame.c_str());

					p->second->DrawRatio();
				
					MyRatio->Build(p->second->GetHisto());
					
					MyRatio->Draw(pad2);
					p->second->DrawLine();
				}
				
				//print formats given as a vector option
				vector<string> printformat;
				if(doPrint && globalOpt->Get<vector<string> >("printformat",printformat)){
					for(int j = 0; j < printformat.size(); j++){
						string pformat = printformat[j];
						string oname = p->first;
						string suff = "";
						if(globalOpt->Get("printsuffix",suff)) oname += "_" + suff;
						oname += "." + pformat;
						can->Print(oname.c_str(),pformat.c_str());
					}
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