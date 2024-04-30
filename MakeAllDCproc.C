//custom headers
#include "MakeAllDCsyst.C"

//customized helper
template <class T>
class KSystProcessorPMSSM : public KSystProcessor<T> {
	public:
		//constructor (does not find members from file)
		KSystProcessorPMSSM(const string& setname_, const string& therootfile_, const ModeInfo& info_) : KSystProcessor<T>(setname_, therootfile_, info_, true){
		}
		//slimmed version
		void process(bool verbose){
			string thenewfile = this->info.outpre+"proc"+this->info.osuff+".root";
			TFile* outfile = KOpen(thenewfile,"RECREATE");

			TFile* infile = KOpen(this->therootfile);
			TKey *key;
			TIter next(infile->GetListOfKeys());
			//first loop: find those to be kept for later
			while ((key = (TKey*)next())) {
				string ntmp = key->GetName();
				T* htmp = KGet<T>(infile,ntmp);
				if(ntmp.find("nominal")!=string::npos){
					if(verbose) cout << ntmp << endl;
					this->nominal = htmp;
					outfile->cd();
					this->nominal->Write();

					//make stat error
					string sname = this->changeHistoName(this->nominal->GetName(),"MCStatErr");
					double stat_yield = 0;
					auto ssyst = this->MakeStat(sname, stat_yield);
					outfile->cd();
					ssyst->Write();
					delete ssyst;
				}
				else if(ntmp.find("SLe")!=string::npos or ntmp.find("SLm")!=string::npos) { //keep same order as below
					continue;
				}
				else if(ntmp.find("genMHT")!=string::npos) { //handled later
					if(verbose) cout << ntmp << endl;
					this->genMHT = htmp;
				}
			}

			//second loop: find those to be processed in place
			TIter next2(infile->GetListOfKeys());
			while ((key = (TKey*)next2())) {
				string ntmp = key->GetName();
				T* htmp = KGet<T>(infile,ntmp);
				if(ntmp.find("nominal")!=string::npos){
					continue;
				}
				else if(ntmp.find("SLe")!=string::npos or ntmp.find("SLm")!=string::npos) { //contams
					if(verbose) cout << ntmp << endl;
					outfile->cd();
					htmp->Write();
					delete htmp;
				}
				else if(ntmp.find("genMHT")!=string::npos){
					continue;
				}
				else { //systematics
					if(verbose) cout << ntmp << endl;
					//divide, bound, set labels (prepended w/ "signal_")
					DivideBoundName(htmp);
					outfile->cd();
					htmp->Write();
					delete htmp;
				}
			}

			//genMHT correction and unc for fastsim
			//do this at the end
			if(this->genMHT){
				//dummy vars
				KMap<double> pctDiffMap;
				double nominal_yield = 0;
				this->MakeGenMHT(pctDiffMap,nominal_yield);
				outfile->cd();
				for(auto isyst : this->hsyst) isyst->Write();
			}

			outfile->Close();
		}
		//expanded helpers
		void DivideBoundName(T* isyst){
			vector<string> inames;
			KParser::process(isyst->GetName(),'_',inames);
			string treename;
			bool up = (inames.back().find("up")!=string::npos or inames.back().find("Up")!=string::npos);
			if(up) treename = inames.back().substr(0,inames.back().size()-2);
			else treename = inames.back().substr(0,inames.back().size()-4); //down
			string binname = "signal_"+treename;
			this->DivideBound(isyst,binname);
		}
};

//recompile:
//root -b -l -q MakeAllDCproc.C++
void MakeAllDCproc(string setname="", string indir="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV11", vector<string> input = {}, vector<string> extras = {}, string region="", bool verbose=false){
	gErrorIgnoreLevel = kBreak;

	if(setname.empty()){
		cout << "Recompiled MakeAllDCproc, exiting." << endl;
		return;
	}

	if(indir[indir.size()-1] != '/') indir = indir + "/";

	ModeInfo info = parseMode(input,extras,setname);
	//handle batch case w/ default region taken from config file
	if(region=="default") region = "";
	if(!region.empty()) info.region = region;

	//check for directory
	if(info.outdir.size()>0) system(("mkdir -p "+info.outdir).c_str());

	string therootfile = indir+info.outpre+info.region+info.osuff+".root";

	//further processing
	if(info.mode==Mode::RA2pmssm){
		KSystProcessorPMSSM<THnSparse> proc(setname,therootfile,info);
		proc.process(verbose);
	}
}

