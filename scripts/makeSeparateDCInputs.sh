#!/bin/bash

for i in T1tttt T1bbbb T2tt T2bb T1qqqq T5qqqqVV T2qq TChiHH_HToBB_HToBB T1tbtb; do
	echo $i
	python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV10/scan/ -n 10 -s $i -x $i
done

echo T1tbtbT1tbbbT1bbbb
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV10/scan/ -n 5 -s T1tbtb,T1tbbb,T1bbbb -w 0.25,0.5,0.25 -x T1tbtbT1tbbbT1bbbb
echo T1tbtbT1tbttT1tttt
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV10/scan/ -n 5 -s T1tbtb,T1tbtt,T1tttt -w 0.25,0.5,0.25 -x T1tbtbT1tbttT1tttt
echo T1ttbb
python makeFastInputMixed.py -d /store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV10/scan/ -n 2 -s T1tbtb,T1tbbb,T1tbtt,T1bbtt,T1bbbb,T1tttt -w 0.25,0.25,0.25,0.125,0.0625,0.0625 -x T1ttbb
