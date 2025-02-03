/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#include "namelist.h"
#include "CodeGen.h"
#include "parser.h"

#include <stdlib.h>

List *constList;
tProcedure *currProcedure;
short numProc = 0;
extern int LenCode;
extern tMorph Morph;
char*  vCode;    /* Pointer auf dynamischen Bereich fuer Code */ //start
char*  pCode; //curr
unsigned int cbuf_size;
int    iCode;

tIdentifier *createIdentifier(char *pIdentifier) {
    tIdentifier *identifier = malloc(sizeof(tIdentifier));
    identifier->kennzeichen = KzIdentifier;
    identifier->pointerName = pIdentifier;
    return identifier;
}

tConst *createConst(int val) {
    tConst *constant = malloc(sizeof(tConst));
    constant->kennzeichen = KzConst;
    constant->value = val;
    constant->index = constList->listLength;
    return constant;
}

tVar *createVar(int relativeAddress) {
    tVar *var = malloc(sizeof(tVar));
    var->kennzeichen = KzVar;
    var->relativeAddress = relativeAddress;
    return var;
}

tProcedure *createProcedure(tProcedure *pParent) {
    tProcedure *procedure = malloc(sizeof(tProcedure));
    procedure->kennzeichen = KzProcedure;

    procedure->indexProcedure = numProc++;

    procedure->pointerParent = pParent;
    procedure->lengthVar = 0;
    procedure->pListIdentifier = createList();
    return procedure;
}

tConst *searchConst(int32_t Val) {
    tConst *temp = getFirst(constList);

    for (int i = 0; i < constList->listLength; i++) {
        if (temp->value == Val) return temp;
        temp = getNext(constList);
    }
    return NULL;
}

tIdentifier *searchIdentifierLocal(tProcedure *pProcedure, char *pIdentifier) {
    tIdentifier *temp = getFirst(pProcedure->pListIdentifier);
    for (int i = 0; i < pProcedure->pListIdentifier->listLength; i++) {
        if (strcmp(temp->pointerName, pIdentifier) == 0) return temp;
        temp = getNext(pProcedure->pListIdentifier);
    }
    return NULL;
}

tIdentifier *searchIdentifierGlobal(char *pIdentifier) {
    tProcedure *tempProcedure = currProcedure;
    while (tempProcedure->indexProcedure >= 0) {
        tIdentifier *tempIdentifier = searchIdentifierLocal(tempProcedure, pIdentifier);
        if (tempIdentifier != NULL) return tempIdentifier;
        if (tempProcedure->indexProcedure == 0) break;
        tempProcedure = tempProcedure->pointerParent;
    }
    return NULL;
}

int addConstIdentifier() { // bl1
    printf("bl1\n");
    char *pIdentifier = Morph.Value.string;
    if (searchIdentifierLocal(currProcedure, pIdentifier) != NULL) {
        printf("Identifier \"%s\" already exists in proc nr.: %d\n", pIdentifier, currProcedure->indexProcedure);
        exit(-1);
    }

    tIdentifier *constIdentifier = createIdentifier(strdup(pIdentifier));
    constIdentifier->kennzeichen = KzConst;
    insertTail(currProcedure->pListIdentifier, constIdentifier);
    return 1;
}

int addConstToIdentifier() {// bl2
    printf("bl2\n");
    int value = Morph.Value.number;
    tConst *temp = searchConst(value);
    if (temp == NULL) {
        temp = createConst(value);
        insertTail(constList, temp);
    }
    tIdentifier *constIdentifier = getFirst(currProcedure->pListIdentifier);
    constIdentifier->pointerObject = temp;
    return 1;
}

int addVarIdentifier() { // bl3
    printf("bl3\n");
    char *pIdentifier = Morph.Value.string;
    if (searchIdentifierLocal(currProcedure, pIdentifier) != NULL) {
        printf("Identifier \"%s\" already exists in proc nr.: %d\n", pIdentifier, currProcedure->indexProcedure);
        exit(-1);
    }

    tIdentifier *identifier = createIdentifier(strdup(pIdentifier));
    identifier->kennzeichen = KzVar;
    insertTail(currProcedure->pListIdentifier, identifier);

    tVar *variable = createVar(currProcedure->lengthVar);
    currProcedure->lengthVar += 8;

    identifier->pointerObject = variable;
    return 1;
}

int addProcedureIdentifier() { // bl4
    printf("bl4\n");
    char *pIdentifier = Morph.Value.string;
    if (searchIdentifierLocal(currProcedure, pIdentifier) != NULL) {
        printf("Identifier \"%s\" already exists in proc nr.: %d\n", pIdentifier, currProcedure->indexProcedure);
        exit(-1);
    }

    tIdentifier *identifier = createIdentifier(strdup(pIdentifier));
    identifier->kennzeichen = KzProcedure;
    insertTail(currProcedure->pListIdentifier, identifier);

    tProcedure *procedure = createProcedure(currProcedure);
    identifier->pointerObject = procedure;
    currProcedure = procedure;
    return 1;
}

int endProcedure() { // bl5
    printf("bl5\n");
    code(retProc);
    //code(entryProc, LenCode, currProcedure->indexProcedure, currProcedure->lengthVar);
    if (CodeOut() != 1) {
        printf("Code out failed.\n");
    }
    deleteList(currProcedure->pListIdentifier);
    currProcedure = currProcedure->pointerParent;
    return 1;
}

int bl6() {
    printf("bl6\n");
    int init_size=1024;
    int len_code=0;
    int pidx=currProcedure->indexProcedure;
    int len_var=currProcedure->lengthVar;

    vCode=malloc(init_size);
    if (!vCode) {
        perror("malloc");
        exit(-1);
    }
    pCode=vCode;
    cbuf_size=init_size;
    code(entryProc, len_code, pidx, len_var);
    return 1;
}

int fa1() {
    printf("fa1\n");
    int value = Morph.Value.number;
    tConst * temp = searchConst(value);
    if (temp == NULL) {
        temp = createConst(value);
        insertTail(constList, temp);
    }
    code(puConst, temp->index);
    return 1;
}

int fa2() {
    printf("fa2\n");
    tIdentifier *bezeichner = searchIdentifierGlobal(Morph.Value.string);
    if (bezeichner == NULL) {
        perror("fa2: Bezeichner nicht gefunden\n");
        exit(-1);
    }

    switch (bezeichner->kennzeichen) {
        case KzConst:
            code(puConst, ((tConst*)bezeichner->pointerObject)->index);
            break;
        case KzVar:
            tVar *variable = bezeichner->pointerObject;
            if (bezeichner->indexProcedure == 0) {
                code(puValVrMain, variable->relativeAddress);
            } else if (bezeichner->indexProcedure == currProcedure->indexProcedure) {
                code(puValVrLocl, variable->relativeAddress);
            } else {
                code(puValVrGlob, variable->relativeAddress, bezeichner->indexProcedure);
            }
            break;
        case KzProcedure:
            perror("fa2: Bezeichner ist eine Prozedur\n");
            return 0;
        default: perror("fa2: unbekanntes Bezeichnertyp\n");;
    }
    return 1;
}

int st1() {
    printf("st1\n");
    tIdentifier *bezeichner = searchIdentifierGlobal(Morph.Value.string);
    if (bezeichner == NULL) {
        perror("st1: Bezeichner nicht gefunden\n");
        exit(-1);
    }

    if (bezeichner->kennzeichen == KzVar) {
        tVar *variable = bezeichner->pointerObject;
        if (bezeichner->indexProcedure == 0) {
            code(puAdrVrMain, variable->relativeAddress);
        } else if (bezeichner->indexProcedure == currProcedure->indexProcedure) {
            code(puAdrVrLocl, variable->relativeAddress);
        } else {
            code(puAdrVrGlob, variable->relativeAddress, bezeichner->indexProcedure);
        }
    } else {
        perror("st1: Bezeichner ist nicht die Variable\n");
        return FAIL;
    }

    return OK;
}

int st2() {
    printf("st2\n");
    code(storeVal);
    return OK;
}

int st8() {
    printf("st8\n");
    tIdentifier *bezeichner = searchIdentifierGlobal(Morph.Value.string);
    if (bezeichner == NULL) {
        perror("st8: Bezeichner nicht gefunden\n");
        exit(-1);
    }

    if (bezeichner->kennzeichen == KzProcedure) {
        code(call, ((tProcedure*)bezeichner->pointerObject)->indexProcedure);
    } else {
        perror("st8: Bezeichner ist nicht Prozedur\n");
        return FAIL;
    }

    return OK;
}

int st9() {
    printf("st9\n");
    tIdentifier *bezeichner = searchIdentifierGlobal(Morph.Value.string);
    if (bezeichner == NULL) {
        perror("st9: Bezeichner nicht gefunden\n");
        exit(-1);
    }

    if (bezeichner->kennzeichen == KzVar) {
        tVar *variable = bezeichner->pointerObject;
        if (bezeichner->indexProcedure == 0) {
            code(puAdrVrMain, variable->relativeAddress);
        } else if (bezeichner->indexProcedure == currProcedure->indexProcedure) {
            code(puAdrVrLocl, variable->relativeAddress);
        } else {
            code(puAdrVrGlob, variable->relativeAddress, bezeichner->indexProcedure);
        }
        code(getVal);
    } else {
        perror("st9: Bezeichner ist nicht die Variable\n");
        return FAIL;
    }

    return OK;
}

int st10() {
    printf("st10\n");
    code(putVal);
    return 1;
}

int pr1() {
    printf("pr1\n");
    write_consts2file();
    return closeOFile();
}


int ex1()
{
    return code(vzMinus);
}

int ex2()
{
    return code(OpAdd);
}

int ex3()
{
    return code(OpSub);
}

int te1()
{
    return code(OpMult);
}

int te2()
{
    return code(OpDiv);
}