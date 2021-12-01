from collections import OrderedDict

selections = OrderedDict([
    ('Dijet', '\\multicolumn{2}{c}{$\\pt(J_{1,2}) > 200\\GeV$, $\\eta(J_{1,2}) < 2.4$}'),
    ('METMTRatio', '\\metmt & >0.15'),
    ('DeltaEta', '\\deta & <1.5'),
    ('MTAK8', '\\mt & >1.5\\TeV'),
    ('MuonVeto', '\\nmuons & =0'),
    ('ElectronVeto', '\\neles & =0'),
    ('METFilter', '\\multicolumn{2}{c}{\\ptmiss filters}'),
    ('PhiSpikeVeto16', '\\DRphispike & >0.1'),
    ('GapJetVeto', '\\text{veto}~f_{\\gamma}(j_{1})>0.7 & \\&~\\pt(j_{1}) > 1.0\\TeV'),
    ('DeltaPhiMinAK8', '\\mindphi & <0.8'),
])
