//
// Created by annfe on 16/01/2025.
//

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



#endif //TESTS_H
