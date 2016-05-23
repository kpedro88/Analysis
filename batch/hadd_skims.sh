#!/bin/bash

source exportProd.sh

./SKcheck.sh -k

SKIMDIR=/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}/
CHECKARGS="-s"

#check arguments
while getopts "n:r" opt; do
  case "$opt" in
  n) CHECKARGS="${CHECKARGS} -n $OPTARG"
    ;;
  r) CHECKARGS="${CHECKARGS} -r"
    ;;
  esac
done

#list folders
IFS=$'\n' DIRS=($(xrdfs root://cmseos.fnal.gov ls ${SKIMDIR} | grep "tree_")); unset IFS

#loop over hadd script for each dir
for DIR in ${DIRS[@]}; do
  echo ${DIR}
  ./HSsub.sh ${CHECKARGS} -d ${DIR}
done

