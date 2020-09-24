import os
from collections import OrderedDict
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter

# make status messages useful
def fprint(msg):
    import sys
    print(msg)
    sys.stdout.flush()

def alpha_val(val):
    result = 0
    if val=="peak": result = -2
    elif val=="high": result = -1
    elif val=="low": result = -3
    else: result = float(val)
    return result

def check_syst(full_name,signal):
    name = full_name.replace(signal+"_","",1)
    for suff in ["Up","Down"]:
        if name.endswith(suff): name = name[:-len(suff)]
    return name

class Sample(object):
    def __init__(self, name, hist):
        self.name = name
        self.hist = hist
        self.hist.SetName(self.name)
        self.integral = self.hist.Integral(1,self.hist.GetNbinsX())
        self.params = OrderedDict()
        self.systs = set()
        self.setargs = []
        self.trkargs = []

parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
parser.add_argument("-o", "--outdir", dest="outdir", default = "svj1", type=str, help="output directory")
parser.add_argument("-f", "--file", dest="file", type=str, default = "root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/trig3/sigfull/datacard_final_SVJ_3100_20_0.3_peak.root", help="input file (full path, local or EOS)")
parser.add_argument("-s", "--signal", dest="signal", default = "SVJ_mZprime3100_mDark20_rinv03_alphapeak", type=str, help="signal name")
parser.add_argument("-r", "--regions", dest="regions", default = ["highCut_2018","lowCut_2018"], type=str, nargs='+', help="list of regions")
parser.add_argument("-b", "--bkg", dest="bkg", type=str, default = ["Bkg"], nargs='+', help="name(s) of bkg histos")
parser.add_argument("-d", "--data", dest="data", default = False, action="store_true", help="use real data")
parser.add_argument("-F", "--flat", dest="flat", default = "flat_svj", type=str, help="file with dictionary of flat systematics")
args = parser.parse_args()

with open('input/dict_xsec_Zprime.txt','r') as xfile:
    xsecs = {int(xline.split('\t')[0]): float(xline.split('\t')[1]) for xline in xfile}

from ROOT import *

f = TFile.Open(args.file)
# assume local file will be kept in same dir as cards when combine runs
if not args.file.startswith("root://"): args.file = os.path.basename(args.file)

if len(args.outdir)==0: args.outdir = "."
elif not os.path.exists(args.outdir): os.makedirs(args.outdir)

flat = None
if len(args.flat)>0:
    flat = getattr(__import__(args.flat,fromlist=["flat"]),"flat")

outhists = []
outfiles = []
sigs_datacards = OrderedDict()
for region in args.regions:
    rdir = f.Get(region)
    names = sorted([k.GetName() for k in rdir.GetListOfKeys()])
    sigs = OrderedDict()
    bkgs = OrderedDict()
    data_obs = None

    for name in names:
        parse=name.split('_')
        if name==args.signal:
            # fix rinv 1.0
            if not "rinv0" in parse[3]: parse[3] = parse[3] + "0"
            sigs[name] = Sample(name,rdir.Get(name))
            sigs[name].params = OrderedDict([
                ("xsec", xsecs[int(parse[1].replace("mZprime",""))]),
                ("mZprime", float(parse[1].replace("mZprime",""))),
                ("mDark", float(parse[2].replace("mDark",""))),
                ("rinv", float(parse[3].replace("rinv",""))/10.),
                ("alpha", alpha_val(parse[4].replace("alpha",""))),
            ])
        elif name.startswith(args.signal):
            sigs[args.signal].systs.add(check_syst(name,args.signal))
        elif name=="data_obs":
            data_obs = Sample("data_obs",rdir.Get(name))
        elif name in args.bkg:
            bkgs[name] = Sample(name,rdir.Get(name))

    # make data_obs if not present
    if not args.data: data_obs = None
    if data_obs is None:
        fprint("Making data_obs from bkgs...")
        hdata = None
        for bkg, h_bkg in bkgs.iteritems():
            if hdata is None:
                hdata = h_bkg.Clone("data_obs")
                hdata.SetDirectory(0)
            else:
                hdata.Add(h_bkg)
        data_obs = Sample("data_obs",hdata)

    # one datacard for each signal
    for sname,sig in sigs.iteritems():
        # make datacard
        dcfname = args.outdir+"/"+"datacard_"+region+"_"+sname+".txt"
        if sname not in sigs_datacards:
            sigs_datacards[sname] = [dcfname]
        else:
            sigs_datacards[sname].append(dcfname)
        with open(dcfname,'w') as dcfile:
            # comment to help freeze sig params
            for p,v in sig.params.iteritems():
                sig.setargs.append(p+"="+str(v))
                sig.trkargs.append(p)
            param_line = "--freezeParameters "+','.join(sig.trkargs)+" --trackParameters "+','.join(sig.trkargs)
            dcfile.write("# "+param_line+"\n")
            dcfile.write("imax 1\n")
            dcfile.write("jmax "+str(len(bkgs))+"\n")
            dcfile.write("kmax *\n")
            dcfile.write("---------------\n")
            dcfile.write("shapes * * "+args.file+" $CHANNEL/$PROCESS $CHANNEL/$PROCESS_$SYSTEMATIC\n")
            dcfile.write("---------------\n")
            dcfile.write("bin "+region+"\n")
            dcfile.write("observation "+str(data_obs.integral)+"\n")
            dcfile.write("---------------\n")
            dcfile.write("bin " + region + ''.join(" "+region for b in bkgs) + "\n")
            dcfile.write("process " + sname + ''.join(" "+b for b in bkgs) + "\n")
            dcfile.write("process 0" + ''.join(" "+str(i+1) for i,b in enumerate(bkgs)) + "\n")
            dcfile.write("rate " + str(sig.integral) + ''.join(" "+str(bkg.integral) for bname,bkg in bkgs.iteritems()) + "\n")
            dcfile.write("---------------\n")
            # signal systematics
            if flat is not None:
                for key,val in flat.iteritems():
                    dcfile.write(key + " " + "lnN" + " " + str(1.+val/100.) + ''.join(" -" for b in bkgs) + "\n")
            for syst in sorted(sig.systs):
                dcfile.write(syst + " " + "shape" + " "+ "1" + ''.join(" -" for b in bkgs) + "\n")
            dcfile.write("---------------\n")
            # set sig params
            for p,v in sig.params.iteritems():
                dcfile.write(p+" extArg "+str(v)+"\n")
            dcfile.write("---------------\n")
            fprint("wrote "+dcfname)

            # basic combine command
            command = "combine -M AsymptoticLimits "+param_line+"  --keyword-value region="+region+" --keyword-value sig="+sig.name+" "+dcfname
            fprint(command)

for sig,datacards in sigs_datacards.iteritems():
    dcfname = args.outdir+"/"+"datacard_"+sig+".txt"
    # combine cards
    command = "combineCards.py "+" ".join(datacards)+" > "+dcfname
    fprint(command)
