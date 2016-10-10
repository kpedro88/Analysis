//custom headers
#include "KPlotDriver.C"

//ROOT headers
#include <TError.h>

//STL headers
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

//recompile:
//root -b -l -q MakeAllDCsyst.C++
void MakeAllDCsyst(int mode=-1, string indir="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV7", string systTypes="nominal,QCD,scaleunc,isrunc,trigStatUnc,trigSystUnc,JEC,JER,btagSFunc,mistagSFunc", string contamTypes="LDP,GJet_CleanVars,GJetLDP_CleanVars", int part=-1, string suffix=""){
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
	string inputQCD = "input/input_RA2bin_DC_QCD.txt";
	string setlist = "";
	string osuff = "";
	
	//process syst/contam types - comma-separated input
	vector<string> contams;
	KParser::process(contamTypes,',',contams);
	vector<string> systs;
	KParser::process(systTypes,',',systs);
	if(mode==1){
		if(part==-1){
			outdir = "datacards_fast"+suffix+"/";
			setlist = "input/input_sets_DC_fast"+suffix+".txt";
		}
		else { //batch mode
			stringstream ss;
			ss << "input/fast"+suffix+"/input_sets_DC_fast"+suffix+"_" << part << ".txt";
			setlist = ss.str();
			ss.str(string());
			ss << "_part" << part;
			osuff = ss.str();
		}
	}
	else {
		outdir = "datacards/";
		if(suffix.size()>0) outdir = "datacards_"+suffix+"/";
		setlist = "input/input_sets_DC_syst.txt";
	}
	
	//check for directory
	if(outdir.size()>0) system(("mkdir -p "+outdir).c_str());
	
	//first do the nominal version
	
	
	//do the variations
	for(unsigned i = 0; i < systs.size(); ++i){
		//special cases
		//skip all in batch mode
		if(systs[i]=="none") break;
		//the nominal version
		else if(systs[i]=="nominal"){
			cout << "nominal" << endl;
			KPlotDriver(indir+inpre+region,{input,setlist},{"OPTION","string:rootfile["+outdir+outpre+region+osuff+"]"});
			continue;
		}
		//qcd sideband
		else if(systs[i]=="QCD"){
			cout << "QCD sideband" << endl;
			KPlotDriver(indir+inpre+region,{inputQCD,setlist},{"OPTION","string:rootfile["+outdir+outpre+region+"_QCD"+osuff+"]"});
			continue;
		}
		
		string ovar = "";
		//up
		cout << systs[i] << " up" << endl;
		if(systs[i]=="JEC") region = "signal_JECup";
		else if(systs[i]=="JER") region = "signal_JERup";
		else ovar = "_"+systs[i]+"Up";
		stringstream ssystup;
		ssystup << "int:" << systs[i] << "[" << 1 << "]";
		KPlotDriver(indir+inpre+region,{input,setlist},{"OPTION","string:rootfile["+outdir+outpre+region+ovar+osuff+"]",ssystup.str()});
		//down
		cout << systs[i] << " down" << endl;
		if(systs[i]=="JEC") region = "signal_JECdown";
		else if(systs[i]=="JER") region = "signal_JERdown";
		else ovar = "_"+systs[i]+"Down";
		stringstream ssystdown;
		ssystdown << "int:" << systs[i] << "[" << 1 << "]";
		KPlotDriver(indir+inpre+region,{input,setlist},{"OPTION","string:rootfile["+outdir+outpre+region+ovar+osuff+"]",ssystdown.str()});
		
		//reset region if altered for jet syst
		if(systs[i]=="JEC" || systs[i]=="JER") region = "signal";
	}
	
	//now do the contaminations
	for(unsigned i = 0; i < contams.size(); ++i){
		if(contams[i]=="none") break;
		cout << contams[i] << endl;
		KPlotDriver(indir+inpre+contams[i],{input,setlist},{"OPTION","string:rootfile["+outdir+outpre+contams[i]+osuff+"]"});
		
		//qcd sideband for LDP & genMHT
		if(contams[i]=="LDP" || contams[i]=="signal_genMHT" || contams[i]=="LDP_genMHT"){
			cout << contams[i] << " QCD sideband" << endl;
			KPlotDriver(indir+inpre+contams[i],{inputQCD,setlist},{"OPTION","string:rootfile["+outdir+outpre+contams[i]+"_QCD"+osuff+"]"});
			continue;
		}
	}

}