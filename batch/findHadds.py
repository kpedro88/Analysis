import sys, os
from optparse import OptionParser

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="", help="directory to search (LFN)")
(options, args) = parser.parse_args()

fileArray = filter(None,os.popen("xrdfs root://cmseos.fnal.gov/ ls "+options.dir+" | grep \"_block\"").read().split('\n'))

#basename
fileArray = [ f.split("/")[-1] for f in fileArray]

#find sample names
fileArray = [ "_".join(f.split("_")[:-( ("block" in f)+("part" in f)+("fast" in f) )]) for f in fileArray ]

#find unique samples
fileSet = set(fileArray)

print "\n".join(sorted(fileSet))
