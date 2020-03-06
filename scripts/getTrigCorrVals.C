#include "corrections/TriggerFuncCorrector.h"

void getTrigCorrVals(string suff="DetaHLTmatch"){
	cout << fixed;
	vector<string> years = {"2016","2017","2018"};
	vector<int> uncs = {0,1};
	vector<double> vals = {1500.0,4500.0};
	for(const auto& year : years){
		TriggerFuncCorrector tfc;
		tfc.SetFunc("corrections/trigEffFit_SingleMuon_"+year+"_"+suff+".root","fit_MTAK8","err_MTAK8");
		cout << year << endl;
		for(auto val : vals){
			vector<double> corrs; corrs.reserve(uncs.size());
			for(auto unc : uncs){
				corrs.push_back(tfc.GetCorrection(val,unc));
			}
			cout << "\t" << int(val) << " : $" << setprecision(3) << corrs[0] << "\\pm" << setprecision(4) << abs(corrs[0]-corrs[1]) << "$" << endl;
		}
	}
}


