import os
from collections import OrderedDict
from optparse import OptionParser
parser = OptionParser()
parser.add_option("-o", "--outdir", dest="outdir", default = "svj1", help="output directory (default = %default)")
parser.add_option("-f", "--file", dest="file", default = "/uscms_data/d3/pedrok/SUSY2015/analysis/CMSSW_7_4_15/src/Limit/test/MTAK8_dijetmtdetahad_fullnewmfbdt.root", help="input file (full path, local or EOS) (default = %default)")
parser.add_option("-s", "--signal", dest="signal", default = "SVJ", help="signal name (default = %default)")
(options, args) = parser.parse_args()

with open('test/dict_xsec_Zprime.txt','r') as xfile:
    xsecs = {int(xline.split('\t')[0]): float(xline.split('\t')[1]) for xline in xfile}

from ROOT import *

f = TFile.Open(options.file)
names = [k.GetName() for k in f.GetListOfKeys()]

sigs = []
sigs_params = []
bkgs = []
data_obs = None

for n in names:
    parse=n.split('_')
    if parse[1]==options.signal: 
        sigs.append(n)
    elif n=="data_obs": data_obs = f.Get(n)
    else: bkgs.append(n)
sigs = sorted(sigs)
for s in sigs:
    parse=s.split('_')
    sigs_params.append(OrderedDict([
        ("xsec", xsecs[int(parse[2])]),
        ("mZprime", float(parse[2])),
        ("mDark", float(parse[3])),
        ("rinv", float(parse[4])),
        ("alpha", float(parse[5])),
    ]))

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

#insert the data_obs
f.Close()
if make_data_obs:
    f = TFile.Open(options.file,"UPDATE")
    data_obs.Write()
    f.Close()
    
if not os.path.exists(options.outdir): os.makedirs(options.outdir)
os.chdir(options.outdir)

#one datacard for each signal
outfiles = []
for i,s in enumerate(sigs):
    #make datacard
    dcfname = "datacard_"+s+".txt"
    dcfile = open(dcfname,'w')
    dcfile.write("imax 1\n")
    dcfile.write("jmax "+str(len(bkgs))+"\n")
    dcfile.write("kmax *\n")
    dcfile.write("---------------\n")
    dcfile.write("shapes * * "+options.file+" $PROCESS $PROCESS_$SYSTEMATIC\n")
    dcfile.write("---------------\n")
    dcfile.write("bin b1\n")
    dcfile.write("observation "+str(obs_total)+"\n")
    dcfile.write("---------------\n")
    dcfile.write("bin b1" + ''.join(" b1" for b in bkgs) + "\n")
    dcfile.write("process " + s + ''.join(" "+b for b in bkgs) + "\n")
    dcfile.write("process 0" + ''.join(" "+str(i+1) for i,b in enumerate(bkgs)) + "\n")
    dcfile.write("rate " + str(yield_sig[i]) + ''.join(" "+str(b) for b in yield_bkg) + "\n")
    dcfile.write("---------------\n")
    # nominal systematic of 10%
    dcfile.write("syst lnN " + '1.1' + ''.join(" "+'1.1' for b in yield_bkg) + "\n")
    dcfile.write("---------------\n")
    # set sig params
    setargs = []
    trkargs = []
    for p,v in sigs_params[i].iteritems():
        dcfile.write(p+" extArg "+str(v)+"\n")
        setargs.append(p+"="+str(v))
        trkargs.append(p)
    dcfile.write("---------------\n")
    print "wrote "+dcfname
    dcfile.close()
    
    #run combine
    print "Calculating limit..."
    #no systematics (option -S 0)
    command = "combine -M Asymptotic -S 0 --setParameters "+','.join(setargs)+" --freezeParameters "+','.join(trkargs)+" --trackParameters "+','.join(trkargs)+" --keyword-value sig="+s+" "+dcfname+" > /dev/null"
    print command
    os.system(command)
    
    outfiles.append("higgsCombineTest.Asymptotic.mH120.sig"+s+".root")
    
#combine outfiles
outname = "limit_"+options.outdir+".root"
command = "hadd -f "+outname+''.join(" "+ofn for ofn in outfiles)
print command
os.system(command)
os.system("mv "+outname+" ../")
