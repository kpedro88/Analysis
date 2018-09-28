#!/bin/bash

export SAMPLES=(
JetHT_2018A1_block0 \
JetHT_2018A1_block1 \
JetHT_2018A1_block2 \
JetHT_2018A1_block3 \
JetHT_2018A1_block4 \
JetHT_2018A1_block5 \
JetHT_2018A1_block6 \
JetHT_2018A1_block7 \
JetHT_2018A1_block8 \
JetHT_2018A1_block9 \
JetHT_2018A1_block10 \
JetHT_2018A1_block11 \
JetHT_2018A1_block12 \
JetHT_2018A1_block13 \
JetHT_2018A1_block14 \
JetHT_2018A1_block15 \
JetHT_2018A1_block16 \
JetHT_2018A1_block17 \
JetHT_2018A1_block18 \
JetHT_2018A1_block19 \
JetHT_2018A1_block20 \
JetHT_2018A1_block21 \
JetHT_2018A1_block22 \
JetHT_2018A1_block23 \
JetHT_2018A1_block24 \
JetHT_2018A1_block25 \
MET_2018A1_block0 \
MET_2018A1_block1 \
MET_2018A1_block2 \
MET_2018A1_block3 \
MET_2018A1_block4 \
MET_2018A1_block5 \
MET_2018A1_block6 \
MET_2018A1_block7 \
MET_2018A1_block8 \
EGamma_2018A1_block0 \
EGamma_2018A1_block1 \
EGamma_2018A1_block2 \
EGamma_2018A1_block3 \
EGamma_2018A1_block4 \
EGamma_2018A1_block5 \
EGamma_2018A1_block6 \
EGamma_2018A1_block7 \
EGamma_2018A1_block8 \
EGamma_2018A1_block9 \
EGamma_2018A1_block10 \
EGamma_2018A1_block11 \
EGamma_2018A1_block12 \
EGamma_2018A1_block13 \
EGamma_2018A1_block14 \
EGamma_2018A1_block15 \
EGamma_2018A1_block16 \
EGamma_2018A1_block17 \
EGamma_2018A1_block18 \
EGamma_2018A1_block19 \
EGamma_2018A1_block20 \
EGamma_2018A1_block21 \
EGamma_2018A1_block22 \
EGamma_2018A1_block23 \
EGamma_2018A1_block24 \
EGamma_2018A1_block25 \
EGamma_2018A1_block26 \
EGamma_2018A1_block27 \
EGamma_2018A1_block28 \
EGamma_2018A1_block29 \
EGamma_2018A1_block30 \
EGamma_2018A1_block31 \
EGamma_2018A1_block32 \
EGamma_2018A1_block33 \
EGamma_2018A1_block34 \
EGamma_2018A1_block35 \
EGamma_2018A1_block36 \
EGamma_2018A1_block37 \
EGamma_2018A1_block38 \
EGamma_2018A1_block39 \
EGamma_2018A1_block40 \
EGamma_2018A1_block41 \
EGamma_2018A1_block42 \
EGamma_2018A1_block43 \
EGamma_2018A1_block44 \
EGamma_2018A1_block45 \
SingleMuon_2018A1_block0 \
SingleMuon_2018A1_block1 \
SingleMuon_2018A1_block2 \
SingleMuon_2018A1_block3 \
SingleMuon_2018A1_block4 \
SingleMuon_2018A1_block5 \
SingleMuon_2018A1_block6 \
SingleMuon_2018A1_block7 \
SingleMuon_2018A1_block8 \
SingleMuon_2018A1_block9 \
SingleMuon_2018A1_block10 \
SingleMuon_2018A1_block11 \
SingleMuon_2018A1_block12 \
SingleMuon_2018A1_block13 \
SingleMuon_2018A1_block14 \
SingleMuon_2018A1_block15 \
SingleMuon_2018A1_block16 \
SingleMuon_2018A1_block17 \
SingleMuon_2018A1_block18 \
SingleMuon_2018A1_block19 \
SingleMuon_2018A1_block20 \
SingleMuon_2018A1_block21 \
SingleMuon_2018A1_block22 \
SingleMuon_2018A1_block23 \
SingleMuon_2018A1_block24 \
SingleMuon_2018A1_block25 \
SingleMuon_2018A1_block26 \
SingleMuon_2018A1_block27 \
JetHT_2018A2_block0 \
JetHT_2018A2_block1 \
JetHT_2018A2_block2 \
JetHT_2018A2_block3 \
JetHT_2018A2_block4 \
MET_2018A2_block0 \
MET_2018A2_block1 \
EGamma_2018A2_block0 \
EGamma_2018A2_block1 \
EGamma_2018A2_block2 \
EGamma_2018A2_block3 \
EGamma_2018A2_block4 \
EGamma_2018A2_block5 \
EGamma_2018A2_block6 \
EGamma_2018A2_block7 \
SingleMuon_2018A2_block0 \
SingleMuon_2018A2_block1 \
SingleMuon_2018A2_block2 \
SingleMuon_2018A2_block3 \
SingleMuon_2018A2_block4 \
SingleMuon_2018A2_block5 \
JetHT_2018A3_block0 \
JetHT_2018A3_block1 \
JetHT_2018A3_block2 \
JetHT_2018A3_block3 \
JetHT_2018A3_block4 \
JetHT_2018A3_block5 \
JetHT_2018A3_block6 \
JetHT_2018A3_block7 \
JetHT_2018A3_block8 \
JetHT_2018A3_block9 \
JetHT_2018A3_block10 \
MET_2018A3_block0 \
MET_2018A3_block1 \
MET_2018A3_block2 \
MET_2018A3_block3 \
EGamma_2018A3_block0 \
EGamma_2018A3_block1 \
EGamma_2018A3_block2 \
EGamma_2018A3_block3 \
EGamma_2018A3_block4 \
EGamma_2018A3_block5 \
EGamma_2018A3_block6 \
EGamma_2018A3_block7 \
EGamma_2018A3_block8 \
EGamma_2018A3_block9 \
EGamma_2018A3_block10 \
EGamma_2018A3_block11 \
EGamma_2018A3_block12 \
EGamma_2018A3_block13 \
EGamma_2018A3_block14 \
EGamma_2018A3_block15 \
EGamma_2018A3_block16 \
SingleMuon_2018A3_block0 \
SingleMuon_2018A3_block1 \
SingleMuon_2018A3_block2 \
SingleMuon_2018A3_block3 \
SingleMuon_2018A3_block4 \
SingleMuon_2018A3_block5 \
SingleMuon_2018A3_block6 \
SingleMuon_2018A3_block7 \
SingleMuon_2018A3_block8 \
SingleMuon_2018A3_block9 \
SingleMuon_2018A3_block10 \
SingleMuon_2018A3_block11 \
SingleMuon_2018A3_block12 \
JetHT_2018B1_block0 \
JetHT_2018B1_block1 \
JetHT_2018B1_block2 \
JetHT_2018B1_block3 \
JetHT_2018B1_block4 \
JetHT_2018B1_block5 \
JetHT_2018B1_block6 \
JetHT_2018B1_block7 \
JetHT_2018B1_block8 \
JetHT_2018B1_block9 \
JetHT_2018B1_block10 \
JetHT_2018B1_block11 \
MET_2018B1_block0 \
MET_2018B1_block1 \
MET_2018B1_block2 \
MET_2018B1_block3 \
MET_2018B1_block4 \
EGamma_2018B1_block0 \
EGamma_2018B1_block1 \
EGamma_2018B1_block2 \
EGamma_2018B1_block3 \
EGamma_2018B1_block4 \
EGamma_2018B1_block5 \
EGamma_2018B1_block6 \
EGamma_2018B1_block7 \
EGamma_2018B1_block8 \
EGamma_2018B1_block9 \
EGamma_2018B1_block10 \
EGamma_2018B1_block11 \
EGamma_2018B1_block12 \
EGamma_2018B1_block13 \
EGamma_2018B1_block14 \
EGamma_2018B1_block15 \
EGamma_2018B1_block16 \
EGamma_2018B1_block17 \
EGamma_2018B1_block18 \
SingleMuon_2018B1_block0 \
SingleMuon_2018B1_block1 \
SingleMuon_2018B1_block2 \
SingleMuon_2018B1_block3 \
SingleMuon_2018B1_block4 \
SingleMuon_2018B1_block5 \
SingleMuon_2018B1_block6 \
SingleMuon_2018B1_block7 \
SingleMuon_2018B1_block8 \
SingleMuon_2018B1_block9 \
SingleMuon_2018B1_block10 \
SingleMuon_2018B1_block11 \
SingleMuon_2018B1_block12 \
SingleMuon_2018B1_block13 \
)
