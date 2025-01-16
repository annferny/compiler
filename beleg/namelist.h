//
// Created by annfe on 12/12/2024.
//

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

#endif //NAMELIST_H

