#!/bin/bash

for YEAR in 2016 2017 2018 2018NoHEM 2018HEM; do
#	root -b -l -q 'plotTrigEff.C+("test/hist_trig_'$YEAR'.root","SingleMuon","'$YEAR'","trigDenom",{"trigNumerJetHT"},{"leadjetAK8pt","MTAK8"},1,1,10,"",{"png","eps"})'
#	root -b -l -q 'plotTrigEff.C+("test/hist_trig_'$YEAR'.root","SingleMuon","'$YEAR'","trigDenom",{"trigNumerJetHT"},{"MTAK8"},1,1,10,"Deta",{"png","eps"})'
	root -b -l -q 'plotTrigEff.C+("test/hist_trig_'$YEAR'.root","SingleMuon","'$YEAR'","trigDenom",{"trigNumerJetHT"},{"MTAK8"},1,1,10,"DetaHLTmatch",{"png","eps"})'
done
