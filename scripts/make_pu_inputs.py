from makeFastCommon import *

sets=[
'T1tttt_1500_100_fast',
'T1tttt_1200_800_fast',
'T1bbbb_1500_100_fast',
'T1bbbb_1000_900_fast',
'T1qqqq_1400_100_fast',
'T1qqqq_1000_800_fast',
'T5qqqqVV_1400_100_fast',
'T5qqqqVV_1000_800_fast',
'T2tt_700_50_fast',
'T2tt_300_200_fast',
'T2bb_650_1_fast',
'T2bb_500_300_fast',
'T2qq_1000_100_fast',
'T2qq_700_400_fast',
]

for set in sets:
    setsp = set.split('_')
    xsec = get_xsec(setsp[0],int(setsp[1]))[0]
    with open("input/input_puset_"+set+".txt",'w') as wfile:        
        wfile.write("OPTION\n")
        wfile.write("string:printsuffix[pu_comp_"+set+"]\n")
        wfile.write("vstring:extra_text["+set+"]\n")
        wfile.write("vstring:denoms["+set+"]\n")
        wfile.write("vstring:numers["+set+"_LowPU,"+set+"_HighPU]\n")
        
        wfile.write("\n")
        wfile.write("SET\n")
        wfile.write("hist\tmc\t"+set+"_HighPU\ts:legname[High PU (NVtx #geq 20)]\tc:color[kRed]\ti:linestyle[2]\n")
        wfile.write("\tbase\tmc\t"+set+"_HighPU\ts:filename[tree_signalHighPU/tree_"+set+".root]\td:xsection["+str(xsec)+"]\tb:signal[1]\tb:fastsim[1]\n")
        wfile.write("hist\tmc\t"+set+"_LowPU\ts:legname[Low PU (NVtx < 20)]\tc:color[kBlue]\ti:linestyle[7]\n")
        wfile.write("\tbase\tmc\t"+set+"_LowPU\ts:filename[tree_signalLowPU/tree_"+set+".root]\td:xsection["+str(xsec)+"]\tb:signal[1]\tb:fastsim[1]\n")
        wfile.write("hist\tmc\t"+set+"\ts:legname[All]\tc:color[kBlack]\n")
        wfile.write("\tbase\tmc\t"+set+"\ts:filename[tree_signalAllPU/tree_"+set+".root]\td:xsection["+str(xsec)+"]\tb:signal[1]\tb:fastsim[1]\n")

