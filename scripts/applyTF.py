import sys
from argparse import ArgumentParser, SUPPRESS

def applyTF(iname,hname,fname,ofile,ename="",snames="",prename="",rname=""):
    from ROOT import TFile, TH1, TF1
    file = TFile.Open(iname)
    hist = file.Get(hname)
    hist.SetDirectory(0)
    fn = file.Get(fname)
    if len(ename)>0:
        efn = file.Get(ename)
        for b in range(1,hist.GetNbinsX()+1):
            hist.SetBinContent(b,efn.Eval(hist.GetBinCenter(b)))
    hist.Multiply(fn)

    sets = []
    if len(snames)>0:
        rfile = TFile.Open(rname)
        sets = snames.split(',')
        hsets = []
        for set in sets:
            hsets.append(rfile.Get(prename+set))
            hsets[-1].SetDirectory(0)
        # compute R = (AD)/(CB)
        hsets[3].Multiply(hsets[1])
        hsets[2].Multiply(hsets[0])
        hsets[3].Divide(hsets[2])
        # pol1 fit to get R
        pol1 = TF1("pol1","pol1",hist.GetXaxis().GetXmin(),hist.GetXaxis().GetXmax())
        hsets[3].Fit(pol1,"N")
        hist.Multiply(pol1)

    ofile.cd()
    ftitle = fname.split("_")[0]
    tname = "pred_"+hname+"_"+ftitle
    hist.Write(tname)
    print(tname)

if __name__=="__main__":
    parser = ArgumentParser(add_help=False)
    parser.add_argument("-i","--input",type=str,required=True,help="input file name")
    parser.add_argument("-o","--output",type=str,default="",help="output file name")
    parser.add_argument("-h","--hist",type=str,required=True,help="histogram name")
    parser.add_argument("-f","--fit",type=str,nargs='+',help="fit function name(s)")
    parser.add_argument("-e","--extrap",type=str,default="",help="extrap function name")
    parser.add_argument("-s","--sets",type=str,default="",help="comma-separated list of sets (B,D,C,A) for correl")
    parser.add_argument("-p","--prefix",type=str,default="",help="prefix for correl")
    parser.add_argument("-r","--rfile",type=str,default="",help="correl input file name")
    parser.add_argument("--help",action="help",default=SUPPRESS,help="Show this help message and exit.")
    args = parser.parse_args()

    sys.argv = []

    from ROOT import TFile
    if len(args.output)==0:
        args.output = args.input.replace("tfs","applytfs")
    ofile = TFile.Open(args.output,"RECREATE")

    for fname in args.fit:
        applyTF(args.input,args.hist,fname,ofile,args.extrap,args.sets,args.prefix,args.rfile)
