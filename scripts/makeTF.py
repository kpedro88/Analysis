import sys
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("-n","--numer",dest="numer",type=str,required=True,help="numer file")
parser.add_argument("-d","--denom",dest="denom",type=str,required=True,help="denom file")
parser.add_argument("-r","--region",dest="region",type=str,required=True,help="region (directory in files)")
parser.add_argument("-s","--sample",dest="sample",type=str,required=True,help="sample")
args = parser.parse_args()

from ROOT import *
import math

f_numer = TFile.Open(args.numer)
r_numer = f_numer.Get(args.region)
f_denom = TFile.Open(args.denom)
r_denom = f_denom.Get(args.region)

h_numer = r_numer.Get(args.sample)
h_denom = r_denom.Get(args.sample)

h_tf = h_numer.Clone()
h_tf.Divide(h_denom)
h_tf.Fit("pol0")
h_tf.GetYaxis().SetRangeUser(0,min(10,h_tf.GetMaximum()))

h_tf.Draw()
