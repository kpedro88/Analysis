import os,sys,glob
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter, RawDescriptionHelpFormatter, RawTextHelpFormatter
import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import uproot as up
from collections import defaultdict

# from ConfigArgParse
class ArgumentDefaultsRawHelpFormatter(
    ArgumentDefaultsHelpFormatter,
    RawTextHelpFormatter,
    RawDescriptionHelpFormatter):
    pass

# make status messages useful
def fprint(msg):
    print(msg)
    sys.stdout.flush()

def parse_fname(fname):
    fname = os.path.basename(fname)
    fsplit = fname.split('.')
    N = int(fsplit[0].replace("higgsCombineTest",""))
    X = fsplit[3].replace("region","")
    Y = fsplit[4].replace("syst","") if fsplit[4].startswith("syst") else None
    return N, X, Y

def get_limit_val(df,q):
    return df[abs(df['quantileExpected']-q)<0.001]['limit'].item()

class Region(object):
    def __init__(self):
        self.x = []
        self.y = []
        self.yerrup = []
        self.yerrdn = []
    def sort(self):
        self.x, self.y, self.yerrup, self.yerrdn = zip(*sorted(zip(self.x, self.y, self.yerrup, self.yerrdn)))

parser = ArgumentParser(formatter_class=ArgumentDefaultsRawHelpFormatter)
parser.add_argument("-d", "--dirs", dest="dirs", type=str, nargs='+', help="directories w/ combine output files")
parser.add_argument("-o", "--outname", dest="outname", type=str, help="plot output name")
args = parser.parse_args()

colors = {
    "highCut_2018": "blue",
    "lowCut_2018": "red",
    "cut": "purple",
    "highSVJ2_2018": "cyan",
    "lowSVJ2_2018": "orange",
    "bdt": "pink",
}

markerstyles = {
    0: 'full',
    1: 'none'
}

xnames = {
    "svj3100": "[TD] MC bkg",
    "svj3100norm": "[TD] MC bkg (norm)",
    "svj3100toy": "[TD] MC bkg (toy)",
    "stat3100": "[WS] fit bkg",
    "stat3100mc": "[WS] MC bkg (norm)",
    "stat3100mctoy": "[WS] MC bkg (toy)",
    "stat3100rename": "[WS] fit bkg mH_region",
}

xvals = {}

# file names look like:
# higgsCombineTestN.AsymptoticLimits.mH120.regionX[.systS0].root
region_info = defaultdict(Region)
region_info_S0 = defaultdict(Region)
for adir in args.dirs:
    combine_files = glob.glob(os.path.join(adir,"higgs*AsymptoticLimits*.root"))
    for cf in combine_files:
        N, region, S0 = parse_fname(cf)
        df = up.open(cf)['limit'].pandas.df(['limit','quantileExpected'])
        rinfo = region_info_S0 if S0 is not None else region_info
        rinfo[region].x.append(N)
        if N not in xvals: xvals[N] = "{{{}}}".format(S0) if S0 is not None else xnames[adir]
        rinfo[region].y.append(get_limit_val(df,0.5))
        rinfo[region].yerrup.append(get_limit_val(df,0.84))
        rinfo[region].yerrdn.append(get_limit_val(df,0.16))

# remove unnecessary gaps
xshifted = {N:iN for iN,N in enumerate(sorted(xvals.keys()))}

fig, (ax, lax) = plt.subplots(ncols=2, gridspec_kw={"width_ratios":[3.0,1]})
leghandles = []
legnames = []
legorder = []
ax.set_xlabel('test')
ax.set_ylabel('r')
xticks = list(sorted(xshifted.values()))
xticklabels = [xvals[x] for x in sorted(xvals.keys())]
xticks = [xticks[0]-1]+xticks+[xticks[-1]+1]
xticklabels = [""]+xticklabels+[""]
for ir,rinfo in enumerate([region_info, region_info_S0]):
    for region,points in rinfo.iteritems():
        label = region.replace("_2018","")
        points.sort()
        print region,ir,points.x,points.y
        #pts = ax.errorbar(points.x, points.y, yerr=[points.yerrdn, points.yerrup], label=label, color=colors[region], marker='o', fillstyle=markerstyles[ir], fmt='o')
        offset = 0.2 if "cut" in region.lower() else 0.0
        pts = ax.plot([xshifted[x]+offset for x in points.x], points.y, 'o', label=label, color=colors[region], marker='o', fillstyle=markerstyles[ir])
        ax.set_xticks(xticks)
        ax.set_xticklabels(xticklabels, rotation=45, ha='right')
        if ir==0:
            legorder.append(points.y[0])
            leghandles.append(pts[0])
            legnames.append(label)

# sensible legend order
legorder, leghandles, legnames = zip(*sorted(zip(legorder, leghandles, legnames),reverse=True))

lax.legend(handles=leghandles, labels=legnames, loc='best', numpoints=1)
# remove unwanted components
lax.axis("off")
fig.subplots_adjust(left=0.1, right=0.95, bottom=0.30, top=0.95)
fig.savefig("test_dc_"+args.outname+".png",**{"dpi":100})

