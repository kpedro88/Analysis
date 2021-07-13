import sys,os,stat
year = sys.argv[1]
input = "input/input_hist_svj_bkg_{}.txt".format(year)
output = input.replace("_hist_","_hp_sets_")
export = "batch/exportHistSVJbkgUnskimmed{}.sh".format(year)

kfactors = {
    "2016": 1.2,
    "2017": 1.9,
    "2018": 1.5,
}

with open(input,'r') as infile, open(output,'w') as outfile, open(export,'w') as expfile:
    outfile.write("SET\n")
    expfile.write("#!/bin/bash\n")
    expfile.write("\n")
    expfile.write("export SAMPLES=(\n")
    for line in infile:
        linesplit = line.rstrip().split('\t')
        if len(linesplit)<=1: continue
        outname = "s:outname[{}_MC{}]".format(linesplit[2].split('_')[0],year)
        settype = "hist"
        subtype = "mc"
        linesplit[1] = "mc"
        setname = linesplit[2]
        if "QCD" in setname: linesplit.append("d:kfactor[{}]".format(kfactors[year]))
        outfile.write('\t'.join([settype,subtype,setname,outname])+'\n')
        outfile.write('\t'+'\t'.join(linesplit)+'\n')
        expfile.write('\t'+setname+' \\\n')
    expfile.write(")\n")
    expfile.close()
    # make the script executable
    st = os.stat(export)
    os.chmod(export, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)
    print output, export
