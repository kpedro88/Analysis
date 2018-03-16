from ROOT import *

def DtoF(hD):
    hF = TH1F()
    hD.Copy(hF)
    return hF

infile = TFile.Open("test/svj_unskimmed_met.root")

# find MET cut efficiencies
met_QCD = infile.Get("met_QCD")
met_edges = {met_QCD.GetXaxis().GetBinLowEdge(i):i for i in xrange(met_QCD.GetNbinsX()+1)}
met_integral = met_QCD.Integral(0,met_QCD.GetNbinsX()+1)

met_vals = [0,100,200]
met_effs = []
met_bins = []
for met_val in met_vals:
    bin = met_edges[met_val]
    sub_integral = met_QCD.Integral(bin,met_QCD.GetNbinsX()+1)
    met_effs.append(sub_integral/met_integral)
    met_bins.append(bin)

#print met_effs

# lists of variables and samples
vars = {
    "met" : [met_bins,met_effs],
    "metMTratio" : [[],[]],
    "metsig" : [[],[]],
}
samples = ["QCD","SVJ_3000_20_0.3_0.2","SVJ_1000_20_0.3_0.2","SVJ_2000_20_0.3_0.2","SVJ_4000_20_0.3_0.2","SVJ_3000_1_0.3_0.2","SVJ_3000_50_0.3_0.2","SVJ_3000_100_0.3_0.2","SVJ_3000_20_0.1_0.2","SVJ_3000_20_0.5_0.2","SVJ_3000_20_0.7_0.2","SVJ_3000_20_0.3_0.1","SVJ_3000_20_0.3_0.5","SVJ_3000_20_0.3_1"]

for sample in samples:
    outfile = TFile.Open("test/unskimmed/MTAK8_"+sample+".root","RECREATE")
    outfile.cd()
    for var,vals in vars.iteritems():
        histo = infile.Get(var+"_"+sample)
        # find appropriate cuts in other vars
        if len(vals[0])==0:
            vals[0] = [0,0,0]
            vals[1] = [100,100,100]
            var_integral = histo.Integral(0,histo.GetNbinsX()+1)
            for bin in xrange(histo.GetNbinsX()+1):
                sub_integral = histo.Integral(bin,histo.GetNbinsX()+1)
                eff = sub_integral/var_integral
                for ieff,meff in enumerate(met_effs):
                    if abs(eff-meff) < abs(vals[1][ieff]-meff):
                        vals[1][ieff] = eff
                        vals[0][ieff] = bin
        # print cut values
        if sample=="QCD":
            print var
            print ",".join([str(histo.GetXaxis().GetBinLowEdge(bin)) for bin in vals[0]])
            print ",".join([str(val) for val in vals[1]])
        # make projections
        histo2D = infile.Get(var+"_"+"MTAK8"+"_"+sample)
        for ival,val in enumerate(vals[0]):
            htmp = DtoF(histo2D.ProjectionY("MTAK8"+"_"+var+str(ival),val,-1))
            htmp.Write()
    outfile.Close()

print vars
