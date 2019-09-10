from collections import OrderedDict
import sys

def parseLog(fname,bkgs):
    with open(fname,'r') as file:
        for line in file:
            line = line.rstrip()
            for bkg in bkgs:
                index = line.find(bkg+":")
                if index>=0:
                    bkgs[bkg].append(line[index+len(bkg)+2:])
                    break

if __name__=="__main__":
    configs = sys.argv[1].split(',')
    suff = sys.argv[2]
    for MVA in ["BDT"]:
#        with open("bkg_yields_"+MVA+"_D3X_maxbvsall.log",'w') as outfile:
        with open("bkg_yields_"+MVA+"_"+suff+".log",'w') as outfile:
            bkgs = OrderedDict([
                ("QCD",[]),
                ("ttbar",[]),
                ("wjets",[]),
                ("zinv",[]),
                ("bkg",[]),
            ])
#            for CONFIG in ["D30","D39","D33","D40","D35","D41"]:
#            for CONFIG in ["D30","D39","D42","UZH2"]:
            for CONFIG in configs:
                logname = "test/log_mt_S-"+MVA+"2_"+CONFIG+".log"
                parseLog(logname,bkgs)
            
            for bkg in bkgs:
                outfile.write(bkg+" "+" ".join(bkgs[bkg])+"\n")
