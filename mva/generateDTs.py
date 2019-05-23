import sys

max_depth = int(sys.argv[1])

# "N" = node, "E" = leaf
# top-level can't be a leaf
params = [["N"]]
for i in range(1,max_depth):
    params.append(["N","E"])
# bottom-level must be a leaf
params.append(["E"])

def makeName(val,left,right):
    if val=="E":
        return (val,"DLeaf")
    else:
        return (val+left[0]+right[0],"DTree<"+left[1]+","+right[1]+">")

# implementation of recursive loop over any number of dimensions
# creates grid of all possible combinations of parameter values
def varyAll(pos,paramlist):
    vals = paramlist[pos]
    # check if last param
    if pos+1==len(paramlist):
        return [makeName(v,(),()) for v in vals]
    else:
        prims = varyAll(pos+1,paramlist)
        combos = set()
        for v in vals:
            for pL in prims:
                for pR in prims:
                    combos.add(makeName(v,pL,pR))
        return combos

fname = "generated_depth"+str(max_depth)
with open(fname+".h",'w') as ofile:
    preamble = [
        '#ifndef '+fname+'_h',
        '#define '+fname+'_h',
        '#include "DTree.h"',
        '#define DTREE_MAX_DEPTH '+str(max_depth),
        '#include "../KCode/KFactory.h"',
        '',
        'typedef KFactory<DTreeBase> KBDTFactory;',
        '#define REGISTER_BDT(a) REGISTER_MACRO(KBDTFactory,a)'
    ]
    ofile.write('\n'.join(preamble)+'\n\n')

    combos = varyAll(0,params)
    for p,c in sorted(combos):
        lines = [
            "template class "+c+";",
            "typedef "+c+" "+p+";",
            "REGISTER_BDT("+p+");",
        ]
        ofile.write("\n".join(lines)+'\n\n')
    ofile.write('#endif\n')
