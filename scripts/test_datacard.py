import os,sys,subprocess,glob,shlex
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter, RawDescriptionHelpFormatter, RawTextHelpFormatter
from itertools import izip
from copy import deepcopy

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

def merge_dict(d1,d2):
    if d1 is None:
        if d2 is None:
            return
        elif d2 is not None:
            d1, d2 = d2, d1
    d3 = deepcopy(d1)
    if d2 is not None:
        for key in d2:
            if key in d3:
                d3[key] = d3[key]+","+d2[key]
            else:
                d3[key] = deepcopy(d2[key])
    return d3

def get_val(df,name,q):
    return df[abs(df['quantileExpected']-q)<0.001][name].item()

def get_param_strings(region,fninfo,source=None):
    order = fninfo[0]
    n = fninfo[1]
    params = ["{}_p{}_{}".format(region.replace("_2018",""),i+1,order) for i in range(n)]
    param_strings = {}
    param_strings["--trackParameters"] = ','.join(params)
    if source is not None:
        import uproot as up
        df = up.open(source)['limit'].pandas.df(['quantileExpected']+['trackedParam_'+p for p in params])
        param_strings["--setParameters"] = ','.join(['{}={}'.format(p, get_val(df, 'trackedParam_'+p, 0.5)) for p in params])
        param_strings["--freezeParameters"] = deepcopy(param_strings["--trackParameters"])
    return param_strings

def fsearch(fname,value):
    with open(fname,'r') as f:
        for line in f:
            if value in line: return True
    return False

def make_command(card,region,param_line,num,no_syst,extra,freeze_bkg=None,freeze_individual=False,regions=None):
    main_params = {
        "highCut": (4,5),
        "lowCut": (4,5),
        "highSVJ2": (1,2),
        "lowSVJ2": (1,2),
    }
    if no_syst:
        param_line = merge_dict(param_line, {"--freezeParameters": "allConstrainedNuisances"})
    if regions is None: regions = [region]
    has_fit = False
    for r in regions:
        # check for bkg fit
        if not fsearch(card,"flatParam"): continue
        else: has_fit = True
        # use params from combined fit when freezing constituents of combined region
        if freeze_bkg is not None and "{}" in freeze_bkg:
            freeze_tmp = freeze_bkg.format(r if freeze_individual else region)
        param_line = merge_dict(param_line, get_param_strings(r, main_params[r.replace("_2018","")], freeze_tmp))

    command = "combine -M AsymptoticLimits --name Test{} --keyword-value region={}".format(num,region)
    if param_line is not None: command = command + " " + ' '.join([k+' '+v for k,v in param_line.iteritems()])
    syst_val = ""
    if no_syst: syst_val += "S0"
    if has_fit and freeze_bkg is not None: syst_val += "B0"+("i" if freeze_individual else "")
    if len(syst_val)>0: command += " --keyword-value syst={}".format(syst_val)
    if extra is not None: command += " {}".format(extra)
    command += " " + card
    return command

parser = ArgumentParser(formatter_class=ArgumentDefaultsRawHelpFormatter)
parser.add_argument("-d", "--dir", dest="dir", type=str, help="directory w/ cards and root files")
parser.add_argument("-r", "--region-group", dest="region_group", type=str, action='append', nargs='+', help="regions to combine: name region region ...")
parser.add_argument("-S", "--no-syst", dest="no_syst", default=False, action='store_true', help="disable systematics")
parser.add_argument("-B", "--freeze-bkg", dest="freeze_bkg", default=None, type=str, help="name of input file to freeze bkg fit params ({} gets formatted with region)")
parser.add_argument("-i", "--freeze-individual", dest="freeze_individual", default=False, action='store_true', help="freeze bkg fit params for combination from individual region files")
parser.add_argument("-D", "--dry-run", dest="dry_run", default=False, action='store_true', help="dry run (print combine commands but don't execute)")
parser.add_argument("-n", "--num", dest="num", type=int, default=0, help="test number")
parser.add_argument("-x", "--extra", dest="extra", default=None, type=str, help="extra args for combine commands")
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

        command = make_command(card,region,param_line,args.num,args.no_syst,args.extra,args.freeze_bkg,args.freeze_individual)
        fprint(command)
        if not args.dry_run: subprocess.check_call(shlex.split(command))

    # only combine if multiple regions given
    if index==1:
        dcfname = "datacard_{}.txt".format(group[0])
        command = "combineCards.py "+' '.join(cards)
        fprint(command+" > "+dcfname)
        with open(dcfname,'w') as outfile:
            subprocess.check_call(shlex.split(command),stdout=outfile)

        command = make_command(dcfname,group[0],param_line,args.num,args.no_syst,args.extra,args.freeze_bkg,args.freeze_individual,group[index:])
        fprint(command)
        if not args.dry_run: subprocess.check_call(shlex.split(command))
