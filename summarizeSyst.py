import os
import math
from optparse import OptionParser

def list_callback(option, opt, value, parser):
    if value is None: return
    setattr(parser.values, option.dest, value.split(','))

def getModel(file):
    return file.split('_')[2]

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
            if pindex2 >= len(rval): result = rval
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

def findMinMax(fname,dir,exclude,output):
    model = getModel(fname)
    if not model in output.keys():
        output[model] = {}
    
    file = TFile.Open("root://cmseos.fnal.gov/"+dir+"/"+fname)
    if file == None: return
    tree = file.Get("tree")
    
    branches = [x.GetName() for x in list(tree.GetListOfBranches())]
    branches.append("total")
    # everything else is a systematic
    for excl in exclude:
        if excl in branches: branches.remove(excl)
    quadsum = "sqrt("
    for branch in branches:
        qty = branch
        if branch=="total":
            if len(quadsum)>5:
                # change trailing + into )
                quadsum = quadsum[:-1]+")"
                tree.Draw(quadsum,"","goff")
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
            tmin = tree.GetMinimum(branch)
            tmax = tree.GetMaximum(branch)
        output[model][branch] = [tmin,tmax]
    
if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-d", "--dir", dest="dir", default="/store/user/pedrok/SUSY2015/Analysis/Datacards/Run2ProductionV12", help="directory (LFN) of systematics files (default = %default)")
    parser.add_option("-k", "--skip", dest="skip", default=[], type="string", action="callback", callback=list_callback, help="comma-separated list of models to skip (default = %default)")
    parser.add_option("-x", "--exclude", dest="exclude", default=[], type="string", action="callback", callback=list_callback, help="comma-separated list of systematics to exclude from calculations (default = %default)")
    parser.add_option("-m", "--minimum", dest="minimum", default=0.01, help="minimum value to display, smaller values rounded to 0 (default = %default)")
    (options, args) = parser.parse_args()

    # get list of files (one per model)
    files = sorted(filter(None,os.popen('eos root://cmseos.fnal.gov/ ls '+options.dir+' | grep "tree_syst"').read().split('\n')))
    
    from ROOT import *
    
    options.exclude.insert(0,"mLSP")
    options.exclude.insert(0,"mMother")
    output = {}
    models = []
    
    # loop over models
    for file in files:
        model = getModel(file)
        if model in options.skip: continue
        models.append(model)
    
        findMinMax(file,options.dir,options.exclude,output)
    
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
    maxdec = int(abs(math.log10(options.minimum)))
    allModels = sorted(models)+["Overall"]
    col0 = "Systematic"
    systMaxLength = max([len(syst) for syst in output2.keys()])
    columnLengths = [max(len(col0),systMaxLength)]
    for im,model in enumerate(allModels):
        columnLengths.append(len(model))
    
    # get column widths
    rowslist = []
    for syst,vals in sorted(output2.iteritems(),key=lambda s: s[0].lower()):
        rowlist = [syst]
        for im,model in enumerate(allModels):
            hasModel = (model in vals.keys())
            tmin = vals[model][0] if hasModel else 0
            tmax = vals[model][1] if hasModel else 0
            smin = printSigFigs(tmin,sigfig,maxdec)
            smax = printSigFigs(tmax,sigfig,maxdec)
            # don't bother to display a range if values are equal within precision
            if abs(tmax-tmin)>options.minimum and smin != smax:
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
