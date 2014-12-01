#include "btag/btagSF.C"
#include "btag/btagEFF.C"
#include "btag/mistagSF.C"
#include "btag/mistagEFF.C"
#include "btag/BTagWeight4.h"

{
	vector<int> v_pfjets;
	 
	BTagWeight b(1);
	//Compute weight due to btag/mistag
	Wtag = 1.;
	vector<vector<BTagWeight::JetInfo> > j;
	for (int i=0;i<v_pfjets.size();++i){
	
		int index = v_pfjets[i];
		double jetpt = PFJetPt->at(index);
		double jeteta = PFJetEta->at(index);
		double jetflavor = TMath::Abs(PFJetPartonFlavour->at(index));
		double discr = 1.7;
		
		double eff = 1.;
		double sf = 1.;
		if (jetflavor==5){
		eff = btagEFF(discr,1);
		sf = btagSF(jetpt,1);
		}else if (jetflavor==4){
		eff = btagEFF(discr,0);
		sf = btagSF(jetpt,0);
		}else{
		eff = mistagEFF(jetpt,jeteta);
		sf = mistagSF(jetpt,jeteta);
		}
		
		BTagWeight::JetInfo jetinfo(eff,sf);
		vector<BTagWeight::JetInfo> jetInfoForAllOPs;
		jetInfoForAllOPs.push_back(jetinfo);
		j.push_back(jetInfoForAllOPs);

	}// End of loop over PF-jets
	Wtag = b.weight(j);
 
 }