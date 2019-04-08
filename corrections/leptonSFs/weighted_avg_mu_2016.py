from ROOT import *

info = [
    {
        "files": ["RunBCDEF_SF_ID_Mu_2016_190404.root", "RunGH_SF_ID_Mu_2016_190404.root"],
        "hists": ["NUM_LooseID_DEN_genTracks_eta_pt","NUM_HighPtID_DEN_genTracks_eta_pair_newTuneP_probe_pt","NUM_TightID_DEN_genTracks_eta_pt","NUM_MediumID_DEN_genTracks_eta_pt"],
    },
    {
        "files": ["RunBCDEF_SF_ISO_Mu_2016_190404.root", "RunGH_SF_ISO_Mu_2016_190404.root"],
        "hists": ["NUM_LooseRelTkIso_DEN_HighPtIDandIPCut_eta_pair_newTuneP_probe_pt","NUM_TightRelIso_DEN_MediumID_eta_pt","NUM_TightRelIso_DEN_TightIDandIPCut_eta_pt","NUM_LooseRelIso_DEN_MediumID_eta_pt","NUM_LooseRelIso_DEN_LooseID_eta_pt","NUM_LooseRelIso_DEN_TightIDandIPCut_eta_pt"],
    },
]

lumi_BCDEF = 5746.370 + 2572.903 + 4242.287 + 3924.254 + 3104.508
lumi_GH = 7575.824 + 8649.019
lumi_tot = lumi_BCDEF + lumi_GH

frac_BCDEF = lumi_BCDEF/lumi_tot
frac_GH = lumi_GH/lumi_tot

for i in info:
    file_BCDEF = TFile.Open(i["files"][0])
    file_GH = TFile.Open(i["files"][1])
    file_out = TFile.Open(i["files"][0].replace("BCDEF","BCDEFGH_weightedAvg"),"RECREATE")
    for h in i["hists"]:
        h_BCDEF = file_BCDEF.Get(h)
        h_GH = file_GH.Get(h)
        h_BCDEF.Add(h_BCDEF,h_GH,frac_BCDEF,frac_GH)
        file_out.cd()
        h_BCDEF.Write()
    file_out.Close()
