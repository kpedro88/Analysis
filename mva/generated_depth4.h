#ifndef generated_depth4_h
#define generated_depth4_h
#include "DTree.h"
#define DTREE_MAX_DEPTH 4
#include "../KCode/KFactory.h"

typedef KFactory<DTreeBase> KBDTFactory;
#define REGISTER_BDT(a) REGISTER_MACRO(KBDTFactory,a)

template class DTree<DLeaf,DLeaf>;
typedef DTree<DLeaf,DLeaf> NEE;
REGISTER_BDT(NEE);

template class DTree<DLeaf,DTree<DLeaf,DLeaf>>;
typedef DTree<DLeaf,DTree<DLeaf,DLeaf>> NENEE;
REGISTER_BDT(NENEE);

template class DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NENENEE;
REGISTER_BDT(NENENEE);

template class DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NENENENEE;
REGISTER_BDT(NENENENEE);

template class DTree<DLeaf,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DLeaf,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NENENNEEE;
REGISTER_BDT(NENENNEEE);

template class DTree<DLeaf,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NENENNEENEE;
REGISTER_BDT(NENENNEENEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NENNEEE;
REGISTER_BDT(NENNEEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NENNEENEE;
REGISTER_BDT(NENNEENEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NENNEENENEE;
REGISTER_BDT(NENNEENENEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NENNEENNEEE;
REGISTER_BDT(NENNEENNEEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NENNEENNEENEE;
REGISTER_BDT(NENNEENNEENEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NENNENEEE;
REGISTER_BDT(NENNENEEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NENNENEENEE;
REGISTER_BDT(NENNENEENEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NENNENEENENEE;
REGISTER_BDT(NENNENEENENEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NENNENEENNEEE;
REGISTER_BDT(NENNENEENNEEE);

template class DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NENNENEENNEENEE;
REGISTER_BDT(NENNENEENNEENEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NENNNEEEE;
REGISTER_BDT(NENNNEEEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NENNNEEENEE;
REGISTER_BDT(NENNNEEENEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NENNNEEENENEE;
REGISTER_BDT(NENNNEEENENEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NENNNEEENNEEE;
REGISTER_BDT(NENNNEEENNEEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NENNNEEENNEENEE;
REGISTER_BDT(NENNNEEENNEENEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NENNNEENEEE;
REGISTER_BDT(NENNNEENEEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NENNNEENEENEE;
REGISTER_BDT(NENNNEENEENEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NENNNEENEENENEE;
REGISTER_BDT(NENNNEENEENENEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NENNNEENEENNEEE;
REGISTER_BDT(NENNNEENEENNEEE);

template class DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DLeaf,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NENNNEENEENNEENEE;
REGISTER_BDT(NENNNEENEENNEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DLeaf>;
typedef DTree<DTree<DLeaf,DLeaf>,DLeaf> NNEEE;
REGISTER_BDT(NNEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>> NNEENEE;
REGISTER_BDT(NNEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNEENENEE;
REGISTER_BDT(NNEENENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNEENENENEE;
REGISTER_BDT(NNEENENENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNEENENNEEE;
REGISTER_BDT(NNEENENNEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNEENENNEENEE;
REGISTER_BDT(NNEENENNEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNEENNEEE;
REGISTER_BDT(NNEENNEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNEENNEENEE;
REGISTER_BDT(NNEENNEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNEENNEENENEE;
REGISTER_BDT(NNEENNEENENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNEENNEENNEEE;
REGISTER_BDT(NNEENNEENNEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNEENNEENNEENEE;
REGISTER_BDT(NNEENNEENNEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNEENNENEEE;
REGISTER_BDT(NNEENNENEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNEENNENEENEE;
REGISTER_BDT(NNEENNENEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNEENNENEENENEE;
REGISTER_BDT(NNEENNENEENENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNEENNENEENNEEE;
REGISTER_BDT(NNEENNENEENNEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNEENNENEENNEENEE;
REGISTER_BDT(NNEENNENEENNEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNEENNNEEEE;
REGISTER_BDT(NNEENNNEEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNEENNNEEENEE;
REGISTER_BDT(NNEENNNEEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNEENNNEEENENEE;
REGISTER_BDT(NNEENNNEEENENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNEENNNEEENNEEE;
REGISTER_BDT(NNEENNNEEENNEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNEENNNEEENNEENEE;
REGISTER_BDT(NNEENNNEEENNEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNEENNNEENEEE;
REGISTER_BDT(NNEENNNEENEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNEENNNEENEENEE;
REGISTER_BDT(NNEENNNEENEENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNEENNNEENEENENEE;
REGISTER_BDT(NNEENNNEENEENENEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNEENNNEENEENNEEE;
REGISTER_BDT(NNEENNNEENEENNEEE);

template class DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNEENNNEENEENNEENEE;
REGISTER_BDT(NNEENNNEENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf> NNENEEE;
REGISTER_BDT(NNENEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>> NNENEENEE;
REGISTER_BDT(NNENEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNENEENENEE;
REGISTER_BDT(NNENEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENEENENENEE;
REGISTER_BDT(NNENEENENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENEENENNEEE;
REGISTER_BDT(NNENEENENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENEENENNEENEE;
REGISTER_BDT(NNENEENENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNENEENNEEE;
REGISTER_BDT(NNENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNENEENNEENEE;
REGISTER_BDT(NNENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENEENNEENENEE;
REGISTER_BDT(NNENEENNEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENEENNEENNEEE;
REGISTER_BDT(NNENEENNEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENEENNEENNEENEE;
REGISTER_BDT(NNENEENNEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNENEENNENEEE;
REGISTER_BDT(NNENEENNENEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNENEENNENEENEE;
REGISTER_BDT(NNENEENNENEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENEENNENEENENEE;
REGISTER_BDT(NNENEENNENEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENEENNENEENNEEE;
REGISTER_BDT(NNENEENNENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENEENNENEENNEENEE;
REGISTER_BDT(NNENEENNENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNENEENNNEEEE;
REGISTER_BDT(NNENEENNNEEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNENEENNNEEENEE;
REGISTER_BDT(NNENEENNNEEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENEENNNEEENENEE;
REGISTER_BDT(NNENEENNNEEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENEENNNEEENNEEE;
REGISTER_BDT(NNENEENNNEEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENEENNNEEENNEENEE;
REGISTER_BDT(NNENEENNNEEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNENEENNNEENEEE;
REGISTER_BDT(NNENEENNNEENEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNENEENNNEENEENEE;
REGISTER_BDT(NNENEENNNEENEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENEENNNEENEENENEE;
REGISTER_BDT(NNENEENNNEENEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENEENNNEENEENNEEE;
REGISTER_BDT(NNENEENNNEENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENEENNNEENEENNEENEE;
REGISTER_BDT(NNENEENNNEENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf> NNENENEEE;
REGISTER_BDT(NNENENEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNENENEENEE;
REGISTER_BDT(NNENENEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNENENEENENEE;
REGISTER_BDT(NNENENEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENENEENENENEE;
REGISTER_BDT(NNENENEENENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENENEENENNEEE;
REGISTER_BDT(NNENENEENENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENENEENENNEENEE;
REGISTER_BDT(NNENENEENENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNENENEENNEEE;
REGISTER_BDT(NNENENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNENENEENNEENEE;
REGISTER_BDT(NNENENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENENEENNEENENEE;
REGISTER_BDT(NNENENEENNEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENENEENNEENNEEE;
REGISTER_BDT(NNENENEENNEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENENEENNEENNEENEE;
REGISTER_BDT(NNENENEENNEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNENENEENNENEEE;
REGISTER_BDT(NNENENEENNENEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNENENEENNENEENEE;
REGISTER_BDT(NNENENEENNENEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENENEENNENEENENEE;
REGISTER_BDT(NNENENEENNENEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENENEENNENEENNEEE;
REGISTER_BDT(NNENENEENNENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENENEENNENEENNEENEE;
REGISTER_BDT(NNENENEENNENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNENENEENNNEEEE;
REGISTER_BDT(NNENENEENNNEEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNENENEENNNEEENEE;
REGISTER_BDT(NNENENEENNNEEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENENEENNNEEENENEE;
REGISTER_BDT(NNENENEENNNEEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENENEENNNEEENNEEE;
REGISTER_BDT(NNENENEENNNEEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENENEENNNEEENNEENEE;
REGISTER_BDT(NNENENEENNNEEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNENENEENNNEENEEE;
REGISTER_BDT(NNENENEENNNEENEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNENENEENNNEENEENEE;
REGISTER_BDT(NNENENEENNNEENEENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENENEENNNEENEENENEE;
REGISTER_BDT(NNENENEENNNEENEENENEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENENEENNNEENEENNEEE;
REGISTER_BDT(NNENENEENNNEENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENENEENNNEENEENNEENEE;
REGISTER_BDT(NNENENEENNNEENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf> NNENNEEEE;
REGISTER_BDT(NNENNEEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>> NNENNEEENEE;
REGISTER_BDT(NNENNEEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNENNEEENENEE;
REGISTER_BDT(NNENNEEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEEENENENEE;
REGISTER_BDT(NNENNEEENENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEEENENNEEE;
REGISTER_BDT(NNENNEEENENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEEENENNEENEE;
REGISTER_BDT(NNENNEEENENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNENNEEENNEEE;
REGISTER_BDT(NNENNEEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNENNEEENNEENEE;
REGISTER_BDT(NNENNEEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEEENNEENENEE;
REGISTER_BDT(NNENNEEENNEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEEENNEENNEEE;
REGISTER_BDT(NNENNEEENNEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEEENNEENNEENEE;
REGISTER_BDT(NNENNEEENNEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNENNEEENNENEEE;
REGISTER_BDT(NNENNEEENNENEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNENNEEENNENEENEE;
REGISTER_BDT(NNENNEEENNENEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEEENNENEENENEE;
REGISTER_BDT(NNENNEEENNENEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEEENNENEENNEEE;
REGISTER_BDT(NNENNEEENNENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEEENNENEENNEENEE;
REGISTER_BDT(NNENNEEENNENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNENNEEENNNEEEE;
REGISTER_BDT(NNENNEEENNNEEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNENNEEENNNEEENEE;
REGISTER_BDT(NNENNEEENNNEEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEEENNNEEENENEE;
REGISTER_BDT(NNENNEEENNNEEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEEENNNEEENNEEE;
REGISTER_BDT(NNENNEEENNNEEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEEENNNEEENNEENEE;
REGISTER_BDT(NNENNEEENNNEEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNENNEEENNNEENEEE;
REGISTER_BDT(NNENNEEENNNEENEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNENNEEENNNEENEENEE;
REGISTER_BDT(NNENNEEENNNEENEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEEENNNEENEENENEE;
REGISTER_BDT(NNENNEEENNNEENEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEEENNNEENEENNEEE;
REGISTER_BDT(NNENNEEENNNEENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEEENNNEENEENNEENEE;
REGISTER_BDT(NNENNEEENNNEENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf> NNENNEENEEE;
REGISTER_BDT(NNENNEENEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNENNEENEENEE;
REGISTER_BDT(NNENNEENEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNENNEENEENENEE;
REGISTER_BDT(NNENNEENEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEENEENENENEE;
REGISTER_BDT(NNENNEENEENENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEENEENENNEEE;
REGISTER_BDT(NNENNEENEENENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEENEENENNEENEE;
REGISTER_BDT(NNENNEENEENENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNENNEENEENNEEE;
REGISTER_BDT(NNENNEENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNENNEENEENNEENEE;
REGISTER_BDT(NNENNEENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEENEENNEENENEE;
REGISTER_BDT(NNENNEENEENNEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEENEENNEENNEEE;
REGISTER_BDT(NNENNEENEENNEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEENEENNEENNEENEE;
REGISTER_BDT(NNENNEENEENNEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNENNEENEENNENEEE;
REGISTER_BDT(NNENNEENEENNENEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNENNEENEENNENEENEE;
REGISTER_BDT(NNENNEENEENNENEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEENEENNENEENENEE;
REGISTER_BDT(NNENNEENEENNENEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEENEENNENEENNEEE;
REGISTER_BDT(NNENNEENEENNENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEENEENNENEENNEENEE;
REGISTER_BDT(NNENNEENEENNENEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNENNEENEENNNEEEE;
REGISTER_BDT(NNENNEENEENNNEEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNENNEENEENNNEEENEE;
REGISTER_BDT(NNENNEENEENNNEEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEENEENNNEEENENEE;
REGISTER_BDT(NNENNEENEENNNEEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEENEENNNEEENNEEE;
REGISTER_BDT(NNENNEENEENNNEEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEENEENNNEEENNEENEE;
REGISTER_BDT(NNENNEENEENNNEEENNEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNENNEENEENNNEENEEE;
REGISTER_BDT(NNENNEENEENNNEENEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNENNEENEENNNEENEENEE;
REGISTER_BDT(NNENNEENEENNNEENEENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNENNEENEENNNEENEENENEE;
REGISTER_BDT(NNENNEENEENNNEENEENENEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNENNEENEENNNEENEENNEEE;
REGISTER_BDT(NNENNEENEENNNEENEENNEEE);

template class DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNENNEENEENNNEENEENNEENEE;
REGISTER_BDT(NNENNEENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf> NNNEEEE;
REGISTER_BDT(NNNEEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>> NNNEEENEE;
REGISTER_BDT(NNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNEEENENEE;
REGISTER_BDT(NNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEEENENENEE;
REGISTER_BDT(NNNEEENENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEEENENNEEE;
REGISTER_BDT(NNNEEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEEENENNEENEE;
REGISTER_BDT(NNNEEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNEEENNEEE;
REGISTER_BDT(NNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEEENNEENEE;
REGISTER_BDT(NNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEEENNEENENEE;
REGISTER_BDT(NNNEEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEEENNEENNEEE;
REGISTER_BDT(NNNEEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEEENNEENNEENEE;
REGISTER_BDT(NNNEEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEEENNENEEE;
REGISTER_BDT(NNNEEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEEENNENEENEE;
REGISTER_BDT(NNNEEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEEENNENEENENEE;
REGISTER_BDT(NNNEEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEEENNENEENNEEE;
REGISTER_BDT(NNNEEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEEENNENEENNEENEE;
REGISTER_BDT(NNNEEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNEEENNNEEEE;
REGISTER_BDT(NNNEEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEEENNNEEENEE;
REGISTER_BDT(NNNEEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEEENNNEEENENEE;
REGISTER_BDT(NNNEEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEEENNNEEENNEEE;
REGISTER_BDT(NNNEEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEEENNNEEENNEENEE;
REGISTER_BDT(NNNEEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEEENNNEENEEE;
REGISTER_BDT(NNNEEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEEENNNEENEENEE;
REGISTER_BDT(NNNEEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEEENNNEENEENENEE;
REGISTER_BDT(NNNEEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEEENNNEENEENNEEE;
REGISTER_BDT(NNNEEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEEENNNEENEENNEENEE;
REGISTER_BDT(NNNEEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf> NNNEENEEE;
REGISTER_BDT(NNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>> NNNEENEENEE;
REGISTER_BDT(NNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNEENEENENEE;
REGISTER_BDT(NNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENEENENENEE;
REGISTER_BDT(NNNEENEENENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENEENENNEEE;
REGISTER_BDT(NNNEENEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENEENENNEENEE;
REGISTER_BDT(NNNEENEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNEENEENNEEE;
REGISTER_BDT(NNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEENEENNEENEE;
REGISTER_BDT(NNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENEENNEENENEE;
REGISTER_BDT(NNNEENEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENEENNEENNEEE;
REGISTER_BDT(NNNEENEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENEENNEENNEENEE;
REGISTER_BDT(NNNEENEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEENEENNENEEE;
REGISTER_BDT(NNNEENEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEENEENNENEENEE;
REGISTER_BDT(NNNEENEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENEENNENEENENEE;
REGISTER_BDT(NNNEENEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENEENNENEENNEEE;
REGISTER_BDT(NNNEENEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENEENNENEENNEENEE;
REGISTER_BDT(NNNEENEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNEENEENNNEEEE;
REGISTER_BDT(NNNEENEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEENEENNNEEENEE;
REGISTER_BDT(NNNEENEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENEENNNEEENENEE;
REGISTER_BDT(NNNEENEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENEENNNEEENNEEE;
REGISTER_BDT(NNNEENEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENEENNNEEENNEENEE;
REGISTER_BDT(NNNEENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEENEENNNEENEEE;
REGISTER_BDT(NNNEENEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEENEENNNEENEENEE;
REGISTER_BDT(NNNEENEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENEENNNEENEENENEE;
REGISTER_BDT(NNNEENEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENEENNNEENEENNEEE;
REGISTER_BDT(NNNEENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENEENNNEENEENNEENEE;
REGISTER_BDT(NNNEENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf> NNNEENENEEE;
REGISTER_BDT(NNNEENENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNNEENENEENEE;
REGISTER_BDT(NNNEENENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNEENENEENENEE;
REGISTER_BDT(NNNEENENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENENEENENENEE;
REGISTER_BDT(NNNEENENEENENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENENEENENNEEE;
REGISTER_BDT(NNNEENENEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENENEENENNEENEE;
REGISTER_BDT(NNNEENENEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNEENENEENNEEE;
REGISTER_BDT(NNNEENENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEENENEENNEENEE;
REGISTER_BDT(NNNEENENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENENEENNEENENEE;
REGISTER_BDT(NNNEENENEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENENEENNEENNEEE;
REGISTER_BDT(NNNEENENEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENENEENNEENNEENEE;
REGISTER_BDT(NNNEENENEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEENENEENNENEEE;
REGISTER_BDT(NNNEENENEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEENENEENNENEENEE;
REGISTER_BDT(NNNEENENEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENENEENNENEENENEE;
REGISTER_BDT(NNNEENENEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENENEENNENEENNEEE;
REGISTER_BDT(NNNEENENEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENENEENNENEENNEENEE;
REGISTER_BDT(NNNEENENEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNEENENEENNNEEEE;
REGISTER_BDT(NNNEENENEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEENENEENNNEEENEE;
REGISTER_BDT(NNNEENENEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENENEENNNEEENENEE;
REGISTER_BDT(NNNEENENEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENENEENNNEEENNEEE;
REGISTER_BDT(NNNEENENEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENENEENNNEEENNEENEE;
REGISTER_BDT(NNNEENENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEENENEENNNEENEEE;
REGISTER_BDT(NNNEENENEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEENENEENNNEENEENEE;
REGISTER_BDT(NNNEENENEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENENEENNNEENEENENEE;
REGISTER_BDT(NNNEENENEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENENEENNNEENEENNEEE;
REGISTER_BDT(NNNEENENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENENEENNNEENEENNEENEE;
REGISTER_BDT(NNNEENENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf> NNNEENNEEEE;
REGISTER_BDT(NNNEENNEEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>> NNNEENNEEENEE;
REGISTER_BDT(NNNEENNEEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNEENNEEENENEE;
REGISTER_BDT(NNNEENNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEEENENENEE;
REGISTER_BDT(NNNEENNEEENENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEEENENNEEE;
REGISTER_BDT(NNNEENNEEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEEENENNEENEE;
REGISTER_BDT(NNNEENNEEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNEENNEEENNEEE;
REGISTER_BDT(NNNEENNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEENNEEENNEENEE;
REGISTER_BDT(NNNEENNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEEENNEENENEE;
REGISTER_BDT(NNNEENNEEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEEENNEENNEEE;
REGISTER_BDT(NNNEENNEEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEEENNEENNEENEE;
REGISTER_BDT(NNNEENNEEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEENNEEENNENEEE;
REGISTER_BDT(NNNEENNEEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEENNEEENNENEENEE;
REGISTER_BDT(NNNEENNEEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEEENNENEENENEE;
REGISTER_BDT(NNNEENNEEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEEENNENEENNEEE;
REGISTER_BDT(NNNEENNEEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEEENNENEENNEENEE;
REGISTER_BDT(NNNEENNEEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNEENNEEENNNEEEE;
REGISTER_BDT(NNNEENNEEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEENNEEENNNEEENEE;
REGISTER_BDT(NNNEENNEEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEEENNNEEENENEE;
REGISTER_BDT(NNNEENNEEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEEENNNEEENNEEE;
REGISTER_BDT(NNNEENNEEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEEENNNEEENNEENEE;
REGISTER_BDT(NNNEENNEEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEENNEEENNNEENEEE;
REGISTER_BDT(NNNEENNEEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEENNEEENNNEENEENEE;
REGISTER_BDT(NNNEENNEEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEEENNNEENEENENEE;
REGISTER_BDT(NNNEENNEEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEEENNNEENEENNEEE;
REGISTER_BDT(NNNEENNEEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEEENNNEENEENNEENEE;
REGISTER_BDT(NNNEENNEEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf> NNNEENNEENEEE;
REGISTER_BDT(NNNEENNEENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNNEENNEENEENEE;
REGISTER_BDT(NNNEENNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNEENNEENEENENEE;
REGISTER_BDT(NNNEENNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENENENEE;
REGISTER_BDT(NNNEENNEENEENENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEENEENENNEEE;
REGISTER_BDT(NNNEENNEENEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENENNEENEE;
REGISTER_BDT(NNNEENNEENEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNEENNEENEENNEEE;
REGISTER_BDT(NNNEENNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEENNEENEENNEENEE;
REGISTER_BDT(NNNEENNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENNEENENEE;
REGISTER_BDT(NNNEENNEENEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEENEENNEENNEEE;
REGISTER_BDT(NNNEENNEENEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENNEENNEENEE;
REGISTER_BDT(NNNEENNEENEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEENNEENEENNENEEE;
REGISTER_BDT(NNNEENNEENEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEENNEENEENNENEENEE;
REGISTER_BDT(NNNEENNEENEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENNENEENENEE;
REGISTER_BDT(NNNEENNEENEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEENEENNENEENNEEE;
REGISTER_BDT(NNNEENNEENEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENNENEENNEENEE;
REGISTER_BDT(NNNEENNEENEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNEENNEENEENNNEEEE;
REGISTER_BDT(NNNEENNEENEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNEENNEENEENNNEEENEE;
REGISTER_BDT(NNNEENNEENEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENNNEEENENEE;
REGISTER_BDT(NNNEENNEENEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEENEENNNEEENNEEE;
REGISTER_BDT(NNNEENNEENEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENNNEEENNEENEE;
REGISTER_BDT(NNNEENNEENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNEENNEENEENNNEENEEE;
REGISTER_BDT(NNNEENNEENEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNEENNEENEENNNEENEENEE;
REGISTER_BDT(NNNEENNEENEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENNNEENEENENEE;
REGISTER_BDT(NNNEENNEENEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNEENNEENEENNNEENEENNEEE;
REGISTER_BDT(NNNEENNEENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNEENNEENEENNNEENEENNEENEE;
REGISTER_BDT(NNNEENNEENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DLeaf> NNNENEEEE;
REGISTER_BDT(NNNENEEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DLeaf>> NNNENEEENEE;
REGISTER_BDT(NNNENEEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNENEEENENEE;
REGISTER_BDT(NNNENEEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEEENENENEE;
REGISTER_BDT(NNNENEEENENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEEENENNEEE;
REGISTER_BDT(NNNENEEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEEENENNEENEE;
REGISTER_BDT(NNNENEEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNENEEENNEEE;
REGISTER_BDT(NNNENEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEEENNEENEE;
REGISTER_BDT(NNNENEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEEENNEENENEE;
REGISTER_BDT(NNNENEEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEEENNEENNEEE;
REGISTER_BDT(NNNENEEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEEENNEENNEENEE;
REGISTER_BDT(NNNENEEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEEENNENEEE;
REGISTER_BDT(NNNENEEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEEENNENEENEE;
REGISTER_BDT(NNNENEEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEEENNENEENENEE;
REGISTER_BDT(NNNENEEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEEENNENEENNEEE;
REGISTER_BDT(NNNENEEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEEENNENEENNEENEE;
REGISTER_BDT(NNNENEEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNENEEENNNEEEE;
REGISTER_BDT(NNNENEEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEEENNNEEENEE;
REGISTER_BDT(NNNENEEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEEENNNEEENENEE;
REGISTER_BDT(NNNENEEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEEENNNEEENNEEE;
REGISTER_BDT(NNNENEEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEEENNNEEENNEENEE;
REGISTER_BDT(NNNENEEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEEENNNEENEEE;
REGISTER_BDT(NNNENEEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEEENNNEENEENEE;
REGISTER_BDT(NNNENEEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEEENNNEENEENENEE;
REGISTER_BDT(NNNENEEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEEENNNEENEENNEEE;
REGISTER_BDT(NNNENEEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEEENNNEENEENNEENEE;
REGISTER_BDT(NNNENEEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DLeaf> NNNENEENEEE;
REGISTER_BDT(NNNENEENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>> NNNENEENEENEE;
REGISTER_BDT(NNNENEENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNENEENEENENEE;
REGISTER_BDT(NNNENEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENEENENENEE;
REGISTER_BDT(NNNENEENEENENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENEENENNEEE;
REGISTER_BDT(NNNENEENEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENEENENNEENEE;
REGISTER_BDT(NNNENEENEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNENEENEENNEEE;
REGISTER_BDT(NNNENEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEENEENNEENEE;
REGISTER_BDT(NNNENEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENEENNEENENEE;
REGISTER_BDT(NNNENEENEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENEENNEENNEEE;
REGISTER_BDT(NNNENEENEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENEENNEENNEENEE;
REGISTER_BDT(NNNENEENEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEENEENNENEEE;
REGISTER_BDT(NNNENEENEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEENEENNENEENEE;
REGISTER_BDT(NNNENEENEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENEENNENEENENEE;
REGISTER_BDT(NNNENEENEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENEENNENEENNEEE;
REGISTER_BDT(NNNENEENEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENEENNENEENNEENEE;
REGISTER_BDT(NNNENEENEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNENEENEENNNEEEE;
REGISTER_BDT(NNNENEENEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEENEENNNEEENEE;
REGISTER_BDT(NNNENEENEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENEENNNEEENENEE;
REGISTER_BDT(NNNENEENEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENEENNNEEENNEEE;
REGISTER_BDT(NNNENEENEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENEENNNEEENNEENEE;
REGISTER_BDT(NNNENEENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEENEENNNEENEEE;
REGISTER_BDT(NNNENEENEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEENEENNNEENEENEE;
REGISTER_BDT(NNNENEENEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENEENNNEENEENENEE;
REGISTER_BDT(NNNENEENEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENEENNNEENEENNEEE;
REGISTER_BDT(NNNENEENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENEENNNEENEENNEENEE;
REGISTER_BDT(NNNENEENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf> NNNENEENENEEE;
REGISTER_BDT(NNNENEENENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNNENEENENEENEE;
REGISTER_BDT(NNNENEENENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNENEENENEENENEE;
REGISTER_BDT(NNNENEENENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENENEENENENEE;
REGISTER_BDT(NNNENEENENEENENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENENEENENNEEE;
REGISTER_BDT(NNNENEENENEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENENEENENNEENEE;
REGISTER_BDT(NNNENEENENEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNENEENENEENNEEE;
REGISTER_BDT(NNNENEENENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEENENEENNEENEE;
REGISTER_BDT(NNNENEENENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENENEENNEENENEE;
REGISTER_BDT(NNNENEENENEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENENEENNEENNEEE;
REGISTER_BDT(NNNENEENENEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENENEENNEENNEENEE;
REGISTER_BDT(NNNENEENENEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEENENEENNENEEE;
REGISTER_BDT(NNNENEENENEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEENENEENNENEENEE;
REGISTER_BDT(NNNENEENENEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENENEENNENEENENEE;
REGISTER_BDT(NNNENEENENEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENENEENNENEENNEEE;
REGISTER_BDT(NNNENEENENEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENENEENNENEENNEENEE;
REGISTER_BDT(NNNENEENENEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNENEENENEENNNEEEE;
REGISTER_BDT(NNNENEENENEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEENENEENNNEEENEE;
REGISTER_BDT(NNNENEENENEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENENEENNNEEENENEE;
REGISTER_BDT(NNNENEENENEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENENEENNNEEENNEEE;
REGISTER_BDT(NNNENEENENEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENENEENNNEEENNEENEE;
REGISTER_BDT(NNNENEENENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEENENEENNNEENEEE;
REGISTER_BDT(NNNENEENENEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEENENEENNNEENEENEE;
REGISTER_BDT(NNNENEENENEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENENEENNNEENEENENEE;
REGISTER_BDT(NNNENEENENEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENENEENNNEENEENNEEE;
REGISTER_BDT(NNNENEENENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENENEENNNEENEENNEENEE;
REGISTER_BDT(NNNENEENENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf> NNNENEENNEEEE;
REGISTER_BDT(NNNENEENNEEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>> NNNENEENNEEENEE;
REGISTER_BDT(NNNENEENNEEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNENEENNEEENENEE;
REGISTER_BDT(NNNENEENNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENENENEE;
REGISTER_BDT(NNNENEENNEEENENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEEENENNEEE;
REGISTER_BDT(NNNENEENNEEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENENNEENEE;
REGISTER_BDT(NNNENEENNEEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNENEENNEEENNEEE;
REGISTER_BDT(NNNENEENNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEENNEEENNEENEE;
REGISTER_BDT(NNNENEENNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENNEENENEE;
REGISTER_BDT(NNNENEENNEEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEEENNEENNEEE;
REGISTER_BDT(NNNENEENNEEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENNEENNEENEE;
REGISTER_BDT(NNNENEENNEEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEENNEEENNENEEE;
REGISTER_BDT(NNNENEENNEEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEENNEEENNENEENEE;
REGISTER_BDT(NNNENEENNEEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENNENEENENEE;
REGISTER_BDT(NNNENEENNEEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEEENNENEENNEEE;
REGISTER_BDT(NNNENEENNEEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENNENEENNEENEE;
REGISTER_BDT(NNNENEENNEEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNENEENNEEENNNEEEE;
REGISTER_BDT(NNNENEENNEEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEENNEEENNNEEENEE;
REGISTER_BDT(NNNENEENNEEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENNNEEENENEE;
REGISTER_BDT(NNNENEENNEEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEEENNNEEENNEEE;
REGISTER_BDT(NNNENEENNEEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENNNEEENNEENEE;
REGISTER_BDT(NNNENEENNEEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEENNEEENNNEENEEE;
REGISTER_BDT(NNNENEENNEEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEENNEEENNNEENEENEE;
REGISTER_BDT(NNNENEENNEEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENNNEENEENENEE;
REGISTER_BDT(NNNENEENNEEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEEENNNEENEENNEEE;
REGISTER_BDT(NNNENEENNEEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEEENNNEENEENNEENEE;
REGISTER_BDT(NNNENEENNEEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf> NNNENEENNEENEEE;
REGISTER_BDT(NNNENEENNEENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNNENEENNEENEENEE;
REGISTER_BDT(NNNENEENNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNENEENNEENEENENEE;
REGISTER_BDT(NNNENEENNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENENENEE;
REGISTER_BDT(NNNENEENNEENEENENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEENEENENNEEE;
REGISTER_BDT(NNNENEENNEENEENENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENENNEENEE;
REGISTER_BDT(NNNENEENNEENEENENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNENEENNEENEENNEEE;
REGISTER_BDT(NNNENEENNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEENNEENEENNEENEE;
REGISTER_BDT(NNNENEENNEENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENNEENENEE;
REGISTER_BDT(NNNENEENNEENEENNEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEENEENNEENNEEE;
REGISTER_BDT(NNNENEENNEENEENNEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENNEENNEENEE;
REGISTER_BDT(NNNENEENNEENEENNEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEENNEENEENNENEEE;
REGISTER_BDT(NNNENEENNEENEENNENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEENNEENEENNENEENEE;
REGISTER_BDT(NNNENEENNEENEENNENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENNENEENENEE;
REGISTER_BDT(NNNENEENNEENEENNENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEENEENNENEENNEEE;
REGISTER_BDT(NNNENEENNEENEENNENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENNENEENNEENEE;
REGISTER_BDT(NNNENEENNEENEENNENEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNENEENNEENEENNNEEEE;
REGISTER_BDT(NNNENEENNEENEENNNEEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNENEENNEENEENNNEEENEE;
REGISTER_BDT(NNNENEENNEENEENNNEEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENNNEEENENEE;
REGISTER_BDT(NNNENEENNEENEENNNEEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEENEENNNEEENNEEE;
REGISTER_BDT(NNNENEENNEENEENNNEEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENNNEEENNEENEE;
REGISTER_BDT(NNNENEENNEENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNENEENNEENEENNNEENEEE;
REGISTER_BDT(NNNENEENNEENEENNNEENEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNENEENNEENEENNNEENEENEE;
REGISTER_BDT(NNNENEENNEENEENNNEENEENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENNNEENEENENEE;
REGISTER_BDT(NNNENEENNEENEENNNEENEENENEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNENEENNEENEENNNEENEENNEEE;
REGISTER_BDT(NNNENEENNEENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNENEENNEENEENNNEENEENNEENEE;
REGISTER_BDT(NNNENEENNEENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DLeaf> NNNNEEEEE;
REGISTER_BDT(NNNNEEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>> NNNNEEEENEE;
REGISTER_BDT(NNNNEEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEEEENENEE;
REGISTER_BDT(NNNNEEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEEENENENEE;
REGISTER_BDT(NNNNEEEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEEENENNEEE;
REGISTER_BDT(NNNNEEEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEEENENNEENEE;
REGISTER_BDT(NNNNEEEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEEEENNEEE;
REGISTER_BDT(NNNNEEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEEENNEENEE;
REGISTER_BDT(NNNNEEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEEENNEENENEE;
REGISTER_BDT(NNNNEEEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEEENNEENNEEE;
REGISTER_BDT(NNNNEEEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEEENNEENNEENEE;
REGISTER_BDT(NNNNEEEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEEENNENEEE;
REGISTER_BDT(NNNNEEEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEEENNENEENEE;
REGISTER_BDT(NNNNEEEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEEENNENEENENEE;
REGISTER_BDT(NNNNEEEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEEENNENEENNEEE;
REGISTER_BDT(NNNNEEEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEEENNENEENNEENEE;
REGISTER_BDT(NNNNEEEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEEEENNNEEEE;
REGISTER_BDT(NNNNEEEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEEENNNEEENEE;
REGISTER_BDT(NNNNEEEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEEENNNEEENENEE;
REGISTER_BDT(NNNNEEEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEEENNNEEENNEEE;
REGISTER_BDT(NNNNEEEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEEENNNEEENNEENEE;
REGISTER_BDT(NNNNEEEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEEENNNEENEEE;
REGISTER_BDT(NNNNEEEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEEENNNEENEENEE;
REGISTER_BDT(NNNNEEEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEEENNNEENEENENEE;
REGISTER_BDT(NNNNEEEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEEENNNEENEENNEEE;
REGISTER_BDT(NNNNEEEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEEEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf> NNNNEEENEEE;
REGISTER_BDT(NNNNEEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>> NNNNEEENEENEE;
REGISTER_BDT(NNNNEEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEEENEENENEE;
REGISTER_BDT(NNNNEEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENEENENENEE;
REGISTER_BDT(NNNNEEENEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENEENENNEEE;
REGISTER_BDT(NNNNEEENEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENEENENNEENEE;
REGISTER_BDT(NNNNEEENEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEEENEENNEEE;
REGISTER_BDT(NNNNEEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEENEENNEENEE;
REGISTER_BDT(NNNNEEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENEENNEENENEE;
REGISTER_BDT(NNNNEEENEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENEENNEENNEEE;
REGISTER_BDT(NNNNEEENEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENEENNEENNEENEE;
REGISTER_BDT(NNNNEEENEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEENEENNENEEE;
REGISTER_BDT(NNNNEEENEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEENEENNENEENEE;
REGISTER_BDT(NNNNEEENEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENEENNENEENENEE;
REGISTER_BDT(NNNNEEENEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENEENNENEENNEEE;
REGISTER_BDT(NNNNEEENEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENEENNENEENNEENEE;
REGISTER_BDT(NNNNEEENEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEEENEENNNEEEE;
REGISTER_BDT(NNNNEEENEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEENEENNNEEENEE;
REGISTER_BDT(NNNNEEENEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENEENNNEEENENEE;
REGISTER_BDT(NNNNEEENEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENEENNNEEENNEEE;
REGISTER_BDT(NNNNEEENEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENEENNNEEENNEENEE;
REGISTER_BDT(NNNNEEENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEENEENNNEENEEE;
REGISTER_BDT(NNNNEEENEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEENEENNNEENEENEE;
REGISTER_BDT(NNNNEEENEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENEENNNEENEENENEE;
REGISTER_BDT(NNNNEEENEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENEENNNEENEENNEEE;
REGISTER_BDT(NNNNEEENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEEENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf> NNNNEEENENEEE;
REGISTER_BDT(NNNNEEENENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNNNEEENENEENEE;
REGISTER_BDT(NNNNEEENENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEEENENEENENEE;
REGISTER_BDT(NNNNEEENENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENENENEE;
REGISTER_BDT(NNNNEEENENEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENENEENENNEEE;
REGISTER_BDT(NNNNEEENENEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENENNEENEE;
REGISTER_BDT(NNNNEEENENEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEEENENEENNEEE;
REGISTER_BDT(NNNNEEENENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEENENEENNEENEE;
REGISTER_BDT(NNNNEEENENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENNEENENEE;
REGISTER_BDT(NNNNEEENENEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENENEENNEENNEEE;
REGISTER_BDT(NNNNEEENENEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENNEENNEENEE;
REGISTER_BDT(NNNNEEENENEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEENENEENNENEEE;
REGISTER_BDT(NNNNEEENENEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEENENEENNENEENEE;
REGISTER_BDT(NNNNEEENENEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENNENEENENEE;
REGISTER_BDT(NNNNEEENENEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENENEENNENEENNEEE;
REGISTER_BDT(NNNNEEENENEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENNENEENNEENEE;
REGISTER_BDT(NNNNEEENENEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEEENENEENNNEEEE;
REGISTER_BDT(NNNNEEENENEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEENENEENNNEEENEE;
REGISTER_BDT(NNNNEEENENEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENNNEEENENEE;
REGISTER_BDT(NNNNEEENENEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENENEENNNEEENNEEE;
REGISTER_BDT(NNNNEEENENEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENNNEEENNEENEE;
REGISTER_BDT(NNNNEEENENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEENENEENNNEENEEE;
REGISTER_BDT(NNNNEEENENEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEENENEENNNEENEENEE;
REGISTER_BDT(NNNNEEENENEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENNNEENEENENEE;
REGISTER_BDT(NNNNEEENENEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENENEENNNEENEENNEEE;
REGISTER_BDT(NNNNEEENENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENENEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEEENENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf> NNNNEEENNEEEE;
REGISTER_BDT(NNNNEEENNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>> NNNNEEENNEEENEE;
REGISTER_BDT(NNNNEEENNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEEENNEEENENEE;
REGISTER_BDT(NNNNEEENNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENENENEE;
REGISTER_BDT(NNNNEEENNEEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEEENENNEEE;
REGISTER_BDT(NNNNEEENNEEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENENNEENEE;
REGISTER_BDT(NNNNEEENNEEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEEENNEEENNEEE;
REGISTER_BDT(NNNNEEENNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEENNEEENNEENEE;
REGISTER_BDT(NNNNEEENNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENNEENENEE;
REGISTER_BDT(NNNNEEENNEEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEEENNEENNEEE;
REGISTER_BDT(NNNNEEENNEEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENNEENNEENEE;
REGISTER_BDT(NNNNEEENNEEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEENNEEENNENEEE;
REGISTER_BDT(NNNNEEENNEEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEENNEEENNENEENEE;
REGISTER_BDT(NNNNEEENNEEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENNENEENENEE;
REGISTER_BDT(NNNNEEENNEEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEEENNENEENNEEE;
REGISTER_BDT(NNNNEEENNEEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENNENEENNEENEE;
REGISTER_BDT(NNNNEEENNEEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEEENNEEENNNEEEE;
REGISTER_BDT(NNNNEEENNEEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEENNEEENNNEEENEE;
REGISTER_BDT(NNNNEEENNEEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENNNEEENENEE;
REGISTER_BDT(NNNNEEENNEEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEEENNNEEENNEEE;
REGISTER_BDT(NNNNEEENNEEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENNNEEENNEENEE;
REGISTER_BDT(NNNNEEENNEEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEENNEEENNNEENEEE;
REGISTER_BDT(NNNNEEENNEEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEENNEEENNNEENEENEE;
REGISTER_BDT(NNNNEEENNEEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENNNEENEENENEE;
REGISTER_BDT(NNNNEEENNEEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEEENNNEENEENNEEE;
REGISTER_BDT(NNNNEEENNEEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEEENNEEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf> NNNNEEENNEENEEE;
REGISTER_BDT(NNNNEEENNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNNNEEENNEENEENEE;
REGISTER_BDT(NNNNEEENNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEEENNEENEENENEE;
REGISTER_BDT(NNNNEEENNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENENENEE;
REGISTER_BDT(NNNNEEENNEENEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEENEENENNEEE;
REGISTER_BDT(NNNNEEENNEENEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENENNEENEE;
REGISTER_BDT(NNNNEEENNEENEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEEENNEENEENNEEE;
REGISTER_BDT(NNNNEEENNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEENNEENEENNEENEE;
REGISTER_BDT(NNNNEEENNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENNEENENEE;
REGISTER_BDT(NNNNEEENNEENEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEENEENNEENNEEE;
REGISTER_BDT(NNNNEEENNEENEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENNEENNEENEE;
REGISTER_BDT(NNNNEEENNEENEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEENNEENEENNENEEE;
REGISTER_BDT(NNNNEEENNEENEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEENNEENEENNENEENEE;
REGISTER_BDT(NNNNEEENNEENEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENNENEENENEE;
REGISTER_BDT(NNNNEEENNEENEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEENEENNENEENNEEE;
REGISTER_BDT(NNNNEEENNEENEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENNENEENNEENEE;
REGISTER_BDT(NNNNEEENNEENEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEEENNEENEENNNEEEE;
REGISTER_BDT(NNNNEEENNEENEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEEENNEENEENNNEEENEE;
REGISTER_BDT(NNNNEEENNEENEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENNNEEENENEE;
REGISTER_BDT(NNNNEEENNEENEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEENEENNNEEENNEEE;
REGISTER_BDT(NNNNEEENNEENEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENNNEEENNEENEE;
REGISTER_BDT(NNNNEEENNEENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEEENNEENEENNNEENEEE;
REGISTER_BDT(NNNNEEENNEENEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEEENNEENEENNNEENEENEE;
REGISTER_BDT(NNNNEEENNEENEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENNNEENEENENEE;
REGISTER_BDT(NNNNEEENNEENEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEEENNEENEENNNEENEENNEEE;
REGISTER_BDT(NNNNEEENNEENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEEENNEENEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEEENNEENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DLeaf> NNNNEENEEEE;
REGISTER_BDT(NNNNEENEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DLeaf>> NNNNEENEEENEE;
REGISTER_BDT(NNNNEENEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEENEEENENEE;
REGISTER_BDT(NNNNEENEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEEENENENEE;
REGISTER_BDT(NNNNEENEEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEEENENNEEE;
REGISTER_BDT(NNNNEENEEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEEENENNEENEE;
REGISTER_BDT(NNNNEENEEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEENEEENNEEE;
REGISTER_BDT(NNNNEENEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEEENNEENEE;
REGISTER_BDT(NNNNEENEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEEENNEENENEE;
REGISTER_BDT(NNNNEENEEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEEENNEENNEEE;
REGISTER_BDT(NNNNEENEEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEEENNEENNEENEE;
REGISTER_BDT(NNNNEENEEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEEENNENEEE;
REGISTER_BDT(NNNNEENEEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEEENNENEENEE;
REGISTER_BDT(NNNNEENEEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEEENNENEENENEE;
REGISTER_BDT(NNNNEENEEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEEENNENEENNEEE;
REGISTER_BDT(NNNNEENEEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEEENNENEENNEENEE;
REGISTER_BDT(NNNNEENEEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEENEEENNNEEEE;
REGISTER_BDT(NNNNEENEEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEEENNNEEENEE;
REGISTER_BDT(NNNNEENEEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEEENNNEEENENEE;
REGISTER_BDT(NNNNEENEEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEEENNNEEENNEEE;
REGISTER_BDT(NNNNEENEEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEEENNNEEENNEENEE;
REGISTER_BDT(NNNNEENEEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEEENNNEENEEE;
REGISTER_BDT(NNNNEENEEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEEENNNEENEENEE;
REGISTER_BDT(NNNNEENEEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEEENNNEENEENENEE;
REGISTER_BDT(NNNNEENEEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEEENNNEENEENNEEE;
REGISTER_BDT(NNNNEENEEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEENEEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DLeaf> NNNNEENEENEEE;
REGISTER_BDT(NNNNEENEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>> NNNNEENEENEENEE;
REGISTER_BDT(NNNNEENEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEENEENEENENEE;
REGISTER_BDT(NNNNEENEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENENENEE;
REGISTER_BDT(NNNNEENEENEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENEENENNEEE;
REGISTER_BDT(NNNNEENEENEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENENNEENEE;
REGISTER_BDT(NNNNEENEENEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEENEENEENNEEE;
REGISTER_BDT(NNNNEENEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEENEENNEENEE;
REGISTER_BDT(NNNNEENEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENNEENENEE;
REGISTER_BDT(NNNNEENEENEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENEENNEENNEEE;
REGISTER_BDT(NNNNEENEENEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENNEENNEENEE;
REGISTER_BDT(NNNNEENEENEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEENEENNENEEE;
REGISTER_BDT(NNNNEENEENEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEENEENNENEENEE;
REGISTER_BDT(NNNNEENEENEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENNENEENENEE;
REGISTER_BDT(NNNNEENEENEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENEENNENEENNEEE;
REGISTER_BDT(NNNNEENEENEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENNENEENNEENEE;
REGISTER_BDT(NNNNEENEENEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEENEENEENNNEEEE;
REGISTER_BDT(NNNNEENEENEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEENEENNNEEENEE;
REGISTER_BDT(NNNNEENEENEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENNNEEENENEE;
REGISTER_BDT(NNNNEENEENEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENEENNNEEENNEEE;
REGISTER_BDT(NNNNEENEENEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENNNEEENNEENEE;
REGISTER_BDT(NNNNEENEENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEENEENNNEENEEE;
REGISTER_BDT(NNNNEENEENEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEENEENNNEENEENEE;
REGISTER_BDT(NNNNEENEENEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENNNEENEENENEE;
REGISTER_BDT(NNNNEENEENEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENEENNNEENEENNEEE;
REGISTER_BDT(NNNNEENEENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEENEENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DLeaf> NNNNEENEENENEEE;
REGISTER_BDT(NNNNEENEENENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNNNEENEENENEENEE;
REGISTER_BDT(NNNNEENEENENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEENEENENEENENEE;
REGISTER_BDT(NNNNEENEENENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENENENEE;
REGISTER_BDT(NNNNEENEENENEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENENEENENNEEE;
REGISTER_BDT(NNNNEENEENENEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENENNEENEE;
REGISTER_BDT(NNNNEENEENENEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEENEENENEENNEEE;
REGISTER_BDT(NNNNEENEENENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEENENEENNEENEE;
REGISTER_BDT(NNNNEENEENENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENNEENENEE;
REGISTER_BDT(NNNNEENEENENEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENENEENNEENNEEE;
REGISTER_BDT(NNNNEENEENENEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENNEENNEENEE;
REGISTER_BDT(NNNNEENEENENEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEENENEENNENEEE;
REGISTER_BDT(NNNNEENEENENEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEENENEENNENEENEE;
REGISTER_BDT(NNNNEENEENENEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENNENEENENEE;
REGISTER_BDT(NNNNEENEENENEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENENEENNENEENNEEE;
REGISTER_BDT(NNNNEENEENENEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENNENEENNEENEE;
REGISTER_BDT(NNNNEENEENENEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEENEENENEENNNEEEE;
REGISTER_BDT(NNNNEENEENENEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEENENEENNNEEENEE;
REGISTER_BDT(NNNNEENEENENEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENNNEEENENEE;
REGISTER_BDT(NNNNEENEENENEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENENEENNNEEENNEEE;
REGISTER_BDT(NNNNEENEENENEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENNNEEENNEENEE;
REGISTER_BDT(NNNNEENEENENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEENENEENNNEENEEE;
REGISTER_BDT(NNNNEENEENENEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEENENEENNNEENEENEE;
REGISTER_BDT(NNNNEENEENENEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENNNEENEENENEE;
REGISTER_BDT(NNNNEENEENENEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENENEENNNEENEENNEEE;
REGISTER_BDT(NNNNEENEENENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENENEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEENEENENEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DLeaf> NNNNEENEENNEEEE;
REGISTER_BDT(NNNNEENEENNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DLeaf>> NNNNEENEENNEEENEE;
REGISTER_BDT(NNNNEENEENNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEENEENNEEENENEE;
REGISTER_BDT(NNNNEENEENNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENENENEE;
REGISTER_BDT(NNNNEENEENNEEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEEENENNEEE;
REGISTER_BDT(NNNNEENEENNEEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENENNEENEE;
REGISTER_BDT(NNNNEENEENNEEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEENEENNEEENNEEE;
REGISTER_BDT(NNNNEENEENNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEENNEEENNEENEE;
REGISTER_BDT(NNNNEENEENNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENNEENENEE;
REGISTER_BDT(NNNNEENEENNEEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEEENNEENNEEE;
REGISTER_BDT(NNNNEENEENNEEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENNEENNEENEE;
REGISTER_BDT(NNNNEENEENNEEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEENNEEENNENEEE;
REGISTER_BDT(NNNNEENEENNEEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEENNEEENNENEENEE;
REGISTER_BDT(NNNNEENEENNEEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENNENEENENEE;
REGISTER_BDT(NNNNEENEENNEEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEEENNENEENNEEE;
REGISTER_BDT(NNNNEENEENNEEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENNENEENNEENEE;
REGISTER_BDT(NNNNEENEENNEEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEENEENNEEENNNEEEE;
REGISTER_BDT(NNNNEENEENNEEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEENNEEENNNEEENEE;
REGISTER_BDT(NNNNEENEENNEEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENNNEEENENEE;
REGISTER_BDT(NNNNEENEENNEEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEEENNNEEENNEEE;
REGISTER_BDT(NNNNEENEENNEEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENNNEEENNEENEE;
REGISTER_BDT(NNNNEENEENNEEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEENNEEENNNEENEEE;
REGISTER_BDT(NNNNEENEENNEEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEENNEEENNNEENEENEE;
REGISTER_BDT(NNNNEENEENNEEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENNNEENEENENEE;
REGISTER_BDT(NNNNEENEENNEEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEEENNNEENEENNEEE;
REGISTER_BDT(NNNNEENEENNEEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEENEENNEEENNNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DLeaf> NNNNEENEENNEENEEE;
REGISTER_BDT(NNNNEENEENNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DLeaf>> NNNNEENEENNEENEENEE;
REGISTER_BDT(NNNNEENEENNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>> NNNNEENEENNEENEENENEE;
REGISTER_BDT(NNNNEENEENNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENENENEE;
REGISTER_BDT(NNNNEENEENNEENEENENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEENEENENNEEE;
REGISTER_BDT(NNNNEENEENNEENEENENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DLeaf,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENENNEENEE;
REGISTER_BDT(NNNNEENEENNEENEENENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>> NNNNEENEENNEENEENNEEE;
REGISTER_BDT(NNNNEENEENNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEENNEENEENNEENEE;
REGISTER_BDT(NNNNEENEENNEENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENNEENENEE;
REGISTER_BDT(NNNNEENEENNEENEENNEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEENEENNEENNEEE;
REGISTER_BDT(NNNNEENEENNEENEENNEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENNEENNEENEE;
REGISTER_BDT(NNNNEENEENNEENEENNEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEENNEENEENNENEEE;
REGISTER_BDT(NNNNEENEENNEENEENNENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEENNEENEENNENEENEE;
REGISTER_BDT(NNNNEENEENNEENEENNENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENNENEENENEE;
REGISTER_BDT(NNNNEENEENNEENEENNENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEENEENNENEENNEEE;
REGISTER_BDT(NNNNEENEENNEENEENNENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DLeaf,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENNENEENNEENEE;
REGISTER_BDT(NNNNEENEENNEENEENNENEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DLeaf>> NNNNEENEENNEENEENNNEEEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DLeaf>>> NNNNEENEENNEENEENNNEEENEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENNNEEENENEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEENEENNNEEENNEEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DLeaf>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENNNEEENNEENEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEEENNEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DLeaf>> NNNNEENEENNEENEENNNEENEEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEENEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DLeaf>>> NNNNEENEENNEENEENNNEENEENEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEENEENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DLeaf,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENNNEENEENENEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEENEENENEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DLeaf>>> NNNNEENEENNEENEENNNEENEENNEEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEENEENNEEE);

template class DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>>;
typedef DTree<DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>,DTree<DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>,DTree<DTree<DLeaf,DLeaf>,DTree<DLeaf,DLeaf>>>> NNNNEENEENNEENEENNNEENEENNEENEE;
REGISTER_BDT(NNNNEENEENNEENEENNNEENEENNEENEE);

#endif
