#!/bin/bash

cd $CMSSW_BASE/src/Analysis/batch
sed 's/_MC2018//' exportSkimSVJScan2018.sh > exportDCSVJScanRun2.sh
