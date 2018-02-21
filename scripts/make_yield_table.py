import sys
from collections import defaultdict

samples = defaultdict(list)

steps = ["Preselection","Lepton veto","MET/MT > 0.15","deta < 1.1","dphi < 0.4"]
headers = ["cuts","SVJ_2000_20_0.3_0.2","zinv","wjets","ttbar","QCD","QCDHT"]

if len(sys.argv)<2:
	sys.exit(1)

name = sys.argv[1]

with open(name,'r') as file:
    for line in file:
        linesplit = line.split(' ')
        sname = linesplit[0][:-7]
        samples[sname].append(str(int(float(linesplit[1]))))

cols = []
for header in headers:
    if header not in samples and header!="cuts": continue
    coltmp = [header]
    coltmp.extend(steps if header=="cuts" else samples[header])
    cols.append(coltmp)

column_lengths = [max(len(entry) for entry in col) for col in cols]

for istep in range(len(cols[0])):
    line = ""
    for icol,col in enumerate(cols):
        line += " | {0:>{1}}".format(col[istep],column_lengths[icol])
    line += " |"
    print line
