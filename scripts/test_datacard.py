import os,sys,subprocess,glob,shlex
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter, RawDescriptionHelpFormatter, RawTextHelpFormatter
from itertools import izip

# from ConfigArgParse
class ArgumentDefaultsRawHelpFormatter(
    ArgumentDefaultsHelpFormatter,
    RawTextHelpFormatter,
    RawDescriptionHelpFormatter):
    pass

# make status messages useful
def fprint(msg):
    print(msg)
    sys.stdout.flush()

def pairwise(iterable):
    a = iter(iterable)
    return izip(a, a)

def make_command(card,region,param_line,num,no_syst):
    if no_syst:
        if param_line is not None: param_line["--freezeParameters"] += ",allConstrainedNuisances"
        else: param_line = {"--freezeParameters": "allConstrainedNuisances"}

    command = "combine -M AsymptoticLimits --name Test{} --keyword-value region={}".format(num,region)
    if param_line is not None: command = command + " " + ' '.join([k+' '+v for k,v in param_line.iteritems()])
    if no_syst:
        command += " --keyword-value syst=S0"
    command += " " + card
    return command

parser = ArgumentParser(formatter_class=ArgumentDefaultsRawHelpFormatter)
parser.add_argument("-d", "--dir", dest="dir", type=str, help="directory w/ cards and root files")
parser.add_argument("-r", "--region-group", dest="region_group", type=str, action='append', nargs='+', help="regions to combine: name region region ...")
parser.add_argument("-S", "--no-syst", dest="no_syst", default=False, action='store_true', help="disable systematics")
parser.add_argument("-n", "--num", dest="num", type=int, default=0, help="test number")
args = parser.parse_args()

if not os.path.isdir(args.dir):
    parser.error("Directory {} not found".format(args.dir))
os.chdir(args.dir)

for group in args.region_group:
    cards = []
    if len(group)==1: index = 0
    else: index = 1
    for region in group[index:]:
        possible_cards = glob.glob("*{}*.txt".format(region))
        if len(possible_cards)>1: raise RuntimeError("Found too many ({}) possible cards for region {}:".format(len(possible_cards),region)+'\n\t'.join(possible_cards))
        elif len(possible_cards)==0: raise RuntimeError("Found no possible cards for region {}:".format(region))
        card = possible_cards[0]
        cards.append(card)

        param_line = None
        with open(card,'r') as cfile:
            for line in cfile:
                # only check first line
                if "freezeParameters" in line: param_line = line[2:]
                break

        if param_line is not None:
            param_line = {k:v for k,v in pairwise(param_line.split())}

        command = make_command(card,region,param_line,args.num,args.no_syst)
        fprint(command)
        subprocess.check_call(shlex.split(command))

    # only combine if multiple regions given
    if index==1:
        dcfname = "datacard_{}.txt".format(group[0])
        command = "combineCards.py "+' '.join(cards)
        fprint(command+" > "+dcfname)
        with open(dcfname,'w') as outfile:
            subprocess.check_call(shlex.split(command),stdout=outfile)
        
        command = make_command(dcfname,group[0],param_line,args.num,args.no_syst)
        fprint(command)
        subprocess.check_call(shlex.split(command))
