import sys
from optparse import OptionParser

btypes = {
"vector<TLorentzVector>" : "VL",
"vector<double>" : "VD",
"vector<bool>": "VB",
"vector<int>": "VI",
"vector<vector<TLorentzVector>>": "VVL",
"vector<vector<bool>>": "VVB",
"vector<vector<int>>": "VVI",
"vector<vector<double>>": "VVD",
"Double_t": "D",
"Int_t": "I",
"Bool_t": "B",
}

def getBranchAndType(line):
    # divide into type and name
    linesplit = line.replace("> >",">>").split()
    btype = btypes[linesplit[0]]
    bname = linesplit[1].replace("*","").replace(";","")
    
    return ('KBranch'+btype+'(&looper->'+bname+',"'+bname+'")',btype)

# define options
parser = OptionParser()
parser.add_option("-n", "--ntuple", dest="ntuple", default="", help="location of NtupleClass.h")
(options, args) = parser.parse_args()

if len(options.ntuple)==0:
    raise ValueError("must specify NtupleClass.h")

v_clean = []
v_jet_scalar = []
v_jet_vector = []

p_clean = []
p_jet_scalar = []
p_jet_vector = []

with open(options.ntuple,'r') as f:
    start = False
    for line in f:
        # check for start of branch list
        if "Declaration of leaf types" in line:
            start = True
            continue
        if start:
            if "clean" in line or "Clean" in line: v_clean.append(line)
            if "JECup" in line and "vector" not in line: v_jet_scalar.append(line)
            if "*Jets_" in line and "origIndex" not in line: v_jet_vector.append(line)
            if "List of branches" in line: break

for line in v_clean:
    branchClean,btype = getBranchAndType(line)
    
    branchOrig = branchClean.replace("clean","").replace("Clean","")
    
    p_clean.append('new KLinkedBranch'+btype+'('+branchOrig+','+branchClean+'),')
    
for line in v_jet_scalar:
    branchJECup,btype = getBranchAndType(line)
    
    branchNull = 'KBranch'+btype+'(NULL,"")'
    branchJECdown = branchJECup.replace("JECup","JECdown")
    branchJERup = branchJECup.replace("JECup","JERup")
    branchJERdown = branchJECup.replace("JECup","JERdown")
    branchOrig = branchJECup.replace("JECup","")
    
    p_jet_scalar.append('new KLinkedBranch'+btype+'('+branchOrig+',vtype==JECup?'+branchJECup+':vtype==JECdown?'+branchJECdown+':vtype==JERup?'+branchJERup+':vtype==JERdown?'+branchJERdown+':'+branchNull+'),')
    
for line in v_jet_vector:
    branch,btype = getBranchAndType(line)
    
    p_jet_vector.append('new KReorderedBranch'+btype+'('+branch+',order),')

print("clean:\n"+"\n".join(p_clean)+"\n")
print("jet scalar:\n"+"\n".join(p_jet_scalar)+"\n")
print("jet vector:\n"+"\n".join(p_jet_vector))
