import numpy as np
from ROOT import TGraph, TCanvas

addr_base = "/eos/uscms/store/user/lpcdarkqcd/boosted/signal_production_1dscans_v01/BDTNPZ_v01/"

params = {
    "rinv": {
        "addr": "s-channel_mMed-350_mDark-10_rinv-{}_alpha-peak_MADPT300_13TeV-madgraphMLM-pythia8.npz",
        "axis": "r_{inv}",
        "values": ["0","0.05","0.1","0.2","0.3","0.4","0.5","0.6","0.7","0.8","0.9","1"],
    },
    "mZprime": {
        "addr": "s-channel_mMed-{}_mDark-10_rinv-0.3_alpha-peak_MADPT300_13TeV-madgraphMLM-pythia8.npz",
        "axis": "m_{Z'}",
        "values": ["200","250","300","350","400","450","500","550"],
    },
    "mDark": {
        "addr": "s-channel_mMed-350_mDark-{}_rinv-0.3_alpha-peak_MADPT300_13TeV-madgraphMLM-pythia8.npz",
        "axis": "m_{dark}",
        "values": ["1","5","10"],
    },
}

for p,pdict in params.iteritems():
    x = []
    y = []
    for xval in pdict["values"]:
        file = np.load(addr_base+pdict["addr"].format(xval))
        cutflow_keys = file["cutflow_keys"]
        cutflow_vals = file["cutflow_vals"]
        raw = cutflow_vals[next(i for i in range(len(cutflow_keys)) if cutflow_keys[i]=='raw')]
        presel = cutflow_vals[next(i for i in range(len(cutflow_keys)) if cutflow_keys[i]=='preselection')]
        x.append(float(xval))
        y.append(float(presel)/float(raw))

    print p+": ["+",".join(["({:g},{:.2g})".format(xx,yy) for xx,yy in zip(x,y)])+"]"
    print '    ("'+p+'", (['+",".join(["{:g}".format(xx) for xx in x])+'],['+",".join(["{:.2g}".format(yy) for yy in y])+'])),'
#    import sys
#    sys.exit(0)
    from array import array
    xx = array('d', x)
    yy = array('d', y)

    gs = TGraph(len(x),xx,yy)
    gs.SetTitle("")
    gs.GetXaxis().SetTitle(pdict["axis"])
    gs.GetYaxis().SetTitle("acceptance")
#    gs.GetYaxis().SetRangeUser(0,0.5)
    gs.GetYaxis().SetRangeUser(0,0.2)
    cy = TCanvas()
    gs.Draw("ap")
    oname = "plots/acc_vs_"+p+"_boosted"
    cy.Print(oname+".png","png")
    cy.Print(oname+".pdf","pdf")
