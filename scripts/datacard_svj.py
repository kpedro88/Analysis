import os
from collections import OrderedDict
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter

# make status messages useful
def fprint(msg):
    import sys
    print(msg)
    sys.stdout.flush()

def alpha_val(val):
    result = 0
    if val=="peak": result = -2
    elif val=="high": result = -1
    elif val=="low": result = -3
    else: result = float(val)
    return result

parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
parser.add_argument("-o", "--outdir", dest="outdir", default = "svj1", type=str, help="output directory")
parser.add_argument("-f", "--file", dest="file", type=str, default = "root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/trig3/sigfull/datacard.root", help="input file (full path, local or EOS)")
parser.add_argument("-s", "--signal", dest="signal", default = "SVJ", type=str, help="signal name")
parser.add_argument("-r", "--regions", dest="regions", default = ["highCut_2018","lowCut_2018"], type=str, nargs='+', help="list of regions")
parser.add_argument("-d", "--data", dest="data", default = False, action="store_true", help="use real data")
args = parser.parse_args()

with open('test/dict_xsec_Zprime.txt','r') as xfile:
    xsecs = {int(xline.split('\t')[0]): float(xline.split('\t')[1]) for xline in xfile}

from ROOT import *

f = TFile.Open(args.file)

if not os.path.exists(args.outdir): os.makedirs(args.outdir)
os.chdir(args.outdir)

outhists = []
outfiles = []
sigs_datacards = OrderedDict()
sigs_setargs = OrderedDict()
sigs_trkargs = OrderedDict()
for iregion,region in enumerate(args.regions):
    rdir = f.Get(region)
    names = [k.GetName() for k in rdir.GetListOfKeys()]
    sigs = OrderedDict()
    sigs_params = OrderedDict()
    sigs_yields = OrderedDict()
    bkgs = OrderedDict()
    bkgs_yields = OrderedDict()
    data_obs = None

    for n in names:
        parse=n.split('_')
        # skip syst
        if len(parse)>5: continue
        # skip summed bkg (and toy)
        if parse[0]=="Bkg" or parse[0]=="Bkg_toy": continue
        short_name = n
        if parse[0]==args.signal:
            # fix rinv 1.0
            if not "rinv0" in parse[3]: parse[3] = parse[3] + "0"
            sigs[short_name] = rdir.Get(n)
            sigs[short_name].SetName(short_name)
            sigs_params[short_name] = OrderedDict([
                ("xsec", xsecs[int(parse[1].replace("mZprime",""))]),
                ("mZprime", float(parse[1].replace("mZprime",""))),
                ("mDark", float(parse[2].replace("mDark",""))),
                ("rinv", float(parse[3].replace("rinv",""))/10.),
                ("alpha", alpha_val(parse[4].replace("alpha",""))),
            ])
            sigs_yields[short_name] = sigs[short_name].Integral(1,sigs[short_name].GetNbinsX())
        elif n=="data_obs":
            data_obs = rdir.Get(n)
            data_obs.SetName(short_name)
        else:
            bkgs[short_name] = rdir.Get(n)
            bkgs[short_name].SetName(short_name)
            bkgs_yields[short_name] = bkgs[short_name].Integral(1,bkgs[short_name].GetNbinsX())

    # make data_obs if not present
    if not args.data: data_obs = None
    if data_obs is None:
        fprint("Making data_obs from bkgs...")
        for bkg, h_bkg in bkgs.iteritems():
            if data_obs is None:
                data_obs = h_bkg.Clone("data_obs")
                data_obs.SetDirectory(0)
            else:
                data_obs.Add(h_bkg)

    # observed yield
    obs_total = data_obs.Integral(1,data_obs.GetNbinsX())

    # write histograms to new file
    outname = "hists_"+region+".root"
    outhists.append(outname)
    outfile = TFile.Open(outname,"RECREATE")
    for s in sigs:
        sigs[s].Write()
    for b in bkgs:
        bkgs[b].Write()
    data_obs.Write()
    outfile.Close()

    # one datacard for each signal
    bin = "b"+str(iregion)
    for sig in sigs:
        # make datacard
        dcfname = "datacard_"+bin+"_"+sig+".txt"
        if sig not in sigs_datacards:
            sigs_datacards[sig] = [dcfname]
        else:
            sigs_datacards[sig].append(dcfname)
        with open(dcfname,'w') as dcfile:
            dcfile.write("imax 1\n")
            dcfile.write("jmax "+str(len(bkgs))+"\n")
            dcfile.write("kmax *\n")
            dcfile.write("---------------\n")
            dcfile.write("shapes * * "+outname+" $PROCESS $PROCESS_$SYSTEMATIC\n")
            dcfile.write("---------------\n")
            dcfile.write("bin "+bin+"\n")
            dcfile.write("observation "+str(obs_total)+"\n")
            dcfile.write("---------------\n")
            dcfile.write("bin " + bin + ''.join(" "+bin for b in bkgs) + "\n")
            dcfile.write("process " + sig + ''.join(" "+b for b in bkgs) + "\n")
            dcfile.write("process 0" + ''.join(" "+str(i+1) for i,b in enumerate(bkgs)) + "\n")
            dcfile.write("rate " + str(sigs_yields[sig]) + ''.join(" "+str(bkgs_yields[b]) for b in bkgs_yields) + "\n")
            dcfile.write("---------------\n")
            # nominal systematic of 10%
            dcfile.write("syst lnN " + '1.1' + ''.join(" "+'1.1' for b in bkgs_yields) + "\n")
            dcfile.write("---------------\n")
            # set sig params
            sigs_setargs[sig] = []
            sigs_trkargs[sig] = []
            for p,v in sigs_params[sig].iteritems():
                dcfile.write(p+" extArg "+str(v)+"\n")
                sigs_setargs[sig].append(p+"="+str(v))
                sigs_trkargs[sig].append(p)
            dcfile.write("---------------\n")
            fprint("wrote "+dcfname)

for sig,datacards in sigs_datacards.iteritems():
    setargs = sigs_setargs[sig]
    trkargs = sigs_trkargs[sig]
    dcfname = "datacard_"+sig+".txt"
    # combine cards
    command = "combineCards.py "+" ".join(datacards)+" > "+dcfname
    fprint(command)
    os.system(command)
    # run combine
    fprint("Calculating limit...")
    # no systematics (option -S 0)
    command = "combine -M Asymptotic -S 0 --setParameters "+','.join(setargs)+" --freezeParameters "+','.join(trkargs)+" --trackParameters "+','.join(trkargs)+" --keyword-value sig="+sig+" "+dcfname+" > /dev/null"
    fprint(command)
    os.system(command)
    
    outfiles.append("higgsCombineTest.Asymptotic.mH120.sig"+sig+".root")
    
#combine outfiles
outname = "limit_"+args.outdir+".root"
command = "hadd -f "+outname+''.join(" "+ofn for ofn in outfiles)
fprint(command)
os.system(command)
os.system("mv "+outname+" ../")
