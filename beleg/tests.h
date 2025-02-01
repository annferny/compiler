/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#ifndef TESTS_H
#define TESTS_H

#include "parser.h"
#include "lex.h"
#include "namelist.h"

extern tMorph Morph;
extern tBog gProgramm[];
extern List* constList;

extern tProcedure* currProcedure;

void testAddConst();
void testLexer();
void testAddVariable();
void testAddProcedure();
void testEndProcedure();


#endif //TESTS_H
