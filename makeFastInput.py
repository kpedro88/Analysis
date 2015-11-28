import sys, os, stat
from optparse import OptionParser

def msplit(line):
    split = line.split('-')
    if len(split)==2: return int(split[1])
    else: return -1

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV4/scan/", help="location of python files")
parser.add_option("-n", "--nfiles", dest="nfiles", default=0, help="number of files per part for datacard input")
parser.add_option("-m", "--mother", dest="mother", default=1000021, help="mother particle PDG ID")
(options, args) = parser.parse_args()

# find the python files
files = os.listdir(options.dir)

# open files
xfile = open("input/dict_xsec.txt",'r')
wfile = open("input/input_sets_skim_fast.txt",'w')
dfile = open("input/input_sets_DC_fast.txt",'w')
sfile = open("batch/exportFast.sh",'w')

nfiles = int(options.nfiles)
if nfiles>0: 
    nparts = len(files)/nfiles + int(len(files)%nfiles!=0)
    dfiles = [open("input/fast/input_sets_DC_fast_"+str(x)+".txt",'w') for x in range(nparts)]
    #preamble
    for df in dfiles:
        df.write("SET\n")

# parse xsec map (taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu)
xsec = {}
for xline in xfile:
    values = xline.split('\t')
    if len(values) < 2: continue
    xsec[int(values[0])] = float(values[1])

# preamble for script
sfile.write("#!/bin/bash\n")
sfile.write("\n")
sfile.write("export SAMPLES=(\n")

# preamble for input files
wfile.write("SET\n")
dfile.write("SET\n")

for ind,file in enumerate(files):
    # parse filename: model, mMother-X, mLSP-Y, fast.root
    fsplit = file.split('_')
    mMother = msplit(fsplit[1])
    mLSP = msplit(fsplit[2])
    # make short name
    short_name = fsplit[0] + "_" + str(mMother) + "_" + str(mLSP) + "_" + "fast"
    # make set list for skimming
    wline = "base" + "\t" + "skim" + "\t" + short_name + "\t" + "s:filename[" + file + "]" + "\t" + "b:fastsim[1]" + ("\t"+"i:mother["+str(options.mother)+"]" if options.mother > 0 else "") + "\n"
    wfile.write(wline)
    # make set list for datacards with xsecs
    dline = "hist" + "\t" + "mc" + "\t" + short_name + "\n"
    dline += "\t" + "base" + "\t" + "mc" + "\t" + short_name + "\t" + "s:filename[tree_" + short_name + ".root]" + "\t" + "d:xsection[" + str(xsec[mMother]) + "]" + "\t" + "b:signal[1]" + "\t" + "b:fastsim[1]" + ("\t"+"i:mother["+str(options.mother)+"]" if options.mother > 0 else "") + "\n"
    dfile.write(dline)
    # make split set lists
    if nfiles>0:
        dfiles[ind/nfiles].write(dline)
    # make script to export array of sample names
    sline = short_name + " \\\n"
    sfile.write(sline)

sfile.write(")\n")

# make the script executable
st = os.stat(sfile.name)
os.chmod(sfile.name, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
