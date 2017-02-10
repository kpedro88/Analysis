#!/bin/bash

source batch/exportProd.sh

python makeFastInput.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan/
echo T1tbtbT1tbbbT1bbbb
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan/ -s T1tbtb,T1tbbb,T1bbbb -w 0.25,0.5,0.25 -x T1tbtbT1tbbbT1bbbb
echo T1tbtbT1tbttT1tttt
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan/ -s T1tbtb,T1tbtt,T1tttt -w 0.25,0.5,0.25 -x T1tbtbT1tbttT1tttt
echo T1ttbb
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan/ -s T1tbtb,T1tbbb,T1tbtt,T1bbtt,T1bbbb,T1tttt -w 0.25,0.25,0.25,0.125,0.0625,0.0625 -x T1ttbb
