//
// Created by annfe on 16/01/2025.
//

#include "tests.h"

#include <ctype.h>

void testAddConst() {
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
    tConst* const2 = constIdentifier2->pointerObject;

    if (const2 != NULL) {
        printf("Found %s = %d in proc: %d\n", constIdentifier2->pointerName, const2->value, currProcedure->indexProcedure);
        printf("Length of the list: %d\n", constList->listLength);
        for (int i = 0; i < constList->listLength; i++) {
            tConst* item = getNext(constList);
            printf("Const block elements: %d\n", item->value);
        }
    }
}


void testLexer() {
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

}

void testAddVariable() {
    // To test:
    // 1. Variable already exist in the procedure
    // 2. No error if variable exist in a global function
    // 3. Identifier and variable exist in the procedure
    // 4. Relative addresses are set correctly

    // Identifier and variable exist in the procedure 0
    // Relative address of the variable is 0, of the procedure is 4
    printf("Test 1: \n");
    addVarIdentifier("var1");
    tIdentifier* identVar1 = searchIdentifierGlobal("var1");
    tVar* var1 = identVar1->pointerObject;
    printf("Found identifier %s with relative address: %d in proc: %d\n", identVar1->pointerName, var1->relativeAddress, currProcedure->indexProcedure);
    printf("Relative address of the procedure: %d\n", currProcedure->lengthVar);
    printf("Length of the list: %d\n", currProcedure->pListIdentifier->listLength);

    // Identifier and variable exist in the procedure 0
    // Relative address of the variable is 4, of the procedure is 8
    printf("\nTest 2: \n");
    addVarIdentifier("var2");
    tIdentifier* identVar2 = searchIdentifierGlobal("var2");
    tVar* var2 = identVar2->pointerObject;
    printf("Found identifier %s with relative address: %d in proc: %d\n", identVar2->pointerName, var2->relativeAddress, currProcedure->indexProcedure);
    printf("Relative address of the procedure: %d\n", currProcedure->lengthVar);
    printf("Length of the list: %d\n", currProcedure->pListIdentifier->listLength);

    // Identifier and variable exist in the procedure 1
    // Relative address of the variable is 0, of the procedure is 4
    printf("\nTest 3: \n");
    tProcedure* proc1 = createProcedure(currProcedure);
    currProcedure = proc1;
    addVarIdentifier("var3");
    tIdentifier* identVar3 = searchIdentifierGlobal("var3");
    tVar* var3 = identVar3->pointerObject;
    printf("Found identifier %s with relative address: %d in proc: %d\n", identVar3->pointerName, var3->relativeAddress, currProcedure->indexProcedure);
    printf("Relative address of the procedure: %d\n", currProcedure->lengthVar);
    printf("Length of the list: %d\n", currProcedure->pListIdentifier->listLength);

    // No error if variable exist in a global function
    printf("\nTest 4: \n");
    addVarIdentifier("var1");
    tIdentifier* identVar4 = searchIdentifierGlobal("var1");
    tVar* var4 = identVar4->pointerObject;
    printf("Found identifier %s with relative address: %d in proc: %d\n", identVar4->pointerName, var4->relativeAddress, currProcedure->indexProcedure);
    printf("Relative address of the procedure: %d\n", currProcedure->lengthVar);
    printf("Length of the list: %d\n", currProcedure->pListIdentifier->listLength);

    // Variable already exist in the procedure
    // Should throw error
    printf("\nTest 5: \n");
    addVarIdentifier("var1");
}

void testAddProcedure() {
    // To test:
    // 1. Procedure already exists
    // 2. Procedures are correctly set

    printf("Test 1: \n");
    tProcedure* mainProcedure = currProcedure;
    addProcedureIdentifier("proc1");
    tIdentifier* identProc1 = searchIdentifierGlobal("proc1");
    tProcedure* proc1 = identProc1->pointerObject;
    printf("Found identifier %s with index: %d\n", identProc1->pointerName, proc1->indexProcedure);
    if (proc1->pointerParent == mainProcedure) {
        printf("parent procedure is set correctly\n");
    }
    if (currProcedure == proc1) {
        printf("current procedure is set correctly\n");
    }

    printf("Length of the list of the main procedure: %d\n", mainProcedure->pListIdentifier->listLength);
    printf("Length of the list of the new procedure: %d\n", proc1->pListIdentifier->listLength);

    printf("\nTest 2: \n");
    currProcedure = mainProcedure;
    addProcedureIdentifier("proc1");
}


void testEndProcedure() {
    // To test:
    // List is empty
    // Identifiers are deleted

    tProcedure* mainProcedure = currProcedure;

    printf("Test 1: \n");
    addProcedureIdentifier("proc1");
    addVarIdentifier("var1");

    endProcedure();

    if (currProcedure == mainProcedure) {
        printf("parent procedure is set correctly\n");
    }

    printf("Length of the list of the main procedure: %d\n", mainProcedure->pListIdentifier->listLength);
    tIdentifier* ident = getFirst(mainProcedure->pListIdentifier);
    tProcedure* proc1 = ident->pointerObject;
    printf("Length of the list of the deleted procedure: %d\n", proc1->pListIdentifier->listLength);
}