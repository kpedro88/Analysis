import os
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-o", "--outdir", dest="outdir", default = "T1bbbb_1400_trig1", help="output directory", metavar="file")
parser.add_option("-f", "--file", dest="file", default = "/uscms_data/d3/pedrok/SUSY2015/analysis/CMSSW_7_3_1_patch2/src/Analysis/test/RA2bin_T1bbbb_1400_trig1.root", help="input file (full path, local or EOS) (default = %default)", metavar="file")
parser.add_option("-s", "--signal", dest="signal", default = "T1bbbb", help="SMS name", metavar="signal")
(options, args) = parser.parse_args()

from ROOT import *

if not os.path.exists(options.outdir): os.makedirs(options.outdir)

f = TFile.Open(options.file)
names = [k.GetName() for k in f.GetListOfKeys()]

sigs = []
sigs_mMom = []
sigs_mLSP = []
bkgs = []
data_obs = None

for n in names:
    parse=n.split('_')
    if parse[1]==options.signal: 
        sigs.append(n)
        sigs_mMom.append(float(parse[2]))
        sigs_mLSP.append(float(parse[3]))
    elif n=="data_obs": data_obs = f.Get(n)
    else: bkgs.append(n)

#make data_obs if not present
make_data_obs = False
if data_obs is None:
    make_data_obs = True
    print "Making data_obs from bkgs..."

#get bkg info
yield_bkg = []
for b in bkgs:
    h_bkg = f.Get(b)
    yield_bkg.append(h_bkg.Integral(0,h_bkg.GetNbinsX()+1))
    if make_data_obs:
        if data_obs is None:
            data_obs = h_bkg.Clone("data_obs")
            data_obs.SetDirectory(0)
        else:
            data_obs.Add(h_bkg)

# observed = sum bkg
obs_total = sum(yield_bkg)

#get sig info
yield_sig = []
for s in sigs:
    h_sig = f.Get(s)
    yield_sig.append(h_sig.Integral(0,h_sig.GetNbinsX()+1))

#figure out which mass is being varied
vary_mMom = True
if sigs_mMom[1:] == sigs_mMom[:-1]: vary_mMom = False
    
#insert the data_obs
f.Close()
if make_data_obs:
    f = TFile.Open(options.file,"UPDATE")
    data_obs.Write()
    f.Close()
    
#one datacard for each signal
outfiles = []
for i,s in enumerate(sigs):
    #make datacard
    dcfname = options.outdir+"/datacard_"+s+".txt"
    wsfname = options.outdir+"/limit_"+s+".root"
    dcfile = open(dcfname,'w')
    dcfile.write("imax 1\n")
    dcfile.write("jmax "+str(len(bkgs))+"\n")
    dcfile.write("kmax *\n")
    dcfile.write("---------------\n")
    dcfile.write("shapes * * "+options.file+" $PROCESS $PROCESS_$SYSTEMATIC\n")
    dcfile.write("---------------\n")
    dcfile.write("bin 1\n")
    dcfile.write("observation "+str(obs_total)+"\n")
    dcfile.write("---------------\n")
    dcfile.write("bin 1" + ''.join(" 1" for b in bkgs) + "\n")
    dcfile.write("process " + s + ''.join(" "+b for b in bkgs) + "\n")
    dcfile.write("process 0" + ''.join(" "+str(i+1) for i,b in enumerate(bkgs)) + "\n")
    dcfile.write("rate " + str(yield_sig[i]) + ''.join(" "+str(b) for b in yield_bkg) + "\n")
    dcfile.write("---------------\n")
    print "wrote "+dcfname
    dcfile.close()
    
    #run combine
    print "Calculating limit..."
    #no systematics (option -S 0)
    command = "combine -M Asymptotic -S 0 -m "+str(sigs_mMom[i] if vary_mMom else sigs_mLSP[i])+" -n "+wsfname+" "+dcfname
    print command
    os.system(command)
    
    outfiles.append(wsfname)
    
#combine outfiles
outname = "limit_"+options.outdir+".root"
command = "hadd "+outname+''.join(" "+ofn for ofn in outfiles)
print command
os.system(command)
    