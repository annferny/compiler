//
// Created by annfe on 12/12/2024.
//

#include "namelist.h"

#include <stdlib.h>

List* constList;

tIdentifier *createIdentifier(char *pIdentifier)
{
    tIdentifier *identifier = malloc(sizeof(tIdentifier));
    identifier->kennzeichen = KzIdentifier;
    identifier->pointerName = pIdentifier;
    return identifier;
}

tConst *createConst(int32_t val)
{
    tConst *constant = malloc(sizeof(tConst));
    constant->kennzeichen = KzConst;
    constant->value = val;
    return constant;
}

tVar *createVar(void)
{
    tVar *var = malloc(sizeof(tVar));
    var->kennzeichen = KzVar;
    return var;
}

tProcedure *createProcedure(tProcedure *pParent)
{
    tProcedure *procedure = malloc(sizeof(tProcedure));
    procedure->kennzeichen = KzProcedure;
    if (pParent == NULL)
    {
        procedure->indexProcedure = 0;
    }
    else
    {
        procedure->indexProcedure = pParent->indexProcedure++;
    }
    procedure->pointerParent = pParent;
    procedure->lengthVar = 0;
    procedure->pListIdentifier = createList();
    return procedure;
}

// TODO: resolve problem with memory leak
tConst* searchConst(int32_t Val) 
{
    tConst* temp = getFirst(constList);
    
    for (int i = 0; i < constList->listLength; i++) {
        printf("bla\n");
        if (temp->value == Val) return temp;
        else temp = getNext(constList);
    }
    return NULL;
}
