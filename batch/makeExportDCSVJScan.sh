#!/bin/bash

SUFF=$1

cd $CMSSW_BASE/src/Analysis/batch
sed 's/_MC2018//' exportSkimSVJScan${SUFF}2018.sh > exportDCSVJScan${SUFF}Run2.sh
