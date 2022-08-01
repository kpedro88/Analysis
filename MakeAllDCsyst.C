//custom headers
#include "KPlotDriver.C"

//ROOT headers
#include <TError.h>
#include <TCollection.h>
#include <TKey.h>

//STL headers
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <exception>
#include <utility>

using namespace std;

//modes
enum class Mode {
	RA2full, RA2fast, RA2data, SVJsig, SVJscan, SVJdata, SVJbkg
};

class ModeInfo {
	public:
		ModeInfo(const string& setname, OptionMap* localOpt){
			getMode(localOpt);
			//selection and potentially setlist are set here
			//but will be overwritten if specified explicitly in config options
			string selmap; localOpt->Get("selmap",selmap);
			getSelYear(setname, selmap, localOpt);
			//fill members from map
			vector<pair<string,string*>> members{
				{"inpre",&inpre},
				{"region",&region},
				{"outpre",&outpre},
				{"outdir",&outdir},
				{"input",&input},
				{"setlist",&setlist},
				{"osuff",&osuff},
				{"systsuff",&systsuff},
				{"selbase",&selbase},
				{"selection",&selection},
			};
			for(const auto& member : members){
				bool has = localOpt->Get(member.first,*member.second);
				if(!has) throw runtime_error("Missing ModeInfo property: "+member.first);
			}
			//fix osuff
			if(!osuff.empty()) osuff += setname;
		}

		//members
		Mode mode;
		string inpre, region, outpre, outdir, input, setlist, osuff, systsuff, selbase, selection;

	private:
		//constructor helpers
		void getMode(OptionMap* localOpt){
			KMap<Mode> modemap;
			modemap.GetTable() = {
				{"RA2full",Mode::RA2full},
				{"RA2fast",Mode::RA2fast},
				{"RA2data",Mode::RA2data},
				{"SVJsig",Mode::SVJsig},
				{"SVJscan",Mode::SVJscan},
				{"SVJdata",Mode::SVJdata},
				{"SVJbkg",Mode::SVJbkg},
			};
			string smode; localOpt->Get("mode",smode);
			if(!modemap.Has(smode)) throw runtime_error("Unknown mode: "+smode);
			mode = modemap.Get(smode);
		}
		void getSelYear(const string& setname, const string& selmap, OptionMap* localOpt){
			//fill 2D vector from option map
			vector<string> selmap_vecs; localOpt->Get(selmap,selmap_vecs);
			vector<vector<string>> selmaps; selmaps.reserve(selmap_vecs.size());
			for(const auto& vec : selmap_vecs){
				selmaps.push_back(vector<string>());
				localOpt->Get(vec,selmaps.back());
			}
			//search
			bool found = false;
			for(const auto& selline : selmaps){
				if(setname.find(selline[0])!=string::npos){
					found = true;
					if(!localOpt->Has("selection")) localOpt->Set("selection",selline[1]);
					if(selline.size()>2 and !localOpt->Has("setlist")) localOpt->Set("selection",selline[2]);
					break;
				}
			}
			if(!found) throw runtime_error("Could not determine year for setname: "+setname);
		}
};

ModeInfo parseMode(const vector<string>& fnames, const vector<string>& extras, const string& setname){
	OptionMap* localOpt = new OptionMap();
	for(const auto& fname : fnames){
		KParser::processOption("in:options["+fname+"]",localOpt);
	}
	for(const auto& extra : extras){
		KParser::processOption(extra,localOpt);
	}
	return ModeInfo(setname,localOpt);
}

//helper - handles TH1 or TH2 cases
template <class T>
class KSystProcessor {
	public:
		//constructor that finds members from file
		KSystProcessor(const string& setname_, const string& therootfile_) : setname(setname_), therootfile(therootfile_) {
			TFile* infile = KOpen(therootfile_);
			TKey *key;
			TIter next(infile->GetListOfKeys());
			while ((key = (TKey*)next())) {
				string ntmp = key->GetName();
				T* htmp = KGet<T>(infile,ntmp);
				if(ntmp.find("nominal")!=string::npos) nominal = htmp;
				else if(ntmp.find("SLe")!=string::npos or ntmp.find("SLm")!=string::npos) hcontam.push_back(htmp);
				else if(ntmp.find("genMHT")!=string::npos) genMHT = htmp;
				else hsyst.push_back(htmp);
			}
		}
	
		void process(ModeInfo info){
			//todo: factorize the operations below
			
			//setup systematics analysis tree (to study changes in yield)
			vector<string> setnames;
			KParser::process(setname,'_',setnames);
			TTree* tree = new TTree("tree","systematics");
			int mParent = KParser::getOptionValue<int>(setnames[1]);
			int mChild = KParser::getOptionValue<int>(setnames[2]);
			double rinv;
			int alpha;
			string thetrfile;
			if(info.mode==Mode::RA2full or info.mode==Mode::RA2fast){
				tree->Branch("mMother",&mParent,"mMother/I");
				tree->Branch("mLSP",&mChild,"mLSP/I");
				//include year in file name
				thetrfile = "tree_syst_"+setnames[0]+"_"+setnames[3]+"_block"+setnames[1]+"-"+setnames[2]+"_"+info.systsuff+".root";
			}
			else if(info.mode==Mode::SVJsig or info.mode==Mode::SVJscan){
				rinv = KParser::getOptionValue<double>(setnames[3]);
				map<string,double> alpha_vals{
					{"peak",-2},
					{"high",-1},
					{"low",-3},
				};
				alpha = alpha_vals[setnames[4]];
				tree->Branch("mZprime",&mParent,"mZprime/I");
				tree->Branch("mDark",&mChild,"mDark/I");
				tree->Branch("rinv",&rinv,"rinv/D");
				tree->Branch("alpha",&alpha,"alpha/I");
				//include year in file name
				thetrfile = "tree_syst_"+setnames[0]+"_"+setnames[5]+"_block"+setnames[1]+"-"+setnames[2]+"-"+setnames[3]+"-"+setnames[4]+"_"+info.systsuff+".root";
			}
			//map to keep track of maximum pct diffs
			KMap<double> pctDiffMap;
			double nominal_yield = Integral(nominal);
			
			//divide, bound, set labels (prepended w/ "signal_")
			for(auto isyst : hsyst){
				vector<string> inames;
				KParser::process(isyst->GetName(),'_',inames);
				string treename;
				bool up = (inames.back().find("up")!=string::npos or inames.back().find("Up")!=string::npos);
				if(up) treename = inames.back().substr(0,inames.back().size()-2);
				else treename = inames.back().substr(0,inames.back().size()-4); //down
				//check yield (keep max diff in case of up and down)
				double iyield = Integral(isyst);
				double pctdiff = fabs(1.-iyield/nominal_yield)*100;
				if(!pctDiffMap.Has(treename) or pctDiffMap.Get(treename) < pctdiff){
					pctDiffMap.Add(treename,pctdiff);
				}
				string binname = "signal_"+treename;
				DivideBound(isyst,binname);
			}

			//add signal contamination (not a pct diff)
			double cyield = 0.;
			for(auto icontam : hcontam){
				string ntmp = icontam->GetName();
				if(ntmp.find("genMHT")!=string::npos) continue;
				cyield += Integral(icontam);
			}
			if(!hcontam.empty()) pctDiffMap.Add("contam",(cyield/nominal_yield)*100);
			
			//make stat error
			string sname = changeHistoName(nominal->GetName(),"MCStatErr");
			double stat_yield = 0;
			auto ssyst = MakeStat(sname, stat_yield);
			hsyst.push_back(ssyst);
			pctDiffMap.Add("MCStatErr",fabs(1-stat_yield/nominal_yield)*100);
			//average rather than max stat err
			pctDiffMap.Add("MCStatOverallErr",MakeStatOverall(nominal)*100);
			
			//genMHT correction and unc for fastsim
			if(genMHT){
				MakeGenMHT(pctDiffMap,nominal_yield);
			}
			
			//add branches to syst tree
			for(auto& systpair : pctDiffMap.GetTable()){
				tree->Branch(systpair.first.c_str(),&systpair.second,(systpair.first+"/D").c_str());
			}
			
			//fill and write tree w/ hadd-able filename
			tree->Fill();
			TFile* trfile = KOpen(thetrfile,"RECREATE");
			trfile->cd();
			tree->Write();
			trfile->Close();

			//write processed syst histos
			if(info.mode==Mode::RA2full or info.mode==Mode::RA2fast){
				string thenewfile = info.outpre+"proc"+info.osuff+".root";
				TFile* outfile = KOpen(thenewfile,"RECREATE");
				outfile->cd();
				nominal->Write();
				for(auto icontam : hcontam) icontam->Write();
				for(auto isyst : hsyst) isyst->Write();
				outfile->Close();
			}
		}
		//helper functions
		string changeHistoName(const string& name, const string& suff){
			vector<string> snames;
			KParser::process(name,'_',snames);
			snames.back() = suff;
			stringstream ssname;
			KParser::printvec(snames,ssname,"_");
			string sname = ssname.str();
			return sname;
		}
		void DivideBound_impl(T* hist, unsigned b){
			//divide
			double unc = 1.0;
			double denom = nominal->GetBinContent(b);
			//if central value is zero, treat syst as a shift
			if(denom==0.) denom = 1.0;
			unc = hist->GetBinContent(b)/denom;
			//zero check
			if(unc==0.) unc = 1.0;
			//bound
			unc = min(max(unc,0.01),1.99);
			//set
			hist->SetBinContent(b,unc);
		}
		void MakeStat_impl(T* hist, unsigned b, double& stat_yield){
			stat_yield += hist->GetBinError(b)+hist->GetBinContent(b);
			//divide
			hist->SetBinContent(b,
				hist->GetBinContent(b)>0. ? 1.0+hist->GetBinError(b)/hist->GetBinContent(b) : 1.0+hist->GetBinError(b)
			);
		}
		//functions to be specialized
		double MakeStatOverall(T* hist);
		double Integral(T* hist);
		void DivideBound(T* hist, const string& binname);
		T* MakeStat(const string& sname, double& yield);
		void MakeGenMHT(KMap<double>& pctDiffMap, double nominal_yield);
	
		//members
		string setname, therootfile;
		T* nominal = NULL;
		T* genMHT = NULL;
		vector<T*> hcontam;
		vector<T*> hsyst;
};

template <> double KSystProcessor<TH1F>::MakeStatOverall(TH1F* hist) {
	double err = 0.;
	double hint = hist->IntegralAndError(-1,-1,err);
	return err/hint;
}

template <> double KSystProcessor<TH2F>::MakeStatOverall(TH2F* hist) {
	double err = 0.;
	double hint = hist->IntegralAndError(-1,-1,-1,-1,err);
	return err/hint;
}

template<> double KSystProcessor<TH1F>::Integral(TH1F* hist){ return hist->Integral(-1,-1); }
template<> double KSystProcessor<TH2F>::Integral(TH2F* hist){ return hist->Integral(-1,-1,-1,-1); }

template<> void KSystProcessor<TH1F>::DivideBound(TH1F* hist, const string& binname){
	for(int b = 1; b <= hist->GetNbinsX(); ++b){
		DivideBound_impl(hist,b);
		hist->GetXaxis()->SetBinLabel(b,binname.c_str());
	}
}
template<> void KSystProcessor<TH2F>::DivideBound(TH2F* hist, const string& binname){
	for(int bx = 1; bx <= hist->GetNbinsX(); ++bx){
		for(int by = 1; by <= hist->GetNbinsY(); ++by){
			int b = hist->GetBin(bx,by);
			DivideBound_impl(hist,b);
			//no labels
		}
	}
}

template<> TH1F* KSystProcessor<TH1F>::MakeStat(const string& sname, double& stat_yield){
	TH1F* hist = (TH1F*)nominal->Clone(sname.c_str());
	for(int b = 1; b <= hist->GetNbinsX(); ++b){
		MakeStat_impl(hist,b,stat_yield);
		string slabel = "signal_MCStatErr_";
		slabel += hist->GetXaxis()->GetBinLabel(b);
		hist->GetXaxis()->SetBinLabel(b,slabel.c_str());
	}
	return hist;
}
template<> TH2F* KSystProcessor<TH2F>::MakeStat(const string& sname, double& stat_yield){
	TH2F* hist = (TH2F*)nominal->Clone(sname.c_str());
	for(int bx = 1; bx <= hist->GetNbinsX(); ++bx){
		for(int by = 1; by <= hist->GetNbinsY(); ++by){
			int b = hist->GetBin(bx,by);
			MakeStat_impl(hist,b,stat_yield);
			//no labels
		}
	}
	return hist;
}

template<> void KSystProcessor<TH1F>::MakeGenMHT(KMap<double>& pctDiffMap, double nominal_yield){
	//keep original nominal histogram
	string nname = changeHistoName(nominal->GetName(),"nominalOrig");
	TH1F* nominalOrig = (TH1F*)nominal->Clone(nname.c_str());
	string gname = changeHistoName(nominal->GetName(),"MHTSyst");
	TH1F* gsyst = (TH1F*)nominal->Clone(gname.c_str());
	double g_yield = 0;
	
	//modify nominal as average of nominal and genMHT & compute syst as difference
	for(int b = 1; b <= nominal->GetNbinsX(); ++b){
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
template<> void KSystProcessor<TH2F>::MakeGenMHT(KMap<double>& pctDiffMap, double nominal_yield){
	//not implemented
}

//recompile:
//root -b -l -q MakeAllDCsyst.C++
void MakeAllDCsyst(string setname="", string indir="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV11", vector<string> input = {}, vector<string> extras = {}, string region="", string systTypes="nominal", string varTypes=""){
	gErrorIgnoreLevel = kBreak;
	
	if(setname.empty()){
		cout << "Recompiled MakeAllDCsyst, exiting." << endl;
		return;
	}

	if(indir[indir.size()-1] != '/') indir = indir + "/";

	ModeInfo info = parseMode(input,extras,setname);
	//handle batch case w/ default region taken from config file
	if(region=="default") region = "";
	if(!region.empty()) info.region = region;

	//handle batch case with no vars
	if(varTypes=="none") varTypes = "";
	//process variation types - comma-separated input, need to be run separately
	vector<string> vars;
	KParser::process(varTypes,',',vars);
	
	//check for directory
	if(info.outdir.size()>0) system(("mkdir -p "+info.outdir).c_str());
	
	//keep a list of root files
	vector<string> rootfiles;
	
	//do the simple systematics all at once
	if(!systTypes.empty()){
		rootfiles.push_back(info.outdir+info.outpre+info.region+info.osuff);
		KPlotDriver(indir+info.inpre+info.region,{info.input,info.setlist},{"INPUT",info.selbase,info.selection,"OPTION","vstring:chosensets["+setname+"]","string:rootfile["+rootfiles.back()+"]","vstring:selections["+systTypes+"]"});
	}

	//do the full variations separately
	//produce a selection for each variation on the fly, cloned from nominal
	vector<string> leptonsigs = {"T1tttt","T2tt","T5qqqqVV"};
	bool islepton = leptonsigs.end()!=find_if(leptonsigs.begin(),leptonsigs.end(),[&](const string& s){return setname.find(s)!=string::npos;});
	for(auto& ivar : vars){
		string selection_base = "nominal";
		//change region
		string region_ = info.region + "_"+ivar;
		//hack for signal contamination
		if(ivar=="SLe" or ivar=="SLm" or ivar=="SLe_genMHT" or ivar=="SLm_genMHT") {
			//skip for non-lepton signals
			if(!islepton) continue;
			selection_base = "singlelep";
			region_ = ivar;
			//terrible hack
			if(ivar.size()>3) ivar[3] = '-';
		}
		rootfiles.push_back(info.outdir+info.outpre+region_+info.osuff);
		KPlotDriver(indir+info.inpre+region_,{info.input,info.setlist},{"INPUT",info.selbase,info.selection,"OPTION","vstring:chosensets["+setname+"]","string:rootfile["+rootfiles.back()+"]","vstring:selections["+ivar+"]","SELECTION",ivar,"\t"+selection_base});
	}
	
	//hadd and put file in pwd (for stageout)
	stringstream slist;
	KParser::printvec(rootfiles,slist,".root ");
	string therootfile = info.outpre+info.region+info.osuff+".root";
	string cmd = "hadd -f "+therootfile+" "+slist.str()+".root"; //add trailing delim
	system(cmd.c_str());

	vector<string> systs;
	KParser::process(systTypes,',',systs);
	if(systs.size()==1 and vars.empty()){ //for data/bkg case without systs
		cout << "Systematics not requested." << endl;
		return;
	}
	if(find(systs.begin(),systs.end(),"nominal")==systs.end()){ //for signal case when checking specific syst
		cout << "Nominal histogram not found, will not make relative systematics." << endl;
		return;		
	}
	
	//further processing
	if(info.mode==Mode::RA2full or info.mode==Mode::RA2fast){
		KSystProcessor<TH1F> proc(setname,therootfile);
		proc.process(info);		
	}
	else if(info.mode==Mode::SVJsig or info.mode==Mode::SVJscan){
		KSystProcessor<TH2F> proc(setname,therootfile);
		proc.process(info);
	}
}
