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
int bl6();

int pr1();

int st1();
int st2();
int st3();
int st4();
int st5();
int st6();
int st7();
int st8();
int st9();
int st10();

int fa1();
int fa2();

int ex1();
int ex2();
int ex3();

int te1();
int te2();

int co1();
int set_cond_opp();
int co8();

#endif //NAMELIST_H

