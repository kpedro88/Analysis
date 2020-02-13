import sys
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("-f","--file",dest="file",type=str,required=True,help="input file")
parser.add_argument("-d","--dir",dest="dir",type=str,default="",help="dir in file if any")
parser.add_argument("-b","--bkg",dest="bkg",type=str,required=True,help="name of background")
parser.add_argument("-s","--sig",dest="sig",type=str,required=True,help="name of signal")
args = parser.parse_args()

from ROOT import *
import math

file = TFile.Open(args.file)
dir = file.Get(args.dir) if len(args.dir)>0 else file

hsig = dir.Get(args.sig)
hbkg = dir.Get(args.bkg)

mZprime = float(args.sig.split('_')[1].replace("mZprime",""))
b1 = hsig.FindBin(mZprime-1000)
b2 = hsig.FindBin(mZprime+1000)
S = hsig.Integral(b1,b2)
B = hbkg.Integral(b1,b2)

if B==0: sys.exit(0)

FOM = math.sqrt(2*((S+B)*math.log(1+S/B)-S))

print "\t".join(["{:.2f}".format(x) for x in [S, B, S/B*100, FOM]])
