// Arnold Beck

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "parser.h"
#include "lex.h"
#include "namelist.h"

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

    tProcedure* mainProcedure = createProcedure(NULL);
    currProcedure = mainProcedure;
    printf("current proc nr: %d\n", currProcedure->indexProcedure);
    constList = createList();

    tIdentifier* identifierProc = createIdentifier("proc1");
    insertHead(currProcedure->pListIdentifier, identifierProc);
    tProcedure* proc1 = createProcedure(currProcedure);
    currProcedure = proc1;
    printf("current proc nr: %d\n", currProcedure->indexProcedure);

    printf("proc1 nr: %d\n", proc1->indexProcedure);

    tIdentifier* identifier = createIdentifier("var1");
    tVar *pV = createVar(currProcedure->lengthVar);
    pV->relativeAddress = currProcedure->lengthVar;
    currProcedure->lengthVar +=4;
    identifier->pointerObject = pV;
    insertHead(currProcedure->pListIdentifier, identifier);

    tIdentifier *searchedIdentifier = searchIdentifierGlobal("var1");
    if (searchedIdentifier != NULL) {
        printf("Found: %s\n", searchedIdentifier->pointerName);
    } else {
        printf("unsuccsess\n");
    }

    /*
    do
    {
        Lexer();
        printf("Line%4d, Col%3d: ", Morph.positionOfLine, Morph.positionOfColumn);
        switch (Morph.morphemeCode)
        {
        case morphemeCodeSymbol:
            if (Morph.Value.symbol == zResult)
                printf("Symbol,:=\n");
            else if (Morph.Value.symbol == zLessOrEqual)
                printf("Symbol,<=\n");
            else if (Morph.Value.symbol == zGreaterOrEqual)
                printf("Symbol,>=\n");
            else if (Morph.Value.symbol == zBegin)
                printf("Symbol,_BEGIN\n");
            else if (Morph.Value.symbol == zCall)
                printf("Symbol,_CALL\n");
            else if (Morph.Value.symbol == zConst)
                printf("Symbol,_CONST\n");
            else if (Morph.Value.symbol == zDo)
                printf("Symbol,_DO\n");
            else if (Morph.Value.symbol == zEnd)
                printf("Symbol,_END\n");
            else if (Morph.Value.symbol == zIf)
                printf("Symbol,_IF\n");
            else if (Morph.Value.symbol == zOdd)
                printf("Symbol,_ODD\n");
            else if (Morph.Value.symbol == zProcedure)
                printf("Symbol,_PROCEDURE\n");
            else if (Morph.Value.symbol == zThen)
                printf("Symbol,_THEN\n");
            else if (Morph.Value.symbol == zVar)
                printf("Symbol,_VAR\n");
            else if (Morph.Value.symbol == zWhile)
                printf("Symbol,_WHILE\n");

            if (isprint(Morph.Value.symbol))
                printf("Symbol,%c\n", (char)Morph.Value.symbol);
            break;
        case morphemeCodeNumber:
            printf("Zahl  ,%ld\n", Morph.Value.number);
            break;
        case morphemeCodeIdentifier:
            printf("Ident ,%s\n", Morph.Value.string);
            break;
        default: printf("Unknown morpheme code type: %d", Morph.morphemeCode); break;
        }

        //getchar();
    } while (!(Morph.morphemeCode == morphemeCodeSymbol && Morph.Value.symbol == -1));

*/

    // int x = pars(gProgramm);
    printf("Successfully parsed\n");

    deleteList(constList);

    return 0;
}