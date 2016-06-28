import sys, os, stat
from optparse import OptionParser

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="Spring16Fast", help="location of python files")
parser.add_option("-k", "--keepdict", dest="keep", default=False, action="store_true", help="keep dictionary file")
parser.add_option("-n", "--nfiles", dest="nfiles", default=0, help="number of files per block for scan input")
(options, args) = parser.parse_args()

# find the python files
CMSSW_BASE = os.getenv("CMSSW_BASE")
dir = CMSSW_BASE + "/src/TreeMaker/Production/python/" + options.dir + "/"
files = os.listdir(dir)

wfname = "input/dict_scan.py"
if not options.keep:
    wfile = open(wfname,'w')
    wfile.write("flist = [\n")
else:
    wname = wfname.replace(".py","").split("/")[-1]
    wfile = imp.load_source(wname,wfname)
    files = []
    for wline in wfile.flist:
        sample_name = wline[0][0].split('.')[1]
        files.append(sample_name+"_cff.py")

nfiles = int(options.nfiles)
for file in files:
    # skip unwanted things (including non-SMS)
    if file[-3:]=="pyc" or file[0:8]=="__init__" or file[0:3]!="SMS": continue
    # add directory, remove "_cff.py"
    full_name = options.dir + "." + file[0:-7]
    # remove "SMS-" and "_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_cff.py"
    short_name = file[4:-46]
    if not options.keep:
        # make dict for makeSkimInput.py
        wline = "[['" + full_name + "'], ['" + short_name + "']],\n"        
        wfile.write(wline)

# close files
if not options.keep: 
    wfile.write("]\n")
    wfile.close()

# run makeSkimInput.py
from makeSkimInput import makeSkimInput
makeSkimInput("input/dict_scan.py","input/input_sets_scan.txt","batch/exportScan.sh",nfiles)
