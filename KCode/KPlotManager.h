#ifndef KPLOTMANAGER_H
#define KPLOTMANAGER_H

//custom headers
#include "KManager.h"
#include "KSet.h"
#include "KPlot.h"
#include "KLegend.h"
#include "KBuilder.h"
#include "KBuilderSelectors.h"
#include "KBuilderVariators.h"

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

//------------------------------------------
//class to manage all objects and draw plots
class KPlotManager : public KManager {
	public:
		//constructor
		KPlotManager() : KManager(), yieldref(0), doPrint(false), out_file(0) {}
		KPlotManager(string indir_, vector<string> input_, vector<string> direct_) : 
			KManager(indir_), yieldref(0), doPrint(false), out_file(0)
		{
			//parse most initializations based on text input
			Initialize(input_,direct_);
			
			//final checks and initializations
			finalizeSets();
		}
		//destructor
		virtual ~KPlotManager() {}
		//helpers
		void processSet(string line){
			//cout << line << endl;
			int indent = 0;
			while(line[0]=='\t'){
				line.erase(0,1);
				indent++;
			}

			//create set from input
			KNamedBase* tmp = KParser::processNamed<3>(line);
			
			//debug: print the contents of curr_sets
			/*for(auto& dit : curr_sets){
				cout << dit.second->fields[2] << ", ";
			}
			cout << endl;*/

			if(tmp){
				//check corner cases for indent
				if(indent==0 && tmp->fields[0]=="base"){
					cout << "Input error: base must have a parent set for plotting! This input will be ignored." << endl;
					return;
				}
				else if(indent>0){
					map<int,KNamedBase*>::iterator it = curr_sets.find(indent-1);
					if(it == curr_sets.end()){
						cout << "Input error: no parent can be identified for the set " << tmp->fields[2] << "! There may be too many indents, or the parent was not input properly. This input will be ignored." << endl;
						return;
					}
				}
				
				//reset entries in the map of current sets that have higher (or equal) indents vs. current line
				map<int,KNamedBase*>::iterator it = curr_sets.lower_bound(indent);
				curr_sets.erase(it,curr_sets.end());
			
				//store and add to parent set
				curr_sets[indent] = tmp;
				if(indent==0) MySetOptions.push_back(curr_sets[indent]); //add top-level set to manager
				else {
					curr_sets[indent-1]->children.push_back(tmp);
					//cout << curr_sets[indent-1]->fields[2] << "->children.push_back(" << tmp->GetName() << ")" << endl;
				}
			}
		}
		void finalizeSets(){
			vector<string> gselection;
			globalOpt->Get("selections",gselection);
			vector<string> lselection;
			bool globalAppend = globalOpt->Get("appendsel",true);
			
			//loop over top level set options
			//to generate sets for each selection
			for(auto& ntmp : MySetOptions){
				bool hasLocalSel = ntmp->localOpt()->Get("selections",lselection);
				//can avoid appending if only one selection specified for this set
				bool localAppend = globalAppend || (hasLocalSel ? lselection.size()>1 : gselection.size()>1);
				for(auto& stmp : (hasLocalSel ? lselection : gselection)){
					KBase* tmp = finalizeSet(ntmp,NULL,stmp,localAppend);
					//set style at the end, in case parent modifies child's style options
					tmp->SetStyle(allStyles);
				}
			}
		}
		KBase* finalizeSet(KNamedBase* ntmp, KBase* parent, string& selection, bool append){
			//append selection to name
			string oldname = ntmp->fields[2];
			if(append) ntmp->fields[2] += "_"+selection;
			
			KBase* tmp = KParser::processBase(ntmp,globalOpt);
			
			//set selection for base
			if(ntmp->fields[0]=="base"){
				KSelection* sntmp = makeSelection(selection);
				if(sntmp) tmp->SetSelection(sntmp);				
			}
			
			//add top-level set to manager
			if(!parent) MySets.push_back(tmp);
			else {
				parent->AddChild(tmp);
			}
			
			//recurse (if necessary) to set up children
			for(auto& ctmp : ntmp->children){
				finalizeSet(ctmp,tmp,selection,append);
			}
			
			//reset tmp name
			ntmp->fields[2] = oldname;
			
			return tmp;
		}
		void processHisto(string line, int dim){
			KNamed* tmp = KParser::processNamed<1>(line);
			
			//keep track of histo dimension (use input format b/c localOpt created on the fly)
			stringstream os;
			os << "i:dimension[" << dim << "]";
			tmp->optfields.push_back(os.str());
			
			//check for cutflows
			if(tmp->fields[0].find("cutflow")!=string::npos) globalOpt->Set<bool>("doCutflow",true);
			
			//store local plot options for later use
			MyPlotOptions.push_back(tmp);
		}		
		//where the magic happens
		void DrawPlots(){
			//do everything
			if(!parsed) return;
			
			//setup root output file if requested
			string rootfilename = "";
			if(globalOpt->Get<string>("rootfile",rootfilename) and rootfilename.size()>0) out_file = TFile::Open((rootfilename+".root").c_str(),"RECREATE");
			
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
			vector<string> s_numers, s_denoms;
			string s_yieldref = ""; //names for special sets
			globalOpt->Get("numers",s_numers);
			globalOpt->Get("denoms",s_denoms);
			globalOpt->Get("yieldref",s_yieldref);
			string s_numer, s_denom; //backward compatibility
			s_numer = s_denom = s_yieldref = "";
			globalOpt->Get("numer",s_numer);
			globalOpt->Get("denom",s_denom);
			vector<string> s_roc_sig, s_roc_bkg; //vectors of names of roc sets
			globalOpt->Get("roc_sig",s_roc_sig);
			globalOpt->Get("roc_bkg",s_roc_bkg);
			
			//check for special status sets
			for(unsigned s = 0; s < MySets.size(); s++){
				//backward compatibility
				if(s_numers.size()==0 && s_numer.size()>0 && numers.size()==0 && MySets[s]->GetName()==s_numer) numers.push_back(MySets[s]);
				if(s_denoms.size()==0 && s_denom.size()>0 && denoms.size()==0 && MySets[s]->GetName()==s_denom) denoms.push_back(MySets[s]);
				
				//only find each ratio set once
				for(unsigned r = 0; r < s_numers.size(); r++){
					if(MySets[s]->GetName()==s_numers[r]){
						numers.push_back(MySets[s]);
						s_numers.erase(s_numers.begin()+r);
						break;
					}
				}
				for(unsigned r = 0; r < s_denoms.size(); r++){
					if(MySets[s]->GetName()==s_denoms[r]){
						denoms.push_back(MySets[s]);
						s_denoms.erase(s_denoms.begin()+r);
						break;
					}
				}
				
				if(s_yieldref.size()>0 && yieldref==0 && MySets[s]->GetName()==s_yieldref) yieldref = MySets[s];
				
				//only find each roc set once
				for(unsigned r = 0; r < s_roc_sig.size(); r++){
					if(MySets[s]->GetName()==s_roc_sig[r]){
						roc_sig.push_back(MySets[s]);
						s_roc_sig.erase(s_roc_sig.begin()+r);
						break;
					}
				}
				for(unsigned r = 0; r < s_roc_bkg.size(); r++){
					if(MySets[s]->GetName()==s_roc_bkg[r]){
						roc_bkg.push_back(MySets[s]);
						s_roc_bkg.erase(s_roc_bkg.begin()+r);
						break;
					}
				}
				
				//compute cutflows if necessary
				if(globalOpt->Get("doCutflow",false)) MySets[s]->MakeCutflows();
			}
			
			//numer:denom ratio cases: 1:1, 1:x, x:1
			bool ratio_allowed = ( (numers.size()==1 && denoms.size()==1) || (numers.size()==1 && denoms.size()>1) || (numers.size()>1 && denoms.size()==1) );
			if(ratio_allowed){
				//add children to ratio
				if(numers.size()==1 && denoms.size()==1){
					KSetRatio* rtmp = new KSetRatio("ratio",NULL,globalOpt);
					rtmp->AddNumerator(numers[0]);
					rtmp->AddDenominator(denoms[0]);
					//default style
					rtmp->SetStyle(allStyles);
					MyRatios.push_back(rtmp);
				}
				else if(numers.size()==1 && denoms.size()>1){
					for(unsigned r = 0; r < denoms.size(); r++){
						KSetRatio* rtmp = new KSetRatio("ratio__"+numers[0]->GetName()+"__"+denoms[r]->GetName(),NULL,globalOpt);
						rtmp->AddNumerator(numers[0]);
						rtmp->AddDenominator(denoms[r]);
						//denom style, but pe drawopt
						rtmp->GetLocalOpt()->Set<string>("drawopt","pe");
						rtmp->SetStyle(denoms[r]->GetStyle());
						MyRatios.push_back(rtmp);
					}
				}
				else if(numers.size()>1 && denoms.size()==1){
					for(unsigned r = 0; r < numers.size(); r++){
						KSetRatio* rtmp = new KSetRatio("ratio__"+numers[r]->GetName()+"__"+denoms[0]->GetName(),NULL,globalOpt);
						rtmp->AddNumerator(numers[r]);
						rtmp->AddDenominator(denoms[0]);
						//numer style, but pe drawopt
						rtmp->GetLocalOpt()->Set<string>("drawopt","pe");
						rtmp->SetStyle(numers[r]->GetStyle());
						MyRatios.push_back(rtmp);
					}
				}
			}
			
			//create plots from local options
			//in 2D case, one plot for each top-level set
			for(unsigned p = 0; p < MyPlotOptions.size(); p++){
				KNamed* ntmp = MyPlotOptions[p];
				OptionMap* omap = ntmp->localOpt();
				if(omap->Get("ratio",true) && !ratio_allowed){ //ratios turned on by default
					omap->Set("ratio",false); //disable ratios if components not available
					if(!globalOpt->Get("roc",false)){
						cout << "Input error: ratio requested for histo " << ntmp->fields[0] << ", but ";
						if(numers.size()>0 && denoms.size()>0) cout << "numers and denoms both > 1. Pick one!";
						else {
							if(numers.size()==0 && denoms.size()==0) cout << "numer(s) and denom(s)";
							else if(numers.size()==0) cout << "numer(s)";
							else if(denoms.size()==0) cout << "denom(s)";
							cout << " not set.";
						}
						cout << " Ratio will not be drawn." << endl;
					}
				}
				int dim = 0;
				omap->Get("dimension",dim);
				if(dim==1){
					KPlot* ptmp = new KPlot(ntmp->fields[0],omap,globalOpt);
					TH1* hptmp = NULL;
					if(MySets[0]->CheckSpecialHistos(ntmp->fields[0])) {
						hptmp = (TH1*)(MySets[0]->GetHisto())->Clone();
						hptmp->Reset();
					}
					if(ptmp->Initialize(hptmp)) MyPlots.Add(ntmp->fields[0],ptmp);
					else {
						cout << "Input error: unable to build histo " << ntmp->fields[0] << ". Check binning options." << endl;
						delete ptmp;
					}
				}
				else if(dim==2){
					PlotMap* p2map = new PlotMap();
					bool ntmp_ratio = omap->Get("ratio",true);
					for(unsigned s = 0; s < MySets.size()+MyRatios.size(); s++){
						KBase* theSet;
						string rationame2D = "";
						if(s>=MySets.size()){ //add ratio set at the end of loop if enabled
							if(ntmp_ratio) {
								rationame2D = MyRatios[s-MySets.size()]->GetRatioName2D();
								theSet = MyRatios[s-MySets.size()];
							}
							else continue;
						}
						else theSet = MySets[s];
						
						KPlot* ptmp = new KPlot2D(ntmp->fields[0],theSet->GetName(),omap,globalOpt);
						if(ptmp->Initialize()) {
							if(!rationame2D.empty()) ptmp->GetLocalOpt()->Set<string>(theSet->GetName()+"_name2D",rationame2D);
							p2map->Add(theSet->GetName(),ptmp);
						}
						else {
							cout << "Input error: unable to build 2D histo " << ntmp->fields[0] << " for set " << theSet->GetName() << ". Check binning options." << endl;
							delete ptmp;
						}
					}
					
					if(p2map->GetTable().begin() != p2map->GetTable().end()) MyPlots2D.Add(ntmp->fields[0],p2map);
					else {
						cout << "Input error: unable to build any 2D histos " << ntmp->fields[0] << ". Check binning options." << endl;
						delete p2map;
					}
				}
			}
		
			//load histos into sets
			for(auto& p : MyPlots.GetTable()){
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->AddHisto(p.first,p.second->GetHisto());
				}
			}
			for(auto& pm : MyPlots2D.GetTable()){
				PlotMap* p2map = pm.second;
				for(unsigned s = 0; s < MySets.size(); s++){
					KPlot* ptmp = p2map->Get(MySets[s]->GetName());
					if(ptmp) MySets[s]->AddHisto(pm.first,ptmp->GetHisto());
				}
			}
			
			//build everything
			for(unsigned s = 0; s < MySets.size(); s++){
				MySets[s]->Build();
			}
			
			//skip all the histo drawing in roc curve mode (by default)
			if(globalOpt->Get("roc",false) && globalOpt->Get("skiphistos",true)) {
				//close canvases
				for(auto& p : MyPlots.GetTable()){
					TCanvas* can = p.second->GetCanvas();
					can->Close();
				}
				DrawROC();
				return;
			}
			
			//draw each plot - normalization, legend, ratio
			for(auto& p : MyPlots.GetTable()){
				//get drawing objects from KPlot
				TCanvas* can = p.second->GetCanvas();
				TPad* pad1 = p.second->GetPad1();
				
				//get legend
				KLegend* kleg = p.second->GetLegend();
				
				//select current histogram in sets
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->GetHisto(p.first);
				}
				
				//check if normalization to yield is desired (disabled by default)
				//BEFORE printing yields
				//needs to be separate loop because yieldref must have current histo selected to get appropriate yield
				//then print yield if enabled
				//BEFORE division by bin width if requested
				//then pass current histogram to legend
				//AFTER bindivide if requested
				//and add to legend
				double yield = 0;
				if(yieldref) yield = yieldref->GetYield();
				if(globalOpt->Get("printyield",false)) cout << p.first << " yield:" << endl;
				for(unsigned s = 0; s < MySets.size(); s++){
					if(yieldref && p.second->GetLocalOpt()->Get("yieldnorm",false) && yield>0 && MySets[s] != yieldref) MySets[s]->Normalize(yield);
					if(globalOpt->Get("printyield",false)) MySets[s]->PrintYield();
					if(globalOpt->Get("unitnorm",false)) MySets[s]->Normalize(1,true);
					if(p.second->GetLocalOpt()->Get("bindivide",false)) MySets[s]->BinDivide();
					MySets[s]->AddToLegend(kleg);
				}
				if(globalOpt->Get("printyield",false)) cout << endl;
				if(p.second->GetLocalOpt()->Get("yieldnorm",false) && !yieldref){
					cout << "Input error: normalization to yield requested for " << p.first << ", but yieldref not set. Normalization will not be performed." << endl;
				}
				
				//build legend
				kleg->Build();

				//draw blank histo for axes
				p.second->DrawHist();
				//draw sets (reverse order, so first set is on top)
				for(int s = MySets.size()-1; s >= 0; s--){
					MySets[s]->Draw(pad1);
					
					//save histos in root file if requested
					if(out_file){
						out_file->cd();
						string oname = p.first + "_" + MySets[s]->GetName();
						MySets[s]->GetHisto()->SetName(oname.c_str());
						MySets[s]->GetHisto()->Write(oname.c_str());
					}
				}
				p.second->GetHisto()->Draw("sameaxis"); //draw again so axes on top
				p.second->DrawText();
				
				//ratio (enabled by default, auto-disabled above if components not set)
				if(p.second->GetLocalOpt()->Get("ratio",true)){
					TPad* pad2 = p.second->GetPad2();

					p.second->DrawRatio();
					
					for(unsigned r = 0; r < MyRatios.size(); ++r){
						MyRatios[r]->Build(p.second->GetHisto());
						MyRatios[r]->Draw(pad2);
					}
					p.second->DrawLine();
				}
				
				//if printing not enabled, does nothing
				PrintCanvas(p.first,can);
			}

			//draw each 2D plot - normalization, etc.
			for(auto& pm : MyPlots2D.GetTable()){
				PlotMap* p2map = pm.second;
				
				//select current histogram in sets
				//needs to happen first in case of normalization to yield
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->GetHisto(pm.first);
				}
				double yield = 0;
				if(p2map->GetTable().begin()->second->GetLocalOpt()->Get("yieldnorm",false) && yieldref){
						yield = yieldref->GetYield();
				}
				else if(p2map->GetTable().begin()->second->GetLocalOpt()->Get("yieldnorm",false) && !yieldref){
					cout << "Input error: normalization to yield requested for " << pm.first << ", but yieldref not set. Normalization will not be performed." << endl;
				}
				
				double zmin = 1e100;
				double zmax = 0;
				//check if normalization to yield is desired (disabled by default)
				//BEFORE printing yields
				//then print yield if enabled
				//BEFORE division by bin width if requested
				if(globalOpt->Get("printyield",false)) cout << pm.first << " yield:" << endl;
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
				for(unsigned s = 0; s < MySets.size()+MyRatios.size(); s++){
					KBase* theSet;
					KPlot* ptmp;
					if(s>=MySets.size()){ //add ratio set at the end of loop if enabled
						ptmp = p2map->Get(MyRatios[s-MySets.size()]->GetName());
						if(ptmp) theSet = MyRatios[s-MySets.size()];
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
					if(s>=MySets.size()) {
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
						string oname = pm.first + "_" + theSet->GetName();
						theSet->GetHisto()->SetName(oname.c_str());
						theSet->GetHisto()->Write(oname.c_str());
					}
					ptmp->GetHisto()->Draw("sameaxis"); //draw again so axes on top
					ptmp->DrawText();
					
					//if printing not enabled, does nothing
					PrintCanvas(pm.first+"_"+theSet->GetName(),can);
				}
			}
			
			//if the histo drawing wasn't skipped in roc curve mode
			if(globalOpt->Get("roc",false) && !globalOpt->Get("skiphistos",true)) {
				DrawROC();
				return;
			}
			
			//close all root files
			CloseFiles();
		}
		//where the ROC happens
		void DrawROC(){
			if(roc_sig.size()==0 || roc_bkg.size()==0){
				cout << "Input error: ROC curves requested, but ";
				if(roc_sig.size()==0 && roc_bkg.size()==0) cout << "roc_sig and roc_bkg";
				else if(roc_sig.size()==0) cout << "roc_sig";
				else if(roc_sig.size()==0) cout << "roc_bkg";
				cout << " not set. ROC curves will not be drawn." << endl;
				return;
			}
			
			//draw curves for each sig vs each bkg
			for(unsigned s = 0; s < roc_sig.size(); s++){
				for(unsigned b = 0; b < roc_bkg.size(); b++){
					//specific qtys not included in roc_name right now
					//anything desired should be specified in printsuffix option
					string roc_name = "roc_" + roc_sig[s]->GetName() + "_vs_" + roc_bkg[b]->GetName();
					if(globalOpt->Get("debugroc",false)) cout << roc_name << endl;
					
					//make base histo: 0..1 on both axes
					TH1F* h_base = new TH1F(roc_name.c_str(),"",10,0.,1.);
					h_base->GetYaxis()->SetRangeUser(0.,1.);
					h_base->GetXaxis()->SetTitle(("#varepsilon_{sig} (" + roc_sig[s]->GetName() + ")").c_str());
					h_base->GetYaxis()->SetTitle(("#varepsilon_{bkg} (" + roc_bkg[b]->GetName() + ")").c_str());
					
					//make plot
					KPlot* p_roc = new KPlot(roc_name,NULL,globalOpt);
					p_roc->GetLocalOpt()->Set("ratio",false);
					p_roc->GetLocalOpt()->Set("logx",false);
					p_roc->GetLocalOpt()->Set("logy",false);
					p_roc->Initialize(h_base);
					
					//get drawing objects from KPlot
					TCanvas* can = p_roc->GetCanvas();
					TPad* pad1 = p_roc->GetPad1();
					
					//get legend
					KLegend* kleg = p_roc->GetLegend();
					kleg->AddHist(p_roc->GetHisto());
					
					//loop over histos and make graphs
					vector<TGraph*> graphs;
					vector<KStyle*> graphstyles;
					for(auto& p : MyPlots.GetTable()){
						//select current histogram in sets
						TH1F* h_sig = (TH1F*)(roc_sig[s]->GetHisto(p.first));
						TH1F* h_bkg = (TH1F*)(roc_bkg[b]->GetHisto(p.first)); //unused right now
						
						//get efficiencies
						//(cached results will be returned if the calculation was already done)
						double* eff_sig = roc_sig[s]->GetEff();
						double* eff_bkg = roc_bkg[b]->GetEff();

						//create graph
						TGraph* gtmp = new TGraph(h_sig->GetNbinsX()+2,eff_sig,eff_bkg);
						gtmp->SetName(p.first.c_str());
						gtmp->SetTitle("");
						
						//format graph using KPlot local options for this histo/qty
						string styleName = "roc";
						KStyle* stytmp = NULL;
						if(allStyles.Has(styleName)){
							KNamed* ntmp = KParser::processNamed<1>(styleName+"\t"+allStyles.Get(styleName));
							stytmp = new KStyle(ntmp->fields[0],ntmp->localOpt(),p.second->GetLocalOpt());
						}
						stytmp->Format(gtmp);
						
						//add to legend using histo x-name & panel
						int panel_tmp = 0;
						p.second->GetLocalOpt()->Get("panel",panel_tmp);
						kleg->AddEntry(gtmp,h_sig->GetXaxis()->GetTitle(),stytmp->GetLegOpt(),panel_tmp);
						
						//store graph
						graphs.push_back(gtmp);
						graphstyles.push_back(stytmp);
					}
						
					//build legend: specified quadrant, no smart resizing
					kleg->Build(KLegend::left, KLegend::top);

					//draw blank histo for axes
					p_roc->DrawHist();
					//draw sets (reverse order, so first set is on top)
					pad1->cd();
					for(unsigned g = 0; g < graphs.size(); g++){
						graphs[g]->Draw(graphstyles[g]->GetDrawOpt("same").c_str()); //should this be "c" instead for smooth curve?
						
						//save graphs in root file if requested
						if(out_file){
							out_file->cd();
							string oname = string(graphs[g]->GetName()) + "_" + roc_name;
							graphs[g]->SetName(oname.c_str());
							graphs[g]->Write(oname.c_str());
						}
					}
					p_roc->GetHisto()->Draw("sameaxis"); //draw again so axes on top
					p_roc->DrawText();
					
					//if printing not enabled, does nothing
					PrintCanvas(roc_name,can);			
				}
			}
			
			//close all root files
			CloseFiles();
		}
		
		//accessors
		bool GetPrint() { return doPrint; }
		void SetPrint(bool p) { doPrint = p; }
		void PrintCanvas(string oname, TCanvas* can){
			//print formats given as a vector option
			vector<string> printformat;
			if(doPrint && globalOpt->Get<vector<string> >("printformat",printformat)){
				for(unsigned j = 0; j < printformat.size(); j++){
					string otmp = oname;
					string pformat = printformat[j];
					string suff = "";
					if(globalOpt->Get("printsuffix",suff)) otmp += "_" + suff;
					if(globalOpt->Get("treesuffix",false)){
						//use only the tree directory name as additional suffix
						string treedir = "";
						globalOpt->Get("treedir",treedir);
						vector<string> dirs;
						KParser::process(treedir,'/',dirs);
						otmp += "_" + dirs.back();
					}
					otmp += "." + pformat;
					KParser::clean(otmp);
					can->Print(otmp.c_str(),pformat.c_str());
				}
			}
		}
		void CloseFiles(){
			if(globalOpt->Get("closefiles",false)){
				out_file->Close();
				for(unsigned s = 0; s < MySets.size(); s++){
					MySets[s]->CloseFile();
				}
			}
		}
		
	private:
		//member variables
		PlotMap MyPlots;
		PlotMapMap MyPlots2D;
		vector<KNamed*> MyPlotOptions;
		vector<KNamedBase*> MySetOptions;
		vector<KBase*> MySets;
		vector<KSetRatio*> MyRatios;
		vector<KBase*> numers, denoms;
		KBase *yieldref; //pointers to special sets
		vector<KBase*> roc_sig, roc_bkg;
		bool doPrint;
		map<int,KNamedBase*> curr_sets;
		TFile* out_file;
};

#endif
