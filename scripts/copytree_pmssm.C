#include "KCode/KMap.h"

#include <TFile.h>
#include <TTree.h>

#include <string>

using namespace std;

void copytree_pmssm(const string& filename, const string& outname, const string& treename){
	auto file = KOpen(filename);
	auto tree = KGet<TTree>(file,treename);

	//deactivate all branches
	tree->SetBranchStatus("*", 0);
	for(const auto& branch : {"chain_index", "Niteration", "xsec_tot_pb", "filter_eff"}){
		tree->SetBranchStatus(branch, 1);
	}

	auto ofile = KOpen(outname,"RECREATE");
	auto mintree = tree->CloneTree();
	ofile->cd();
	mintree->Write();
}
