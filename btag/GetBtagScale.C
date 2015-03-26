#include "btagSF.C"
#include "btagEFF.C"
#include "mistagSF.C"
#include "mistagEFF.C"
#include "BTagWeight4.h"

double GetBtagScale(vector<double> *PFJetPt_, vector<double> *PFJetEta_, vector<double> *PFJetPartonFlavour_, int do_unc_bSF=0, int do_unc_mSF=0){
	BTagWeight b(1);
	double wtag = 1.;
	vector<vector<BTagWeight::JetInfo> > jb;
	
	//loop over all PF jets
	for(unsigned j = 0; j < PFJetPt_->size(); j++){
		double jetpt = PFJetPt_->at(j);
		double jeteta = PFJetEta_->at(j);
		double jetflavor = fabs(PFJetPartonFlavour_->at(j));
		double discr = 1.7;
		
		double eff = 1.;
		double sf = 1.;
		if (jetflavor==5){
			eff = btagEFF(discr,1);
			sf = btagSF(jetpt,1,do_unc_bSF);
		}
		else if (jetflavor==4){
			eff = btagEFF(discr,0);
			sf = btagSF(jetpt,0,do_unc_bSF);
		}
		else{
			eff = mistagEFF(jetpt,jeteta);
			sf = mistagSF(jetpt,jeteta,do_unc_mSF);
		}
		
		BTagWeight::JetInfo jetinfo(eff,sf);
		vector<BTagWeight::JetInfo> jetInfoForAllOPs;
		jetInfoForAllOPs.push_back(jetinfo);
		jb.push_back(jetInfoForAllOPs);
	}
	wtag = b.weight(jb);

//	std::cout << wtag << std::endl;
	
	return wtag;
}
