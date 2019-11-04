from collections import OrderedDict

selections = OrderedDict([
    ('Dijet', '\\multicolumn{2}{c}{$\\pt(j_{1,2}) > 200\\GeV$, $\\eta(j_{1,2}) < 2.4$}'),
    ('METMTRatio', '\\metmt & <0.15'),
    ('DeltaEta', '\\deta & <1.5'),
    ('MTAK8', '\\mt & <1500\\GeV'),
    ('MuonVeto', '\\nmuons & =0'),
    ('ElectronVeto', '\\neles & =0'),
    ('METFilter', '\\multicolumn{2}{c}{MET filters}'),
    ('PhiSpikeVeto16', '\\multicolumn{2}{c}{Phi spike filter}'),
    ('DeltaPhiMinAK8', '\\mindphi & <0.8'),
])
