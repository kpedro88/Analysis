import sys, os, stat
from optparse import OptionParser

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
parser.add_option("-n", "--nfiles", dest="nfiles", default=0, help="number of files per part for datacard input")
(options, args) = parser.parse_args()

# find the root files
files = filter(None,os.popen("xrdfs root://cmseos.fnal.gov/ ls "+options.dir).read().split('\n'))
# basename
files = [ f.split("/")[-1] for f in files]

# open files
xfile = open("input/dict_xsec.txt",'r')
xfileT2 = open("input/dict_xsec_T2.txt",'r')
xfileT2qq = open("input/dict_xsec_T2qq.txt",'r')
xfileTChiHH = open("input/dict_xsec_TChiHH.txt",'r')
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
xsec = xsec_parse(xfile)

# parse xsec map (taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVstopsbottom)
xsecT2 = xsec_parse(xfileT2)

# parse xsec map (taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVsquarkantisquark)
xsecT2qq = xsec_parse(xfileT2qq)

# parse xsec map (taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVhino)
xsecTChiHH = xsec_parse(xfileTChiHH)

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
    model = '_'.join(fsplit[0:-3])
    mMother = msplit(fsplit[-3])
    mLSP = msplit(fsplit[-2])
    mother_ID = []
    # get cross section
    if model.find("T2tt")!=-1:
        this_xsec = xsecT2[mMother] if mMother in xsecT2.keys() else 1
        mother_ID.append(1000006)
    elif model.find("T2bb")!=-1:
        this_xsec = xsecT2[mMother] if mMother in xsecT2.keys() else 1
        mother_ID.append(1000005)
    elif model.find("T2qq")!=-1:
        this_xsec = xsecT2qq[mMother] if mMother in xsecT2qq.keys() else 1
        mother_ID.extend([1000001,1000002,1000003,1000004,2000001,2000002,2000003,2000004])
    elif model.find("TChiHH")!=-1:
        this_xsec = xsecTChiHH[mMother] if mMother in xsecTChiHH.keys() else 1
        mother_ID.extend([1000023,1000025])
    else:
        this_xsec = xsec[mMother] if mMother in xsec.keys() else 1
        mother_ID.append(1000021)
    # make short name
    short_name = model + "_" + str(mMother) + "_" + str(mLSP) + "_" + "fast"
    # make set list for skimming
    wline = "base" + "\t" + "skim" + "\t" + short_name + "\t" + "s:filename[" + file + "]" + "\t" + "b:fastsim[1]" + "\t" + "vi:mother[" + str(','.join(str(m) for m in mother_ID)) + "]" + "\n"
    wfile.write(wline)
    # make set list for datacards with xsecs
    dline = "hist" + "\t" + "mc" + "\t" + short_name + "\n"
    dline += "\t" + "base" + "\t" + "mc" + "\t" + short_name + "\t" + "s:filename[tree_" + short_name + ".root]" + "\t" + "d:xsection[" + str(this_xsec) + "]" + "\t" + "b:signal[1]" + "\t" + "b:fastsim[1]" + "\t" + "vi:mother[" + str(','.join(str(m) for m in mother_ID)) + "]" + "\n"
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
