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
void addConstIdentifier(char* pIdentifier);
void addConstToIdentifier(int32_t value);
void addVarIdentifier(char* pIdentifier);
void addProcedureIdentifier(char* pIdentifier);
int endProcedure();

int pr1();
int st10();
int fa1();
int bl6();

#endif //NAMELIST_H

