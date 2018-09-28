periods = {
    "2018A1": "Run2018A-PromptReco-v1",
    "2018A2": "Run2018A-PromptReco-v2",
    "2018A3": "Run2018A-PromptReco-v3",
    "2018B1": "Run2018B-PromptReco-v1",
}

datasets = ["JetHT","MET","EGamma","SingleMuon"]

flist = []
for pshort,plong in sorted(periods.iteritems()):
    for dataset in datasets:
        flist.append([[plong+"."+dataset],[dataset+"_"+pshort]])
