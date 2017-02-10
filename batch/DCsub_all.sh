#!/bin/bash

./DCsub.sh -k -s exportSkip.sh

for i in T1tbtbT1tbbbT1bbbb T1tbtbT1tbttT1tttt T1ttbb; do
  ./DCsub.sh -k -x $i
done
