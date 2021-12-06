import sys, os, subprocess

#default: print bin contents instead of modifying
findBins = len(sys.argv)<2

from collections import OrderedDict
import ROOT as r

def check_code(cmd,throw=True):
    FNULL = open(os.devnull, 'w')
    exitcode = subprocess.Popen(cmd, stdout=FNULL, stderr=FNULL).wait()
    if throw and exitcode!=0:
        raise RuntimeError("Command failed: {}".format(' '.join(cmd)))
    return exitcode

def getHistName(fname,qty,sel):
    year = fname.split('_')[3]
    return "{1}_QCD_MC{0}_{2}{0}".format(year,qty,sel)

redir = "root://cmseos.fnal.gov/"
lfn = "/store/user/pedrok/SVJ2017/Nm1/hist/"
dir = redir+lfn
orig = "orig/"
tmp = "test/"

qtys = OrderedDict([
    ("metMTratio","preselMinusRT"),
    ("deltaphiminAK8","presel"),
    ("deltaetaAK8","preselMinusDeta"),
    ("met","preselMinusRT"),
])

files = OrderedDict([
    ("svj_Nm1_bkg_2017_part2.root", {"metMTratio": [81], "deltaphiminAK8": [81], "deltaetaAK8": [75], "met": [61]}),
    ("svj_Nm1_bkg_2017_part3.root", {"met": [69], "deltaetaAK8": [83]}),
    ("svj_Nm1_bkg_2018_part2.root", {"deltaetaAK8": [68]}),
    ("svj_Nm1_bkg_2018_part3.root", {"met": [79,85], "deltaetaAK8": [84]}),
])

for f, h in files.iteritems():
    # check for backup copies
    exitcode = check_code(["eos",redir,"ls",lfn+orig+f],throw=False)
    if exitcode!=0:
        # make backup copy
        check_code(["xrdcp",redir+lfn+f,redir+lfn+orig+f])

    # make a local copy from backup copy that will be modified
    check_code(["xrdcp","-f",redir+lfn+orig+f,tmp+f])

    file = r.TFile.Open(tmp+f,"UPDATE")
    for q,bins in h.iteritems():
        hist = file.Get(getHistName(f,q,qtys[q]))
        if findBins:
            print f,q
            hist.Print("all")
        else:
            for b in bins:
                hist.SetBinContent(b,0)
                hist.SetBinError(b,0)
    if not findBins:
        file.Write("",r.TObject.kOverwrite)
        # copy back
        check_code(["xrdcp","-f",tmp+f,redir+lfn+f])
