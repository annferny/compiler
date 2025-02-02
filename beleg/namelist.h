/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#ifndef NAMELIST_H
#define NAMELIST_H

#include "structs.h"
#include "list.h"

tIdentifier* createIdentifier(char* pIdentifier);
tConst* createConst(int32_t val);
tConst* searchConst(int32_t Val);
tVar* createVar(int relativeAddress);
tProcedure* createProcedure(tProcedure* pParent);
tIdentifier* searchIdentifierLocal(tProcedure* pProcedure,char* pIdentifier);
tIdentifier* searchIdentifierGlobal(char* pIdentifier);
int addConstIdentifier();
int addConstToIdentifier();
int addVarIdentifier();
int addProcedureIdentifier();
int endProcedure();

int pr1();
int st10();
int fa1();
int fa2();
int bl6();

int ex1();
int ex2();
int ex3();

int te1();
int te2();

#endif //NAMELIST_H

