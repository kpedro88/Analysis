from ROOT import *
import math
from array import array
import numpy as np

sels = "trigOptDeta0.5,trigOptDeta0.6,trigOptDeta0.7,trigOptDeta0.8,trigOptDeta0.9,trigOptDeta1.0,trigOptDeta1.1,trigOptDeta1.2,trigOptDeta1.3,trigOptDeta1.4,trigOptDeta1.5,trigOptDeta1.6,trigOptDeta1.7,trigOptDeta1.8,trigOptDeta1.9,trigOptDeta2.0".split(',')
selvals = [float(sel.replace("trigOptDeta","")) for sel in sels]

sigs = [
"SVJ_1000_20_0.3_peak",
"SVJ_2000_20_0.3_peak",
"SVJ_3000_20_0.3_peak",
"SVJ_4000_20_0.3_peak",
]

file = TFile.Open("test/hist_svj_mt_trigopt.root")
keys = [k.GetName() for k in file.GetListOfKeys()]

for sig in sorted(sigs):
    foms = []
    byields = []
    syields = []
    for sel in sels:
        hsig = None
        hbkg = None
        for key in keys:
            if sig in key and sel in key: hsig = file.Get(key)
            if "bkg" in key and sel in key: hbkg = file.Get(key)
        # integrate in [mZprime-1000,mZprime+1000]
        mZprime = float(sig.split('_')[1])
        b1 = hsig.FindBin(mZprime-1000)
        b2 = hsig.FindBin(mZprime+1000)
        S = hsig.Integral(b1,b2)
        B = hbkg.Integral(b1,b2)
        FOM = math.sqrt(2*((S+B)*math.log(1+S/B)-S))
        foms.append(FOM)
        byields.append(B)
        syields.append(S)
    # make yield graph
    x = array('d', selvals)
    sy = array('d', syields)
    by = array('d', byields)
    gs = TGraph(len(selvals),x,sy)
    gb = TGraph(len(selvals),x,by)
    gs.SetTitle("")
    gs.GetXaxis().SetTitle("#Delta#eta cut")
    gs.GetYaxis().SetTitle("yield in [m_{Z'} - 1 TeV, m_{Z'} + 1 TeV]")
    gs.GetYaxis().SetRangeUser(.1,100000)
    gs.SetMarkerColor(kBlue)
    gb.SetMarkerColor(kRed)
    cy = TCanvas()
    cy.SetLogy()
    gs.Draw("ap")
    gb.Draw("p same")
    cy.Print("yield_"+sig+".png","png")
    # make fom graph
    y = array('d', foms)
    g = TGraph(len(selvals),x,y)
    g.SetTitle("")
    g.GetXaxis().SetTitle("#Delta#eta cut")
    g.GetYaxis().SetTitle("#sqrt{2((S+B)ln(1+S/B)-S)}")
    c = TCanvas()
    g.Draw("ap")
    c.Print("fom_"+sig+".png","png")
    # print some info
    max = np.argmax(foms)
    print(sig+" : max at "+str(selvals[max]))
