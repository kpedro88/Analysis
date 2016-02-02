from optparse import OptionParser

def makeSkimLine(short_name,full_name,fileMin,fileMax,mother,block=-1,data=False):
    return "base" + "\t" + "skim" + "\t" + short_name + ("_block"+str(block) + "\t" + "u:block[" + str(block) + "]" if block >= 0 else "") + "\t" + "b:chain[1]" + "\t" + "s:filepre[" + full_name + "_]" + "\t" + "vi:filerange[" + str(fileMin) + "," + str(fileMax) + "]" + "\t" + "s:filesuff[_RA2AnalysisTree.root]" + "\t" + "s:chainsuff[/TreeMaker2/PreSelection]" + ("\t"+"vi:mother["+str(','.join(str(m) for m in mother))+"]" if len(mother) > 0 else "") + ("\t"+"b:data[1]" if data else "") + "\n"

def makeSkimInput(read,write,nfiles=0,data=False):
    rfile = open(read,'r')
    wfile = open(write,'w')

    # preamble for file
    wfile.write("SET\n")

    tot = 0
    for rline in rfile:
        values = rline.split('\t')
        if len(values)<2: continue
        full_name = values[0].rstrip()
        short_name = values[1].rstrip()
        mother = []
        if len(values)>2: mother.append(int(values[2].rstrip()))
        readFilesImport = getattr(__import__("TreeMaker.Production." + full_name + "_cff",fromlist=["readFiles"]),"readFiles")
        fileListLen = len(readFilesImport)
        
        # check for splitting into blocks
        nblocks = 0
        if nfiles>0: nblocks = fileListLen/nfiles + int(fileListLen%nfiles!=0)
        
        if nblocks<2: # all files in one chain (either no splitting or nfiles < fileListLen)
            wline = makeSkimLine(short_name,full_name,0,fileListLen-1,mother,-1,data)
            wfile.write(wline)
        else: # split chains into several blocks
            for block in range(0,nblocks):
                fileMin = block*nfiles
                fileMax = min((block+1)*nfiles-1,fileListLen-1)
                wline = makeSkimLine(short_name,full_name,fileMin,fileMax,mother,block,data)
                wfile.write(wline)
        tot += len(readFilesImport)
    print "njobs: " + str(tot)
        
#####################################################################################################################

if __name__ == "__main__":
    # define options
    parser = OptionParser()
    parser.add_option("-r", "--read", dest="read", default="input/dict_skim.txt", help="input file to read")
    parser.add_option("-w", "--write", dest="write", default="input/input_sets_skim.txt", help="output file to write")
    parser.add_option("-n", "--nfiles", dest="nfiles", default=0, help="number of files per block for skim input")
    parser.add_option("-d", "--data", dest="data", default=False, action="store_true", help="denotes data file (instead of MC)")
    (options, args) = parser.parse_args()

    makeSkimInput(options.read,options.write,int(options.nfiles),options.data)
