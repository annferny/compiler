/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#include "namelist.h"
#include "CodeGen.h"
#include "parser.h"
#include "stack.h"

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
static int condition_operator;

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
        insertTail(constList, temp);
    }
    code(puConst, temp->index);
    return 1;
}

int fa2() {
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
    code(storeVal);
    return OK;
}

int st3() {
    tLabel *label = createLabel();
    pushLabel(label);
    code(jnot, 0);

    return 1;
}

int st4() {
    tLabel *label = popLabel();

    if (label == NULL) {
        perror("Label stack is empty");
        exit(-1);
    }

    int relativeAddress = (pCode - vCode) - label->indexJump - 3;
    wr2ToCodeAtP(relativeAddress, vCode + label->indexJump + 1);

    free(label);

    return 1;
}

int st5() {
    tLabel *label1 = createLabel();
    pushLabel(label1);

    return 1;
}

int st6() {
    tLabel *label2 = createLabel();

    pushLabel(label2);

    code(jnot, 0);

    return 1;
}

int st7() {
    tLabel *label2 = popLabel();

    if (label2 == NULL) {
        perror("Label stack is empty");
        exit(-1);
    }

    int relativeAddress = (pCode - vCode) - label2->indexJump - 3; // -3 for jnot instruction size

    wr2ToCodeAtP(relativeAddress, vCode + label2->indexJump + 1);

    tLabel *label1 = popLabel();

    if (label1 == NULL) {
        perror("Label stack is empty");
        exit(-1);
    }

    relativeAddress = label1->indexJump - (pCode - vCode) - 3; // -3 for jmp instruction size

    code(jmp, relativeAddress);

    free(label1);
    free(label2);

    return 1;
}

int st8() {
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
    code(putVal);
    return 1;
}

int pr1() {
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

int co1()
{
    return code(odd);
}
//co2-7
int set_cond_opp()
{
    condition_operator=Morph.Value.symbol;
    return 1;
}
//co8
int co8()
{
    switch(condition_operator) {
        case '=':
            code(cmpEQ);
        break;
        case '#':
            code(cmpNE);
        break;
        case '<':
            code(cmpLT);
        break;
        case '>':
            code(cmpGT);
        break;
        case zLessOrEqual:
            code(cmpLE);
        break;
        case zGreaterOrEqual:
            code(cmpGE);
        break;
        default:
            return FAIL;
    }
    return OK;
}