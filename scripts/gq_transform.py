import numpy as np

class Coupling(object):
    def __init__(self, Nf, Nc, g=None, w=None, wtot=None):
        if g is not None and w is not None:
            raise ValueError("Specify g or w, not both")

        self.Nf = Nf
        self.Nc = Nc
        if g is not None:
            self.g = g
            self.w = self._w()
        else:
            self.w = w
            self.g = self._g()

        if wtot is not None:
            self.B = self._B(wtot)
        else:
            self.B = 0

    def _B(self, wtot):
        return self.w/wtot

    def _w(self):
        return self.Nf*self.Nc*(self.g**2)/(12*np.pi)

    def _g(self):
        return np.sqrt(self.w*(12*np.pi)/(self.Nf*self.Nc))

    def __repr__(self):
        return "g = {:.3g}, w = {:.2%}, B = {:.2%}".format(self.g, self.w, self.B)

class Model(object):
    def __init__(self, **kwargs):
        self.coup = {}

        cases = [
            ["gq","gchi","wtot"], # 1. specify gq & gchi; scale to wtot
            ["gq","gchi"], # 2. specify gq & gchi; no scaling
            ["wq","gchi"], # 3. specify wq & gchi
        ]
        casenum = 1+next(ic for ic,case in enumerate(cases) if sorted(case)==sorted(kwargs.keys()))

        # common to all cases
        self.coup["chi"] = Coupling(1,1,g=kwargs["gchi"])

        # gq cases
        gqargs = {}
        if casenum==3:
            gqargs["w"] = kwargs["wq"]
        else:
            gqargs["g"] = kwargs["gq"]
        self.coup["q"] = Coupling(6,3,**gqargs)

        # total width & branching
        self.wtot = self._wtot()
        self.coup["q"].B = self.coup["q"]._B(self.wtot)
        self.coup["chi"].B = self.coup["chi"]._B(self.wtot)

        # scaling
        if casenum==1:
            self.wtot = kwargs["wtot"]
            self.coup["q"] = Coupling(self.coup["q"].Nf,self.coup["q"].Nc,w=self.coup["q"].B*self.wtot,wtot=self.wtot)
            self.coup["chi"] = Coupling(self.coup["chi"].Nf,self.coup["chi"].Nc,w=self.coup["chi"].B*self.wtot,wtot=self.wtot)

    def _wtot(self):
        return self.coup["q"].w + self.coup["chi"].w

    def sigma(self, initial, final):
        wi = sum([self.coup[i].w for i in initial])
        wf = sum([self.coup[f].w for f in final])
        return wi*wf/self.wtot

    def __repr__(self):
        return '\n'.join(["wtot = {:.2%}".format(self.wtot),"Cq: {}".format(self.coup["q"]),"Cchi: {}".format(self.coup["chi"])])

# assumes gchi does not change between orig and new models
def newModel(case, limit, orig, verbose=True):
    allowed_cases = ["dijetqq","dijetall","monojet"]
    if not case in allowed_cases:
        raise ValueError("Unknown new model case: {}".format(case))

    b = 5./6. # branching fraction for dijetall case
    wchi = orig.coup["chi"].w
    f_str = "{f:.4g}".format(f=12*np.pi/(orig.coup["q"].Nf*orig.coup["q"].Nc))
    if case=="dijetqq":
        sigma = orig.sigma(["q"],["q"])
        rsigma = limit*sigma
        wq = rsigma/2*(1+np.sqrt(1+4*wchi/rsigma))
        if verbose:
            wq_str = "{a:.4g}*limit*(1+sqrt(1+{b:.4g}/limit))".format(a=sigma/2, b=4*wchi/sigma)
            gq_str = "sqrtpos({f}*{w},2.5)".format(f=f_str, w=wq_str)
            print(gq_str)
    elif case=="dijetall":
        sigma = b*orig.sigma(["q"],["q"])+orig.sigma(["q"],["chi"])
        rsigma = limit*sigma
        wq = rsigma/(2*b)*(1-wchi/rsigma+np.sqrt((1-wchi/rsigma)**2+4*b*wchi/rsigma))
        if verbose:
            wq_str = "{a:.4g}*limit*(1-{b:.4g}/limit+sqrt((1-{b:.4g}/limit)^2+{c:.4g}/limit))".format(a=sigma/(2*b), b=wchi/sigma, c=4*b*wchi/sigma)
            gq_str = "sqrtpos({f}*{w},2.5)".format(f=f_str, w=wq_str)
            print(gq_str)
    elif case=="monojet":
        sigma = orig.sigma(["q"],["chi"])
        rsigma = limit*sigma
        wq = rsigma*wchi/(wchi-rsigma)
        if verbose:
            wq_str = "{a:.4g}*limit/({b:.4g}-{c:.4g}*limit)".format(a=sigma*wchi, b=wchi, c=sigma)
            # use special sqrt function to avoid issues w/ negative args
            gq_str = "sqrtpos({f}*{w},2.5)".format(f=f_str, w=wq_str)
            print(gq_str)

    return Model(wq=wq,gchi=orig.coup["chi"].g)

# reference model
gq_ref = 0.25
gchi_ref = 1.0
ref = Model(gq=gq_ref,gchi=gchi_ref)
BRqq_ref = ref.coup["q"].B
print("ref:\n{}\n".format(ref))

# signal strength limit at mZp=3000
limit = 0.021692/0.015873688

new_dijetqq = newModel("dijetqq", limit, ref)
print("dijetqq:\n{}\n".format(new_dijetqq))

new_dijetall = newModel("dijetall", limit, ref)
print("dijetall:\n{}\n".format(new_dijetall))

# should use monojet or svj limit, not dijet
new_monojet = newModel("monojet", limit, ref)
print("monojet:\n{}\n".format(new_monojet))
