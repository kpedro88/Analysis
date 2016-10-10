#!/bin/bash

source exportProd.sh

./SKcheck.sh -k

SKIMDIR=/store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}/
ARGS="-k -s -g _part"

#check arguments
while getopts "n:ru" opt; do
  case "$opt" in
  n) ARGS="${ARGS} -n $OPTARG"
    ;;
  r) ARGS="${ARGS} -r"
    ;;
  u) ARGS="${ARGS} -u"
    ;;
  esac
done

#list folders
IFS=$'\n' DIRS=($(xrdfs root://cmseos.fnal.gov ls ${SKIMDIR})); unset IFS

#loop over hadd script for each dir
for DIR in ${DIRS[@]}; do
  echo ${DIR}
  ./HSsub.sh ${ARGS} -d ${DIR}
done

