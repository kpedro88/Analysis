
ref = open("data_12p9ifb.log",'r')
test = open("MT2_events_T1bbbb.txt",'r')

ref_dict = {}
for line in ref:
    if line[0:2] != "27": continue
    line_split = line.rstrip().split('\t')
    run = line_split[0]
    lumi = line_split[1]
    event = line_split[2]
    binnum = line_split[3]
    binname = line_split[4]
    ref_dict[(run,lumi,event)] = [binnum,binname]
    
for line in test:
    if line[1:3] != "27": continue
    line_split = filter(None,line.rstrip().replace("*","").split(' '))
    run = line_split[0]
    lumi = line_split[1]
    event = line_split[2]
    key = (run,lumi,event)
    if key in ref_dict:
        print run+"\t"+lumi+"\t"+event+"\t"+ref_dict[key][0]+"\t"+ref_dict[key][1]
    else:
        print run+"\t"+lumi+"\t"+event+"\t"+"NOT FOUND"
