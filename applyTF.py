import sys
from argparse import ArgumentParser, SUPPRESS

def applyTF(iname,hname,fname,ename="",oname=""):
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
    if len(oname)==0:
        oname = iname.replace("tfs","applytfs")
    ofile = TFile.Open(oname,"RECREATE")
    ofile.cd()
    cr = fname.split("/")[0]
    tname = "pred_"+hname
    hist.Write(tname)
    print(tname)

if __name__=="__main__":
    parser = ArgumentParser(add_help=False)
    parser.add_argument("-i","--input",type=str,required=True,help="input file name")
    parser.add_argument("-o","--output",type=str,default="",help="output file name")
    parser.add_argument("-h","--hist",type=str,required=True,help="histogram name")
    parser.add_argument("-f","--fit",type=str,required=True,help="fit function name")
    parser.add_argument("-e","--extrap",type=str,default="",help="extrap function name")
    parser.add_argument("--help",action="help",default=SUPPRESS,help="Show this help message and exit.")
    args = parser.parse_args()

    sys.argv = []

    applyTF(args.input,args.hist,args.fit,args.extrap,args.output)
