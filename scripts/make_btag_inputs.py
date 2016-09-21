sets=[
['SMStt850','T2tt_850_100'],
['SMStt500','T2tt_500_325'],
['SMStt425','T2tt_425_325'],
['SMSqqqq1000','T1qqqq_1000_800'],
['SMSqqqq1400','T1qqqq_1400_100'],
['SMStttt1200','T1tttt_1200_800'],
['SMStttt1500','T1tttt_1500_100'],
['SMSbbbb1000','T1bbbb_1000_900'],
['SMSbbbb1500','T1bbbb_1500_100'],
]

for set in sets:
    wfile = open("input_btag_"+set[0]+".txt",'w')
    
    wfile.write("OPTION\n")
    wfile.write("string:printsuffix[btag_comp_"+set[0]+"]\n")
    wfile.write("vstring:extra_text[#color[6]{N_{jet} bin},N_{b-jet} bin,"+set[1]+"]\n")
    wfile.write("vint:extra_text_panels[0,0,1]\n")
    wfile.write("vstring:numers["+set[1]+"SF16]\n")
    wfile.write("vstring:denoms["+set[1]+"SF15]\n")
    
    wfile.write("\n")
    wfile.write("SET\n")
    wfile.write("hist\tdata\t"+set[1]+"SF16\ts:legname[SF16]\tc:color[kBlue]\ti:panel[1]\n")
    wfile.write("\tbase\text\t"+set[1]+"SF16\ts:extfilename[datacards/RA2bin_signal.root]\tvs:exthisto_in[RA2bin_"+set[0]+"]\tvs:exthisto_out[RA2bin]\n")
    wfile.write("hist\tdata\t"+set[1]+"SF15\ts:legname[SF15]\tc:color[kRed]\ti:panel[1]\n")
    wfile.write("\tbase\text\t"+set[1]+"SF15\ts:extfilename[datacards_btag2015/RA2bin_signal.root]\tvs:exthisto_in[RA2bin_"+set[0]+"]\tvs:exthisto_out[RA2bin]\n")

    wfile.close()
