//custom headers
#include "Helper.h"

//ROOT headers
#include <TFile.h>
#include <TH1.h>

//STL headers
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;

vector<double> GetDoubles(const vector<string>& fields){
	vector<double> result;
	result.reserve(fields.size());
	for(unsigned f = 0; f < fields.size(); ++f){
		stringstream ss(fields[f]);
		double tmp;
		ss >> tmp;
		result.push_back(tmp);
	}
	return result;
}

//root -b -l -q GetISRWeights.C+
void GetISRWeights(string fname="ISRWeights"){
	//to store results
	vector<double> xbins;
	vector<double> weights;
	vector<double> weightsUp;
	vector<double> weightsDown;
	
	//input weights
	ifstream infile((fname+".txt").c_str());
	string line;
	int ctr = 0;
	if(infile.is_open()){
		while(getline(infile,line)){
			vector<string> fields;
			//tab-separated
			helper::process(line,'\t',fields);
			//convert input data to doubles
			if(ctr==0) xbins = GetDoubles(fields);
			else if(ctr==1) weights = GetDoubles(fields);
			else if(ctr==2) weightsUp = GetDoubles(fields);
			else if(ctr==3) weightsDown = GetDoubles(fields);
			
			ctr++;
		}
	}
	else {
		cout << "Input error: could not open input file " << fname+".txt" << endl;
	}
	
	//open output file
	TFile* outfile = helper::Open(fname+".root","RECREATE");
	outfile->cd();
	
	//make histos (central + variations)
	TH1F* h_central = new TH1F("isr_weights_central","",xbins.size()-1,&xbins[0]);
	TH1F* h_up = new TH1F("isr_weights_up","",xbins.size()-1,&xbins[0]);
	TH1F* h_down = new TH1F("isr_weights_down","",xbins.size()-1,&xbins[0]);
	for(unsigned b = 1; b < xbins.size(); ++b){
		h_central->SetBinContent(b, weights[b-1]);
		h_up->SetBinContent(b, weightsUp[b-1]);
		h_down->SetBinContent(b, weightsDown[b-1]);
	}
	
	//save histos
	h_central->Write();
	h_up->Write();
	h_down->Write();
	
	//close file
	outfile->Close();
}
