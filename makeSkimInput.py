from optparse import OptionParser

def makeSkimInput(read,write):
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
        mother = 0
        if len(values)>2: mother = int(values[2].rstrip())
        readFilesImport = getattr(__import__("TreeMaker.Production." + full_name + "_cff",fromlist=["readFiles"]),"readFiles")
        wline = "base" + "\t" + "skim" + "\t" + short_name + "\t" + "b:chain[1]" + "\t" + "s:filepre[" + full_name + "_]" + "\t" + "vi:filerange[0," + str(len(readFilesImport)-1) + "]" + "\t" + "s:filesuff[_RA2AnalysisTree.root]" + "\t" + "s:chainsuff[/TreeMaker2/PreSelection]" + ("\t"+"i:mother["+str(mother)+"]" if mother > 0 else "") + "\n"
        wfile.write(wline)
        tot += len(readFilesImport)
    print "njobs: " + str(tot)
        
#####################################################################################################################

if __name__ == "__main__":
    # define options
    parser = OptionParser()
    parser.add_option("-r", "--read", dest="read", default="input/dict_skim.txt", help="input file to read")
    parser.add_option("-w", "--write", dest="write", default="input/input_sets_skim.txt", help="output file to write")
    (options, args) = parser.parse_args()

    makeSkimInput(options.read,options.write)
