#!/bin/bash

export SAMPLES=(
T1bbbb_MC2017_block0 \
T1bbbb_MC2017_block1 \
T1bbbb_MC2017_block2 \
T1bbbb_MC2017_block3 \
T1bbbb_MC2017_block4 \
T1bbbb_MC2017_block5 \
T1bbbb_MC2017_block6 \
T1bbbb_MC2017_block7 \
T1bbbb_MC2017_block8 \
T1bbbb_MC2017_block9 \
T1bbbb_MC2017_block10 \
T1bbbb_MC2017_block11 \
T1bbbb_MC2017_block12 \
T1bbbb_MC2017_block13 \
T1bbbb_MC2017_block14 \
T1bbbb_MC2017_block15 \
T1bbbb_MC2017_block16 \
T1bbbb_MC2017_block17 \
T1bbbb_MC2017_block18 \
T1bbbb_MC2017_block19 \
T1bbbb_MC2017_block20 \
T1bbbb_MC2017_block21 \
T1bbbb_MC2017_block22 \
T1bbbb_MC2017_block23 \
T1bbbb_MC2017_block24 \
T1bbbb_MC2017_block25 \
T1bbbb_MC2017_block26 \
T1bbbb_MC2017_block27 \
T1bbbb_MC2017_block28 \
T1bbbb_MC2017_block29 \
T1bbbb_MC2017_block30 \
T1bbbb_MC2017_block31 \
T1bbbb_MC2017_block32 \
T1bbbb_MC2017_block33 \
T1bbbb_MC2017_block34 \
T1bbbb_MC2017_block35 \
T1bbbb_MC2017_block36 \
T1bbbb_MC2017_block37 \
T1bbbb_MC2017_block38 \
T1bbbb_MC2017_block39 \
T1bbbb_MC2017_block40 \
T1bbbb_MC2017_block41 \
T1bbbb_MC2017_block42 \
T1bbbb_MC2017_block43 \
T1bbbb_MC2017_block44 \
T1bbbb_MC2017_block45 \
T1bbbb_MC2017_block46 \
T1bbbb_MC2017_block47 \
T1bbbb_MC2017_block48 \
T1bbbb_MC2017_block49 \
T1bbbb_MC2017_block50 \
T1bbbb_MC2017_block51 \
T1bbbb_MC2017_block52 \
T1bbbb_MC2017_block53 \
T1bbbb_MC2017_block54 \
T1bbbb_MC2017_block55 \
T1bbbb_MC2017_block56 \
T1bbbb_MC2017_block57 \
T1bbbb_MC2017_block58 \
T1bbbb_MC2017_block59 \
T1bbbb_MC2017_block60 \
T1bbbb_MC2017_block61 \
T1bbbb_MC2017_block62 \
T1bbbb_MC2017_block63 \
T1bbbb_MC2017_block64 \
T1bbbb_MC2017_block65 \
T1bbbb_MC2017_block66 \
T1bbbb_MC2017_block67 \
T1bbbb_MC2017_block68 \
T1bbbb_MC2017_block69 \
T1bbbb_MC2017_block70 \
T1bbbb_MC2017_block71 \
T1qqqq_MC2017_block0 \
T1qqqq_MC2017_block1 \
T1qqqq_MC2017_block2 \
T1qqqq_MC2017_block3 \
T1qqqq_MC2017_block4 \
T1qqqq_MC2017_block5 \
T1qqqq_MC2017_block6 \
T1qqqq_MC2017_block7 \
T1qqqq_MC2017_block8 \
T1qqqq_MC2017_block9 \
T1qqqq_MC2017_block10 \
T1qqqq_MC2017_block11 \
T1qqqq_MC2017_block12 \
T1qqqq_MC2017_block13 \
T1qqqq_MC2017_block14 \
T1qqqq_MC2017_block15 \
T1qqqq_MC2017_block16 \
T1qqqq_MC2017_block17 \
T1qqqq_MC2017_block18 \
T1qqqq_MC2017_block19 \
T1qqqq_MC2017_block20 \
T1qqqq_MC2017_block21 \
T1qqqq_MC2017_block22 \
T1qqqq_MC2017_block23 \
T1qqqq_MC2017_block24 \
T1qqqq_MC2017_block25 \
T1qqqq_MC2017_block26 \
T1qqqq_MC2017_block27 \
T1qqqq_MC2017_block28 \
T1qqqq_MC2017_block29 \
T1qqqq_MC2017_block30 \
T1qqqq_MC2017_block31 \
T1qqqq_MC2017_block32 \
T1qqqq_MC2017_block33 \
T1qqqq_MC2017_block34 \
T1qqqq_MC2017_block35 \
T1qqqq_MC2017_block36 \
T1qqqq_MC2017_block37 \
T1qqqq_MC2017_block38 \
T1qqqq_MC2017_block39 \
T1qqqq_MC2017_block40 \
T1qqqq_MC2017_block41 \
T1qqqq_MC2017_block42 \
T1qqqq_MC2017_block43 \
T1qqqq_MC2017_block44 \
T1qqqq_MC2017_block45 \
T1qqqq_MC2017_block46 \
T1qqqq_MC2017_block47 \
T1qqqq_MC2017_block48 \
T1qqqq_MC2017_block49 \
T1qqqq_MC2017_block50 \
T1qqqq_MC2017_block51 \
T1qqqq_MC2017_block52 \
T1qqqq_MC2017_block53 \
T1qqqq_MC2017_block54 \
T1qqqq_MC2017_block55 \
T1qqqq_MC2017_block56 \
T1qqqq_MC2017_block57 \
T1qqqq_MC2017_block58 \
T1qqqq_MC2017_block59 \
T1qqqq_MC2017_block60 \
T1qqqq_MC2017_block61 \
T1qqqq_MC2017_block62 \
T1qqqq_MC2017_block63 \
T1qqqq_MC2017_block64 \
T1qqqq_MC2017_block65 \
T1qqqq_MC2017_block66 \
T1qqqq_MC2017_block67 \
T1qqqq_MC2017_block68 \
T1qqqq_MC2017_block69 \
T1qqqq_MC2017_block70 \
T1qqqq_MC2017_block71 \
T1qqqq_MC2017_block72 \
T1qqqq_MC2017_block73 \
T1qqqq_MC2017_block74 \
T1qqqq_MC2017_block75 \
T1tttt_MC2017_block0 \
T1tttt_MC2017_block1 \
T1tttt_MC2017_block2 \
T1tttt_MC2017_block3 \
T1tttt_MC2017_block4 \
T1tttt_MC2017_block5 \
T1tttt_MC2017_block6 \
T1tttt_MC2017_block7 \
T1tttt_MC2017_block8 \
T1tttt_MC2017_block9 \
T1tttt_MC2017_block10 \
T1tttt_MC2017_block11 \
T1tttt_MC2017_block12 \
T1tttt_MC2017_block13 \
T1tttt_MC2017_block14 \
T1tttt_MC2017_block15 \
T1tttt_MC2017_block16 \
T1tttt_MC2017_block17 \
T1tttt_MC2017_block18 \
T1tttt_MC2017_block19 \
T1tttt_MC2017_block20 \
T1tttt_MC2017_block21 \
T1tttt_MC2017_block22 \
T1tttt_MC2017_block23 \
T1tttt_MC2017_block24 \
T1tttt_MC2017_block25 \
T1tttt_MC2017_block26 \
T1tttt_MC2017_block27 \
T1tttt_MC2017_block28 \
T1tttt_MC2017_block29 \
T1tttt_MC2017_block30 \
T1tttt_MC2017_block31 \
T1tttt_MC2017_block32 \
T1tttt_MC2017_block33 \
T1tttt_MC2017_block34 \
T1tttt_MC2017_block35 \
T1tttt_MC2017_block36 \
T1tttt_MC2017_block37 \
T1tttt_MC2017_block38 \
T1tttt_MC2017_block39 \
T1tttt_MC2017_block40 \
T1tttt_MC2017_block41 \
T1tttt_MC2017_block42 \
T1tttt_MC2017_block43 \
T1tttt_MC2017_block44 \
T1tttt_MC2017_block45 \
T1tttt_MC2017_block46 \
T1tttt_MC2017_block47 \
T1tttt_MC2017_block48 \
T1tttt_MC2017_block49 \
T1tttt_MC2017_block50 \
T1tttt_MC2017_block51 \
T1tttt_MC2017_block52 \
T1tttt_MC2017_block53 \
T1tttt_MC2017_block54 \
T1tttt_MC2017_block55 \
T1tttt_MC2017_block56 \
T1tttt_MC2017_block57 \
T1tttt_MC2017_block58 \
T1tttt_MC2017_block59 \
T1tttt_MC2017_block60 \
T1tttt_MC2017_block61 \
T1tttt_MC2017_block62 \
T1tttt_MC2017_block63 \
T1tttt_MC2017_block64 \
T1tttt_MC2017_block65 \
T1tttt_MC2017_block66 \
T1tttt_MC2017_block67 \
T1tttt_MC2017_block68 \
T1tttt_MC2017_block69 \
T1tttt_MC2017_block70 \
T1tttt_MC2017_block71 \
T1tttt_MC2017_block72 \
T1tttt_MC2017_block73 \
T1tttt_MC2017_block74 \
T1tttt_MC2017_block75 \
T1tttt_MC2017_block76 \
T1tttt_MC2017_block77 \
T1tttt_MC2017_block78 \
T1tttt_MC2017_block79 \
T1tttt_MC2017_block80 \
T1tttt_MC2017_block81 \
T1tttt_MC2017_block82 \
T1tttt_MC2017_block83 \
T1tttt_MC2017_block84 \
T1tttt_MC2017_block85 \
T1tttt_MC2017_block86 \
T1tttt_MC2017_block87 \
T2bb_MC2017_block0 \
T2bb_MC2017_block1 \
T2bb_MC2017_block2 \
T2bb_MC2017_block3 \
T2bb_MC2017_block4 \
T2bb_MC2017_block5 \
T2bb_MC2017_block6 \
T2bb_MC2017_block7 \
T2bb_MC2017_block8 \
T2bb_MC2017_block9 \
T2bb_MC2017_block10 \
T2bb_MC2017_block11 \
T2bb_MC2017_block12 \
T2bb_MC2017_block13 \
T2bb_MC2017_block14 \
T2bb_MC2017_block15 \
T2bb_MC2017_block16 \
T2bb_MC2017_block17 \
T2bb_MC2017_block18 \
T2bb_MC2017_block19 \
T2bb_MC2017_block20 \
T2bb_MC2017_block21 \
T2bb_MC2017_block22 \
T2bb_MC2017_block23 \
T2bb_MC2017_block24 \
T2bb_MC2017_block25 \
T2bb_MC2017_block26 \
T2bb_MC2017_block27 \
T2bb_MC2017_block28 \
T2bb_MC2017_block29 \
T2bb_MC2017_block30 \
T2bb_MC2017_block31 \
T2bb_MC2017_block32 \
T2bb_MC2017_block33 \
T2bb_MC2017_block34 \
T2bb_MC2017_block35 \
T2bb_MC2017_block36 \
T2bb_MC2017_block37 \
T2bb_MC2017_block38 \
T2bb_MC2017_block39 \
T2bb_MC2017_block40 \
T2bb_MC2017_block41 \
T2bb_mSbot-1650to2600_MC2017_block0 \
T2bb_mSbot-1650to2600_MC2017_block1 \
T2bb_mSbot-1650to2600_MC2017_block2 \
T2bb_mSbot-1650to2600_MC2017_block3 \
T2bb_mSbot-1650to2600_MC2017_block4 \
T2bb_mSbot-1650to2600_MC2017_block5 \
T2bb_mSbot-1650to2600_MC2017_block6 \
T2bb_mSbot-1650to2600_MC2017_block7 \
T2bb_mSbot-1650to2600_MC2017_block8 \
T2bb_mSbot-1650to2600_MC2017_block9 \
T2bb_mSbot-1650to2600_MC2017_block10 \
T2bb_mSbot-1650to2600_MC2017_block11 \
T2bb_mSbot-1650to2600_MC2017_block12 \
T2bb_mSbot-1650to2600_MC2017_block13 \
T2bb_mSbot-1650to2600_MC2017_block14 \
T2qq_MC2017_block0 \
T2qq_MC2017_block1 \
T2qq_MC2017_block2 \
T2qq_MC2017_block3 \
T2qq_MC2017_block4 \
T2qq_MC2017_block5 \
T2qq_MC2017_block6 \
T2qq_MC2017_block7 \
T2qq_MC2017_block8 \
T2qq_MC2017_block9 \
T2qq_MC2017_block10 \
T2qq_MC2017_block11 \
T2qq_MC2017_block12 \
T2qq_MC2017_block13 \
T2qq_MC2017_block14 \
T2qq_MC2017_block15 \
T2qq_MC2017_block16 \
T2qq_MC2017_block17 \
T2qq_MC2017_block18 \
T2qq_MC2017_block19 \
T2qq_MC2017_block20 \
T2qq_MC2017_block21 \
T2qq_MC2017_block22 \
T2qq_MC2017_block23 \
T2qq_MC2017_block24 \
T2qq_MC2017_block25 \
T2qq_MC2017_block26 \
T2qq_MC2017_block27 \
T2qq_MC2017_block28 \
T2qq_MC2017_block29 \
T2qq_MC2017_block30 \
T2qq_MC2017_block31 \
T2qq_MC2017_block32 \
T2qq_MC2017_block33 \
T2qq_MC2017_block34 \
T2qq_MC2017_block35 \
T2qq_MC2017_block36 \
T2qq_MC2017_block37 \
T2qq_MC2017_block38 \
T2qq_MC2017_block39 \
T2qq_MC2017_block40 \
T2qq_MC2017_block41 \
T2qq_MC2017_block42 \
T2qq_MC2017_block43 \
T2qq_MC2017_block44 \
T2qq_MC2017_block45 \
T2qq_MC2017_block46 \
T2qq_MC2017_block47 \
T2qq_MC2017_block48 \
T2qq_MC2017_block49 \
T2qq_MC2017_block50 \
T2qq_MC2017_block51 \
T2qq_MC2017_block52 \
T2qq_MC2017_block53 \
T2qq_MC2017_block54 \
T2qq_MC2017_block55 \
T2qq_MC2017_block56 \
T2qq_MC2017_block57 \
T2qq_MC2017_block58 \
T2qq_MC2017_block59 \
T2qq_MC2017_block60 \
T2qq_MC2017_block61 \
T2qq_MC2017_block62 \
T2qq_MC2017_block63 \
T2qq_MC2017_block64 \
T2qq_MC2017_block65 \
T2qq_mSq-1850to2600_MC2017_block0 \
T2qq_mSq-1850to2600_MC2017_block1 \
T2qq_mSq-1850to2600_MC2017_block2 \
T2qq_mSq-1850to2600_MC2017_block3 \
T2qq_mSq-1850to2600_MC2017_block4 \
T2qq_mSq-1850to2600_MC2017_block5 \
T2qq_mSq-1850to2600_MC2017_block6 \
T2qq_mSq-1850to2600_MC2017_block7 \
T2qq_mSq-1850to2600_MC2017_block8 \
T2qq_mSq-1850to2600_MC2017_block9 \
T2qq_mSq-1850to2600_MC2017_block10 \
T2qq_mSq-1850to2600_MC2017_block11 \
T2qq_mSq-1850to2600_MC2017_block12 \
T2tt_mStop-1200to2000_MC2017_block0 \
T2tt_mStop-1200to2000_MC2017_block1 \
T2tt_mStop-1200to2000_MC2017_block2 \
T2tt_mStop-1200to2000_MC2017_block3 \
T2tt_mStop-1200to2000_MC2017_block4 \
T2tt_mStop-1200to2000_MC2017_block5 \
T2tt_mStop-1200to2000_MC2017_block6 \
T2tt_mStop-1200to2000_MC2017_block7 \
T2tt_mStop-1200to2000_MC2017_block8 \
T2tt_mStop-1200to2000_MC2017_block9 \
T2tt_mStop-1200to2000_MC2017_block10 \
T2tt_mStop-1200to2000_MC2017_block11 \
T2tt_mStop-1200to2000_MC2017_block12 \
T2tt_mStop-1200to2000_MC2017_block13 \
T2tt_mStop-1200to2000_MC2017_block14 \
T2tt_mStop-1200to2000_MC2017_block15 \
T2tt_mStop-1200to2000_MC2017_block16 \
T2tt_mStop-1200to2000_MC2017_block17 \
T2tt_mStop-1200to2000_MC2017_block18 \
T2tt_mStop-1200to2000_MC2017_block19 \
T2tt_mStop-1200to2000_MC2017_block20 \
T2tt_mStop-1200to2000_MC2017_block21 \
T2tt_mStop-1200to2000_MC2017_block22 \
T2tt_mStop-1200to2000_MC2017_block23 \
T2tt_mStop-1200to2000_MC2017_block24 \
T2tt_mStop-1200to2000_MC2017_block25 \
T2tt_mStop-1200to2000_MC2017_block26 \
T2tt_mStop-1200to2000_MC2017_block27 \
T2tt_mStop-1200to2000_MC2017_block28 \
T2tt_mStop-1200to2000_MC2017_block29 \
T2tt_mStop-1200to2000_MC2017_block30 \
T2tt_mStop-1200to2000_MC2017_block31 \
T2tt_mStop-1200to2000_MC2017_block32 \
T2tt_mStop-1200to2000_MC2017_block33 \
T2tt_mStop-1200to2000_MC2017_block34 \
T2tt_mStop-150to250_MC2017_block0 \
T2tt_mStop-150to250_MC2017_block1 \
T2tt_mStop-150to250_MC2017_block2 \
T2tt_mStop-150to250_MC2017_block3 \
T2tt_mStop-150to250_MC2017_block4 \
T2tt_mStop-150to250_MC2017_block5 \
T2tt_mStop-150to250_MC2017_block6 \
T2tt_mStop-150to250_MC2017_block7 \
T2tt_mStop-150to250_MC2017_block8 \
T2tt_mStop-150to250_MC2017_block9 \
T2tt_mStop-150to250_MC2017_block10 \
T2tt_mStop-150to250_MC2017_block11 \
T2tt_mStop-150to250_MC2017_block12 \
T2tt_mStop-150to250_MC2017_block13 \
T2tt_mStop-150to250_MC2017_block14 \
T2tt_mStop-150to250_MC2017_block15 \
T2tt_mStop-150to250_MC2017_block16 \
T2tt_mStop-150to250_MC2017_block17 \
T2tt_mStop-150to250_MC2017_block18 \
T2tt_mStop-150to250_MC2017_block19 \
T2tt_mStop-150to250_MC2017_block20 \
T2tt_mStop-150to250_MC2017_block21 \
T2tt_mStop-150to250_MC2017_block22 \
T2tt_mStop-150to250_MC2017_block23 \
T2tt_mStop-150to250_MC2017_block24 \
T2tt_mStop-150to250_MC2017_block25 \
T2tt_mStop-150to250_MC2017_block26 \
T2tt_mStop-150to250_MC2017_block27 \
T2tt_mStop-150to250_MC2017_block28 \
T2tt_mStop-150to250_MC2017_block29 \
T2tt_mStop-150to250_MC2017_block30 \
T2tt_mStop-150to250_MC2017_block31 \
T2tt_mStop-150to250_MC2017_block32 \
T2tt_mStop-150to250_MC2017_block33 \
T2tt_mStop-150to250_MC2017_block34 \
T2tt_mStop-150to250_MC2017_block35 \
T2tt_mStop-150to250_MC2017_block36 \
T2tt_mStop-150to250_MC2017_block37 \
T2tt_mStop-150to250_MC2017_block38 \
T2tt_mStop-150to250_MC2017_block39 \
T2tt_mStop-150to250_MC2017_block40 \
T2tt_mStop-150to250_MC2017_block41 \
T2tt_mStop-150to250_MC2017_block42 \
T2tt_mStop-150to250_MC2017_block43 \
T2tt_mStop-150to250_MC2017_block44 \
T2tt_mStop-150to250_MC2017_block45 \
T2tt_mStop-150to250_MC2017_block46 \
T2tt_mStop-150to250_MC2017_block47 \
T2tt_mStop-150to250_MC2017_block48 \
T2tt_mStop-150to250_MC2017_block49 \
T2tt_mStop-150to250_MC2017_block50 \
T2tt_mStop-150to250_MC2017_block51 \
T2tt_mStop-150to250_MC2017_block52 \
T2tt_mStop-150to250_MC2017_block53 \
T2tt_mStop-150to250_MC2017_block54 \
T2tt_mStop-150to250_MC2017_block55 \
T2tt_mStop-150to250_MC2017_block56 \
T2tt_mStop-150to250_MC2017_block57 \
T2tt_mStop-150to250_MC2017_block58 \
T2tt_mStop-150to250_MC2017_block59 \
T2tt_mStop-150to250_MC2017_block60 \
T2tt_mStop-150to250_MC2017_block61 \
T2tt_mStop-150to250_MC2017_block62 \
T2tt_mStop-150to250_MC2017_block63 \
T2tt_mStop-150to250_MC2017_block64 \
T2tt_mStop-150to250_MC2017_block65 \
T2tt_mStop-150to250_MC2017_block66 \
T2tt_mStop-150to250_MC2017_block67 \
T2tt_mStop-150to250_MC2017_block68 \
T2tt_mStop-150to250_MC2017_block69 \
T2tt_mStop-150to250_MC2017_block70 \
T2tt_mStop-150to250_MC2017_block71 \
T2tt_mStop-150to250_MC2017_block72 \
T2tt_mStop-150to250_MC2017_block73 \
T2tt_mStop-150to250_MC2017_block74 \
T2tt_mStop-150to250_MC2017_block75 \
T2tt_mStop-150to250_MC2017_block76 \
T2tt_mStop-250to350_MC2017_block0 \
T2tt_mStop-250to350_MC2017_block1 \
T2tt_mStop-250to350_MC2017_block2 \
T2tt_mStop-250to350_MC2017_block3 \
T2tt_mStop-250to350_MC2017_block4 \
T2tt_mStop-250to350_MC2017_block5 \
T2tt_mStop-250to350_MC2017_block6 \
T2tt_mStop-250to350_MC2017_block7 \
T2tt_mStop-250to350_MC2017_block8 \
T2tt_mStop-250to350_MC2017_block9 \
T2tt_mStop-250to350_MC2017_block10 \
T2tt_mStop-250to350_MC2017_block11 \
T2tt_mStop-250to350_MC2017_block12 \
T2tt_mStop-250to350_MC2017_block13 \
T2tt_mStop-250to350_MC2017_block14 \
T2tt_mStop-250to350_MC2017_block15 \
T2tt_mStop-250to350_MC2017_block16 \
T2tt_mStop-250to350_MC2017_block17 \
T2tt_mStop-250to350_MC2017_block18 \
T2tt_mStop-250to350_MC2017_block19 \
T2tt_mStop-250to350_MC2017_block20 \
T2tt_mStop-250to350_MC2017_block21 \
T2tt_mStop-250to350_MC2017_block22 \
T2tt_mStop-250to350_MC2017_block23 \
T2tt_mStop-250to350_MC2017_block24 \
T2tt_mStop-250to350_MC2017_block25 \
T2tt_mStop-250to350_MC2017_block26 \
T2tt_mStop-250to350_MC2017_block27 \
T2tt_mStop-250to350_MC2017_block28 \
T2tt_mStop-250to350_MC2017_block29 \
T2tt_mStop-250to350_MC2017_block30 \
T2tt_mStop-250to350_MC2017_block31 \
T2tt_mStop-250to350_MC2017_block32 \
T2tt_mStop-250to350_MC2017_block33 \
T2tt_mStop-250to350_MC2017_block34 \
T2tt_mStop-250to350_MC2017_block35 \
T2tt_mStop-250to350_MC2017_block36 \
T2tt_mStop-250to350_MC2017_block37 \
T2tt_mStop-250to350_MC2017_block38 \
T2tt_mStop-250to350_MC2017_block39 \
T2tt_mStop-250to350_MC2017_block40 \
T2tt_mStop-250to350_MC2017_block41 \
T2tt_mStop-250to350_MC2017_block42 \
T2tt_mStop-250to350_MC2017_block43 \
T2tt_mStop-250to350_MC2017_block44 \
T2tt_mStop-250to350_MC2017_block45 \
T2tt_mStop-250to350_MC2017_block46 \
T2tt_mStop-250to350_MC2017_block47 \
T2tt_mStop-250to350_MC2017_block48 \
T2tt_mStop-250to350_MC2017_block49 \
T2tt_mStop-250to350_MC2017_block50 \
T2tt_mStop-250to350_MC2017_block51 \
T2tt_mStop-250to350_MC2017_block52 \
T2tt_mStop-250to350_MC2017_block53 \
T2tt_mStop-250to350_MC2017_block54 \
T2tt_mStop-250to350_MC2017_block55 \
T2tt_mStop-250to350_MC2017_block56 \
T2tt_mStop-250to350_MC2017_block57 \
T2tt_mStop-250to350_MC2017_block58 \
T2tt_mStop-250to350_MC2017_block59 \
T2tt_mStop-250to350_MC2017_block60 \
T2tt_mStop-250to350_MC2017_block61 \
T2tt_mStop-350to400_MC2017_block0 \
T2tt_mStop-350to400_MC2017_block1 \
T2tt_mStop-350to400_MC2017_block2 \
T2tt_mStop-350to400_MC2017_block3 \
T2tt_mStop-350to400_MC2017_block4 \
T2tt_mStop-350to400_MC2017_block5 \
T2tt_mStop-350to400_MC2017_block6 \
T2tt_mStop-350to400_MC2017_block7 \
T2tt_mStop-350to400_MC2017_block8 \
T2tt_mStop-350to400_MC2017_block9 \
T2tt_mStop-350to400_MC2017_block10 \
T2tt_mStop-350to400_MC2017_block11 \
T2tt_mStop-350to400_MC2017_block12 \
T2tt_mStop-350to400_MC2017_block13 \
T2tt_mStop-350to400_MC2017_block14 \
T2tt_mStop-350to400_MC2017_block15 \
T2tt_mStop-350to400_MC2017_block16 \
T2tt_mStop-350to400_MC2017_block17 \
T2tt_mStop-350to400_MC2017_block18 \
T2tt_mStop-350to400_MC2017_block19 \
T2tt_mStop-350to400_MC2017_block20 \
T2tt_mStop-350to400_MC2017_block21 \
T2tt_mStop-350to400_MC2017_block22 \
T2tt_mStop-350to400_MC2017_block23 \
T2tt_mStop-350to400_MC2017_block24 \
T2tt_mStop-350to400_MC2017_block25 \
T2tt_mStop-350to400_MC2017_block26 \
T2tt_mStop-350to400_MC2017_block27 \
T2tt_mStop-350to400_MC2017_block28 \
T2tt_mStop-350to400_MC2017_block29 \
T2tt_mStop-350to400_MC2017_block30 \
T2tt_mStop-350to400_MC2017_block31 \
T2tt_mStop-350to400_MC2017_block32 \
T2tt_mStop-350to400_MC2017_block33 \
T2tt_mStop-350to400_MC2017_block34 \
T2tt_mStop-350to400_MC2017_block35 \
T2tt_mStop-350to400_MC2017_block36 \
T2tt_mStop-350to400_MC2017_block37 \
T2tt_mStop-350to400_MC2017_block38 \
T2tt_mStop-350to400_MC2017_block39 \
T2tt_mStop-350to400_MC2017_block40 \
T2tt_mStop-350to400_MC2017_block41 \
T2tt_mStop-350to400_MC2017_block42 \
T2tt_mStop-350to400_MC2017_block43 \
T2tt_mStop-350to400_MC2017_block44 \
T2tt_mStop-350to400_MC2017_block45 \
T2tt_mStop-350to400_MC2017_block46 \
T2tt_mStop-350to400_MC2017_block47 \
T2tt_mStop-350to400_MC2017_block48 \
T2tt_mStop-350to400_MC2017_block49 \
T2tt_mStop-350to400_MC2017_block50 \
T2tt_mStop-350to400_MC2017_block51 \
T2tt_mStop-350to400_MC2017_block52 \
T2tt_mStop-350to400_MC2017_block53 \
T2tt_mStop-350to400_MC2017_block54 \
T2tt_mStop-350to400_MC2017_block55 \
T2tt_mStop-350to400_MC2017_block56 \
T2tt_mStop-350to400_MC2017_block57 \
T2tt_mStop-350to400_MC2017_block58 \
T2tt_mStop-400to1200_MC2017_block0 \
T2tt_mStop-400to1200_MC2017_block1 \
T2tt_mStop-400to1200_MC2017_block2 \
T2tt_mStop-400to1200_MC2017_block3 \
T2tt_mStop-400to1200_MC2017_block4 \
T2tt_mStop-400to1200_MC2017_block5 \
T2tt_mStop-400to1200_MC2017_block6 \
T2tt_mStop-400to1200_MC2017_block7 \
T2tt_mStop-400to1200_MC2017_block8 \
T2tt_mStop-400to1200_MC2017_block9 \
T2tt_mStop-400to1200_MC2017_block10 \
T2tt_mStop-400to1200_MC2017_block11 \
T2tt_mStop-400to1200_MC2017_block12 \
T2tt_mStop-400to1200_MC2017_block13 \
T2tt_mStop-400to1200_MC2017_block14 \
T2tt_mStop-400to1200_MC2017_block15 \
T2tt_mStop-400to1200_MC2017_block16 \
T2tt_mStop-400to1200_MC2017_block17 \
T2tt_mStop-400to1200_MC2017_block18 \
T2tt_mStop-400to1200_MC2017_block19 \
T2tt_mStop-400to1200_MC2017_block20 \
T2tt_mStop-400to1200_MC2017_block21 \
T2tt_mStop-400to1200_MC2017_block22 \
T2tt_mStop-400to1200_MC2017_block23 \
T2tt_mStop-400to1200_MC2017_block24 \
T2tt_mStop-400to1200_MC2017_block25 \
T2tt_mStop-400to1200_MC2017_block26 \
T2tt_mStop-400to1200_MC2017_block27 \
T2tt_mStop-400to1200_MC2017_block28 \
T2tt_mStop-400to1200_MC2017_block29 \
T2tt_mStop-400to1200_MC2017_block30 \
T2tt_mStop-400to1200_MC2017_block31 \
T2tt_mStop-400to1200_MC2017_block32 \
T2tt_mStop-400to1200_MC2017_block33 \
T2tt_mStop-400to1200_MC2017_block34 \
T2tt_mStop-400to1200_MC2017_block35 \
T2tt_mStop-400to1200_MC2017_block36 \
T2tt_mStop-400to1200_MC2017_block37 \
T2tt_mStop-400to1200_MC2017_block38 \
T2tt_mStop-400to1200_MC2017_block39 \
T2tt_mStop-400to1200_MC2017_block40 \
T2tt_mStop-400to1200_MC2017_block41 \
T2tt_mStop-400to1200_MC2017_block42 \
T2tt_mStop-400to1200_MC2017_block43 \
T2tt_mStop-400to1200_MC2017_block44 \
T2tt_mStop-400to1200_MC2017_block45 \
T2tt_mStop-400to1200_MC2017_block46 \
T2tt_mStop-400to1200_MC2017_block47 \
T2tt_mStop-400to1200_MC2017_block48 \
T2tt_mStop-400to1200_MC2017_block49 \
T2tt_mStop-400to1200_MC2017_block50 \
T2tt_mStop-400to1200_MC2017_block51 \
T2tt_mStop-400to1200_MC2017_block52 \
T2tt_mStop-400to1200_MC2017_block53 \
T2tt_mStop-400to1200_MC2017_block54 \
T2tt_mStop-400to1200_MC2017_block55 \
T2tt_mStop-400to1200_MC2017_block56 \
T2tt_mStop-400to1200_MC2017_block57 \
T2tt_mStop-400to1200_MC2017_block58 \
T2tt_mStop-400to1200_MC2017_block59 \
T2tt_mStop-400to1200_MC2017_block60 \
T2tt_mStop-400to1200_MC2017_block61 \
T2tt_mStop-400to1200_MC2017_block62 \
T2tt_mStop-400to1200_MC2017_block63 \
T2tt_mStop-400to1200_MC2017_block64 \
T2tt_mStop-400to1200_MC2017_block65 \
T2tt_mStop-400to1200_MC2017_block66 \
T2tt_mStop-400to1200_MC2017_block67 \
T2tt_mStop-400to1200_MC2017_block68 \
T2tt_mStop-400to1200_MC2017_block69 \
T2tt_mStop-400to1200_MC2017_block70 \
T2tt_mStop-400to1200_MC2017_block71 \
T2tt_mStop-400to1200_MC2017_block72 \
T2tt_mStop-400to1200_MC2017_block73 \
T2tt_mStop-400to1200_MC2017_block74 \
T2tt_mStop-400to1200_MC2017_block75 \
T2tt_mStop-400to1200_MC2017_block76 \
T2tt_mStop-400to1200_MC2017_block77 \
T2tt_mStop-400to1200_MC2017_block78 \
T2tt_mStop-400to1200_MC2017_block79 \
T2tt_mStop-400to1200_MC2017_block80 \
T2tt_mStop-400to1200_MC2017_block81 \
T2tt_mStop-400to1200_MC2017_block82 \
T2tt_mStop-400to1200_MC2017_block83 \
T5qqqqVV_MC2017_block0 \
T5qqqqVV_MC2017_block1 \
T5qqqqVV_MC2017_block2 \
T5qqqqVV_MC2017_block3 \
T5qqqqVV_MC2017_block4 \
T5qqqqVV_MC2017_block5 \
T5qqqqVV_MC2017_block6 \
T5qqqqVV_MC2017_block7 \
T5qqqqVV_MC2017_block8 \
T5qqqqVV_MC2017_block9 \
T5qqqqVV_MC2017_block10 \
T5qqqqVV_MC2017_block11 \
T5qqqqVV_MC2017_block12 \
T5qqqqVV_MC2017_block13 \
T5qqqqVV_MC2017_block14 \
T5qqqqVV_MC2017_block15 \
T5qqqqVV_MC2017_block16 \
T5qqqqVV_MC2017_block17 \
T5qqqqVV_MC2017_block18 \
T5qqqqVV_MC2017_block19 \
T5qqqqVV_MC2017_block20 \
T5qqqqVV_MC2017_block21 \
T5qqqqVV_MC2017_block22 \
T5qqqqVV_MC2017_block23 \
T5qqqqVV_MC2017_block24 \
T5qqqqVV_MC2017_block25 \
T5qqqqVV_MC2017_block26 \
T5qqqqVV_MC2017_block27 \
T5qqqqVV_MC2017_block28 \
T5qqqqVV_MC2017_block29 \
T5qqqqVV_MC2017_block30 \
T5qqqqVV_MC2017_block31 \
T5qqqqVV_MC2017_block32 \
T5qqqqVV_MC2017_block33 \
T5qqqqVV_MC2017_block34 \
T5qqqqVV_MC2017_block35 \
T5qqqqVV_MC2017_block36 \
T5qqqqVV_MC2017_block37 \
T5qqqqVV_MC2017_block38 \
T5qqqqVV_MC2017_block39 \
T5qqqqVV_MC2017_block40 \
T5qqqqVV_MC2017_block41 \
T5qqqqVV_MC2017_block42 \
T5qqqqVV_MC2017_block43 \
T5qqqqVV_MC2017_block44 \
T5qqqqVV_MC2017_block45 \
T5qqqqVV_MC2017_block46 \
T5qqqqVV_MC2017_block47 \
T5qqqqVV_MC2017_block48 \
T5qqqqVV_MC2017_block49 \
T5qqqqVV_MC2017_block50 \
T5qqqqVV_MC2017_block51 \
T5qqqqVV_MC2017_block52 \
T5qqqqVV_MC2017_block53 \
T5qqqqVV_MC2017_block54 \
T5qqqqVV_MC2017_block55 \
T5qqqqVV_MC2017_block56 \
T5qqqqVV_MC2017_block57 \
T5qqqqVV_MC2017_block58 \
T5qqqqVV_MC2017_block59 \
T5qqqqVV_MC2017_block60 \
T5qqqqVV_MC2017_block61 \
T5qqqqVV_MC2017_block62 \
T5qqqqVV_MC2017_block63 \
T5qqqqVV_MC2017_block64 \
T5qqqqVV_MC2017_block65 \
T5qqqqVV_MC2017_block66 \
T5qqqqVV_MC2017_block67 \
T5qqqqVV_MC2017_block68 \
T5qqqqVV_MC2017_block69 \
T5qqqqVV_MC2017_block70 \
T5qqqqVV_MC2017_block71 \
T5qqqqVV_MC2017_block72 \
T5qqqqVV_MC2017_block73 \
T5qqqqVV_MC2017_block74 \
T5qqqqVV_MC2017_block75 \
T5qqqqVV_MC2017_block76 \
T5qqqqVV_MC2017_block77 \
)
