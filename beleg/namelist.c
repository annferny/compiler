//
// Created by annfe on 12/12/2024.
//

#include "namelist.h"

#include <stdlib.h>

tIdentifier* createIdentifier(char* pIdentifier) {
    tIdentifier* identifier = malloc(sizeof(tIdentifier));
    identifier->kennzeichen = KzIdentifier;
    identifier->pointerName = pIdentifier;
    return identifier;
}

tConst* createConst(int32_t val) {
    tConst* constant = malloc(sizeof(tConst));
    constant->kennzeichen = KzConst;
    constant->value = val;
    return constant;
}

tVar* createVar(void) {
    tVar* var = malloc(sizeof(tVar));
    var->kennzeichen = KzVar;
    return var;
}
tProcedure* createProcedure(tProcedure* pParent) {
    tProcedure* procedure = malloc(sizeof(tProcedure));
    procedure->kennzeichen = KzProcedure;
    procedure->indexProcedure = pParent->indexProcedure ++;
    procedure->pointerParent = pParent;
    procedure->pListIdentifier = createList();
    return procedure;
}

