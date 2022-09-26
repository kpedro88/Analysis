#!/bin/bash

FIX=rootfix
FIXDIR=$CMSSW_BASE/$FIX
ANADIR=$PWD/$FIX
TOOLS=$CMSSW_BASE/config/toolbox/slc7_amd64_gcc530/tools/selected

mkdir $FIXDIR
cd $FIXDIR
lndir $ROOTSYS
cp -f $ANADIR/libHist.so lib/
cp -f $ANADIR/root bin/
cp -f $ANADIR/root.exe bin/
cp -f $ANADIR/root_interface.xml ${TOOLS}/

scram setup root_interface

