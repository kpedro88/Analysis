from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter

parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
parser.add_argument("-m", "--mass", dest="mass", type=int, required=True, help="Zprime mass")
parser.add_argument("-n", "--name", dest="name", type=str, default="Test", help="test name (higgsCombine[name])")
parser.add_argument("-d", "--dir", dest="dir", type=str, default="test", help="directory for file")
parser.add_argument("-q", "--quantile", dest="quantile", type=str, required=True, help="quantile name")
args = parser.parse_args()

import ROOT as r

regions = [
    "highCut",
    "lowCut",
    "highSVJ2",
    "lowSVJ2",
]
for region in regions:
    fname = "{}/fit_toy_mZprime{}_{}_{}_{}.root".format(args.dir,args.mass,region,args.name,args.quantile)
    fitname = "{}_{}_MTAK8_data".format("g" if "Alt" in args.name else "f",args.quantile)
    datname = "MTAK8_data"
    signame = "MTAK8_SVJ_{}_20_0.3_peak_{}".format(args.mass,args.quantile)

    file = r.TFile.Open(fname)
    if file==None: continue

    fit = file.Get(fitname)
    dat = file.Get(datname)
    sig = file.Get(signame)

    # make s+b histogram
    fit_ndf = fit.GetNpar() # include normalization & fit params
    sig_ndf = 0
    dat_ndf = 0
    for b in range(sig.GetNbinsX()-1):
        bin = b+1
        old = sig.GetBinContent(bin)
        if old>0: sig_ndf += 1
        if dat.GetBinContent(bin)>0: dat_ndf += 1
        sig.SetBinContent(bin, old + fit.Integral(sig.GetBinLowEdge(bin),sig.GetBinLowEdge(bin+1))/sig.GetBinWidth(bin))

    # compute chi2 wrt data: unweighted vs. weighted comparison
    chi2 = r.Double(0.0)
    ndf = r.Long(0)
    igood = r.Long(0)
    prob = dat.Chi2TestX(sig, chi2, ndf, igood, "UW")
    
    output = "{} {} {}: chi2 / ndof = {:.2f} / {} ({}, {}, {})".format(region,args.mass,args.quantile,chi2,ndf,dat_ndf,sig_ndf,fit_ndf)
    print output

