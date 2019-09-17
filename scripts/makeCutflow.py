import os, stat, sys
from optparse import OptionParser
from collections import OrderedDict

# avoid relative import
import sys
sys.path = [os.getcwd()] + sys.path
from makeFastCommon import *

def sig_callback(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(','))
    
# define options
parser = OptionParser()
parser.add_option("-d", "--dir", dest="dir", default="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV17/scan/supplementary/tree_signal/", help="location of root files (PFN)")
parser.add_option("-s", "--signals", dest="signals", type='string', action='callback', callback=sig_callback, help="list of signal model names")
parser.add_option("-o", "--outname", dest="outname", type='string', default="cutflow.tex", help="output TeX file name")
parser.add_option("-n", "--names", dest="names", type='string', default="selections", help="dict of fancy selection names, in order")
parser.add_option("-u", "--uncompressed", dest="uncompressed", action='store_true', default=False, help="specify uncompressed signals")
parser.add_option("-c", "--compressed", dest="compressed", action='store_true', default=False, help="specify compressed signals")
parser.add_option("-g", "--gluino", dest="gluino", action='store_true', default=False, help="specify gluino signals")
parser.add_option("-q", "--squark", dest="squark", action='store_true', default=False, help="specify squark signals")
(options, args) = parser.parse_args()

# check the signal models
if len(options.signals)==0:
    raise ValueError("no signal models specified!")
# check the exclusive options    
if options.uncompressed and options.compressed:
    parser.error("Can't use -u and -c together, pick one!")
if options.gluino and options.squark:
    parser.error("Can't use -g and -q together, pick one!")

namesDict = getattr(__import__(options.names,fromlist=["selections"]),"selections")

lumis = OrderedDict([
    ("2016", 35900),
    ("2017", 41500),
    ("2018", 21000),
    ("2018HEM", 38600),
])

num2words = {
0 : "zero",
1 : "one",
2 : "two",
3 : "three",
4 : "four",
5 : "five",
6 : "six",
7 : "seven",
8 : "eight",
9 : "nine",
10 : "ten"
}

outDict = OrderedDict([])
outDict["header1"] = "\\multicolumn{2}{c}{Selection}"
outDict["header2"] = "\\multicolumn{2}{c}{}"
outDict["header3"] = "\\multicolumn{2}{c}{}"

outDict = OrderedDict(list(outDict.items())+list(namesDict.items()))

for signal in options.signals:
    # signal name
    ssplit = signal.split('_')
    model = ssplit[0]
    mMother = ssplit[1]
    mLSP = ssplit[2]

    mother_ID = []
    # get cross section
    this_xsec, mother_ID = get_xsec(model,int(mMother))

    mothername = "\\PSg"
    mothername2 = "\\PSg\\PSg"
    decayname = ""
    if model=="T1tttt":     decayname = "\\ttbar"
    elif model=="T1bbbb":   decayname = "\\bbbar"
    elif model=="T1qqqq":   decayname = "\\qqbar"
    elif model=="T5qqqqVV": decayname = "\\qqbar \\text{V}"
    elif model=="T1tbtb":   decayname = "\\tbbar \\text{W}^{*-}"
    elif model=="T2tt":     decayname = "\\text{t}"; mothername = "\\sTop"; mothername2 = "\\sTop\\PASQt"
    elif model=="T2bb":     decayname = "\\text{b}"; mothername = "\\sBot"; mothername2 = "\\sBot\\PASQb"
    elif model=="T2qq":     decayname = "\\text{q}"; mothername = "\\sQua"; mothername2 = "\sQua\\PASQ"
    outDict["header1"] += " & " + "\\multicolumn{2}{r}{$\\text{p}\\text{p} \\to "+mothername2+", "+mothername+" \\to "+decayname+" \\PSGczDo$}"
    outDict["header2"] += " & " + "\\multicolumn{2}{r}{$m_{"+mothername+"}="+mMother+"\\GeV$}"
    outDict["header3"] += " & " + "\\multicolumn{2}{r}{$m_{\\PSGczDo}="+mLSP+"\\GeV$}"
    
    # add up years
    samples = []
    for year in lumis:
        samples.append('{"'+signal+'_MC'+year+'_fast",'+str(this_xsec)+','+str(lumis[year])+'}')

    # process cutflow lines (summed over years)
    cmd = "root -b -l -q 'CutflowSum.C("+'"'+options.dir+'",{'+','.join(samples)+'},1,1)'+"'"
    cutflow = filter(None,os.popen(cmd).read().split('\n'))
    started = False
    for line in cutflow:
        if "NEventProc" in line:
            started = True
        elif started and len(line)>0:
            splitline = filter(None,line.split(' '))
            if splitline[0] not in outDict: continue
            outDict[splitline[0]] += " & \\colspace"+splitline[4]+" & "+splitline[6]

wfile = open(options.outname,'w')

# caption
mothername = ""
caption = "\\caption{Absolute cumulative efficiencies in \% for each step of the event selection process, listed for "
caption += num2words[len(options.signals)]
caption += " representative "
if options.gluino:
    caption += "gluino pair production"
    mothername = "\\PSg"
elif options.squark:
    caption += "squark pair production"
    mothername = "\\sQua"
caption += " signal models with "
if options.compressed:
    caption += "${m_{"+mothername+"} \sim m_{\\PSGczDo}}$"
elif options.uncompressed:
    caption += "${m_{"+mothername+"} \gg m_{\\PSGczDo}}$"
caption += ". Only statistical uncertainties are shown.}"

# preamble
wfile.write("\\begin{table}[htb]\n")
wfile.write(caption+"\n")
wfile.write("\\begin{center}\n")
wfile.write("\\begin{tabular}{M"+'S'*len(options.signals)+"}\n")
wfile.write("\\hline\n")

for key,val in outDict.iteritems():
    wfile.write(val+" \\\\"+"\n")
    if key=="header3": wfile.write("\\hline\n")

# postamble    
wfile.write("\\hline\n")
wfile.write("\\end{tabular}\n")
wfile.write("\\label{tab:sel-eff-"+options.outname.replace(".tex","")+"}\n")
wfile.write("\\end{center}\n")
wfile.write("\\end{table}\n")

wfile.close()
