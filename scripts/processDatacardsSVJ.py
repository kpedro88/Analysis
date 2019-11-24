import sys
from collections import OrderedDict
from ROOT import *

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
    return newname+newname2

class Sample(object):
    def __init__(self, name, file=None, names=None):
        # basic constructor
        self.name = name
        self.years = OrderedDict()
        self.systs = []
        # stores added TH2s
        self.output = []

        if file is None and names is None: return
        
        # full constructor
        names2 = [n for n in names if self.name in n]
        for year in usort([get_year(n) for n in names2]):
            self.years[year] = OrderedDict()
        for n in names2:
            year = get_year(n)
            syst = get_syst(n)
            # store the histogram
            htmp = file.Get(n)
            htmp.SetDirectory(0)
            self.years[year][syst] = htmp
        # keep separate list of all systs
        self.systs = usort([get_syst(n) for n in names2])
    def hadd(self, hists, outname):
        htmp = hists[0].Clone(outname)
        for h in hists[1:]:
            htmp.Add(h)
        self.output.append(htmp)
    def hadd_nominal(self, outname):
        self.hadd([syst["nominal"] for year,syst in self.years.iteritems()], outname)
    def hadd_syst(self, syst):
        if syst=="nominal": return
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
                else: hlist.append(self.years[year2]["nominal"])
            self.hadd(hlist,self.name+"_"+syst+"_"+subyear)
            done_years.append(subyear)

class SplitDir(object):
    def __init__(self, name, min, max=None):
        self.name = name
        self.min = min
        self.max = max if max is not None else min
        self.output = []
    def split(self, hist, store=True):
        htemp = hist.ProjectionX(hist.GetName(),self.min,self.max)
        htempF = DtoF(htemp)
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

# these should be CLI eventually
prefix = "MTAK8_RA2bin"
data = ["data"]
bkgs = ["QCD","TT","WJets","ZJets"]
file = "root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/Run2ProductionV17_v1/MTAK8_dijetmtdetahadloosefull.root"
outfile = "test/datacards_Run2ProductionV17_v1.root"

f = TFile.Open(file)
names = [k.GetName() for k in f.GetListOfKeys() if k.GetName().startswith(prefix)]

# get list of unique sample names
unique_names = usort([get_sample(n,prefix) for n in names])
# categories
samples = OrderedDict([
    ("data",[]),
    ("bkg",[]),
    ("sig",[]),
])

for name in unique_names:
    tmp = Sample(name,f,names)
    if name in data:
        samples["data"].append(tmp)
    elif name in bkgs:
        samples["bkg"].append(tmp)
    else:
        samples["sig"].append(tmp)

f.Close()

# add up nominals for data, bkg
samples["data"][0].hadd_nominal("data_obs")
fprint("Added data")
for bkg in samples["bkg"]:
    bkg.hadd_nominal(bkg.name)
    fprint("Added "+bkg.name)

# make tot for bkg
bname = "Bkg"
btmp = Sample(bname)
btmp.hadd([h for s in samples["bkg"] for h in s.output],bname)
samples["bkg"].append(btmp)
fprint("Added "+bname)

for sig in samples["sig"]:
    # add up nominals for sig
    sig.hadd_nominal(sig.name)
    # add up uncorrelated systs for sig
    for syst in sig.systs:
        sig.hadd_syst(syst)
    fprint("Added "+sig.name)

outf = TFile.Open(outfile,"RECREATE")
# split into TH1s and dirs (including cut-based using multiple bin projection)
dirs = [
    SplitDir("lowSVJ0_2018",1),
    SplitDir("lowSVJ1_2018",2),
    SplitDir("lowSVJ2_2018",3),
    SplitDir("highSVJ0_2018",4),
    SplitDir("highSVJ1_2018",5),
    SplitDir("highSVJ2_2018",6),
    # aggregate into cut-based
    SplitDir("lowCut_2018",1,3),
    SplitDir("highCut_2018",4,6),
]
for dir in dirs:
    dir.setup(outf)
    for cat in samples:
        for samp in samples[cat]:
            for h in samp.output:
                dir.split(h)
    fprint("Splitting done for "+dir.name)

    # make signal stat histos for this dir
    dir_samples = []
    for sig in samples["sig"]:
        stmp = Sample(sig.name+"_mcstat")
        for year in sig.years:
            stmp.years[year] = OrderedDict()
            # split first to get correct bin numbers
            htmp = dir.split(sig.years[year]["nominal"],False)
            stmp.years[year]["nominal"] = htmp
            stmp.systs.append("nominal")
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
