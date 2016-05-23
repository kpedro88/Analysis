import sys, os, stat
from optparse import OptionParser
import imp

def makeSkimLine(short_name,full_names,file_mins,file_maxs,mothers,block=-1,data=False):
    the_name = short_name + ("_block"+str(block) if block >= 0 else "")
    line = "base" + "\t" + "skim" + "\t" + the_name + ("\t" + "u:block[" + str(block) + "]" if block >= 0 else "") + "\t" + "b:chain[1]" + "\t"
    line += "ch:filenames[" + str(','.join(full_names[i]+"_,"+str(file_mins[i])+","+str(file_maxs[i])+","+"_RA2AnalysisTree.root" for i, x in enumerate(full_names))) + "]" + "\t" 
    line += "s:chainsuff[/TreeMaker2/PreSelection]" + ("\t"+"vi:mother["+str(','.join(str(m) for m in mothers))+"]" if len(mothers) > 0 else "") + ("\t"+"b:data[1]" if data else "") + "\n"
    expline = the_name + " \\\n"
    return (line,expline)

def makeSkimInput(read,write,export,nfiles=0,data=False):
    readname = read.replace(".py","").split("/")[-1]
    rfile = imp.load_source(readname,read)
    wfile = open(write,'w')
    efile = open(export,'w')

    # preamble for file
    wfile.write("SET\n")
    
    # preamble for script to export array of sample names
    efile.write("#!/bin/bash\n")
    efile.write("\n")
    efile.write("export SAMPLES=(\n")

    totfiles = 0
    totjobs = 0
    for rline in rfile.flist:
        full_names = rline[0]
        file_lens = []
        file_mins = []
        file_maxs = []
        short_name = rline[1][0]
        mother = []
        if len(rline[1])>1: mother = rline[1][1:]
        
        for full_name in full_names:
            readFilesImport = getattr(__import__("TreeMaker.Production." + full_name + "_cff",fromlist=["readFiles"]),"readFiles")
            fileListLen = len(readFilesImport)
            file_lens.append(fileListLen)
            file_mins.append(0)
            file_maxs.append(fileListLen-1)
            totfiles += fileListLen
        
        # check for splitting into blocks
        fileListLen = sum(file_lens)
        nblocks = 0
        if nfiles>0: nblocks = fileListLen/nfiles + int(fileListLen%nfiles!=0)
        
        if nblocks<2: # all files in one chain (either no splitting or nfiles < fileListLen)
            wline,eline = makeSkimLine(short_name,full_names,file_mins,file_maxs,mother,-1,data)
            wfile.write(wline)
            efile.write(eline)
        else: # split chains into several blocks
            #make list of file indices and part numbers
            indices = []
            pnumbers = []
            for ind, full_name in enumerate(full_names):
                indices.extend([ind]*file_lens[ind])
                pnumbers.extend(range(0,file_lens[ind]))
        
            for block in range(0,nblocks):
                fileMin = block*nfiles
                fileMax = min((block+1)*nfiles-1,fileListLen-1)
                #check lists
                indexMin = indices[fileMin]
                indexMax = indices[fileMax]
                pnumberMin = pnumbers[fileMin]
                pnumberMax = pnumbers[fileMax]
                #check for crossover between two samples
                if indexMin == indexMax: wline,eline = makeSkimLine(short_name,[full_names[indexMin]],[pnumberMin],[pnumberMax],mother,block,data)
                else: wline,eline = makeSkimLine(short_name,[full_names[indexMin],full_names[indexMax]],[pnumberMin,0],[file_lens[indexMin]-1,pnumberMax],mother,block,data)
                wfile.write(wline)
                efile.write(eline)
        print short_name + " : nfiles = " + str(fileListLen) + ", njobs = " + str(max(nblocks,1))
        totjobs += max(nblocks,1)
    print "total nfiles = " + str(totfiles) + ", total njobs = " + str(totjobs)
    
    # make the script executable
    st = os.stat(efile.name)
    os.chmod(efile.name, st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

    # close files
    wfile.close()
    efile.write(")\n")
    efile.close()
    
#####################################################################################################################

if __name__ == "__main__":
    # define options
    parser = OptionParser()
    parser.add_option("-r", "--read", dest="read", default="input/dict_skim.txt", help="input file to read")
    parser.add_option("-w", "--write", dest="write", default="input/input_sets_skim.txt", help="output file to write")
    parser.add_option("-e", "--export", dest="export", default="batch/exportSkim.sh", help="export file to write")
    parser.add_option("-n", "--nfiles", dest="nfiles", default=0, help="number of files per block for skim input")
    parser.add_option("-d", "--data", dest="data", default=False, action="store_true", help="denotes data file (instead of MC)")
    (options, args) = parser.parse_args()

    makeSkimInput(options.read,options.write,options.export,int(options.nfiles),options.data)
