from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
from array import array
import re

def getAxis(obj, axis):
    if axis=="x": return obj.GetXaxis()
    elif axis=="y": return obj.GetYaxis()
    elif axis=="z": return obj.GetZaxis()
    else: raise RuntimeError("Unknown axis: {}".format(axis))

def scaleGraph(obj, axis, scale):
    if obj.InheritsFrom("TGraph"):
        obj.Scale(scale,axis)

# based on https://root-forum.cern.ch/t/can-we-shift-histogram-for-several-channels/12050
def scaleHist(obj, axis, scale):
    if not obj.InheritsFrom("TH1"): return
    if not obj.InheritsFrom("TH2") and axis=="y": return
    axis = getAxis(obj, axis)
    if axis.GetXbins().GetSize():
        bins = axis.GetXbins()
        newbins = [b*scale for b in bins]
        binarray = array('d',newbins)
        axis.Set(len(binarray)-1, binarray)
    else:
        axis.Set(axis.GetNbins(),axis.GetXmin()*scale,axis.GetXmax()*scale)

def scaleDir(dir,ofile,args,first=False):
    objs = []
    for key in dir.GetListOfKeys():
        if args.verbose: print dir.GetName(),key.GetName()
        obj = dir.Get(key.GetName())
        if obj.InheritsFrom("TDirectory"): scaleDir(obj, ofile, args)
        else:
            # actually do the rescaling
            included = len(args.include)==0 or any([re.search(match,obj.GetName()) for match in args.include])
            excluded = len(args.exclude)>0 and any([re.search(match,obj.GetName()) for match in args.exclude])
            if included and not excluded:
                if (len(args.include)>0 or len(args.exclude)>0) and args.verbose: print "selected"
                for axis,scale in zip(args.axes,args.scales):
                    if obj.InheritsFrom("TGraph"): scaleGraph(obj, axis, scale)
                    elif obj.InheritsFrom("TH1"): scaleHist(obj, axis, scale)
            objs.append(obj)
    # recreate subdirectories
    if not first:
        newdir = ofile.mkdir(dir.GetName())
        newdir.cd()
    else:
        ofile.cd()
    # write this dir's contents
    for obj in objs:
        obj.Write()

if __name__=="__main__":
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter)
    parser.add_argument("-f", "--file", dest="file", type=str, required=True, help="input file")
    parser.add_argument("-a", "--axes", dest="axes", type=str, nargs="+", choices=["x","y","z"], help="axes to rescale")
    parser.add_argument("-s", "--scales", dest="scales", type=float, nargs="+", help="scales corresponding to axes")
    parser.add_argument("-x", "--suffix", dest="suffix", type=str, required=True, help="suffix for output file")
    parser.add_argument("-i", "--include", dest="include", type=str, nargs="*", default=[], help="include only objects with names matching (regex) provided strings")
    parser.add_argument("-e", "--exclude", dest="exclude", type=str, nargs="*", default=[], help="exclude objects with names matching (regex) provided strings")
    parser.add_argument("-v", "--verbose", dest="verbose", default=False, action="store_true", help="verbose printouts")
    args = parser.parse_args()

    # check args
    if len(args.axes)!=len(args.scales): parser.error("Axes and scales must be same length")
    for clude in [args.include, args.exclude]:
        clude = [re.compile(x) for x in clude]

    import ROOT as r
    ofname = args.file.replace(".root","_{}.root".format(args.suffix))
    infile = r.TFile.Open(args.file)
    outfile = r.TFile.Open(ofname,"RECREATE")
    # recurse through all directories
    scaleDir(infile,outfile,args,True)

