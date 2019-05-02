#!/bin/bash

DIR=root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV16/Skims/tree_dijetmtmutrig/hist
OUTDIR=$CMSSW_BASE/src/Analysis/test

NAME=hist_trig_2016; hadd -f ${OUTDIR}/$NAME.root $DIR/${NAME}B.root $DIR/${NAME}C.root $DIR/${NAME}D.root $DIR/${NAME}E.root $DIR/${NAME}F.root $DIR/${NAME}G.root $DIR/${NAME}H.root
NAME=hist_trig_2017; hadd -f ${OUTDIR}/$NAME.root $DIR/${NAME}B.root $DIR/${NAME}C.root $DIR/${NAME}D.root $DIR/${NAME}E.root $DIR/${NAME}F.root
NAME=hist_trig_2018; hadd -f ${OUTDIR}/$NAME.root $DIR/${NAME}A.root $DIR/${NAME}B.root $DIR/${NAME}C.root $DIR/${NAME}D.root
NAME=hist_trig_2018NoHEM; hadd -f ${OUTDIR}/$NAME.root $DIR/${NAME}A.root $DIR/${NAME}B.root $DIR/${NAME}C.root $DIR/${NAME}D.root
NAME=hist_trig_2018HEM; hadd -f ${OUTDIR}/$NAME.root $DIR/${NAME}A.root $DIR/${NAME}B.root $DIR/${NAME}C.root $DIR/${NAME}D.root

