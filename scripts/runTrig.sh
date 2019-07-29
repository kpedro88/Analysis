#!/bin/bash

source batch/exportProd.sh

DIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/${RUN2PRODV}/Skims/tree_dijetmtmutrig/hist/

#for YEAR in 2016 2017 2018 2018NoHEM 2018HEM; do
for YEAR in 2016 2017 2018; do
#	root -b -l -q 'plotTrigEff.C+("'$DIR'/hist_svj_trig'$YEAR'.root","SingleMuon","'$YEAR'","trigDenom",{"trigNumerJetHT"},{"leadjetAK8pt","MTAK8"},1,1,10,"",{"png","eps"})'
#	root -b -l -q 'plotTrigEff.C+("'$DIR'/hist_svj_trig'$YEAR'.root","SingleMuon","'$YEAR'","trigDenom",{"trigNumerJetHT"},{"MTAK8"},1,1,10,"Deta",{"png","eps"})'
#	root -b -l -q 'plotTrigEff.C+("'$DIR'/hist_svj_trig'$YEAR'.root","SingleMuon","'$YEAR'","trigDenom",{"trigNumerJetHT"},{"MTAK8"},1,1,10,"DetaHLTmatch",{"png","eps"})'
	root -b -l -q 'plotTrigEff.C+("'$DIR'/hist_svj_trig'$YEAR'.root","SingleMuon","'$YEAR'","trigDenom",{"trigNumerJetHT"},{"MTAK8"},1,1,10,"HighDetaHLTmatch",{"png","eps"})'
done
