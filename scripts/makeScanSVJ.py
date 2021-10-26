import ROOT as r

with open("input/input_sets_DC_svj_scan_2016.txt",'r') as infile, open("input/input_sets_svj_scan.txt",'w') as outfile:
    for line in infile:
        if line.startswith("SET"):
            outfile.write(line)
        elif line.startswith("hist"):
            line = line.replace("_MC2016","")
            outfile.write(line)
        else:
            # fix filenames
            linesplit = line.split()
            fsplit = linesplit[3].split('_')
            linesplit[3] = "s:filename[SVJ_mZprime-{}_mDark-{}_rinv-{}_alpha-{}_MC2016_scan.root]".format(fsplit[2],fsplit[3],fsplit[4],fsplit[5])
            line = '\t'+'\t'.join(linesplit)+'\n'

            line2 = line.replace("MC2016","MC2017")
            line3 = line.replace("MC2016","MC2018")

            # get neventproc from tree
            neventproc = {}
            for year in [2016,2017,2018]:
                file = r.TFile.Open("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV17/scan/"+linesplit[3].replace("s:filename[","").replace("]","").replace("2016",str(year)))
                tree = file.Get("tree")
                neventproc[year] = tree.GetEntries()

            # use kfactor for lumi
            line = line.replace("\n","\ti:nEventProc[{}]".format(neventproc[2016])+"\td:kfactor[36330]\n")
            line2 = line2.replace("\n","\ti:nEventProc[{}]".format(neventproc[2017])+"\td:kfactor[41530]\n")
            line3 = line3.replace("\n","\ti:nEventProc[{}]".format(neventproc[2018])+"\td:kfactor[59740]\n")

            outfile.write(line)
            outfile.write(line2)
            outfile.write(line3)
