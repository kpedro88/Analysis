//custom headers
#include "KPlotDriver.C"

//ROOT headers
#include <TError.h>
#include <TCollection.h>
#include <TKey.h>

//STL headers
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

//helper
string changeHistoName(string name, string suff){
	vector<string> snames;
	KParser::process(name,'_',snames);
	snames.back() = suff;
	stringstream ssname;
	copy(snames.begin(),snames.end(),ostream_iterator<string>(ssname,"_"));
	string sname = ssname.str(); sname.pop_back(); //remove trailing _
	return sname;
}

//recompile:
//root -b -l -q MakeAllDCsyst.C++
void MakeAllDCsyst(int mode=-1, string setname="", string indir="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV11", string systTypes="nominal,scaleuncUp,scaleuncDown,isruncUp,isruncDown,triguncUp,triguncDown,btagSFuncUp,btagSFuncDown,mistagSFuncUp,mistagSFuncDown,isotrackuncUp,isotrackuncDown,lumiuncUp,lumiuncDown,prefireuncUp,prefireuncDown", string varTypes="JECup,JECdown,JERup,JERdown"){
	gErrorIgnoreLevel = kBreak;
	
	if(mode==-1){
		cout << "Recompiled MakeAllDCsyst, exiting." << endl;
		return;
	}
	
	if(indir[indir.size()-1] != '/') indir = indir + "/";
	string inpre = "tree_";
	string region = "signal";
	string outpre = "RA2bin_";
	string outdir = "";
	string input = "input/input_RA2bin_DC_syst.txt";
	//string inputQCD = "input/input_RA2bin_DC_QCD.txt";
	string setlist = "";
	string osuff = "";
	
	//process variaton types - comma-separated input, need to be run separately
	vector<string> vars;
	KParser::process(varTypes,',',vars);
	
	if(mode==1){
		outdir = "datacards_fast/";
		setlist = "input/fast/input_set_DC_"+setname+".txt";
		osuff = "_"+setname;
	}
	else {
		outdir = "datacards_syst/";
		setlist = "input/input_set_DC_"+setname+".txt";
		osuff = "_"+setname;
	}
	
	//check for directory
	if(outdir.size()>0) system(("mkdir -p "+outdir).c_str());
	
	//keep a list of root files
	vector<string> rootfiles;
	
	//do the simple systematics all at once
	rootfiles.push_back(outdir+outpre+region+osuff);
	KPlotDriver(indir+inpre+region,{input,setlist},{"OPTION","string:rootfile["+rootfiles.back()+"]","vstring:selections["+systTypes+"]"});
	
	//do the full variations separately
	//produce a selection for each variation on the fly, cloned from nominal
	for(auto& ivar : vars){
		//change region
		string region_ = region + "_"+ivar;
		rootfiles.push_back(outdir+outpre+region_+osuff);
		KPlotDriver(indir+inpre+region_,{input,setlist},{"OPTION","string:rootfile["+rootfiles.back()+"]","vstring:selections["+ivar+"]","SELECTION",ivar,"\tnominal"});
	}
	
	//hadd
	stringstream slist;
	copy(rootfiles.begin(),rootfiles.end(),ostream_iterator<string>(slist,".root "));
	string therootfile = outpre+region+osuff+".root";
	string cmd = "hadd -f "+therootfile+" "+slist.str();
	system(cmd.c_str());
	
	//further processing
	TFile* infile = TFile::Open(therootfile.c_str());
	TH1F* nominal = NULL;
	TH1F* genMHT = NULL;
	vector<TH1F*> hsyst;
	TKey *key;
	TIter next(infile->GetListOfKeys());
	while ((key = (TKey*)next())) {
		string ntmp = key->GetName();
		TH1F* htmp = (TH1F*)infile->Get(ntmp.c_str());
		if(ntmp.find("nominal")!=string::npos) nominal = htmp;
		else if(ntmp.find("genMHT")!=string::npos) genMHT = htmp;
		else hsyst.push_back(htmp);
	}
	
	if(!nominal){
		cout << "Nominal histogram not found, will not make relative systematics." << endl;
		return;
	}
	
	//todo: factorize the operations below
	
	//setup systematics analysis tree (to study changes in yield)
	vector<string> setnames;
	KParser::process(setname,'_',setnames);
	TTree* tree = new TTree("tree","systematics");
	int mMother = KParser::getOptionValue<int>(setnames[1]);
	int mLSP = KParser::getOptionValue<int>(setnames[2]);
	tree->Branch("mMother",&mMother,"mMother/I");
	tree->Branch("mLSP",&mLSP,"mLSP/I");
	//map to keep track of maximum pct diffs
	KMap<double> pctDiffMap;
	double nominal_yield = nominal->Integral(0,nominal->GetNbinsX()+1);
	
	//divide, bound, set labels (prepended w/ "signal_")
	for(auto isyst : hsyst){
		vector<string> inames;
		KParser::process(isyst->GetName(),'_',inames);
		string binname;
		bool up = (inames.back().find("up")!=string::npos or inames.back().find("Up")!=string::npos);
		if(up) binname = inames.back().substr(0,inames.back().size()-2);
		else binname = inames.back().substr(0,inames.back().size()-4); //down
		string treename = binname;
		binname = "signal_"+binname;
		//check yield (keep max diff in case of up and down)
		double iyield = isyst->Integral(0,isyst->GetNbinsX()+1);
		double pctdiff = fabs(1.-iyield/nominal_yield)*100;
		if(!pctDiffMap.Has(treename) or pctDiffMap.Get(treename) < pctdiff){
			pctDiffMap.Add(treename,pctdiff);
		}
		for(unsigned b = 1; b <= isyst->GetNbinsX(); ++b){
			//divide
			double unc = 1.0;
			double denom = nominal->GetBinContent(b);
			//if central value is zero, treat syst as a shift
			if(denom==0.) denom = 1.0;
			unc = isyst->GetBinContent(b)/denom;
			//bound
			unc = min(max(unc,0.01),3.0);
			//set
			isyst->SetBinContent(b,unc);
			isyst->GetXaxis()->SetBinLabel(b,binname.c_str());
		}
	}
	
	//make stat error
	string sname = changeHistoName(nominal->GetName(),"MCStatErr");
	TH1F* ssyst = (TH1F*)nominal->Clone(sname.c_str());
	double stat_yield = 0;
	for(unsigned b = 1; b <= ssyst->GetNbinsX(); ++b){
		//label
		string slabel = "signal_MCStatErr_";
		slabel += ssyst->GetXaxis()->GetBinLabel(b);
		ssyst->GetXaxis()->SetBinLabel(b,slabel.c_str());
		stat_yield += ssyst->GetBinError(b)+ssyst->GetBinContent(b);
		//divide
		if(ssyst->GetBinContent(b)>0.) ssyst->SetBinContent(b, 1.0+ssyst->GetBinError(b)/ssyst->GetBinContent(b));
		else ssyst->SetBinContent(b, 1.0+ssyst->GetBinError(b));
	}
	hsyst.push_back(ssyst);
	pctDiffMap.Add("MCStatErr",fabs(1-stat_yield/nominal_yield)*100);
	
	//genMHT correction and unc for fastsim
	if(genMHT){
		//keep original nominal histogram
		string nname = changeHistoName(nominal->GetName(),"nominalOrig");
		TH1F* nominalOrig = (TH1F*)nominal->Clone(nname.c_str());
		string gname = changeHistoName(nominal->GetName(),"MHTSyst");
		TH1F* gsyst = (TH1F*)nominal->Clone(gname.c_str());
		double g_yield = 0;
		
		//modify nominal as average of nominal and genMHT & compute syst as difference
		for(unsigned b = 1; b <= nominal->GetNbinsX(); ++b){
			gsyst->SetBinContent(b, 1.0+abs(nominal->GetBinContent(b) - genMHT->GetBinContent(b))/2.0);
			gsyst->GetXaxis()->SetBinLabel(b,"signal_MHTSyst");
			nominal->SetBinContent(b, (nominal->GetBinContent(b) + genMHT->GetBinContent(b))/2.0);
			g_yield += (gsyst->GetBinContent(b)-1.0)+nominal->GetBinContent(b);
		}
		
		hsyst.push_back(gsyst);
		hsyst.push_back(nominalOrig);
		hsyst.push_back(genMHT);
		pctDiffMap.Add("MHTSyst",fabs(1-g_yield/nominal_yield)*100);
	}
	
	//add branches to syst tree
	for(auto& systpair : pctDiffMap.GetTable()){
		tree->Branch(systpair.first.c_str(),&systpair.second,(systpair.first+"/D").c_str());
	}
	
	//fill and write tree w/ hadd-able filename
	tree->Fill();
	//include year in file name
	string thetrfile = "tree_syst_"+setnames[0]+"_"+setnames[3]+"_block"+setnames[1]+"-"+setnames[2]+"_fast.root";
	TFile* trfile = TFile::Open(thetrfile.c_str(),"RECREATE");
	trfile->cd();
	tree->Write();
	trfile->Close();

	//write processed syst histos
	string thenewfile = outpre+"proc"+osuff+".root";
	TFile* outfile = TFile::Open(thenewfile.c_str(),"RECREATE");
	outfile->cd();
	nominal->Write();
	for(auto isyst : hsyst) isyst->Write();
	outfile->Close();
}
