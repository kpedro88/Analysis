def DtoF(hD):
    from ROOT import TH1F
    hF = TH1F()
    hD.Copy(hF)
    return hF

def splitRegionsSVJ(iname):
    from ROOT import TFile, TH2, TH1

    f = TFile.Open(iname)
    names = [k.GetName() for k in f.GetListOfKeys()]

    outhists = []
    for name in names:
        hbase = f.Get(name)
        # split into individual BDT categories
        for nsvj in [0,1,2]:
            hname = name.replace("_nsvj","")+str(nsvj)
            htemp = hbase.ProjectionX(hname,nsvj+1,nsvj+1)
            htempF = DtoF(htemp)
            outhists.append(htempF)
        # aggregate into cut-based category
        if "-BDT" in name:
            hname = name.replace("_nsvj","").replace("-BDT","-Cut")
            htemp = hbase.ProjectionX(hname,1,3)
            htempF = DtoF(htemp)
            outhists.append(htempF)

    oname = iname.replace(".root","_split.root")
    fout = TFile.Open(oname,"RECREATE")
    for h in outhists:
        h.Write()
    fout.Close()

if __name__=="__main__":
    import sys
    iname = sys.argv[1]
    splitRegionsSVJ(iname)
