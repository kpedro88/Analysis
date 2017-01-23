#!/bin/bash

for i in T1tttt T1bbbb T2tt T2bb T1qqqq T5qqqqVV T2qq T1tbtb T1tbtbT1tbbbT1bbbb T1tbtbT1tbttT1tttt T1ttbb; do
  ./DCsub.sh -k -x $i
done
