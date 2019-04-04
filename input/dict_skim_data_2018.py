periods = {
    "2018A": "Run2018A-17Sep2018-v1",
    "2018B": "Run2018B-17Sep2018-v1",
    "2018C": "Run2018C-17Sep2018-v1",
    "2018D": ["Run2018D-PromptReco-v2","Run2018D-PromptReco-v2","Run2018D-22Jan2019-v2","Run2018D-22Jan2019-v2"],
}

datasets = ["JetHT","MET","EGamma","SingleMuon"]

flist = []
for pshort,plong in sorted(periods.iteritems()):
    for idata,dataset in enumerate(datasets):
        plong2 = plong
        if isinstance(plong,list): plong2 = plong[idata]
        flist.append([[plong2+"."+dataset],[dataset+"_"+pshort]])
