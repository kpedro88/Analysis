import ROOT as r

def DtoF(hD):
    hF = r.TH1F()
    hD.Copy(hF)
    return hF

file = r.TFile.Open("root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Nm1/hist2d/svj_Nm1.root")
keys = [k.GetName() for k in file.GetListOfKeys()]
prefix = "met_metMTratio"
outfile = r.TFile.Open("test/paper/Nm1/svj_Nm1_met_new.root","RECREATE")
for key in keys:
    if prefix not in key:
        continue
    histo2D = file.Get(key)
    htmp = DtoF(histo2D.ProjectionX(key.replace("_metMTratio","")))
    outfile.cd()
    htmp.Write()

outfile.Close()

