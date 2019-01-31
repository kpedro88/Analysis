import sys, os
from optparse import OptionParser

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="", help="directory to search (LFN)")
parser.add_option("-g", "--grep", dest="grep", default="", help="string to grep in file list")
parser.add_option("-c", "--checks", dest="checks", default="block,part,fast,MC20", help="strings to remove from base name (comma-separated)")
(options, args) = parser.parse_args()

fileArray = filter(None,os.popen("xrdfs root://cmseos.fnal.gov/ ls "+options.dir+" | grep \""+options.grep+"\"").read().split('\n'))

#basename
fileArray = [ f.split("/")[-1] for f in fileArray]

#find sample names
checks = options.checks.replace(" ","").split(',')
fileArray = [ "_".join([ff for ff in f.split("_") if not any(c in ff for c in checks)]) for f in fileArray ]

#find unique samples
fileSet = set(fileArray)

print "\n".join(sorted(fileSet))
