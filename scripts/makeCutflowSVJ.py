import os, stat, sys
from optparse import OptionParser
from collections import OrderedDict

# avoid relative import
import sys
sys.path = [os.getcwd()] + sys.path

# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17/scan/supplementary/tree_signal/", help="location of root files (PFN)")
parser.add_option("-o", "--outname", dest="outname", type='string', default="svj_cutflow.tex", help="output TeX file name")
parser.add_option("-n", "--names", dest="names", type='string', default="selections", help="dict of fancy selection names, in order")
parser.add_option("-t", "--type", dest="type", type='choice', default="abs", choices=['abs','rel','raw','rawrel'], help="type of cutflow (abs, rel, raw, rawrel)")
parser.add_option("-p", "--prec", dest="prec", type='int', default=1, help="numerical precision of output")
parser.add_option("-e", "--no-error", dest="error", default=True, action="store_false", help="don't display statistical errors")
parser.add_option("-v", "--verbose", dest="verbose", default=False, action="store_true", help="verbose output")
(options, args) = parser.parse_args()

namesDict = getattr(__import__(options.names,fromlist=["selections"]),"selections")

lumis = OrderedDict([
    ("2016", 35920),
    ("2017", 41530),
    ("2018", 59740),
])

outDict = OrderedDict([])
outDict["header1"] = "\\multicolumn{2}{c}{Selection}"
if 'raw' in options.type:
    outDict["NormType"] = "\\multicolumn{2}{c}{}"

outDict = OrderedDict(list(outDict.items())+list(namesDict.items()))
if 'raw' in options.type:
    outDict["efficiency"] = "\\multicolumn{2}{c}{Efficiency [\%]}"

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
    ('$\\PZprime \\to \\Pqdark\\Paqdark$',[
        ('SVJ_mZprime-3000_mDark-20_rinv-0.3_alpha-peak', .0155),
    ])
])

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
            samples.append('{"'+p[0]+'_MC'+year+'",'+str(p[1])+','+str(lumi)+'}')

    # process cutflow lines (summed over years)
    cmd = "root -b -l -q 'CutflowSum.C("+'"'+options.dir+'",{'+','.join(samples)+'},1,'+str(options.prec)+')'+"'"
    if options.verbose: print(cmd)
    cutflow = filter(None,os.popen(cmd).read().split('\n'))
    started = False
    first = 0
    last = 0
    for line in cutflow:
        if ('raw' in options.type and "NEventProc" in line) or ('raw' not in options.type and "NormType" in line):
            started = True
        elif started and len(line)>0:
            splitline = filter(None,line.split(' '))
            if "PhiSpike" in splitline[0]: splitline[0] = "PhiSpikeVeto16"
            if splitline[0] not in outDict: continue
            outDict[splitline[0]] += " & \\colspace"+splitline[cutflow_ind]+(" & "+splitline[cutflow_ind+2] if options.error else "")
            if options.type=='rawrel': outDict[splitline[0]] += " & "+splitline[cutflow_ind+6]
            if first==0: first = float(splitline[cutflow_ind])
            else: last = float(splitline[cutflow_ind])
    if 'raw' in options.type:
        outDict["efficiency"] += " & \\multicolumn{"+str(multicol)+"}{r}{"+"{:.2g}".format(last/first*100)+"}"

wfile = open(options.outname,'w')

# caption
captions = {
    'raw': "Expected number of events for {:.1f}\\fbinv".format(sum([y for x,y in lumis.iteritems()])/1000.), # convert pbinv to fbinv
    'abs': "Absolute cumulative efficiencies in \%",
    'rel': "Relative efficiencies in \%",
}
captions['rawrel'] = captions['raw']+" (relative efficiency in \%)"
caption = "\\caption{"+captions[options.type]+" for each step of the event selection process for the major background processes and benchmark signal model ($\\mZprime = 3000\\GeV$, $\\mDark = 20\\GeV$, $\\rinv = 0.3$, $\\aDark = \\aDarkPeak$)."+(" Only statistical uncertainties are shown." if options.error else "")+"}"

# preamble
coltype = "S"
if options.type=="rawrel":
    if options.error: coltype = "SP"
    else: coltype = "rP"
wfile.write("\\begin{table}[htb]\n")
wfile.write(caption+"\n")
wfile.write("\\begin{center}\n")
wfile.write("\\begin{tabular}{M"+coltype*len(procs)+"}\n")
wfile.write("\\hline\n")

for key,val in outDict.iteritems():
    wfile.write(val+" \\\\"+"\n")
    if key=="header1" or ('raw' in options.type and key==list(namesDict)[-1]): wfile.write("\\hline\n")

# postamble
wfile.write("\\hline\n")
wfile.write("\\end{tabular}\n")
wfile.write("\\label{tab:sel-eff-"+options.outname.replace(".tex","")+"}\n")
wfile.write("\\end{center}\n")
wfile.write("\\end{table}\n")

wfile.close()
