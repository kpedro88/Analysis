import sys

year = sys.argv[1]

puname1_dict = {
    "2016": "corrections/PileupHistograms_0121_69p2mb_pm4p6.root",
    "2017": "corrections/PileupHistograms_0328_63mb_pm5.root",
    "2018": "corrections/PileupHistograms_0118_63mb_pm5.root",
    "2018Pre": "corrections/PileupHistograms_0118_63mb_pm5.root",
    "2018Post": "corrections/PileupHistograms_0118_63mb_pm5.root",
}

puname1 = puname1_dict[year]

from glob import glob
flist = glob("test/PileupHistograms_"+year+"_*.root")
flist.append("none")

plist = []
for f in flist:
    if f=="none": p = "none"
    else: p = f.split('_')[-2]
    plist.append(p)
    with open("input/input_putest_"+year+"_"+p+".txt",'w') as outfile:
        lines = [
            "OPTION",
            "string+:printsuffix[_putest_"+year+"_"+p+"]",
            "",
            "SELECTION",
            "MCWeightPU",
            "\tMCWeight\tb:puupdcorr[1]\ts:puname1["+puname1+"]\ts:puname2["+f+"]"
        ]
        if p=="none": lines[-1] = "\tMCWeight"
        outfile.write('\n'.join(lines))

print ' '.join(plist)
