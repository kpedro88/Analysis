import sys, os, stat
from optparse import OptionParser
from makeFastCommon import *

def sig_callback(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))

def wt_callback(option, opt, value, parser):
    if value is None: return
    setattr(parser.values, option.dest, [float(x) for x in value.split(',')])

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

# special list for T1ttbb
special_model_base = "T1ttbb"
special_models = ["T1bbtt","T1tbtb","T1tbbb","T1tbtt"]
has_special_model = not set(special_models).isdisjoint(options.signals)
    
# find the python files
files = filter(None,os.popen("xrdfs root://cmseos.fnal.gov/ ls "+options.dir).read().split('\n'))
# basename
files = [ f.split("/")[-1] for f in files]

# only need to make datacard input file (use existing skims)
dfile = open("input/input_sets_DC_fast"+options.suffix+".txt",'w')
sfile = open("batch/exportFast"+options.suffix+".sh",'w')

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
    model = '_'.join(fsplit[0:-3])
    mMother = msplit(fsplit[-3])
    mLSP = msplit(fsplit[-2])
    is_special_model = (has_special_model and model==special_model_base)
    if not ((model in options.signals) or is_special_model): continue
    
    for model_ in (special_models if is_special_model else [model]):
        mlist = masspts.get((mMother, mLSP),[])
        mlist.append(model)
        masspts[(mMother, mLSP)] = mlist
    
present_list = []
missing_list = []
dline_list = []
for masspt, models in sorted(masspts.iteritems()):
    #print "%s: "%(masspt,) + str(models).strip('[]')
    mMother = masspt[0]
    mLSP = masspt[1]
    # make short name
    short_name_comb = options.suffix + "_" + str(mMother) + "_" + str(mLSP) + "_" + "fast"
    # make set list for datacards with xsecs
    dline = makeLineDCHist(short_name_comb)
    
    if len(models)!=len(options.signals):
        missing_list.append(masspt)
        continue
    else:
        present_list.append(masspt)
    
    for model in models:
        # get cross section
        this_xsec, mother_ID = get_xsec(model,mMother)
        # multiply xsec by weight
        this_xsec *= options.weights[options.signals.index(model)]
        # make short name
        short_name = model + "_" + str(mMother) + "_" + str(mLSP) + "_" + "fast"
        # make base list for datacards with xsec
        is_special_model = model in special_models
        basetmp = makeLineDCBase(short_name,this_xsec,mother_ID,(model if is_special_model else ""))
        if is_special_model: basetmp = basetmp.replace("tree_"+short_name,"tree_"+short_name.replace(model,special_model_base))
        dline += basetmp

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
    dfiles = ["input/fast"+options.suffix+"/input_sets_DC_fast"+options.suffix+"_"+str(x)+".txt" for x in range(nparts)]
    # preamble
    for df in dfiles:
        with open(df,'w') as dftmp:
            dftmp.write("SET\n")
    # make split set lists
    dftmp = None
    curr_ind = -1
    for ind,dline in enumerate(dline_list):
        if ind/nfiles != curr_ind:
            if curr_ind > -1: dftmp.close()
            curr_ind = ind/nfiles
            dftmp = open(dfiles[curr_ind],'a')
        dftmp.write(dline)
    dftmp.close()

# report on missing mass points
print "Mass points present for  all signal models: %s" % (len(present_list))
print "Mass points missing for some signal models: %s" % (len(missing_list))

# make the script executable
st = os.stat(sfile.name)
os.chmod(sfile.name, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
