import sys

fname = sys.argv[1]
obs = len(sys.argv)>2
inter_npoints = int(sys.argv[3]) if len(sys.argv)>3 else 10

import ROOT as r
import numpy as np

def find_crossing(array, value):
    array = np.asarray(array)
    array = array - value
    across = (array < 0)
    idx = across.argmax() if across.any() else -1
    if idx==-1: return idx
    # check if previous or next point crosses zero and is closer to zero
    other_idx = []
    if idx>0 and array[idx-1]>0: other_idx.append(idx-1)
    if len(array)>idx+1 and array[idx+1]>0: other_idx.append(idx+1)
    for idx2 in other_idx:
        if abs(array[idx2]) < abs(array[idx]): idx = idx2
    if len(other_idx)==0: idx = -1
    return idx

# get graphs
file = r.TFile.Open(fname)
g_ex = file.Get("g_obs" if obs else "g_central")
g_th = file.Get("g_xsec")

# get points
n = g_ex.GetN()
x = g_ex.GetX()
y_ex = g_ex.GetY()
y_th = g_th.GetY()

if inter_npoints >= 1:
    # convert to log scale
    logy_ex = np.log10(y_ex)
    logy_th = np.log10(y_th)
    
    # new graphs for interp
    g_ex_interp = r.TGraph(n,x,logy_ex)
    g_th_interp = r.TGraph(n,x,logy_th)
    
    x_interp = []
    logy_ex_interp = []
    logy_th_interp = []
    for ind in range(n-1):
        x1 = x[ind]
        x2 = x[ind+1]
        for xval in np.linspace(x1, x2, inter_npoints+1):
            x_interp.append(xval)
            logy_ex_interp.append(g_ex_interp.Eval(xval,0,""))
            logy_th_interp.append(g_th_interp.Eval(xval,0,""))
    diff = np.subtract(logy_ex_interp, logy_th_interp)
else:
    x_interp = x
    diff = np.subtract(y_ex, y_th)

idx = 0
while idx >= 0 and len(diff)>0:
    idx = find_crossing(diff,0)
    if idx < 0: idx = 0
    else: print x_interp[idx]

    diff = diff[idx+1:]
    x_interp = x_interp[idx+1:]
