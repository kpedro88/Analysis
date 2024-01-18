#!/bin/bash

FIX=rootfix
FIXDIR=${CMSSW_BASE}/${FIX}
ANADIR=${PWD}/${FIX}
TOOLS=${CMSSW_BASE}/config/toolbox/${SCRAM_ARCH}/tools/selected

mkdir $FIXDIR
cd $FIXDIR
lndir $ROOTSYS
cp -f ${ANADIR}/libGraf.so lib/
cp -f ${ANADIR}/root bin/
cp -f ${ANADIR}/root.exe bin/
cp -f ${ANADIR}/root_interface.xml ${TOOLS}/

scram setup root_interface

