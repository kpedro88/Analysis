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
    if len(split)==2: return int(split[1])
    else: return -1

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV8/scan/", help="location of root files (LFN)")
parser.add_option("-k", "--skip", dest="skip", default="", help="topologies to skip (comma-separated)")
parser.add_option("-m", "--match", dest="match", default="", help="pick only files matching this string")
parser.add_option("-s", "--skim", dest="skim", default="input/input_sets_skim_fast.txt", help="skim output file to write")
parser.add_option("-c", "--card", dest="card", default="input/input_sets_DC_fast.txt", help="datacard output file to write")
parser.add_option("-e", "--export", dest="export", default="batch/exportFast.sh", help="export file to write")
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
    # parse filename: model, mMother-X, mLSP-Y, [MC####], fast.root
    fsplit = file.split('_')
    model = '_'.join(fsplit[0:-3])
    year = fsplit[-4]
    mMother = msplit(fsplit[-3])
    mLSP = msplit(fsplit[-2])
    mother_ID = []
    # get cross section
    this_xsec, mother_ID = get_xsec(model,mMother)
    # make short name
    short_name = model + "_" + str(mMother) + "_" + str(mLSP) + ("_" + year if len(fsplit)>4 else "") + "_" + "fast"
    # make set list for skimming
    wline = "base" + "\t" + "skim" + "\t" + short_name + "\t" + "s:filename[" + file + "]" + "\t" + "b:fastsim[1]" + "\t" + "vi:mother[" + str(','.join(str(m) for m in mother_ID)) + "]" + "\t" + "b:data[0]" + "\n"
    wfile.write(wline)
    # make set list for datacards with xsecs
    dline = makeLineDCHist(short_name)
    dline += makeLineDCBase(short_name,this_xsec,mother_ID)
    dfile.write(dline)
    # make per-set list
    with open("input/fast/input_set_DC_"+short_name+".txt",'w') as ofile:
        ofile.write("SET\n")
        ofile.write(dline)
    # make script to export array of sample names
    sline = short_name + " \\\n"
    sfile.write(sline)
    
sfile.write(")\n")

# make the script executable
st = os.stat(sfile.name)
os.chmod(sfile.name, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
