#!/bin/bash

cd $CMSSW_BASE/src
git clone https://github.com/TreeMaker/TreeMaker -b Run2_2017
cd $CMSSW_BASE/src/TreeMaker/Production/python
scram b -j 8
cd $CMSSW_BASE/src/TreeMaker/Reflex
scram b -j 8
cd $CMSSW_BASE/lib/$SCRAM_ARCH/
mv TreeMakerReflex_xr.rootmap TreeMakerReflex.rootmap
mv TreeMakerReflex_xr_rdict.pcm TreeMakerReflex_rdict.pcm
