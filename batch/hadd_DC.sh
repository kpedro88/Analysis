#!/bin/bash

source exportProd.sh

DIR=/store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}

./SKcheck.sh -k

#initialize parameters
RUN=0
UPDATE=0
SUFFIX=""
ARGS="-g _part"

#check arguments
while getopts "rux:" opt; do
  case "$opt" in
  r) RUN=1
     ARGS="${ARGS} -r"
    ;;
  u) UPDATE=1
     ARGS="${ARGS} -u"
    ;;
  x) SUFFIX=$OPTARG
     # do NOT include suffix in args for haddEOS (used differently there)
    ;;
  esac
done

if [ -n "$SUFFIX" ]; then
  DIR=${DIR}/${SUFFIX}
fi

echo ${DIR}
ARGS="${ARGS} -d ${DIR}"

#if updating, part number may not start with 0
if [[ $UPDATE -eq 1 ]]; then
  IFS=$'\n' SAMPLES=($(python findHadds.py -d ${DIR} -g "_part")); unset IFS
else
  IFS=$'\n' SAMPLES=($(python findHadds.py -d ${DIR} -g "_part0")); unset IFS
fi

if [[ ${#SAMPLES[@]} -eq 0 ]]; then
  echo "nothing to hadd in $DIR"
  exit 0
fi

for BASE in ${SAMPLES[@]}; do
  ./haddEOS.sh ${ARGS} -i ${BASE}
done

