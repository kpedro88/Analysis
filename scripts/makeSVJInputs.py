import os, sys
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
from collections import OrderedDict
sys.path.insert(0,os.getcwd())
from makeFastCommon import *

def parse(sample):
    params = {}
    samplesplit = sample.split("_")
    for s in samplesplit:
        if "-" in s:
            kv = s.split("-")
            params[kv[0]] = kv[1]

    return params

def parse_short(sample):
    params = {}
    samplesplit = sample.split("_")
    params["mZprime"] = samplesplit[2]
    params["mDark"] = samplesplit[3]
    params["rinv"] = samplesplit[4]
    params["alpha"] = samplesplit[5]
    return params

# define options
parser = ArgumentParser(add_help=False, formatter_class=ArgumentDefaultsHelpFormatter)
parser.add_argument("--help", dest="help", default=False, action="store_true", help="show this help message and exit")
parser.add_argument("-d", "--dir", dest="dir", default="/store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/Skims/tree_dijetmtdetahadloosemf/", help="location of root files (LFN)")
parser.add_argument("-h", "--hist", dest="hist", default="input/input_svj_hp_sets_sig.txt", help="hist output file to write")
parser.add_argument("-s", "--skim", dest="skim", default="input/input_svj_sets_sig.txt", help="skim output file to write")
parser.add_argument("-o", "--option", dest="option", default="input/input_svj_train_options.txt", help="option output file to write")
parser.add_argument("-c", "--scan", dest="scan", default=False, action="store_true", help="enable special options for scans")
args = parser.parse_args()

if args.help:
    parser.print_help()
    sys.exit(0)

# find the root files
files = filter(None,os.popen("xrdfs root://cmseos.fnal.gov/ ls "+args.dir).read().split('\n'))
# basename
files = [f.split("/")[-1] for f in files]
files = [x for x in files if "SVJ" in x]

# style
colors = ["kBlack","kBlue","kMagenta + 2","kRed","kCyan + 2","kMagenta","kOrange + 7","kYellow + 3","kGreen + 2","kPink - 9","kGray + 1"]
styles = [1,2,3,4,7,9]
cc = 0
cs = 0

# alpha val translation
alphavals = {
    "high": -1,
    "peak": -2,
    "low": -3,
}

# default params
params_default = OrderedDict([
    ("mZprime", 3000),
    ("mDark", 20),
    ("rinv", 0.3),
    ("alpha", "peak"),
])

# open files
dfile = open(args.hist,'w')
dfile.write("SET\n")
sfile = open(args.skim,'w')
sfile.write("SET\n")
ifile = open(args.option,'w')
numers = []
branches = []
for sample in files:
    # parse filename: tree_SVJ_mZprime-1000_mDark-20_rinv-0.3_alpha-peak_MC2017.root
    #             or: tree_SVJ_1000_20_0.3_peak_MC2018.root
    params = parse(sample) if "-" in sample else parse_short(sample)
    name = sample.replace("tree_","").replace(".root","")
    # make short name
    sname = "SVJ"+"_"+str(params["mZprime"])+"_"+str(params["mDark"])+"_"+str(params["rinv"])+"_"+str(params["alpha"])
    year = "MC2017" if "MC2017" in sample else "MC2018" if "MC2018" in sample else "MC2016"
    # make flat branch name
    found_nondefault = False
    for p in params_default:
        pv = params[p]
        if str(params_default[p]) != str(pv):
            if p=="mZprime": branches.append("Z"+str(int(float(pv)/100)))
            elif p=="mDark": branches.append("D"+str(pv))
            elif p=="rinv": branches.append("R"+str(int(float(pv)*10)))
            elif p=="alpha":
                pv2 = "Pk" if pv=="peak" else "Hi" if pv=="high" else "Lo" if pv=="low" else str(int(float(pv)*10))
                branches.append("A"+pv2)
            found_nondefault = True
            break
    if not found_nondefault: # default
        branches.append("Z"+str(int(float(params["mZprime"])/100)))
    # make lines
    dline1 = "\t".join(["hist", "mc", sname+"_"+year, "s:legname["+sname+"]","c:color["+colors[cc]+"]","i:linestyle["+str(styles[cs])+"]"])
    alphaval = alphavals[params["alpha"]] if params["alpha"] in alphavals else params["alpha"]
    plines = ["d:mZprime["+str(params["mZprime"])+"]","d:mDark["+str(params["mDark"])+"]","d:rinv["+str(params["rinv"])+"]","d:alpha["+str(alphaval)+"]"]
    if args.scan:
        plines.append("d:xsection["+str(get_xsec("SVJ",int(params["mZprime"]))[0])+"]")
    dline2 = "\t".join(["", "base", "mc", sname+"_"+year, "s:filename["+sample+"]"] + plines)
    dfile.write(dline1+"\n")
    dfile.write(dline2+"\n")
    # make skim line
    sline = "\t".join(["base", "skim", name, "s:filename["+sample+"]","b:data[0]","s:flatsuff["+sname+"_"+year+"]"] + plines)
    sfile.write(sline+"\n")
    # append to training lines
    numers.append(sname+"_"+year)
    
    # increment style counters
    cc += 1
    if cc>=len(colors):
        cc = 0
        cs += 1
        if cs>=len(styles):
            cs = 0

# write options
ifile.write("vs:flatnumers["+",".join(numers)+"]"+"\n")
ifile.write("vs:flatbranches["+",".join(branches)+"]"+"\n")
