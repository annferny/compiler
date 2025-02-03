/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#ifndef STACK_H
#define STACK_H

#include "structs.h"


tLabel* createLabel();
void pushLabel(tLabel *label);
tLabel* popLabel();

typedef struct tLabelStackNode {
    tLabel *label;
    struct tLabelStackNode *next;
} tLabelStackNode;


#endif //STACK_H
