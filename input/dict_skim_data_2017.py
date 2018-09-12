periods = {
    "2017B": "Run2017B-31Mar2018-v1",
    "2017C": "Run2017C-31Mar2018-v1",
    "2017D": "Run2017D-31Mar2018-v1",
    "2017E": "Run2017E-31Mar2018-v1",
    "2017F": "Run2017F-31Mar2018-v1",
}

datasets = ["HTMHT","JetHT","MET","SingleElectron","SingleMuon","SinglePhoton"]

flist = []
for pshort,plong in sorted(periods.iteritems()):
    for dataset in datasets:
        flist.append([[plong+"."+dataset],[dataset+"_"+pshort]])
