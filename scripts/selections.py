from collections import OrderedDict

selections = OrderedDict([
    ("NJet"                 , "\\njets     & \geq2"),
    ("HT"                   , "\\HT        & >300\GeV"),
    ("MHT"                  , "\\MHT       & >300\GeV"),
    ("MHTHTRatio"           , "\\MHT/\\HT   & \leq1"),
    ("MuonVeto"             , "\\nmuons    & =0"),
    ("IsoMuonTrackVeto"     , "\\nisomuons & =0"),
    ("ElectronVeto"         , "\\neles     & =0"),
    ("IsoElectronTrackVeto" , "\\nisoeles  & =0"),
    ("IsoPionTrackVeto"     , "\\nisohads  & =0"),
    ("PhotonVeto"           , "\\nphos     & =0"),
    ("DeltaPhiJ1"           , "\\dphimht1  & >0.5"),
    ("DeltaPhiJ2"           , "\\dphimht2  & >0.5"),
    ("DeltaPhiJ3"           , "\\dphimht3  & >0.3"),
    ("DeltaPhiJ4"           , "\\dphimht4  & >0.3"),
    ("EventCleaning"        , "\\multicolumn{2}{c}{Event quality filter}")
])
