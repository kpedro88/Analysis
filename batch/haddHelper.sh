#!/bin/bash

NAME=$1
PARTS=$2
SUFF=$3

ALLFILES=""
for i in $(seq 0 $PARTS); do
  ALLFILES="${ALLFILES} ${NAME}_part${i}${SUFF}.root"
done

hadd ${NAME}${SUFF}.root ${ALLFILES}

for FILE in ${ALLFILES}; do
  rm ${FILE}
done
