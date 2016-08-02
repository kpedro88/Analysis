#include <TROOT.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLegend.h>
#include <TMarker.h>
#include <TBox.h>
#include <TMath.h>
#include <TAxis.h>
#include <TVirtualPadPainter.h>

#include "KCode/KParser.h"
#include "KCode/KMap.h"
#include "KCode/KPlot.h"
#include "KCode/KLegend.h"

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>

using namespace std;

struct inputLine {
	string name;
	double raw, abs, rel;
};

std::ostream& operator<< (std::ostream& out, const inputLine& k) {
  cout << k.name << " " << k.raw << " " << k.abs << " " << k.rel << endl;
  return out;
}

vector<inputLine> parseFile(string fname){
	vector<inputLine> results;
	string line;
	ifstream infile(fname.c_str());
	int line_counter = -1;
	if(infile.is_open()){
		while(getline(infile,line)){
			++line_counter;
			if(line_counter<3) continue; //skip header lines
			vector<string> fields;
			KParser::process(line,' ',fields);
			//copy(fields.begin(), fields.end(), ostream_iterator<string>(cout, ",")); cout << endl; //debugging
			inputLine itmp;
			//default values
			itmp.raw = -1; itmp.abs = -1; itmp.rel = -1;
			int counter = 0;
			for(unsigned i = 0; i < fields.size(); ++i){
				if(fields[i].size()==0) continue;
				if(counter==0) itmp.name = fields[i];
				else if(counter==1) itmp.raw = KParser::getOptionValue<double>(fields[i]);
				else if(counter==2) itmp.abs = KParser::getOptionValue<double>(fields[i]);
				else if(counter==3) itmp.rel = KParser::getOptionValue<double>(fields[i]);
				else break;
				++counter;
			}
			results.push_back(itmp);
		}
	}
	else {
		cout << "Input error: could not open input file \"" << fname << "\"." << endl;
	}
	//copy(results.begin(), results.end(), ostream_iterator<inputLine>(cout, "")); cout << endl; //debugging
	return results;
}

vector<KNamed*> parseInput(string fname){
	vector<KNamed*> results;
	string line;
	ifstream infile(fname.c_str());
	if(infile.is_open()){
		while(getline(infile,line)){
			//skip commented lines
			if(line[0]=='#') continue;
			//skip blank lines
			if(line.size()<2) continue;
			
			vector<string> fields;
			KNamed* tmp = KParser::processNamed(line);
			results.push_back(tmp);
		}
	}
	else {
		cout << "Input error: could not open input file \"" << fname << "\"." << endl;
	}
	return results;
}

//usage:
//root -l 'plotCutflow.C+(".","input_cutflow.txt")'
//root -l 'plotCutflow.C+(".","input_cutflow_1400.txt","rel","1400")'

void plotCutflow(string dir, string input, string column="rel", string osuff="", bool logy=false){
	//list of methods and options
	vector<KNamed*> samples = parseInput(input);
	if(samples.size()==0) return;
	
	//extract info from method files
	vector<vector<inputLine> > results;
	for(unsigned s = 0; s < samples.size(); ++s){
		string fname; samples[s]->second->Get("filename",fname);
		results.push_back(parseFile(dir+"/"+fname));
	}
	if(results.size()==0) return;
	
	vector<TH1F*> hist(samples.size(),NULL);
	
	//figure out number of bins
	int nbins = 0;
	for(unsigned r = 0; r < results[0].size(); ++r){
		double val = 0;
		if(column=="raw") val = results[0][r].raw;
		else if(column=="abs") val = results[0][r].abs;
		else if(column=="rel") val = results[0][r].rel;
		
		if(val==-1) continue;
		++nbins;
	}
	
	double ymin = 1e10;
	double ymax = 0;
	for(unsigned h = 0; h < hist.size(); ++h){
		string hname[2];
		stringstream ss;
		ss << "hist" << h; hname[0] = ss.str();
		hist[h] = new TH1F(hname[0].c_str(),"",nbins,0,nbins);
		
		int nbinsh = 0;
		for(unsigned r = 0; r < results[h].size(); ++r){
			double val = 0;
			if(column=="raw") val = results[h][r].raw;
			else if(column=="abs") val = results[h][r].abs;
			else if(column=="rel") val = results[h][r].rel;
			
			if(val==-1) continue;
			++nbinsh;
			
			string xname = results[h][r].name;
			//formatting
			if(xname=="NJet") xname = "N_{jet} #geq 4";
			else if(xname=="HT") xname = "H_{T} > 500 GeV";
			else if(xname=="MHT") xname = "H_{T}^{miss} > 200 GeV";
			else if(xname=="MuonVeto") xname = "N_{muon} = 0";
			else if(xname=="ElectronVeto") xname = "N_{electron} = 0";
			else if(xname=="IsoMuonTrackVeto") xname = "N_{isolated tracks}^{(muon)} = 0";
			else if(xname=="IsoElectronTrackVeto") xname = "N_{isolated tracks}^{(electron)} = 0";
			else if(xname=="IsoPionTrackVeto") xname = "N_{isolated tracks}^{(pion)} = 0";
			else if(xname=="DeltaPhi") xname = "#Delta#phi_{H_{T}^{miss},j_{i}} > 0.5,0.5,0.3,0.3";
			
			hist[h]->GetXaxis()->SetBinLabel(nbinsh,xname.c_str());
			hist[h]->SetBinContent(nbinsh,val);
			if(val<ymin) ymin = val;
			if(val>ymax) ymax = val;
		}
		
		Color_t color = kBlack; samples[h]->second->Get("color",color);
		hist[h]->SetLineColor(color);
		hist[h]->SetLineWidth(2);
	}
	ymin = ymin*0.9;
	
	//setup plotting options
	OptionMap* globalOpt = new OptionMap();
	globalOpt->Set<string>("lumi_text","(13 TeV)");
	globalOpt->Set<string>("prelim_text","Supplementary (Simulation)");
	//globalOpt->Set<bool>("checkerr",false);
	globalOpt->Set<int>("npanel",2);
	globalOpt->Set<bool>("balance_panels",false);
	globalOpt->Set<double>("ymin",ymin);
	globalOpt->Set<double>("marginB",195);
	//globalOpt->Set<double>("sizeLeg",21.5);
	globalOpt->Set<double>("sizeSymb",0.15);
	//globalOpt->Set<double>("canvasW",1000);
	//globalOpt->Set<double>("canvasH",600);

	OptionMap* localOpt = new OptionMap();
	localOpt->Set<bool>("ratio",false);
	localOpt->Set<bool>("logy",logy);
	
	//make plot
	TH1F* hbase = (TH1F*)hist[0]->Clone();
	hbase->Reset();
	hbase->LabelsOption("v","X");
	if(column=="raw") hbase->GetYaxis()->SetTitle("Raw # Events");
	else if(column=="abs") hbase->GetYaxis()->SetTitle("Abs. Eff. [%]");
	else if(column=="rel") hbase->GetYaxis()->SetTitle("Rel. Eff. [%]");
	string oname = "plotCutflow_"+column;
	if(osuff.size()>0) oname += "_"+osuff;
	if(logy) oname += "_logy";
	KParser::clean(oname);
	KPlot* plot = new KPlot(oname,localOpt,globalOpt);
	plot->Initialize(hbase);
	KLegend* kleg = plot->GetLegend();
	TCanvas* can = plot->GetCanvas();
	TPad* pad1 = plot->GetPad1();
	pad1->cd();
	pad1->SetGridx();
	pad1->SetGridy();
	
	//build legend
	kleg->AddHist(plot->GetHisto()); //for tick sizes
	kleg->AddEntry((TObject*)NULL,"#color[4]{arXiv:1602.06581}","",1);
	for(unsigned h = 0; h < hist.size(); ++h){
		string legname; samples[h]->second->Get("legname",legname);
		string legopt; samples[h]->second->Get("legopt",legopt);
		kleg->AddEntry(hist[h],legname,legopt,1);
	}
	//extra legend stuff for public plot
	kleg->AddEntry((TObject*)NULL,"pp #rightarrow #tilde{g}#tilde{g}, #tilde{g} #rightarrow t b ^{}#tilde{#chi}^{+}_{1}","",0);
	kleg->AddEntry((TObject*)NULL,"^{}m_{#tilde{g}} = 1400 GeV","",0);
	kleg->AddEntry((TObject*)NULL,"m_{#tilde{#chi}^{+}_{1}} - m_{#tilde{#chi}^{0}_{1}} = 5 GeV","",0);
	kleg->Build();
	
	//draw blank histo for axes
	plot->DrawHist();
	
	//draw histos
	for(unsigned h = 0; h < hist.size(); ++h){
		string drawopt; samples[h]->second->Get("drawopt",drawopt);
		hist[h]->Draw((drawopt+" same").c_str());
	}

	plot->GetHisto()->Draw("sameaxis"); //draw again so axes on top
	plot->DrawText();
	
	//print image
	string printformats[] = {"png","eps"};
	for(unsigned p = 0; p < 2; ++p){
		can->Print((oname+"."+printformats[p]).c_str(),printformats[p].c_str());
		if(printformats[p]=="eps") system(("epstopdf "+oname+".eps").c_str());
	}
}