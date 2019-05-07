from ROOT import *

addr_base = "root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV16/Skims/tree_dijetmtdetahadmf/"

params = {
    "rinv": {
        "addr": addr_base+"tree_SVJ_mZprime-3000_mDark-20_rinv-{}_alpha-peak_MC2017.root",
        "axis": "r_{inv}",
        "values": ["0","0.1","0.2","0.3","0.4","0.5","0.6","0.7","0.8","0.9","1"],
    },
    "mZprime": {
        "addr": addr_base+"tree_SVJ_mZprime-{}_mDark-20_rinv-0.3_alpha-peak_MC2017.root",
        "axis": "m_{Z'}",
        "values": ["500","600","700","800","900","1000","1100","1200","1300","1400","1500","1600","1700","1800","1900","2000","2100","2200","2300","2400","2500","2600","2700","2800","2900","3000","3100","3200","3300","3400","3500","3600","3700","3800","3900","4000","4100","4200","4300","4400","4500"],
    },
    "mDark": {
        "addr": addr_base+"tree_SVJ_mZprime-3000_mDark-{}_rinv-0.3_alpha-peak_MC2017.root",
        "axis": "m_{dark}",
        "values": ["1","5","10","20","30","40","50","60","70","80","90","100"],
    },
    "alpha": {
        "addr": addr_base+"tree_SVJ_mZprime-3000_mDark-20_rinv-0.3_alpha-{}_MC2017.root",
        "axis": "#alpha_{dark}",
        "values": ["peak","high","low"],
    },
}

alpha_vals = {
    "peak": -2,
    "high": -1,
    "low": -3,
}

for p,pdict in params.iteritems():
    x = []
    y = []
    for xval in pdict["values"]:
        file = TFile.Open(pdict["addr"].format(xval))
        nEventProc = file.Get("nEventProc")
        denom = nEventProc.GetBinContent(1)
        cutflow = file.Get("cutflow")
#        numer = cutflow.GetBinContent(cutflow.GetNbinsX())
        tree = file.Get("tree")
        numer = tree.Draw("MT_AK8","MET/MT_AK8>0.25&&DeltaPhiMin_AK8<0.75", "goff")
        x.append(float(xval) if p is not "alpha" else alpha_vals[xval])
        y.append(float(numer)/float(denom))

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
    oname = "acc_vs_"+p+"_final"
    cy.Print(oname+".png","png")
    cy.Print(oname+".eps","eps")
    import os
    os.popen("epstopdf "+oname+".eps")
