import os
import math
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter

unc_names = {
    "isotrackunc": "Isolated track",
    "isrunc": "ISR",
    "JEC": "JEC",
    "JER": "JER",
    "jetidunc": "Jet ID",
    "lumiunc": "Luminosity",
    "MCStatErr": "MC statistical",
    "MHTSyst": "\\MHT modeling",
    "prefireunc": "Prefiring weight",
    "puunc": "Pileup reweighting",
    "scaleunc": "Scales",
    "trigunc": "Trigger statistical",
    "trigsystunc": "Trigger systematic",
# SVJ uncs below
    "pdfallunc": "PDF",
    "psfsrunc": "FSR (parton shower)",
    "psisrunc": "ISR (parton shower)",
    "trigfnunc": "Trigger statistical",
}

def getUncName(syst):
    if syst not in unc_names: return syst
    else: return unc_names[syst]

def getModel(file):
    # include year
    return '_'.join(file.split('_')[2:4])

def printSigFigs(num,fig,maxdec):
    # use scientific notation to get sig figs, then convert back to regular notation
    sci = "{0:>.{1}e}".format(num,fig-1)
    val = sci.split('e')[0]
    expo = int(sci.split('e')[1])
    pindex = val.find('.')
    if expo==0:
        result = val
    elif expo>0:
        if pindex==-1:
            result = val
        else:
            # move decimal point
            rval = val.replace('.','')
            pindex2 = pindex+expo
            # trailing zeroes
            if pindex2 >= len(rval): result = rval+'0'*(pindex2-len(rval))
            else: result = rval[0:pindex2]+'.'+rval[pindex2:]
    else: # expo<0
        rval = val if pindex==-1 else val.replace('.','')
        # add leading zeroes
        result = "0."+'0'*(abs(expo)-1)+rval
    # recursive truncation
    if '.' in result and len(result.split('.')[1])>maxdec:
        if fig==1: 
            rnum = round(num,abs(expo)-1)
            if rnum==0.0: return "0.0"
            else: return printSigFigs(rnum,fig,maxdec)
        else: return printSigFigs(num,fig-1,maxdec)
    else:
        if set(result.replace('.',''))==set([0]): return "0.0"
        else: return result

def findMinMax(fname,dir,exclude,include,cut,flat,output):
    model = getModel(fname)
    if not model in output.keys():
        output[model] = {}
    
    if dir[:6]=="/store": dir2 = "root://cmseos.fnal.gov/"+dir
    else: dir2 = dir
    file = TFile.Open(dir2+"/"+fname)
    if file == None: return
    tree = file.Get("tree")
    
    branches = [x.GetName() for x in list(tree.GetListOfBranches())]
    # everything else is a systematic
    if len(include)>0:
        branches = [b for b in branches if b in include]
        flat = {k:v for (k,v) in flat.iteritems() if k in include}
    else:
        branches = [b for b in branches if not b in exclude]
        flat = {k:v for (k,v) in flat.iteritems() if not k in exclude}
    branches.append("total")
    quadsum = "sqrt("

    # first handle flat uncs
    for unc,val in flat.iteritems():
        quadsum += str(val)+"^2+"
        output[model][unc] = [val,val]

    for branch in branches:
        qty = branch
        if branch=="total":
            if len(quadsum)>5:
                # change trailing + into )
                quadsum = quadsum[:-1]+")"
                tree.Draw(quadsum,cut,"goff")
                v1 = tree.GetV1(); v1.SetSize(tree.GetSelectedRows())
                tmin = min(v1)
                tmax = max(v1)
            # in case only "total" was specified
            else:
                tmin = 0
                tmax = 0
        else:
            # build up quadrature sum
            quadsum += branch+"^2+"
            if len(cut)>0:
                tree.Draw(branch,cut,"goff")
                v1 = tree.GetV1(); v1.SetSize(tree.GetSelectedRows())
                tmin = min(v1)
                tmax = max(v1)
            else:
                tmin = tree.GetMinimum(branch)
                tmax = tree.GetMaximum(branch)
        output[model][branch] = [tmin,tmax]
    
if __name__ == "__main__":
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument("-d", "--dir", dest="dir", type=str, default="/store/user/pedrok/SUSY2015/Analysis/Datacards/Run2ProductionV12", help="directory (LFN) of systematics files")
    parser.add_argument("-k", "--skip", dest="skip", type=str, default=[], nargs='*', help="list of models to skip")
    parser.add_argument("-n", "--include", dest="include", type=str, default=[], nargs='*', help="list of systematics to include in calculations (empty = all)")
    parser.add_argument("-x", "--exclude", dest="exclude", type=str, default=["contam"], help="list of systematics to exclude from calculations")
    parser.add_argument("-m", "--minimum", dest="minimum", type=float, default=0.01, help="minimum value to display, smaller values rounded to 0")
    parser.add_argument("-g", "--grep", dest="grep", type=str, default="", help="select only models matching this string")
    parser.add_argument("-c", "--cut", dest="cut", type=str, default="", help="apply cut when computing syst min/max")
    parser.add_argument("-p", "--prefix", dest="prefix", type=str, default="tree_syst", help="prefix for file names")
    parser.add_argument("-f", "--flat", dest="flat", type=str, default="", help="name of file containing supplemental dict of flat uncertainties and values")
    args = parser.parse_args()

    # get list of files (one per model)
    cmd = 'ls '+args.dir+' | grep '+args.prefix
    if args.dir[:6]=="/store": cmd = 'eos root://cmseos.fnal.gov/ '+cmd
    if len(args.grep)>0: cmd += ' | grep "'+args.grep+'"'
    files = sorted(filter(None,os.popen(cmd).read().split('\n')))
    
    from ROOT import *
    
    if len(args.exclude)==1 and args.exclude[0]=='': args.exclude = []
    if len(args.exclude)>0 and len(args.include)>0: parser.error("include and exclude can't be used together")

    if len(args.include)==0:
        args.exclude = ["mMother","mLSP","mZprime","mDark","rinv","alpha"] + args.exclude
    output = {}
    models = []

    # get flat unc list
    flat = {}
    if len(args.flat)>0: flat = getattr(__import__(args.flat,fromlist=["flat"]),"flat")
    
    # loop over models
    for file in files:
        model = getModel(file)
        if model in args.skip: continue
        models.append(model)
    
        findMinMax(file,args.dir,args.exclude,args.include,args.cut,flat,output)

    # transpose output (syst = rows, model = columns)
    output2 = {}
    for model in output.keys():
        for syst in output[model].keys():
            if syst not in output2.keys(): output2[syst] = {}
            output2[syst][model] = output[model][syst]
            
    # add overall
    for syst in output2.keys():
        tmin = min([val[0] for model,val in output2[syst].iteritems()])
        tmax = max([val[1] for model,val in output2[syst].iteritems()])
        output2[syst]["Overall"] = [tmin,tmax]

    # make table header
    sigfig = 2
    maxdec = int(abs(math.log10(args.minimum)))
    allModels = sorted(models)+["Overall"]
    col0 = "Systematic"
    columnLengths = [len(col0)]
    for im,model in enumerate(allModels):
        columnLengths.append(len(model))
    
    # get column widths
    rowslist = []
    for syst,vals in sorted(output2.iteritems(),key=lambda s: s[0].lower()):
        rowlist = [getUncName(syst)]
        columnLengths[0] = max(columnLengths[0],len(rowlist[0]))
        for im,model in enumerate(allModels):
            hasModel = (model in vals.keys())
            tmin = vals[model][0] if hasModel else 0
            tmax = vals[model][1] if hasModel else 0
            smin = printSigFigs(tmin,sigfig,maxdec)
            smax = printSigFigs(tmax,sigfig,maxdec)
            # don't bother to display a range if values are equal within precision
            if abs(tmax-tmin)>args.minimum and smin != smax:
                trange = smin+"--"+smax
            else:
                trange = smax
            columnLengths[im+1] = max(columnLengths[im+1],len(trange))
            rowlist.append(trange)
        rowslist.append(rowlist)
    
    # print header and syst ranges
    header = "| "+"{0:<{1}} | ".format(col0,columnLengths[0])
    for im,model in enumerate(allModels):
        header += "{0:<{1}} | ".format(model,columnLengths[im+1])        
    print header
    totalrow = ""    
    for rowlist in rowslist:
        syst = rowlist[0]
        row = "| "+"{0:<{1}} | ".format(syst,columnLengths[0])
        for im,rowitem in enumerate(rowlist[1:]):
            row += "{0:>{1}} | ".format(rowitem,columnLengths[im+1])
        if syst=="total":
            totalrow = row
            continue
        print row
    print totalrow
