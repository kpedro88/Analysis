periods = {
    "2016B": "Run2016B-17Jul2018_ver2-v1",
    "2016C": "Run2016C-17Jul2018-v1",
    "2016D": "Run2016D-17Jul2018-v1",
    "2016E": "Run2016E-17Jul2018-v1",
    "2016F": "Run2016F-17Jul2018-v1",
    "2016G": "Run2016G-17Jul2018-v1",
    "2016H": "Run2016H-17Jul2018-v1",
}

datasets = ["HTMHT","JetHT","MET","SingleElectron","SingleMuon","SinglePhoton"]

flist = []
for pshort,plong in sorted(periods.iteritems()):
    for dataset in datasets:
        plong2 = plong
        if (dataset=="JetHT" and pshort=="2016B") or (dataset=="MET" and pshort=="2016H"): plong2 = plong2.replace("v1","v2")
        flist.append([[plong2+"."+dataset],[dataset+"_"+pshort]])
