import sys
import numpy as np
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
from collections import OrderedDict
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
from ROOT import TFile,TH1,gRandom,Math

# axis setup (only draw bottom spine, etc.)
# based on https://matplotlib.org/examples/ticks_and_spines/tick-locators.html
def setup(ax):
    ax.spines['right'].set_color('none')
    ax.spines['left'].set_color('none')
    ax.yaxis.set_major_locator(ticker.NullLocator())
    ax.spines['top'].set_color('none')
    ax.xaxis.set_ticks_position('bottom')
    ax.tick_params(which='major', width=1.00)
    ax.tick_params(which='major', length=5)
    ax.tick_params(which='minor', width=0.0)
    ax.tick_params(which='minor', length=0.0)
    ax.set_ylim(0, 1)
    ax.patch.set_alpha(0.0)

def getMin(bin_min,sigma):
    # get confidence interval (e.g. 1 - 0.6827 for 1 sigma)
    if sigma==0: return bin_min
    alpha = 1 - (Math.gaussian_cdf(sigma) - Math.gaussian_cdf(-sigma))
    return Math.gamma_quantile_c(alpha/2.,bin_min+1,1)

def getHist(eosdir,fname,region,hname,hnorm=None):
    file = TFile.Open(eosdir+"/"+fname)
    hist = file.Get(region+"/"+hname)
    hist.SetDirectory(0)
    if hnorm is not None:
        hist_norm = file.Get(region+"/"+hnorm)
        yield_orig = hist_norm.Integral(0,hist_norm.GetNbinsX()+1)
        hist.Scale(yield_orig/hist.Integral(0,hist.GetNbinsX()+1))

    # prevent negative bins (gives nan in GetRandom())
    for b in range(0,hist.GetNbinsX()+2):
        if hist.GetBinContent(b)<0: hist.SetBinContent(b,0)

    return hist

def makeToy(hist):
    # generate toy data
    yield_orig = hist.Integral(0,hist.GetNbinsX()+1)
    hist_rand = hist.Clone()
    hist_rand.SetDirectory(0)
    hist_rand.Reset("ICESM")

    # randomize total yield
    yield_rand = int(gRandom.Poisson(yield_orig))
    for n in range(0,yield_rand):
        x = hist.GetRandom()
        hist_rand.Fill(x)

    return hist_rand

def finalBinning(args):
    regions = OrderedDict([
        ("highCut",OrderedDict()),
        ("lowCut",OrderedDict()),
        ("highSVJ2",OrderedDict()),
        ("lowSVJ2",OrderedDict()),
    ])

    # plot the final binning
    fig, axs = plt.subplots(nrows=len(regions))
    for region,ax in zip(regions.keys(),axs):
        hist = getHist(args.dir,"datacard.root",region+"_2018","Bkg","data_obs")

        # loop over sigmas
        fig, axs = plt.subplots(nrows=len(args.sigma))
        fig.suptitle(region)
        try:
            axs = list(axs)
        except:
            axs = [axs]
        for sigma,ax in zip(args.sigma,axs):
            # add poisson uncertainty
            bin_min = getMin(args.bin_min,sigma)

            # find the optimal binning
            newbins = []
            sum = 0
            firstnewbin = 0
            oldbinsize = hist.GetBinLowEdge(2)-hist.GetBinLowEdge(1)
            newbins.append(hist.GetBinLowEdge(1))
            # temp vars to keep track of bin width - want monotonically increasing widths
            binlo = hist.GetBinLowEdge(1)
            binhi = 0
            minbinsize = oldbinsize
            for b in range(hist.GetNbinsX()):
                bin = b+1
                sum += hist.GetBinContent(bin)
                binhi = hist.GetBinLowEdge(bin+1)
                if (sum>bin_min and (binhi-binlo)>=minbinsize) or (args.max_width>0 and (binhi-binlo)>=args.max_width) or bin==hist.GetNbinsX():
                    newbins.append(hist.GetBinLowEdge(bin+1))
                    if firstnewbin==0 and newbins[-1]-newbins[-2]>oldbinsize: firstnewbin = len(newbins)-2
                    minbinsize = binhi-binlo
                    binlo = binhi
                    binhi = 0
                    sum = 0

            # plot new binning
            setup(ax)
            ax.tick_params(axis='x', labelsize=8)
            ax.set_xlim(min(newbins),max(newbins))
            ax.set_xlabel('$\sigma$ = {}'.format(sigma))
            # exclude unchanged bins
            ax.xaxis.set_major_locator(ticker.FixedLocator([newbins[0]]+newbins[firstnewbin:]))
            for label in ax.get_xmajorticklabels():
                label.set_rotation(70)

            # use high binning for low
            if args.final and region.startswith("low"): newbins = regions[region.replace("low","high")][sigma][0][:]
            # store new binning
            regions[region][sigma] = [newbins[:],0]

            # make toys
            below_min = 0
            for ntoy in range(args.ntoys):
                htoy = makeToy(hist)
                # set new binning
                htoy = htoy.Rebin(len(newbins)-1,hist.GetName()+"_rebin",np.array(newbins))

                # check for bins below min (exclude last bin)
                for b in range(htoy.GetNbinsX()-1):
                    bin = b+1
                    if htoy.GetBinContent(bin) < args.bin_min: below_min += 1

            # store result
            regions[region][sigma][1] = float(below_min)/float(len(newbins)*args.ntoys)

        # Push the top of the top axes outside the figure because we only show the bottom spine.
        fig.subplots_adjust(left=0.05, right=0.95, bottom=0.15, top=1.05)
        nmaxw = ""
        if args.max_width>0: nmaxw = "_maxw"+str(args.max_width)
        if args.doprint: fig.savefig("finalbins_"+region+"_min"+str(args.bin_min)+"_sigma"+','.join([str(sigma) for sigma in args.sigma])+nmaxw+".png",**{"dpi":100})

    # print results
    if args.doprint:
        for region in regions:
            print '"{}": {{'.format(region)
            for sigma in regions[region]:
                print '    {}: {},'.format(sigma,regions[region][sigma][0])
            print '},'
        print ""
        for sigma in args.sigma:
            print "sigma = {}, bin_min = {:.2f}".format(sigma,getMin(args.bin_min,sigma))
            print ""
            for region in regions:
                print region+": "+str(regions[region][sigma][1])
            print ""
            
    return regions

def main(argv=None):
    if argv==None: argv = sys.argv[1:]
    # define options
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument("-d", "--dir", dest="dir", type=str, default="root://cmseos.fnal.gov//store/user/pedrok/SVJ2017/Datacards/trig2x/sigfull", help="eos dir w/ datacard.root")
    parser.add_argument("-n", "--ntoys", dest="ntoys", type=int, default=5, help="number of toys")
    parser.add_argument("-m", "--min", dest="bin_min", type=int, default=10, help="min # events per bin")
    parser.add_argument("-s", "--sigma", dest="sigma", type=float, default=[], nargs='+', help="list of number of sigmas")
    parser.add_argument("-f", "--final", dest="final", action="store_true", default=False, help="use final binning (apply high to low)")
    parser.add_argument("-w", "--max-width", dest="max_width", type=float, default=0., help="max bin width")
    parser.add_argument("-p", "--print", dest="doprint", action="store_true", default=False, help="print figures and text")
    args = parser.parse_args(args=argv)

    return finalBinning(args)

if __name__=="__main__":
    main()
