import sys
from numpy import array
from collections import OrderedDict
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter

# make status messages useful
def fprint(msg):
    print(msg)
    sys.stdout.flush()

# names = prefix_sample_year_syst
def get_sample(n,prefix):
    return '_'.join(n.replace(prefix+"_","").split('_')[:-2])
def get_year(n):
    return n.split('_')[-2].replace("MC","")
def get_syst(n):
    return n.split('_')[-1]
def usort(tmp):
    return sorted(list(set(tmp)))

# for TH2 projections
def DtoF(hD):
    from ROOT import TH1F
    hF = TH1F()
    hD.Copy(hF)
    return hF

# converts to UZH naming convention for signal histograms
def convertName(outname,dirname):
    # convert signal name: SVJ_X_Y_Z_W -> SVJ_mZprimeX_mDarkY_rinvZ_alphaW
    namesplit = outname.split('_')
    newname = '_'.join([namesplit[0],"mZprime"+namesplit[1],"mDark"+namesplit[2],"rinv"+namesplit[3].replace(".",""),"alpha"+namesplit[4]])
    newname2 = ""
    if len(namesplit)>5:
        newname2 = "_"
        # statistics formatting: mcstat_binNUp_2016 -> mcstat_dirname_sample_MC2016binNUp
        if "mcstat" in namesplit:
            newname2 += '_'.join([namesplit[5],dirname,newname,"MC"+namesplit[7]+namesplit[6]])
        # systematics formatting: systUp_2016 -> MC2016systUp
        else:
            newname2 += "MC"+namesplit[6]+namesplit[5]
        # capitalization check
        if newname2[-2:]=="up": newname2 = newname2[:-2] + "Up"
        elif newname2[-4:]=="down": newname2 = newname2[:-4] + "Down"
    return newname+newname2

class Sample(object):
    def __init__(self, name, names_hists=None, nominal="nominal"):
        # basic constructor
        self.name = name
        self.years = OrderedDict()
        self.systs = []
        # stores added TH2s
        self.output = []
        self.nominal = nominal

        if names_hists is None: return
        
        # full constructor
        names2 = [n for n in names_hists if self.name in n]
        for year in usort([get_year(n) for n in names2]):
            self.years[year] = OrderedDict()
        for n in names2:
            year = get_year(n)
            syst = get_syst(n)
            # store the histogram
            self.years[year][syst] = names_hists[n]
        # keep separate list of all systs
        self.systs = usort([get_syst(n) for n in names2])
    def hadd(self, hists, outname):
        htmp = hists[0].Clone(outname)
        for h in hists[1:]:
            htmp.Add(h)
        self.output.append(htmp)
    def hadd_nominal(self, outname):
        self.hadd([syst[self.nominal] for year,syst in self.years.iteritems()], outname)
    def hadd_syst(self, syst, correl=False):
        if syst==self.nominal: return
        if correl:
            # add up for all years
            hlist = []
            for year in self.years:
                hlist.append(self.years[year][syst])
                self.hadd(hlist,self.name+"_"+syst+"_"+"Run2")
        else:
            # make each year variation (adding nominal histos from other years)
            done_years = []
            for year1 in self.years:
                if syst not in self.years[year1]:
                    fprint("Warning: syst "+syst+" not present for year "+year1)
                hlist = []
                # 2018PRE and 2018POST systs should be considered together as 2018
                subyear = year1[:4]
                if subyear in done_years: continue
                for year2 in self.years:
                    if subyear==year2[:4]: hlist.append(self.years[year2][syst])
                    else: hlist.append(self.years[year2][self.nominal])
                self.hadd(hlist,self.name+"_"+syst+"_"+subyear)
                done_years.append(subyear)

class SplitDir(object):
    def __init__(self, name, min, max=None, bins=None):
        self.name = name
        self.min = min
        self.max = max if max is not None else min
        self.output = []
        self.bins = bins
    def split(self, hist, store=True):
        htemp = hist.ProjectionX(hist.GetName(),self.min,self.max)
        htempF = DtoF(htemp)

        # rebin and divide by bin width (if variable)
        if bins is not None:
            htempF = htempF.Rebin(len(self.bins)-1,htempF.GetName(),array(self.bins))
            if htempF.GetXaxis().IsVariableBinSize():
                for b in range(htempF.GetNbinsX()):
                    bin = b+1
                    htempF.SetBinContent(bin,htempF.GetBinContent(bin)/htempF.GetXaxis().GetBinWidth(bin))
                    htempF.SetBinError(bin,htempF.GetBinError(bin)/htempF.GetXaxis().GetBinWidth(bin))

        if store: self.output.append(htempF)
        else: return htempF
    def setup(self, file):
        file.cd()
        self.dir = file.mkdir(self.name)
        self.dir.cd()
    def write(self):
        self.dir.cd()
        for hist in self.output:
            outname = hist.GetName()
            if "SVJ" in outname:
                outname = convertName(outname,self.name)
                hist.SetName(outname)
            hist.Write(outname)

if __name__=="__main__":
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument("-p", "--prefix", dest="prefix", type=str, default="MTAK8_RA2bin", help="prefix for histogram names")
    parser.add_argument("-d", "--data", dest="data", type=str, default=["data"], nargs="*", help="list of names for data category")
    parser.add_argument("-b", "--bkgs", dest="bkgs", type=str, default=["QCD","TT","WJets","ZJets"], nargs="*", help="list of names for bkg category")
    parser.add_argument("-i", "--dir", dest="dir", type=str, default="", help="directory for input files")
    parser.add_argument("-f", "--files", dest="files", type=str, nargs="+", help="list of input files", required=True)
    parser.add_argument("-o", "--out", dest="out", type=str, help="output file name", required=True)
    parser.add_argument("-n", "--no-split", dest="no_split", default=False, action="store_true", help="disable split into dirs for regions")
    parser.add_argument("-m", "--nominal", dest="nominal", default="nominal", type=str, help="name of nominal selection")
    parser.add_argument("-r", "--rebin", dest="rebin", default=-1, type=float, help="rebin using bins for n*sigma (if >=0)")
    parser.add_argument("-s", "--no-stat", dest="no_stat", default=False, action="store_true", help="disable signal stat histos")
    args = parser.parse_args()

    # bins
    # todo: make finalBinning importable and generate on the fly?
    bins = {
        "highCut": {
            0.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3150.0, 3200.0, 3250.0, 3300.0, 3400.0, 3500.0, 3600.0, 3750.0, 3950.0, 4300.0, 5900.0, 8000.0],
            0.5: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3150.0, 3250.0, 3350.0, 3450.0, 3600.0, 3800.0, 4100.0, 4900.0, 8000.0],
            1.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3150.0, 3250.0, 3350.0, 3450.0, 3600.0, 3800.0, 4150.0, 8000.0],
            1.5: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3200.0, 3300.0, 3400.0, 3550.0, 3750.0, 4100.0, 8000.0],
            2.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3100.0, 3200.0, 3300.0, 3400.0, 3550.0, 3800.0, 4300.0, 8000.0],
        },
        "lowCut": {
            0.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3150.0, 3200.0, 3250.0, 3300.0, 3350.0, 3400.0, 3450.0, 3500.0, 3550.0, 3600.0, 3650.0, 3700.0, 3750.0, 3800.0, 3850.0, 3900.0, 3950.0, 4050.0, 4150.0, 4250.0, 4400.0, 4550.0, 4800.0, 5250.0, 8000.0],
            0.5: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3150.0, 3200.0, 3250.0, 3300.0, 3350.0, 3400.0, 3450.0, 3500.0, 3550.0, 3600.0, 3650.0, 3700.0, 3750.0, 3800.0, 3850.0, 3900.0, 4000.0, 4100.0, 4200.0, 4350.0, 4550.0, 4850.0, 5700.0, 8000.0],
            1.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3150.0, 3200.0, 3250.0, 3300.0, 3350.0, 3400.0, 3450.0, 3500.0, 3550.0, 3600.0, 3650.0, 3700.0, 3750.0, 3800.0, 3900.0, 4000.0, 4100.0, 4250.0, 4450.0, 4750.0, 5450.0, 8000.0],
            1.5: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3150.0, 3200.0, 3250.0, 3300.0, 3350.0, 3400.0, 3450.0, 3500.0, 3550.0, 3600.0, 3650.0, 3700.0, 3750.0, 3850.0, 3950.0, 4050.0, 4200.0, 4400.0, 4700.0, 5450.0, 8000.0],
            2.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0, 3050.0, 3100.0, 3150.0, 3200.0, 3250.0, 3300.0, 3350.0, 3400.0, 3450.0, 3500.0, 3550.0, 3600.0, 3650.0, 3700.0, 3800.0, 3900.0, 4000.0, 4150.0, 4350.0, 4650.0, 5450.0, 8000.0],
        },
        "highSVJ2": {
            0.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1850.0, 1950.0, 2050.0, 2200.0, 2550.0, 8000.0],
            0.5: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1800.0, 1900.0, 2050.0, 2250.0, 2900.0, 8000.0],
            1.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1800.0, 1900.0, 2050.0, 2300.0, 8000.0],
            1.5: [1500.0, 1550.0, 1600.0, 1650.0, 1750.0, 1850.0, 2000.0, 2250.0, 8000.0],
            2.0: [1500.0, 1550.0, 1600.0, 1650.0, 1750.0, 1850.0, 2000.0, 2300.0, 8000.0],
        },
        "lowSVJ2": {
            0.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2450.0, 2550.0, 2700.0, 3000.0, 8000.0],
            0.5: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2400.0, 2500.0, 2650.0, 2950.0, 8000.0],
            1.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2300.0, 2400.0, 2500.0, 2700.0, 3200.0, 8000.0],
            1.5: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2300.0, 2400.0, 2550.0, 2850.0, 8000.0],
            2.0: [1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2250.0, 2350.0, 2500.0, 2750.0, 8000.0],
        },
    }

    missing_bins = [k for k,v in bins.iteritems() if args.rebin not in v]
    if args.rebin>=0 and len(missing_bins)>0:
        parser.error("Bins not available for {} for sigma = {}".format(','.join(missing_bins),args.rebin))

    # categories
    samples = OrderedDict([
        ("data",[]),
        ("bkg",[]),
        ("sig",[]),
    ])

    # hardcoded list of correlated systematics
    correl_systs = set([
        "JESup",
        "JESdown",
    ])

    from ROOT import *

    names_hists = {}
    for file in args.files:
        if len(args.dir)>0: file = args.dir+"/"+file
        f = TFile.Open(file)
        names = [k.GetName() for k in f.GetListOfKeys() if k.GetName().startswith(args.prefix)]
        for name in names:
            htmp = f.Get(name)
            htmp.SetDirectory(0)        
            names_hists[name] = htmp
        f.Close()

    # get list of unique sample names
    unique_names = usort([get_sample(n,args.prefix) for n in names_hists])

    for name in unique_names:
        tmp = Sample(name,names_hists,args.nominal)
        if name in args.data:
            samples["data"].append(tmp)
        elif name in args.bkgs:
            samples["bkg"].append(tmp)
        else:
            samples["sig"].append(tmp)

    # add up nominals for data, bkg
    if len(samples["data"])>0:
        samples["data"][0].hadd_nominal("data_obs")
        fprint("Added data")

    # add up nominals for bkg
    if len(samples["bkg"])>0:
        for bkg in samples["bkg"]:
            bkg.hadd_nominal(bkg.name)
            fprint("Added "+bkg.name)

        # make tot for bkg
        bname = "Bkg"
        btmp = Sample(bname,nominal=args.nominal)
        btmp.hadd([h for s in samples["bkg"] for h in s.output],bname)
        samples["bkg"].append(btmp)
        fprint("Added "+bname)

    for sig in samples["sig"]:
        # add up nominals for sig
        sig.hadd_nominal(sig.name)
        # add up uncorrelated systs for sig
        for syst in sig.systs:
            sig.hadd_syst(syst, syst in correl_systs)
        fprint("Added "+sig.name)

    outf = TFile.Open(args.out,"RECREATE")

    # alternative option, just save combined histos
    if args.no_split:
        outf.cd()
        for cat in samples:
            for samp in samples[cat]:
                for h in samp.output:
                    h.Write()
        sys.exit(0)

    # use high-X bins for low-X regions
    bins_bdt = bins["highSVJ2"][args.rebin] if args.rebin>=0 else None
    bins_cut = bins["highCut"][args.rebin] if args.rebin>=0 else None
    # split into TH1s and dirs (including cut-based using multiple bin projection)
    dirs = [
        SplitDir("lowSVJ0_2018",1,bins=bins_bdt),
        SplitDir("lowSVJ1_2018",2,bins=bins_bdt),
        SplitDir("lowSVJ2_2018",3,bins=bins_bdt),
        SplitDir("highSVJ0_2018",4,bins=bins_bdt),
        SplitDir("highSVJ1_2018",5,bins=bins_bdt),
        SplitDir("highSVJ2_2018",6,bins=bins_bdt),
        # aggregate into cut-based
        SplitDir("lowCut_2018",1,3,bins=bins_cut),
        SplitDir("highCut_2018",4,6,bins=bins_cut),
    ]
    for dir in dirs:
        dir.setup(outf)
        for cat in samples:
            for samp in samples[cat]:
                for h in samp.output:
                    dir.split(h)
        fprint("Splitting done for "+dir.name)

        if not args.no_stat:
            # make signal stat histos for this dir
            dir_samples = []
            for sig in samples["sig"]:
                stmp = Sample(sig.name+"_mcstat",nominal=args.nominal)
                for year in sig.years:
                    stmp.years[year] = OrderedDict()
                    # split first to get correct bin numbers
                    htmp = dir.split(sig.years[year][args.nominal],False)
                    stmp.years[year][args.nominal] = htmp
                    stmp.systs.append(args.nominal)
                    # generate stat variations for this year
                    for b in range(1,htmp.GetNbinsX()+1):
                        nameUp = "bin"+str(b)+"Up"
                        htmpUp = htmp.Clone(htmp.GetName()+"_"+nameUp)
                        htmpUp.SetBinContent(b,htmpUp.GetBinContent(b)+htmpUp.GetBinError(b))
                        stmp.years[year][nameUp] = htmpUp
                        nameDown = "bin"+str(b)+"Down"
                        htmpDown = htmp.Clone(htmp.GetName()+"_"+nameDown)
                        htmpDown.SetBinContent(b,htmpDown.GetBinContent(b)-htmpDown.GetBinError(b))
                        stmp.years[year][nameDown] = htmpDown
                        stmp.systs.extend([nameUp,nameDown])
                # add up uncorrelated
                stmp.systs = usort(stmp.systs)
                for syst in stmp.systs:
                    stmp.hadd_syst(syst)
                # since these are already split, they can just be appended to the dir
                dir_samples.extend(stmp.output)
                fprint("Added "+stmp.name+" ("+dir.name+")")

            # write out content of dir (handles names)
            dir.output.extend(dir_samples)

        dir.write()
