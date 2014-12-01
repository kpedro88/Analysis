DataBaseManager* dbm;
dbm = new DataBaseManager(); //for muon efficiency corrections
dbm->loadDb("muID","muIDTight.db");

//for each event
if(dbm) w *= dbm->getDBValue("muID",fabs(MuonEta),MuonPt);