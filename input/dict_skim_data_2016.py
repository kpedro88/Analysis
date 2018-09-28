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
        flist.append([[plong+"."+dataset],[dataset+"_"+pshort]])
