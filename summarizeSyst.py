import os
import subprocess
from itertools import izip
import array
from optparse import OptionParser

def list_callback(option, opt, value, parser):
    if value is None: return
    setattr(parser.values, option.dest, value.split(','))

def getModel(file):
    return file.split('_')[2]

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
    # range format: xx.xx--yy.yy
    allModels = sorted(models)+["Overall"]
    systRangeLength = 5+2+5
    col0 = "Systematic"
    systMaxLength = max([len(syst) for syst in output2.keys()])
    columnLengths = [max(len(col0),systMaxLength)]
    header = "| "+"{0:<{1}} | ".format(col0,columnLengths[0])
    for im,model in enumerate(allModels):
        columnLengths.append(systRangeLength)
        header += "{0:<{1}} | ".format(model,columnLengths[im+1])        
    print header
    
    # print syst ranges
    totalrow = ""
    for syst,vals in sorted(output2.iteritems(),key=lambda s: s[0].lower()):
        row = "| "+"{0:<{1}} | ".format(syst,columnLengths[0])
        for im,model in enumerate(allModels):
            hasModel = (model in vals.keys())
            tmin = vals[model][0] if hasModel else 0
            tmax = vals[model][1] if hasModel else 0
            # don't bother to display a range if values are within 0.01
            if abs(tmax-tmin)>0.01:
                trange = "{0:>.2f}--{1:>.2f}".format(tmin,tmax)
                row += "{0:>{1}} | ".format(trange,columnLengths[im+1])
            else:
                row += "{0:>{1}.2f} | ".format(tmax,columnLengths[im+1])
        if syst=="total":
            totalrow = row
            continue
        print row
    # make sure total is last
    print totalrow