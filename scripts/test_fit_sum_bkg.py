import ROOT as r
from numpy import array

# disable root garbage collection
def ngc(obj):
    r.SetOwnership(obj,False)

def get_residual(htmp,fn,can=None):
    hist = htmp.Clone(fn.GetName()); ngc(hist)
    hist.Add(fn,-1)
    hist.Divide(fn)
    if can is not None:
        can.cd()
        hist.SetLineColor(fn.GetLineColor())
        hist.SetMarkerColor(fn.GetLineColor())
        hist.SetMarkerStyle(20)
        hist.Draw("p same")
    return hist

def fit_histo_status(hist,fn,opt):
    if "Q" not in opt: print(fn.GetName())
    status = hist.Fit(fn,opt)
    fn.SetTitle(str(int(status)))

def fit_legend(fn,name,leg,pars=True,chi2=True):
    if chi2: name = name+", #chi^{2}/n_{dof} = "+"{:.3g}".format(fn.GetChisquare()/fn.GetNDF())+" ("+fn.GetTitle()+")"
    leg.AddEntry(fn,name,"l")
    if pars:
        allpars = [fn.GetParameter(p) for p in range(0,fn.GetNpar())]
        parname = "p = "+', '.join(["{:.2g}".format(p) for p in allpars])
        leg.AddEntry(None,parname,"")

def fit_iter(hsum,draw=False,fitopt="NQ"):
    hsum.Rebin(2)
    xmin = hsum.GetXaxis().GetXmin()
    xmax = hsum.GetXaxis().GetXmax()

    # simple fit first
    fsingle = r.TF1("fsingle","[0]*(1-x/13000)^([1]+[2]*log(x/13000))",xmin,xmax); ngc(fsingle)
    fsingle.SetParameters(1000,15,-25)
    fit_histo_status(hsum,fsingle,fitopt+"R")

    # try to fit the low-x part
    ffirst = r.TF1("ffirst","[0]*(1-x/13000)^([1]+[2]*log(x/13000))",xmin,xmax*0.5); ngc(ffirst)
    ffirst.SetParameters(1000,15,-25)
    fit_histo_status(hsum,ffirst,fitopt+"R")
    ffirst.SetRange(xmin,xmax)
    
    # subtract and try to fit the remaining high-x part (look for best chi2)
    ffirst_p0 = ffirst.GetParameter(0)
    pvals = []
    chi2s = []
    for x in range(40):
        np0 = (0.5+x*0.01)*ffirst_p0
        htmp = hsum.Clone("htmp")
        ffirst.SetParameter(0,np0)
        htmp.Add(ffirst,-1)
        ftmp = r.TF1("ftmp","[0]*(1-x/13000)^([1]+[2]*log(x/13000))",xmax*0.5,xmax)
        fit_histo_status(htmp,ftmp,fitopt+"RQ")
        pvals.append(np0)
        chi2s.append(ftmp.GetChisquare()/ftmp.GetNDF())
        
    if draw:
        giter = r.TGraph(len(pvals),array(pvals),array(chi2s)); ngc(giter)
        giter.GetXaxis().SetTitle("f_{first} p_{0}")
        giter.GetYaxis().SetTitle("f_{second} #chi^{2}/n_{dof}")
        giter.SetTitle("")
        cang = r.TCanvas(); ngc(cang)
        cang.cd()
        cang.SetLogy()
        giter.Draw("ap")
        
        cang.Print("opt_iter_"+hsum.GetName()+".png","png")
    
    op0 = pvals[chi2s.index(min(chi2s))]
    print "Optimum = "+str(op0/ffirst_p0)+" (rel)"
    ffirst.SetParameter(0,op0)
    hsub = hsum.Clone("hsub"); ngc(hsub)
    hsub.Add(ffirst,-1)
    fsecond = r.TF1("fsecond","[0]*(1-x/13000)^([1]+[2]*log(x/13000))",xmin,xmax); ngc(fsecond)
    fit_histo_status(hsub,fsecond,fitopt+"R")

    fthird = r.TF1("fthird","[0]*(1-x/13000)^([1]+[2]*log(x/13000))+[3]*(1-x/13000)^([4]+[5]*log(x/13000))",xmin,xmax); ngc(fthird)
    fthird.SetParameter(0,ffirst.GetParameter(0))
    fthird.SetParameter(1,ffirst.GetParameter(1))
    fthird.SetParameter(2,ffirst.GetParameter(2))
    fthird.SetParameter(3,fsecond.GetParameter(0))
    fthird.SetParameter(4,fsecond.GetParameter(1))
    fthird.SetParameter(5,fsecond.GetParameter(2))
    fit_histo_status(hsum,fthird,fitopt+"R")

    if draw:
        hsum.SetLineColor(r.kBlack)
        hsum.SetLineWidth(2)

        hsub.SetLineColor(r.kMagenta+2)
        hsub.SetLineWidth(2)

        fsingle.SetLineColor(r.kMagenta)
        fsingle.SetLineStyle(7)
        fsingle.SetLineWidth(2)

        ffirst.SetLineColor(r.kOrange+2)
        ffirst.SetLineStyle(7)
        ffirst.SetLineWidth(2)

        fsecond.SetLineColor(r.kPink-9)
        fsecond.SetLineStyle(7)
        fsecond.SetLineWidth(2)

        fthird.SetLineColor(r.kGreen+3)
        fthird.SetLineStyle(7)
        fthird.SetLineWidth(2)

        can = r.TCanvas(); ngc(can)
        can.cd()
        can.SetLogy()

        # two legends for input vs. output
        legin = r.TLegend(0.17,0.15,0.35,0.3); ngc(legin)
        legin.SetFillColor(0)
        legin.SetBorderSize(0)
        legin.SetTextSize(0.037)
        legin.SetTextFont(42)
        fit_legend(hsum,"h_{sum} = bkg",legin,pars=False,chi2=False)
        fit_legend(hsub,"h_{sub} = h_{sum}-f_{first}",legin,pars=False,chi2=False)

        legout = r.TLegend(0.4,0.6,0.9,0.93); ngc(legout)
        legout.SetFillColor(0)
        legout.SetBorderSize(0)
        legout.SetTextSize(0.037)
        legout.SetTextFont(42)
        legout.SetMargin(0.1)
        fit_legend(fsingle,"f_{single}",legout)
        fit_legend(ffirst,"f_{first}",legout)
        fit_legend(fsecond,"f_{second}",legout)
        fit_legend(fthird,"f_{third}",legout)

        hsum.Draw("hist")
        fsingle.Draw("same")
        ffirst.Draw("same")
        hsub.Draw("hist same")
        fsecond.Draw("same")
        fthird.Draw("same")
        legin.Draw()
        legout.Draw()
        
        can.Print("fit_iter_"+hsum.GetName()+".png","png")

        # separate canvas for residuals
        hrat = r.TH1F("hrat","",100,xmin,xmax); ngc(hrat)
        hrat.GetXaxis().SetTitle("x")
        hrat.GetYaxis().SetTitle("residual (f-h)/f")
        hrat.GetYaxis().SetRangeUser(-1.0,1.0)
        can2 = r.TCanvas(); ngc(can2)
        can2.cd()
        hrat.Draw("hist")
        rsingle = get_residual(hsum,fsingle,can2)
        rfirst = get_residual(hsum,ffirst,can2)
        rdouble = get_residual(hsum,fthird,can2)
        can2.Print("res_iter_"+hsum.GetName()+".png","png")
        
    return ffirst.GetChisquare()/ffirst.GetNDF(), fsecond.GetChisquare()/fsecond.GetNDF()

if __name__=="__main__":
    file = r.TFile.Open("test/toys.root"); ngc(file)
    r.TVirtualFitter.SetMaxIterations(20000)
    hnames = ["toy_sigfull_highCut_2018_Bkg","toy_sigfull_lowCut_2018_Bkg"]
    for hname in hnames:
        hsum = file.Get(hname); ngc(hsum)
        hsum.SetName(hname)
        fit_iter(hsum,draw=True,fitopt="N")
