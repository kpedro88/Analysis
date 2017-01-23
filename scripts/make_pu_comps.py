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

from ROOT import *

# print header
max_set_len = max([len(set) for set in sets])
num_col_len = 16
row = "| {0:<{1}} | ".format("Signal",max_set_len)
row += "{0:<{1}} | ".format("1 - Low/All [%]",num_col_len)
row += "{0:<{1}} | ".format("1 - High/All [%]",num_col_len)

print "Acceptance after baseline selection (normalized)"
print row

for set in sets:
    file = TFile.Open("plots/cutflows_PU_"+set+".root")
    h_all = file.Get("cutflowAbs_"+set)
    h_low = file.Get("cutflowAbs_"+set+"_LowPU")
    h_high = file.Get("cutflowAbs_"+set+"_HighPU")

    low_all = h_low.GetBinContent(h_low.GetNbinsX())/h_all.GetBinContent(h_all.GetNbinsX())
    high_all = h_high.GetBinContent(h_high.GetNbinsX())/h_all.GetBinContent(h_all.GetNbinsX())
    
    row = "| {0:<{1}} | ".format(set,max_set_len)
    row += "{0:>{1}.3f} | ".format((1-low_all)*100,num_col_len)
    row += "{0:>{1}.3f} | ".format((1-high_all)*100,num_col_len)
    print row
