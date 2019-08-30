# python version of EventListFilter.h using set of tuples to represent events to be filtered
class EventListFilter(object):
    def __init__(self, files):
        self.formats = [int, int, long, int]
        self.eventList = set()
        for file in files:
            with open(file,'r') as input:
                # combine sets with union
                self.eventList |= set([tuple([self.formats[i](x) for i,x in enumerate(line.rstrip().split(":"))]) for line in input])

    def CheckEvent(self,*args):
        return tuple(args) not in self.eventList

# specialization for MC to deal with rounding of extra qty
class EventListFilterMC(EventListFilter):
    def CheckEvent(self,*args):
        tup = tuple(args)
        tupm = tuple([tup[0],tup[1],tup[2],tup[3]-1])
        tupp = tuple([tup[0],tup[1],tup[2],tup[3]+1])
        return not (tup in self.eventList or tupm in self.eventList or tupp in self.eventList)

