import sys, os, stat
from optparse import OptionParser

def sig_callback(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))

def wt_callback(option, opt, value, parser):
    if value is None: return
    setattr(parser.values, option.dest, [float(x) for x in value.split(',')])

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
parser.add_option("-s", "--signals", dest="signals", type='string', action='callback', callback=sig_callback, help="list of signal model names")
parser.add_option("-w", "--weights", dest="weights", type='string', action='callback', callback=wt_callback, help="list of signal model weights")
parser.add_option("-x", "--suffix", dest="suffix", default="Mixed", help="suffix for output files")
(options, args) = parser.parse_args()

# check the signal models
if len(options.signals)==0:
    print "Input error: no signal models specified!"
    sys.exit()
if options.weights is None:
    print "Using equal weight for all signal models"
    options.weights = [1/float(len(options.signals)) for x in options.signals]
elif len(options.weights)!=len(options.signals):
    print "Input error: length of signal names != length of signal weights"
    sys.exit()
else:
    #normalize weights
    sumwt = sum(options.weights)
    options.weights = [x/sumwt for x in options.weights]

# find the python files
files = filter(None,os.popen("xrdfs root://cmseos.fnal.gov/ ls "+options.dir).read().split('\n'))
# basename
files = [ f.split("/")[-1] for f in files]

# open files
xfile = open("input/dict_xsec.txt",'r')
xfileT2 = open("input/dict_xsec_T2.txt",'r')
xfileT2qq = open("input/dict_xsec_T2qq.txt",'r')
# only need to make datacard input file (use existing skims)
dfile = open("input/input_sets_DC_fast"+options.suffix+".txt",'w')
sfile = open("batch/exportFast"+options.suffix+".sh",'w')

# parse xsec map (taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu)
xsec = xsec_parse(xfile)

# parse xsec map (taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVstopsbottom)
xsecT2 = xsec_parse(xfileT2)

# parse xsec map (taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVsquarkantisquark)
xsecT2qq = xsec_parse(xfileT2qq)
    
# preamble for script
sfile.write("#!/bin/bash\n")
sfile.write("\n")
sfile.write("export SAMPLES=(\n")

# preamble for input files
dfile.write("SET\n")

# dictionary of mass points
masspts = {}

for ind,file in enumerate(files):
    # parse filename: model, mMother-X, mLSP-Y, fast.root
    fsplit = file.split('_')
    model = fsplit[0]
    mMother = msplit(fsplit[1])
    mLSP = msplit(fsplit[2])
    if not model in options.signals: continue
    
    mlist = masspts.get((mMother, mLSP),[])
    mlist.append(model)
    masspts[(mMother, mLSP)] = mlist
    
present_list = []
missing_list = []
dline_list = []
for masspt, models in masspts.iteritems():
    #print "%s: "%(masspt,) + str(models).strip('[]')
    mMother = masspt[0]
    mLSP = masspt[1]
    # make short name
    short_name_comb = options.suffix + "_" + str(mMother) + "_" + str(mLSP) + "_" + "fast"
    # make set list for datacards with xsecs
    dline = "hist" + "\t" + "mc" + "\t" + short_name_comb + "\n"
    
    if len(models)!=len(options.signals):
        missing_list.append(masspt)
        continue
    else:
        present_list.append(masspt)
    
    for model in models:
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
            mother_ID.extend((1000001,1000002,1000003,1000004,2000001,2000002,2000003,2000004))
        else:
            this_xsec = xsec[mMother] if mMother in xsec.keys() else 1
            mother_ID.append(1000021)
        # multiply xsec by weight
        this_xsec *= options.weights[options.signals.index(model)]
        # make short name
        short_name = model + "_" + str(mMother) + "_" + str(mLSP) + "_" + "fast"
        # make base list for datacards with xsec
        dline += "\t" + "base" + "\t" + "mc" + "\t" + short_name + "\t" + "s:filename[tree_" + short_name + ".root]" + "\t" + "d:xsection[" + str(this_xsec) + "]" + "\t" + "b:signal[1]" + "\t" + "b:fastsim[1]" + "\t" + "vi:mother[" + str(','.join(str(m) for m in mother_ID)) + "]" + "\n"
        
    dfile.write(dline)
    dline_list.append(dline)
    # make script to export array of sample names
    sline = short_name_comb + " \\\n"
    sfile.write(sline)

sfile.write(")\n")

#number of sets unknown until the end
nfiles = int(options.nfiles)
if nfiles>0:
    # check for directory
    if not os.path.isdir("input/fast"+options.suffix):
        os.mkdir("input/fast"+options.suffix)
    nparts = len(dline_list)/nfiles + int(len(dline_list)%nfiles!=0)
    dfiles = [open("input/fast"+options.suffix+"/input_sets_DC_fast"+options.suffix+"_"+str(x)+".txt",'w') for x in range(nparts)]
    # preamble
    for df in dfiles:
        df.write("SET\n")
    # make split set lists
    for ind,dline in enumerate(dline_list):
        dfiles[ind/nfiles].write(dline)

# report on missing mass points
print "Mass points present for  all signal models: %s" % (len(present_list))
print "Mass points missing for some signal models: %s" % (len(missing_list))

# make the script executable
st = os.stat(sfile.name)
os.chmod(sfile.name, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
