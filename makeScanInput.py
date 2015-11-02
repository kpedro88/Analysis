import sys, os, stat
from optparse import OptionParser

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="Spring15Fastv2", help="location of python files")
(options, args) = parser.parse_args()

# find the python files
CMSSW_BASE = os.getenv("CMSSW_BASE")
dir = CMSSW_BASE + "/src/TreeMaker/Production/python/" + options.dir + "/"
files = os.listdir(dir)

wfile = open("input/dict_scan.txt",'w')
sfile = open("batch/exportScan.sh",'w')

# preamble for script
sfile.write("#!/bin/bash\n")
sfile.write("\n")
sfile.write("export SAMPLES=(\n")

for file in files:
    # skip unwanted things
    if file[-3:]=="pyc" or file[0:8]=="__init__": continue
    # add directory, remove "_cff.py"
    full_name = options.dir + "." + file[0:-7]
    # remove "SMS-" and "_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_cff.py"
    short_name = file[4:-46]
    # make dict for makeSkimInput.py
    wline = full_name + "\t" + short_name + "\n"
    wfile.write(wline)
    # make script to export array of sample names
    sline = short_name + " \\\n"
    sfile.write(sline)

sfile.write(")\n")

# make the script executable
st = os.stat(sfile.name)
os.chmod(sfile.name, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

# close files
wfile.close()
sfile.close()

# run makeSkimInput.py
from makeSkimInput import makeSkimInput
makeSkimInput("input/dict_scan.txt","input/input_sets_scan.txt")
