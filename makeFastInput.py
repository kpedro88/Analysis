import sys, os, stat
from optparse import OptionParser
from makeFastCommon import *

def xsec_parse(xfile):
    xsec = {}
    for xline in xfile:
        values = xline.split('\t')
        if len(values) < 2: continue
        xsec[int(values[0])] = float(values[1])
    return xsec

def msplit(line):
    split = line.split('-')
    if len(split)==2:
        return split[1] if split[1].isalpha() else int(split[1]) if not "." in split[1] else float(split[1])
    else: return -1

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV8/scan/", help="location of root files (LFN)")
parser.add_option("-k", "--skip", dest="skip", default="", help="topologies to skip (comma-separated)")
parser.add_option("-m", "--match", dest="match", default="", help="pick only files matching this string")
parser.add_option("-y", "--year", dest="year", default="", help="year name for output files (if not default)")
parser.add_option("-s", "--skim", dest="skim", default="input/input_sets_skim_fast.txt", help="skim output file to write")
parser.add_option("-c", "--card", dest="card", default="input/input_sets_DC_fast.txt", help="datacard output file to write")
parser.add_option("-e", "--export", dest="export", default="batch/exportFast.sh", help="export file to write")
parser.add_option("-p", "--params", dest="params", default=2, help="number of signal params")
parser.add_option("-f", "--full", dest="full", default=False, action="store_true", help="fullsim (rather than fastsim)")
parser.add_option("-x", "--extra-params", dest="extra", default=2, help="number of extra params")
(options, args) = parser.parse_args()

# parse skip list
skiplist = options.skip.split(",") if len(options.skip)>0 else []

# find the root files
files = filter(None,os.popen("xrdfs root://cmseos.fnal.gov/ ls "+options.dir).read().split('\n'))
# basename
files = [ f.split("/")[-1] for f in files]
# match
if len(options.match)>0:
    files = [f for f in files if options.match in f]

# open files
wfile = open(options.skim,'w')
dfile = open(options.card,'w')
sfile = open(options.export,'w')

# preamble for script
sfile.write("#!/bin/bash\n")
sfile.write("\n")
sfile.write("export SAMPLES=(\n")

# preamble for input files
wfile.write("SET\n")
dfile.write("SET\n")

for file in files:
    # check skiplist
    skip = False
    for topo in skiplist:
        if topo in file:
            skip = True
            break
    if skip: continue
    # parse filename: model, mMother-X, mLSP-Y, MC####, fast.root
    #             or: model, mZprime-X, mDark-Y, rinv-Z, alpha-W, MC####, scan.root
    splitnum = int(options.params) + int(options.extra)
    fsplit = file.split('_')
    model = '_'.join(fsplit[0:-splitnum])
    params = []
    while splitnum>int(options.extra):
        params.append(msplit(fsplit[-splitnum]))
        splitnum -= 1
    year = options.year if len(options.year)>0 else fsplit[-splitnum]
    fyear = fsplit[-splitnum]
    mother_ID = []
    # get cross section
    this_xsec, mother_ID = get_xsec(model,params[0])
    # make short name
    param_string = "_".join([str(p) for p in params])
    short_name = model + "_" + param_string + "_" + year + ("" if options.full else "_" + "fast")
    # make dc input file name (maybe different from short name)
    fname = model + "_" + param_string + "_" + fyear + ("" if options.full else "_" + "fast")
    # make set list for skimming
    wline = "base" + "\t" + "skim" + "\t" + short_name + "\t" + "s:filename[" + file + "]" + ("" if options.full else "\t" + "b:fastsim[1]") + "\t" + "vi:mother[" + ','.join([str(m) for m in mother_ID]) + "]" + "\t" + "b:data[0]" + "\n"
    wfile.write(wline)
    # make set list for datacards with xsecs
    dline = makeLineDCHist(short_name)
    dline += makeLineDCBase(short_name,fname,this_xsec,mother_ID,options.full)
    dfile.write(dline)
    # make script to export array of sample names
    sline = short_name + " \\\n"
    sfile.write(sline)
    
sfile.write(")\n")

# make the script executable
st = os.stat(sfile.name)
os.chmod(sfile.name, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
