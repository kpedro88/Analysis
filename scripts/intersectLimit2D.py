import sys
from collections import OrderedDict

fname = sys.argv[1]
obs = len(sys.argv)>2

import ROOT as r

# get graphs
name = "obs" if obs else "central"
file = r.TFile.Open(fname)
graphs = []
ctr = 0
while True:
    graph = file.Get("contour_{}_{}".format(name,ctr))
    if graph==None: break
    graphs.append(graph)
    ctr += 1

vals = OrderedDict([
    ("xmin",[]),
    ("xmax",[]),
    ("ymin",[]),
    ("ymax",[]),
])

for graph in graphs:
    vals["xmin"].append(r.TMath.MinElement(graph.GetN(),graph.GetX()))
    vals["xmax"].append(r.TMath.MaxElement(graph.GetN(),graph.GetX()))
    vals["ymin"].append(r.TMath.MinElement(graph.GetN(),graph.GetY()))
    vals["ymax"].append(r.TMath.MaxElement(graph.GetN(),graph.GetY()))

for key,val in vals.iteritems():
    print key+"s: "+str(["{:.2f}".format(v) for v in val])

for key,val in vals.iteritems():
    print key+": {:.2f}".format(min(val) if "min" in key else max(val))
