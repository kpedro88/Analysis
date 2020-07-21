import sys, os, stat
from optparse import OptionParser

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="Spring16Fast", help="location of python files")
parser.add_option("-k", "--keepdict", dest="keep", default=False, action="store_true", help="keep dictionary file")
parser.add_option("-n", "--nfiles", dest="nfiles", default=0, help="number of files per block for scan input")
parser.add_option("-r", "--read", dest="read", default="input/dict_scan.py", help="input file to be generated for reading")
parser.add_option("-w", "--write", dest="write", default="input/input_sets_scan.txt", help="output file to write")
parser.add_option("-e", "--export", dest="export", default="batch/exportScan.sh", help="export file to write")
parser.add_option("-y", "--year", dest="year", default="", help="year (for MC suffix)")
parser.add_option("-i", "--identifier", dest="id", default="SMS", help="identifier for desired files")
(options, args) = parser.parse_args()

# find the python files
CMSSW_BASE = os.getenv("CMSSW_BASE")
dir = CMSSW_BASE + "/src/TreeMaker/Production/python/" + options.dir + "/"
files = os.listdir(dir)

wfname = options.read
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
yearsuff = "_MC"+options.year if len(options.year)>0 else ""
fdict = {}
for file in files:
    # skip unwanted things (including non-SMS)
    if file[-3:]=="pyc" or file[0:8]=="__init__" or (len(options.id)>0 and not file.startswith(options.id)): continue
    # add directory, remove "_cff.py"
    full_name = options.dir + "." + file[0:-7]
    # remove "_Tune..."
    fsplit = file.split('_')
    short_name = ""
    for fpart in fsplit:
        if "Tune" in fpart: break
        short_name += fpart + '_'
    # remove SMS-
    if "SMS" in options.id: short_name = short_name.replace("SMS-","")
    short_name = short_name[:-1]+yearsuff
    if short_name not in fdict: fdict[short_name] = [full_name]
    else: fdict[short_name].append(full_name)

if not options.keep:
    for key,val in sorted(fdict.iteritems()):
        # make dict for makeSkimInput.py
        wline = "[" + str(val) + ", ['" + key + "']],\n"        
        wfile.write(wline)

# close files
if not options.keep: 
    wfile.write("]\n")
    wfile.close()

# preamble
preamble = "OPTION\nstring:outsuff["+yearsuff+"_fast]\n"
# run makeSkimInput.py
from makeSkimInput import makeSkimInput
makeSkimInput(wfname,options.write,options.export,"scan",nfiles,preamble=preamble)
