import sys
from ROOT import TFile,TH1,TRandom3

eosdir = "root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards"

def makeToy(ofile,fname,region,hname):
    file = TFile.Open(eosdir+"/"+fname)
    hist = file.Get(region+"/"+hname)
    hist.SetDirectory(0)

    # randomize total yield
    yield_orig = hist.Integral(0,hist.GetNbinsX()+1)
    yield_rand = int(gRandom.Poisson(yield_orig))

    # prevent negative bins (gives nan in GetRandom())
    for b in range(0,hist.GetNbinsX()+2):
        if hist.GetBinContent(b)<0: hist.SetBinContent(b,0)

    # generate toy data
    hist_rand = hist.Clone()
    hist_rand.Reset("ICESM")
    for n in range(0,yield_rand):
        x = hist.GetRandom()
        hist_rand.Fill(hist.GetRandom())

    ofile.cd()
    cr = fname.split("/")[0]
    oname = "toy_"+cr+"_"+region+"_"+hname
    hist_rand.Write(oname)
    print(oname)

if __name__=="__main__":
    ofile = TFile.Open("test/toys.root","RECREATE")
    gRandom = TRandom3(1234)
    # cut-based
    makeToy(ofile,"sigfull/datacard.root","highCut_2018","Bkg")
    makeToy(ofile,"sigfull/datacard.root","lowCut_2018","Bkg")
    makeToy(ofile,"highdeta/datacard.root","highCut_2018","Bkg")
    makeToy(ofile,"highdeta/datacard.root","lowCut_2018","Bkg")
    makeToy(ofile,"lowrt/datacard.root","lowCut_2018","Bkg")
    makeToy(ofile,"lowrthighdeta/datacard.root","lowCut_2018","Bkg")
    makeToy(ofile,"lowrt2/datacard.root","lowCut_2018","Bkg")
    makeToy(ofile,"lowrt2highdeta/datacard.root","lowCut_2018","Bkg")
    # bdt-based
    makeToy(ofile,"sigfull/datacard.root","highSVJ0_2018","Bkg")
    makeToy(ofile,"sigfull/datacard.root","lowSVJ0_2018","Bkg")
    makeToy(ofile,"sigfull/datacard.root","highSVJ2_2018","Bkg")
    makeToy(ofile,"sigfull/datacard.root","lowSVJ2_2018","Bkg")
    makeToy(ofile,"lowrt/datacard.root","lowSVJ0_2018","Bkg")
    makeToy(ofile,"lowrt/datacard.root","lowSVJ2_2018","Bkg")
    makeToy(ofile,"lowrt2/datacard.root","lowSVJ0_2018","Bkg")
    makeToy(ofile,"lowrt2/datacard.root","lowSVJ2_2018","Bkg")
