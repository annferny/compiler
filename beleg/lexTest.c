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
    constList = createList();

    tIdentifier* identifierVarGlob = createIdentifier("var2");
    tVar *pVGlob = createVar(currProcedure->lengthVar);
    pVGlob->relativeAddress = currProcedure->lengthVar;
    currProcedure->lengthVar +=4;
    identifierVarGlob->pointerObject = pVGlob;
    insertHead(currProcedure->pListIdentifier, identifierVarGlob);

    addConstIdentifier("const1");
    addConstToIdentifier(3);
    tIdentifier* constIdentifier1 = searchIdentifierLocal(currProcedure, "const1");
    tConst* const1 = constIdentifier1->pointerObject;

    if (const1 != NULL) {
        printf("Found %s = %d in proc: %d\n", constIdentifier1->pointerName, const1->value, currProcedure->indexProcedure);
    }

    tIdentifier* identifierProc = createIdentifier("proc1");
    insertHead(currProcedure->pListIdentifier, identifierProc);
    tProcedure* proc1 = createProcedure(currProcedure);
    currProcedure = proc1;

    tIdentifier* identifier = createIdentifier("var1");
    tVar *pV = createVar(currProcedure->lengthVar);
    pV->relativeAddress = currProcedure->lengthVar;
    currProcedure->lengthVar +=4;
    identifier->pointerObject = pV;
    insertHead(currProcedure->pListIdentifier, identifier);

    tIdentifier *searchedIdentifier = searchIdentifierGlobal("var2");
    if (searchedIdentifier != NULL) {
        printf("Found: %s\n", searchedIdentifier->pointerName);
    } else {
        printf("unsuccessful\n");
    }

    addConstIdentifier("const2");
    addConstToIdentifier(3);
    tIdentifier* constIdentifier2 = searchIdentifierLocal(currProcedure, "const2");
    tConst* const2 = constIdentifier1->pointerObject;


    // TODO: check the function getNext. Seems not to work
    if (const2 != NULL) {
        printf("Found %s = %d in proc: %d\n", constIdentifier2->pointerName, const2->value, currProcedure->indexProcedure);
        printf("%d\n", constList->listLength);
        for (int i = 0; i < constList->listLength; i++) {
            printf("bla\n");
            tConst* item = getCurrentItem(constList);
            printf("bla\n");
            if (item == NULL) {
                printf("bla\n");
            }
            printf("Const block elements: %d\n", item->value);
        }
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