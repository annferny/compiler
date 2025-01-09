//
// Created by annfe on 12/12/2024.
//

#include "namelist.h"

#include <stdlib.h>

List* constList;
tProcedure* currProcedure;
int numProc = 0;

// TODO: probably add an update for the current procedure
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

tVar *createVar(int relativeAddress)
{
    tVar *var = malloc(sizeof(tVar));
    var->kennzeichen = KzVar;
    var->relativeAddress = relativeAddress;
    return var;
}

// TODO: probably add an update for the current procedure
tProcedure *createProcedure(tProcedure *pParent)
{
    tProcedure *procedure = malloc(sizeof(tProcedure));
    procedure->kennzeichen = KzProcedure;

    procedure->indexProcedure = numProc++;
    printf("bla %d %d\n", procedure->indexProcedure, numProc);
    
    procedure->pointerParent = pParent;
    procedure->lengthVar = 0;
    procedure->pListIdentifier = createList();
    return procedure;
}

tConst* searchConst(int32_t Val) 
{
    tConst* temp = getFirst(constList);
    
    for (int i = 0; i < constList->listLength; i++) {
        if (temp->value == Val) return temp;
        else temp = getNext(constList);
    }
    return NULL;
}

tIdentifier* searchIdentifier(tProcedure* pProcedure, char* pIdentifier) {
    tIdentifier* temp = getFirst(pProcedure->pListIdentifier);
    
    for (int i = 0; i < pProcedure->pListIdentifier->listLength; i++) {
        if (temp->pointerName == pIdentifier) return temp;
        else temp = getNext(pProcedure->pListIdentifier);
    }
    return NULL;
}

tIdentifier* searchIdentifierGlobal(char* pIdentifier){
    tProcedure *tempProcedure = currProcedure;
    tIdentifier *tempIdentifier;
    while (tempProcedure->indexProcedure >= 0)
    {
        tempIdentifier = searchIdentifier(tempProcedure, pIdentifier);
        if (tempIdentifier != NULL) {
            printf("Found in proc nr.: %d\n", tempProcedure->indexProcedure);
            return tempIdentifier;
        } 
        else tempProcedure = tempProcedure->pointerParent;
        if (tempProcedure->indexProcedure == 0) break;
    }
    return NULL;
}
