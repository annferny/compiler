/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "parser.h"
#include "lex.h"
#include "namelist.h"
#include "tests.h"
#include "CodeGen.h"

extern tMorph Morph;
extern tBog gProgramm[];
extern List* constList;

extern tProcedure* currProcedure;

int main(int argc, void *argv[])
{
    if (argc != 2) {
        printf("Error: enter file name\n");
        exit(-1);
    }
   
    initLexer(argv[1]);
    openOFile(argv[1]);

    tProcedure* mainProcedure = createProcedure(NULL);
    currProcedure = mainProcedure;
    constList = createList();


    if (pars(gProgramm) == 1) printf("OK\n");

    deleteList(constList);
    return 0;
}
