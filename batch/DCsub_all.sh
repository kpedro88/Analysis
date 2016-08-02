#!/bin/bash

for i in T1tttt T1bbbb T2tt T2bb T1qqqq T5qqqqVV T2qq; do
  ./DCsub.sh -k -x $i
done
