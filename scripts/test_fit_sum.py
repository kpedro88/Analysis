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
    status = hist.Fit(fn,opt)
    fn.SetTitle(str(int(status)))

def fit_legend(fn,name,leg,pars=True,chi2=True):
    if chi2: name = name+", #chi^{2}/n_{dof} = "+"{:.3g}".format(fn.GetChisquare()/fn.GetNDF())+" ("+fn.GetTitle()+")"
    leg.AddEntry(fn,name,"l")
    if pars:
        allpars = [fn.GetParameter(p) for p in range(0,fn.GetNpar())]
        parname = "p = "+', '.join(["{:.2f}".format(p) for p in allpars])
        leg.AddEntry(None,parname,"")

def make_sum(p0,p1,q0,q1,draw=False):
    xmin = 0.1
    xmax = 0.5

    # first component
    f1 = r.TF1("f1","[0]*(1-x)^[1]",xmin,xmax); ngc(f1)
    f1.SetParameter(0,p0)
    f1.SetParameter(1,p1)
    
    # second component
    f2 = r.TF1("f2","[0]*(1-x)^[1]",xmin,xmax); ngc(f2)
    f2.SetParameter(0,q0)
    f2.SetParameter(1,q1)
    
    # sum of both components
    fsum = r.TF1("fsum","[0]*(1-x)^[1]+[2]*(1-x)^[3]",xmin,xmax); ngc(fsum)
    fsum.SetParameter(0,p0)
    fsum.SetParameter(1,p1)
    fsum.SetParameter(2,q0)
    fsum.SetParameter(3,q1)
    
    # create histogram from distribution
    hsum = r.TH1F("hsum","",100,xmin,xmax); ngc(hsum)
    for b in range(1,hsum.GetNbinsX()+1):
        hsum.SetBinContent(b,fsum.Eval(hsum.GetBinCenter(b)))

    if draw:
        f1.SetLineColor(r.kBlue)
        f1.SetLineStyle(7)
        f1.SetLineWidth(2)

        f2.SetLineColor(r.kRed)
        f2.SetLineStyle(7)
        f2.SetLineWidth(2)
        
        fsum.SetLineColor(r.kBlack)
        fsum.SetLineWidth(2)
        hsum.SetLineColor(r.kBlack)
        hsum.SetLineWidth(2)
        hsum.GetXaxis().SetTitle("x")
        hsum.GetYaxis().SetTitle("function value")

    return xmin,xmax,f1,f2,fsum,hsum

def fit_iter(p0,p1,q0,q1,draw=False,fitopt="NQ"):
    xmin,xmax,f1,f2,fsum,hsum = make_sum(p0,p1,q0,q1,draw)

    # try to fit the low-x part
    ffirst = r.TF1("ffirst","[0]*(1-x)^[1]",xmin,xmax*0.5); ngc(ffirst)
    fit_histo_status(hsum,ffirst,fitopt+"R")
    ffirst.SetRange(xmin,xmax)
    
    # subtract and try to fit the remaining high-x part (look for best chi2)
    ffirst_p0 = ffirst.GetParameter(0)
    pvals = []
    chi2s = []
    for x in range(50):
        np0 = 0.5+x*0.01
        htmp = hsum.Clone("htmp")
        ffirst.SetParameter(0,np0)
        htmp.Add(ffirst,-1)
        ftmp = r.TF1("ftmp","[0]*(1-x)^[1]",xmax*0.5,xmax)
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
        
        cang.Print("opt_iter_"+','.join([str(x) for x in [p0,p1,q0,q1]])+".png","png")
    
    op0 = pvals[chi2s.index(min(chi2s))]
    print "Optimum = "+str(op0)
    ffirst.SetParameter(0,op0)
    hsub = hsum.Clone("hsub"); ngc(hsub)
    hsub.Add(ffirst,-1)
    fsecond = r.TF1("fsecond","[0]*(1-x)^[1]",xmin,xmax); ngc(fsecond)
    fit_histo_status(hsub,fsecond,fitopt+"R")

    fthird = r.TF1("fthird","[0]*(1-x)^[1]+[2]*(1-x)^[3]",xmin,xmax); ngc(fthird)
    fthird.SetParameter(0,ffirst.GetParameter(0))
    fthird.SetParameter(1,ffirst.GetParameter(1))
    fthird.SetParameter(2,fsecond.GetParameter(0))
    fthird.SetParameter(3,fsecond.GetParameter(1))
    fit_histo_status(hsum,fthird,fitopt+"R")

    if draw:
        hsub.SetLineColor(r.kMagenta+2)

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
        legin = r.TLegend(0.17,0.15,0.35,0.4); ngc(legin)
        legin.SetFillColor(0)
        legin.SetBorderSize(0)
        legin.SetTextSize(0.037)
        legin.SetTextFont(42)
        fit_legend(f1,"f_{1} = p_{0}(1-x)^{p_{1}}",legin,chi2=False)
        fit_legend(f2,"f_{2} = p_{0}(1-x)^{p_{1}}",legin,chi2=False)
        fit_legend(fsum,"f_{sum} = f_{1}+f_{2}",legin,pars=False,chi2=False)
        fit_legend(hsub,"f_{sub} = f_{sum}-f_{first}",legin,pars=False,chi2=False)

        legout = r.TLegend(0.6,0.65,0.9,0.93); ngc(legout)
        legout.SetFillColor(0)
        legout.SetBorderSize(0)
        legout.SetTextSize(0.037)
        legout.SetTextFont(42)
        fit_legend(ffirst,"f_{first}",legout)
        fit_legend(fsecond,"f_{second}",legout)
        fit_legend(fthird,"f_{third}",legout)

        hsum.Draw("hist")
        f1.Draw("same")
        f2.Draw("same")
        ffirst.Draw("same")
        hsub.Draw("same")
        fsecond.Draw("same")
        fthird.Draw("same")
        legin.Draw()
        legout.Draw()
        
        can.Print("fit_iter_"+','.join([str(x) for x in [p0,p1,q0,q1]])+".png","png")

        # separate canvas for residuals
        hrat = r.TH1F("hrat","",100,xmin,xmax); ngc(hrat)
        hrat.GetXaxis().SetTitle("x")
        hrat.GetYaxis().SetTitle("residual (f-h)/f")
        hrat.GetYaxis().SetRangeUser(-1.0,1.0)
        can2 = r.TCanvas(); ngc(can2)
        can2.cd()
        hrat.Draw("hist")
        rfirst = get_residual(hsum,ffirst,can2)
        rthird = get_residual(hsum,fthird,can2)
        can2.Print("res_iter_"+','.join([str(x) for x in [p0,p1,q0,q1]])+".png","png")
        
    return ffirst.GetChisquare()/ffirst.GetNDF(), fsecond.GetChisquare()/fsecond.GetNDF()

def fit_sum(p0,p1,q0,q1,draw=False,fitopt="NQ"):
    xmin,xmax,f1,f2,fsum,hsum = make_sum(p0,p1,q0,q1,draw)

    # try to fit sum with single term function
    fsingle = r.TF1("fsingle","[0]*(1-x)^[1]",xmin,xmax); ngc(fsingle)
    fit_histo_status(hsum,fsingle,fitopt)
    
    # try to fit sum with multi term function, unconstrained
    fdouble = r.TF1("fdouble","[0]*(1-x)^[1]+[2]*(1-x)^[3]",xmin,xmax); ngc(fdouble)
    fit_histo_status(hsum,fdouble,fitopt)

    # try to fit sum with multi term function, constrained
    fconstr = r.TF1("fconstr","[0]*(1-x)^[1]+[2]*(1-x)^[3]",xmin,xmax); ngc(fconstr)
    fconstr.SetParameter(0,p0)
    fconstr.SetParLimits(0,max(0,p0*0.5),min(1,p0*1.5))
    fconstr.SetParameter(2,q0)
    fconstr.SetParLimits(2,max(0,q0*0.5),min(1,q0*1.5))
    fconstr.SetParameter(1,(p1+q1)/2)
    fconstr.SetParameter(3,(p1+q1)/2)
    fit_histo_status(hsum,fconstr,fitopt)

    # couple parameters (& constrain)
    fcouple = r.TF1("fcouple","[0]*(1-x)^[1]+(1-[0])*(1-x)^[2]",xmin,xmax); ngc(fcouple)
    fcouple.SetParameter(0,p0)
    fcouple.SetParLimits(0,max(0,p0*0.5),min(1,p0*1.5))
    fit_histo_status(hsum,fcouple,fitopt)

    # initialize w/ the actual param values
    ftruth = r.TF1("ftruth","[0]*(1-x)^[1]+[2]*(1-x)^[3]",xmin,xmax); ngc(ftruth)
    ftruth.SetParameter(0,p0)
    ftruth.SetParameter(1,p1)
    ftruth.SetParameter(2,q0)
    ftruth.SetParameter(3,q1)
    fit_histo_status(hsum,ftruth,fitopt)

    if draw:
        fsingle.SetLineColor(r.kMagenta)
        fsingle.SetLineStyle(7)
        fsingle.SetLineWidth(2)

        fdouble.SetLineColor(r.kPink-9)
        fdouble.SetLineStyle(7)
        fdouble.SetLineWidth(2)

        fconstr.SetLineColor(r.kMagenta+2)
        fconstr.SetLineStyle(7)
        fconstr.SetLineWidth(2)
        
        fcouple.SetLineColor(r.kOrange+2)
        fcouple.SetLineStyle(7)
        fcouple.SetLineWidth(2)

        ftruth.SetLineColor(r.kGreen+3)
        ftruth.SetLineStyle(7)
        ftruth.SetLineWidth(2)

        can = r.TCanvas(); ngc(can)
        can.cd()
        can.SetLogy()
        
        # two legends for input vs. output
        legin = r.TLegend(0.2,0.2,0.5,0.4); ngc(legin)
        legin.SetFillColor(0)
        legin.SetBorderSize(0)
        legin.SetTextSize(0.037)
        legin.SetTextFont(42)
        fit_legend(f1,"f_{1} = p_{0}(1-x)^{p_{1}}",legin,chi2=False)
        fit_legend(f2,"f_{2} = p_{0}(1-x)^{p_{1}}",legin,chi2=False)
        fit_legend(fsum,"f_{sum} = f_{1}+f_{2}",legin,pars=False,chi2=False)

        legout = r.TLegend(0.6,0.53,0.9,0.93); ngc(legout)
        legout.SetFillColor(0)
        legout.SetBorderSize(0)
        legout.SetTextSize(0.037)
        legout.SetTextFont(42)
        fit_legend(fsingle,"f_{single}",legout)
        fit_legend(fdouble,"f_{double}",legout)
        fit_legend(fconstr,"f_{constr}",legout)
        fit_legend(fcouple,"f_{couple}",legout)
        fit_legend(ftruth,"f_{truth}",legout)
        
        hsum.Draw("hist")
        f1.Draw("same")
        f2.Draw("same")
        fsingle.Draw("same")
        fdouble.Draw("same")
        fconstr.Draw("same")
        fcouple.Draw("same")
        ftruth.Draw("same")
        legin.Draw()
        legout.Draw()
        can.Print("fit_sum_"+','.join([str(x) for x in [p0,p1,q0,q1]])+".png","png")
        
        # separate canvas for residuals
        hrat = r.TH1F("hrat","",100,xmin,xmax); ngc(hrat)
        hrat.GetXaxis().SetTitle("x")
        hrat.GetYaxis().SetTitle("residual (f-h)/f")
        hrat.GetYaxis().SetRangeUser(-1.0,1.0)
        can2 = r.TCanvas(); ngc(can2)
        can2.cd()
        hrat.Draw("hist")
        rsingle = get_residual(hsum,fsingle,can2)
        rdouble = get_residual(hsum,fdouble,can2)
        rconstr = get_residual(hsum,fconstr,can2)
        rcouple = get_residual(hsum,fcouple,can2)
        rtruth = get_residual(hsum,ftruth,can2)
        can2.Print("res_sum_"+','.join([str(x) for x in [p0,p1,q0,q1]])+".png","png")
        
    return fsingle.GetChisquare()/fsingle.GetNDF(), fdouble.GetChisquare()/fdouble.GetNDF(), fconstr.GetChisquare()/fconstr.GetNDF()

if __name__=="__main__":
    fit_sum(0.9,10.0,0.1,5.0,draw=True,fitopt="N")
    fit_iter(0.9,10.0,0.1,5.0,draw=True,fitopt="N")

    fit_sum(0.9,10.0,0.1,8.0,draw=True,fitopt="N")
    fit_iter(0.9,10.0,0.1,8.0,draw=True,fitopt="N")
