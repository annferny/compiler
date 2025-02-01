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

// TODO: probably add an update for the current procedure
tIdentifier *createIdentifier(char *pIdentifier) {
    tIdentifier *identifier = malloc(sizeof(tIdentifier));
    identifier->kennzeichen = KzIdentifier;
    identifier->pointerName = pIdentifier;
    return identifier;
}

tConst *createConst(int32_t val) {
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
        if (temp->pointerName == pIdentifier) return temp;
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

void addConstIdentifier(char *pIdentifier) {
    // bl1
    if (searchIdentifierLocal(currProcedure, pIdentifier) != NULL) {
        printf("Identifier \"%s\" already exists in proc nr.: %d\n", pIdentifier, currProcedure->indexProcedure);
        exit(-1);
    }

    tIdentifier *constIdentifier = createIdentifier(pIdentifier);
    insertHead(currProcedure->pListIdentifier, constIdentifier);
}

void addConstToIdentifier(int32_t value) {
    // bl2
    tConst *temp = searchConst(value);
    if (temp == NULL) {
        temp = createConst(value);
        insertHead(constList, temp);
    }
    tIdentifier *constIdentifier = getFirst(currProcedure->pListIdentifier);
    constIdentifier->pointerObject = temp;
}

void addVarIdentifier(char *pIdentifier) {
    // bl3
    if (searchIdentifierLocal(currProcedure, pIdentifier) != NULL) {
        printf("Identifier \"%s\" already exists in proc nr.: %d\n", pIdentifier, currProcedure->indexProcedure);
        exit(-1);
    }

    tIdentifier *identifier = createIdentifier(pIdentifier);
    insertHead(currProcedure->pListIdentifier, identifier);

    tVar *variable = createVar(currProcedure->lengthVar);
    currProcedure->lengthVar += 8;

    identifier->pointerObject = variable;
}

void addProcedureIdentifier(char* pIdentifier) { // bl4
    if (searchIdentifierLocal(currProcedure, pIdentifier) != NULL) {
        printf("Identifier \"%s\" already exists in proc nr.: %d\n", pIdentifier, currProcedure->indexProcedure);
        exit(-1);
    }

    tIdentifier *identifier = createIdentifier(pIdentifier);
    insertHead(currProcedure->pListIdentifier, identifier);

    tProcedure *procedure = createProcedure(currProcedure);
    identifier->pointerObject = procedure;
    currProcedure = procedure;
}

int endProcedure() { // bl5
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
    int value = Morph.Value.number;
    tConst * temp = searchConst(value);
    if (temp == NULL) {
        temp = createConst(value);
        insertHead(constList, temp);
    }
    code(puConst, temp->index);
    return 1;
}

int st10() {
    code(putVal);
    return 1;
}

int pr1() {
    write_consts2file();
    return closeOFile();
}