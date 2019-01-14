periods = {
    "2018A": "Run2018A-17Sep2018-v1",
    "2018B": "Run2018B-17Sep2018-v1",
    "2018C": "Run2018C-17Sep2018-v1",
    "2018D": "Run2018D-PromptReco-v2",
}

datasets = ["JetHT","MET","EGamma","SingleMuon"]

flist = []
for pshort,plong in sorted(periods.iteritems()):
    for dataset in datasets:
        flist.append([[plong+"."+dataset],[dataset+"_"+pshort]])
