def xsec_parse(xfile):
    xsec = {}
    for xline in xfile:
        values = xline.split('\t')
        if len(values) < 2: continue
        xsec[int(values[0])] = float(values[1])
    return xsec

def msplit(line):
    split = line.split('-')
    if len(split)==2: return int(split[1])
    else: return -1

def makeLineDCHist(name):
    return "hist" + "\t" + "mc" + "\t" + name + "\n"

def makeLineDCBase(name, fname, xsec, mother, full):
    return "\t" + "base" + "\t" + "mc" + "\t" + name + "\t" + "s:filename[tree_" + fname + ".root]" + "\t" + "d:xsection[" + str(xsec) + "]" + "\t" + "b:signal[1]" + ("" if full else "\t" + "b:fastsim[1]") + "\t" + "vi:mother[" + str(','.join(str(m) for m in mother)) + "]" + "\n"

def get_xsec(model,mMother):
    # get cross section
    if model.find("SVJ")!=-1:
        this_xsec = xsecs["SVJ"][mMother] if mMother in xsecs["SVJ"].keys() else 1
        mother_ID = [4900023]
    elif model.find("T2tt")!=-1:
        this_xsec = xsecs["T2"][mMother] if mMother in xsecs["T2"].keys() else 1
        mother_ID = [1000006]
    elif model.find("T2bb")!=-1:
        this_xsec = xsecs["T2"][mMother] if mMother in xsecs["T2"].keys() else 1
        mother_ID = [1000005]
    elif model.find("T2qq")!=-1:
        this_xsec = xsecs["T2qq"][mMother] if mMother in xsecs["T2qq"].keys() else 1
        mother_ID = [1000001,1000002,1000003,1000004,2000001,2000002,2000003,2000004]
    elif model.find("TChiHH")!=-1:
        this_xsec = xsecs["TChiHH"][mMother] if mMother in xsecs["TChiHH"].keys() else 1
        mother_ID = [1000023,1000025]
    else:
        this_xsec = xsecs["T1"][mMother] if mMother in xsecs["T1"].keys() else 1
        mother_ID = [1000021]
    return (this_xsec,mother_ID)

xsec_list = [("input/dict_xsec_T1_NNLO.txt","T1"), # taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVgluglu
             ("input/dict_xsec_T2_NNLO.txt","T2"), # taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVstopsbottom
             ("input/dict_xsec_T2qq_NNLO.txt","T2qq"), # taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVsquarkantisquark
             ("input/dict_xsec_TChiHH.txt","TChiHH"), # taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVhino
             ("input/dict_xsec_Zprime.txt","SVJ"), # taken from https://github.com/nhanvtran/dijets/blob/ee5b0de0f3958590d36f84ccf0266a4ce3415e52/limits/dijet.py#L301-L340
            ]

xsecs = {}

# parse xsec maps
for xsec_entry in xsec_list:
    with open(xsec_entry[0],'r') as xfile:
        xsecs[xsec_entry[1]] = xsec_parse(xfile)
