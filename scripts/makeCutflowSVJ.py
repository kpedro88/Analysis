import os, stat, sys
from optparse import OptionParser
from collections import OrderedDict, defaultdict

# avoid relative import
import sys
sys.path = [os.getcwd()] + sys.path

with open('input/dict_xsec_Zprime.txt','r') as xfile:
    xsecs = {xline.split('\t')[0]: float(xline.split('\t')[1]) for xline in xfile}

def getNames(opt):
    return getattr(__import__(opt,fromlist=["selections"]),"selections")

# implementation of recursive loop over any number of dimensions
# creates grid of all possible combinations of parameter values
def varyAll(pos,paramlist,sig,sigs):
    param = paramlist[pos][0]
    vals = paramlist[pos][1]
    for v in vals:
        stmp = sig[:]+[v]
        # check if last param
        if pos+1==len(paramlist):
            sigs.append(tuple(stmp))
        else:
            varyAll(pos+1,paramlist,stmp,sigs)

# converts to UZH naming convention for signal histograms
def convertName(outname):
    # convert signal name: SVJ_X_Y_Z_W -> SVJ_mZprimeX_mDarkY_rinvZ_alphaW
    namesplit = outname.split('_')
    newname = '_'.join([namesplit[0],"mZprime"+namesplit[1],"mDark"+namesplit[2],"rinv"+namesplit[3].replace(".",""),"alpha"+namesplit[4]])
    return newname

def texVal(qty, val):
    if qty=="aDark":
        return "\\"+qty+val.capitalize()
    else:
        return val

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17/scan/supplementary/tree_signal/", help="location of root files (PFN)")
parser.add_option("-o", "--outname", dest="outname", type='string', default="svj_cutflow.tex", help="output TeX file name")
parser.add_option("-n", "--names", dest="names", type='string', default="selections", help="dict of fancy selection names, in order")
parser.add_option("-t", "--type", dest="type", type='choice', default="abs", choices=['abs','rel','raw','rawrel'], help="type of cutflow (abs, rel, raw, rawrel)")
parser.add_option("-p", "--prec", dest="prec", type='int', default=1, help="numerical precision of output")
parser.add_option("-e", "--no-error", dest="error", default=True, action="store_false", help="don't display statistical errors")
parser.add_option("-z", "--skipzeros", dest="skipzeros", default=False, action="store_true", help="ignore errors on zero-count bins")
parser.add_option("-v", "--verbose", dest="verbose", default=False, action="store_true", help="verbose output")
parser.add_option("-r", "--regionfile", dest="regionfile", default="", help="root file containing signal regions")
parser.add_option("-s", "--signalregions", dest="signalregions", type='string', default="regions", help="dict of fancy signal region names, in order")
parser.add_option("-P", "--procs", dest="procs", type='string', default="all", help="categories of procs to include")
parser.add_option("-E", "--efficiency", dest="efficiency", default=False, action="store_true", help="include line for overall efficiency")
parser.add_option("-S", "--small", dest="small", default=False, action="store_true", help="smaller font size")
(options, args) = parser.parse_args()

options.efficiency = options.efficiency or 'raw' in options.type

namesDict = getNames(options.names)

lumis = OrderedDict([
    ("2016", 35920),
    ("2017", 41530),
    ("2018", 59740),
])

outDict = OrderedDict([])
outDict["header1"] = "\\multicolumn{2}{c}{Selection}"
if options.efficiency:
    outDict["NormType"] = "\\multicolumn{2}{c}{}"

outDict = OrderedDict(list(outDict.items())+list(namesDict.items()))
if options.efficiency:
    outDict["efficiency"] = "\\multicolumn{2}{c}{Efficiency [\%]}"

signalRegions = OrderedDict([])
if len(options.regionfile)>0: signalRegions = getNames(options.signalregions)
outDict = OrderedDict(list(outDict.items())+list(signalRegions.items()))

procs = OrderedDict()
if options.procs=="all" or options.procs=="bkg":
    procs = OrderedDict([
        ('QCD',[
            ('QCD_Pt_80to120', 2762530.0),
            ('QCD_Pt_120to170', 471100.0),
            ('QCD_Pt_170to300', 117276.0),
            ('QCD_Pt_300to470', 7823.0),
            ('QCD_Pt_470to600', 648.2),
            ('QCD_Pt_600to800', 186.9),
            ('QCD_Pt_800to1000', 32.293),
            ('QCD_Pt_1000to1400', 9.4183),
            ('QCD_Pt_1400to1800', 0.84265),
            ('QCD_Pt_1800to2400', 0.114943),
            ('QCD_Pt_2400to3200', 0.00682981),
            ('QCD_Pt_3200toInf', 0.000165445),
        ]),
        ('\\ttbar',[
            ('TTJets', 831.76),
            ('TTJets_DiLept', 88.34),
            ('TTJets_DiLept_genMET-80', 22.46),
            ('TTJets_DiLept_genMET-150', 5.919026),
            ('TTJets_SingleLeptFromT', 182.72),
            ('TTJets_SingleLeptFromT_genMET-80', 32.23),
            ('TTJets_SingleLeptFromT_genMET-150', 9.683904),
            ('TTJets_SingleLeptFromTbar', 182.72),
            ('TTJets_SingleLeptFromTbar_genMET-80', 31.78),
            ('TTJets_SingleLeptFromTbar_genMET-150', 9.657872),
            ('TTJets_HT-600to800', 2.68455),
            ('TTJets_HT-800to1200', 1.0959472),
            ('TTJets_HT-1200to2500', 0.1942638),
            ('TTJets_HT-2500toInf', 0.002351015),
        ]),
        ('\\wjets',[
            ('WJetsToLNu_HT-100to200', 1627.45),
            ('WJetsToLNu_HT-200to400', 435.24),
            ('WJetsToLNu_HT-400to600', 59.18),
            ('WJetsToLNu_HT-600to800', 14.58),
            ('WJetsToLNu_HT-800to1200', 6.66),
            ('WJetsToLNu_HT-1200to2500', 1.608),
            ('WJetsToLNu_HT-2500toInf', 0.03891),
        ]),
        ('\\zjets',[
            ('ZJetsToNuNu_HT-100to200', 344.8305),
            ('ZJetsToNuNu_HT-200to400', 95.5341),
            ('ZJetsToNuNu_HT-400to600', 13.1979),
            ('ZJetsToNuNu_HT-600to800', 3.14757),
            ('ZJetsToNuNu_HT-800to1200', 1.450908),
            ('ZJetsToNuNu_HT-1200to2500', 0.3546459),
            ('ZJetsToNuNu_HT-2500toInf', 0.00854235),
        ]),
#    ('single top',[
#        ('ST_s-channel', 3.34),
#        ('ST_t-channel_antitop', 80.95),
#        ('ST_t-channel_top', 136.02),
#        ('ST_tW_antitop', 19.4674),
#        ('ST_tW_top', 35.6),
#    ]),
#    ('gamma+jets',[
#        ('GJets_HT-100to200', 9226.0),
#        ('GJets_HT-200to400', 2300.0),
#        ('GJets_HT-400to600', 277.4),
#        ('GJets_HT-600toInf', 93.38),
#    ]),
])
if options.procs=="all":
    procs['$\\PZprime \\to \\Pqdark\\Paqdark$'] = [
        ('SVJ_mZprime-3000_mDark-20_rinv-0.3_alpha-peak', xsecs["3000"]),
    ]

# signal groups
benchmarks = {"mZprime": 3100, "mDark": 20, "rinv": 0.3, "aDark": "peak"}
sig_vals = OrderedDict([
    ("mZprime", [2100, 3100, 4100]),
    ("mDark", [5, 20, 50, 100]),
    ("rinv", [0.1, 0.3, 0.6, 0.9]),
    ("aDark", ["low","peak","high"]),
])
sig_units = {"mZprime": "\\GeV", "mDark": "\\GeV", "rinv": "", "aDark": ""}
if options.procs.startswith("sig"):
    procsplit = options.procs.split('_')
    varied = procsplit[1]
    mZprime = int(procsplit[2])
    sig_captions = []
    for i,p in enumerate(sig_vals):
        if p==varied:
            varied_ind = i
            sig_captions.append("varying \\{} values".format(p))
            continue
        elif p=="mZprime": sig_vals[p] = [mZprime]
        else: sig_vals[p] = [benchmarks[p]]
        sig_captions.append("$\\{} = {}{}$".format(p,texVal(p,sig_vals[p][0]),sig_units[p]))
    sig_captions.insert(-1,"and")
    sigs = []
    varyAll(0,list(sig_vals.iteritems()),[],sigs)
    for point in sigs:
        procs['$\\{} = {}{}$'.format(varied,texVal(varied,point[varied_ind]),sig_units[varied])] = [
            ('_'.join(str(x) for x in ["SVJ"]+list(point)), xsecs[str(point[0])]),
        ]

kfactors = defaultdict(lambda: defaultdict(lambda: 1.0))
kfactors['QCD']['2016'] = 1.2
kfactors['QCD']['2017'] = 1.9
kfactors['QCD']['2018'] = 1.5

cutflow_inds = {
    'raw': 1,
    'rawrel': 1,
    'abs': 4,
    'rel': 7,
}
cutflow_ind = cutflow_inds[options.type]
multicol = 3 if options.type=='rawrel' and options.error else 2
for procname,proc in procs.iteritems():
    outDict["header1"] += " & " + "\\multicolumn{"+str(multicol)+"}{r}{"+procname+"}"
    
    # add up years
    samples = []
    for year,lumi in lumis.iteritems():
        for p in proc:
            if ("genMET-80" in p[0] and year!="2018") or ("genMET-150" in p[0] and year=="2018"):
                continue
            if "SVJ" in p[0]:
                year = "2017"
            if "ST_t-channel" in p[0] and year=="2018":
                year = "2017"
            xsec = p[1]*kfactors[procname][year]
            samples.append('{"'+p[0]+'_MC'+year+'",'+str(xsec)+','+str(lumi)+(',"'+convertName(p[0])+'_MC'+year+'"' if options.procs.startswith("sig") else "")+'}')

    # process cutflow lines (summed over years)
    cmd = """root -b -l -q 'CutflowSum.C("{}",{{{}}},1,{},{}{})'""".format(
        options.dir,
        ','.join(samples),
        options.prec,
        "1" if options.skipzeros else "0",
		',"{}",{{{}}}'.format(options.regionfile, ','.join('"{}"'.format(s) for s in signalRegions)) if len(options.regionfile)>0 else "",
    )
    if options.verbose: print(cmd)
    cutflow = filter(None,os.popen(cmd).read().split('\n'))
    started = False
    first = 0
    last = 0
    for line in cutflow:
        if (options.efficiency and "nEventProc" in line) or (not options.efficiency and "NormType" in line):
            started = True
        elif started and len(line)>0:
            splitline = filter(None,line.split(' '))
            if "PhiSpike" in splitline[0]: splitline[0] = "PhiSpikeVeto16"
            if splitline[0] not in outDict: continue
            outDict[splitline[0]] += " & \\colspace"+splitline[cutflow_ind]+(" & "+splitline[cutflow_ind+2] if options.error else "")
            if options.type=='rawrel': outDict[splitline[0]] += " & "+splitline[cutflow_ind+6]
            if first==0: first = float(splitline[cutflow_inds['raw']])
            elif splitline[0]==list(namesDict)[-1]: last = float(splitline[cutflow_inds['raw']])
    if options.efficiency:
        outDict["efficiency"] += " & \\multicolumn{"+str(multicol)+"}{r}{"+"{:.2g}".format(last/first*100)+"}"

wfile = open(options.outname,'w')

# caption
captions = {
    'raw': "Expected number of events for {:.1f}\\fbinv".format(sum([y for x,y in lumis.iteritems()])/1000.), # convert pbinv to fbinv
    'abs': "Absolute cumulative efficiencies in \%",
    'rel': "Relative efficiencies in \%",
}
captions['rawrel'] = captions['raw']+" (relative efficiency in \%)"
staterr_caption = " Only statistical uncertainties are shown."
eff_caption = " The line ``Efficiency [\%]'' is the absolute efficiency after the final selection."
region_caption = " The subsequent lines show the efficiency for each signal region, relative to the final selection."
if options.procs=="bkg" or options.procs=="all":
    caption = "\\caption{{{} for each step of the event selection process for the major background processes{}.{}{}{}}}".format(
        captions[options.type],
        " and benchmark signal model ($\\mZprime = 3000\\GeV$, $\\mDark = 20\\GeV$, $\\rinv = 0.3$, $\\aDark = \\aDarkPeak$)" if options.procs=="all" else "",
        staterr_caption if options.error else "",
        eff_caption if options.efficiency else "",
        region_caption if len(options.regionfile)>0 else "",
    )
else:
    caption = "\\caption{{{} for each step of the event selection process for signals with {}.{}{}{}}}".format(
        captions[options.type],
        ', '.join(sig_captions).replace("and,","and"),
        staterr_caption if options.error else "",
        eff_caption if options.efficiency else "",
        region_caption if len(options.regionfile)>0 else "",
    )

# preamble
coltype = "S"
if options.type=="rawrel":
    if options.error: coltype = "SP"
    else: coltype = "rP"
wfile.write("\\begin{table}[htb]\n")
wfile.write(caption+"\n")
wfile.write("\\begin{center}\n")
if options.small: wfile.write("{\\footnotesize\n")
wfile.write("\\begin{tabular}{M"+coltype*len(procs)+"}\n")
wfile.write("\\hline\n")

for key,val in outDict.iteritems():
    if key=="NormType" and not 'raw' in options.type: continue
    if key==list(signalRegions)[0]: wfile.write("\\hline\n")
    wfile.write(val+" \\\\"+"\n")
    if key=="header1" or (options.efficiency and key==list(namesDict)[-1]): wfile.write("\\hline\n")

# postamble
wfile.write("\\hline\n")
wfile.write("\\end{tabular}\n")
if options.small: wfile.write("}\n")
wfile.write("\\label{tab:sel-eff-"+options.outname.replace(".tex","")+"}\n")
wfile.write("\\end{center}\n")
wfile.write("\\end{table}\n")

wfile.close()
