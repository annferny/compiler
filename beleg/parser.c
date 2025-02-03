/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#include "parser.h"
#include <stdlib.h>
#include "namelist.h"

typedef unsigned long ul;
extern tMorph Morph;

int    LenCode;  /* Laenge des Codeausgabebereiches           */


tBog gStatement[], gBlock[], gTerm[], gFactor[], gCondition[], gProgramm[];

tBog gProgramm[] =
    {
        /* 0*/ {BgGr, {.G = gBlock}, NULL, 1, 0}, /*(0)---block--->(1)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgSy, {(ul)'.'}, pr1, 2, 0},                            /*(1)---'.'--->(E)*/
        /* 2*/ {BgEn, {(ul)0}, NULL, 0, 0}                               /*(E)--------(ENDE) */
};

tBog gBlock[] =
    {
        /* 0*/ {BgSy, {(ul)zConst}, NULL, 1, 6}, /*(0)---'CONST'-->(1)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgMo, {(ul)morphemeCodeIdentifier}, addConstIdentifier, 2, 0},         /*(1)---ident---->(2)*/
        /* 2*/ {BgSy, {(ul)'='}, NULL, 3, 0},                            /*(2)----'='----->(3)*/
        /* 3*/ {BgMo, {(ul)morphemeCodeNumber}, addConstToIdentifier, 4, 0},             /*(3)---number--->(4)*/
        /* 4*/ {BgSy, {(ul)','}, NULL, 1, 5},                            /*(4)----','----->(1)*/
        /* 5*/ {BgSy, {(ul)';'}, NULL, 6, 0},                            /*(5)----';'----->(6)*/
        /* 6*/ {BgSy, {(ul)zVar}, NULL, 7, 10},                          /*(6)---'Var'---->(7)*/
        /* 7*/ {BgMo, {(ul)morphemeCodeIdentifier}, addVarIdentifier, 8, 0},         /*(7)---ident---->(8)*/
        /* 8*/ {BgSy, {(ul)','}, NULL, 7, 9},                            /*(8)----','----->(7)*/
        /* 9*/ {BgSy, {(ul)';'}, NULL, 10, 0},                           /*(9)----';'----->(10)*/
        /*10*/ {BgSy, {(ul)zProcedure}, NULL, 11, 17},                   /*(10)--'Var'---->(11)*/
        /*11*/ {BgMo, {(ul)morphemeCodeIdentifier}, addProcedureIdentifier, 12, 0},        /*(11)---ident--->(12)*/
        /*12*/ {BgSy, {(ul)';'}, NULL, 13, 0},                           /*(12)----';'---->(13)*/
        /*13*/ {BgGr, {.G = gBlock}, NULL, 14, 0},                       /*(13)---block--->(14)*/
        /*14*/ {BgSy, {(ul)';'}, NULL, 10, 0},                           /*(14)----';'---->(10)*/
        /*15*/ {BgGr, {.G = gStatement}, endProcedure, 16, 0},           /*(15)---block--->(16)*/
        /*16*/ {BgEn, {(ul)0}, NULL, 0, 0},                              /*(E)--------(ENDE) */
        /*17*/ {BgNl, {(ul)0}, bl6, 15, 0},                              /*(3)---Nil------>(4)*/
};

tBog gExpression[] =
    {
        /* 0*/ {BgSy, {(ul)'-'}, NULL, 1, 2}, /*(0)----'-'----->(1)*/
        /* 1*/ {BgGr, {.G = gTerm}, ex1, 3, 0},  /*(1)---term----->(3)*/
        /* 2*/ {BgGr, {.G = gTerm}, NULL, 3, 0}, /*(2)---term----->(3)*/
        /* 3*/ {BgNl, {.S = 0}, NULL, 4, 0},     /*(3)---Nil------>(4)*/
        /* 4*/ {BgSy, {.S = '+'}, NULL, 5, 6},   /*(4)----'+'----->(5)*/
        /* 5*/ {BgGr, {.G = gTerm}, ex2, 3, 0}, /*(5)---term----->(3)*/
        /* 6*/ {BgSy, {.S = '-'}, NULL, 7, 8},   /*(6)----'-'----->(7)*/
        /* 7*/ {BgGr, {.G = gTerm}, ex3, 3, 0}, /*(7)---term----->(3)*/
        /* 8*/ {BgEn, {.S = 0}, NULL, 0, 0}      /*(E)----------(ENDE)*/
};

tBog gTerm[] =
    {
        /* 0*/ {BgGr, {.G = gFactor}, NULL, 1, 0}, /*(0)---factor--->(1)*/
        /* 1*/ {BgNl, {.S = 0}, NULL, 2, 0},       /*(1)---Nil------>(2)*/
        /* 2*/ {BgSy, {.S = '*'}, NULL, 3, 4},     /*(2)----'*'----->(3)*/
        /* 3*/ {BgGr, {.G = gFactor}, te1, 1, 0}, /*(3)---factor--->(1)*/
        /* 4*/ {BgSy, {.S = '/'}, NULL, 5, 6},     /*(4)----'*'----->(5)*/
        /* 5*/ {BgGr, {.G = gFactor}, te2, 1, 0}, /*(5)---factor--->(1)*/
        /* 6*/ {BgEn, {.S = 0}, NULL, 0, 0}        /*(E)----------(ENDE)*/
};

tBog gStatement[] =
    {
        /* 0*/ {BgMo, {.M = morphemeCodeIdentifier}, st1, 1, 3},  /*(0)---ident-------->(1)*/
        /* 1*/ {BgSy, {(ul)zResult}, NULL, 2, 0},                  /*(1)---':='--------->(2)*/
        /* 2*/ {BgGr, {.G = gExpression}, st2, 21, 0},            /*(2)---express----->(20)*/
        /* 3*/ {BgSy, {(ul)zIf}, NULL, 4, 7},                      /*(3)---'if'--------->(4)*/
        /* 4*/ {BgGr, {.G = gCondition}, st3, 5, 0},              /*(4)---condition---->(5)*/
        /* 5*/ {BgSy, {(ul)zThen}, NULL, 6, 0},                    /*(5)---'then'------->(6)*/
        /* 6*/ {BgGr, {.G = gStatement}, st4, 21, 0},             /*(6)---statement--->(20)*/
        /* 7*/ {BgSy, {(ul)zWhile}, st5, 8, 11},                  /*(7)---'while'------>(8)*/
        /* 8*/ {BgGr, {.G = gCondition}, st6, 9, 0},              /*(8)---condition---->(9)*/
        /* 9*/ {BgSy, {(ul)zDo}, NULL, 10, 0},                      /*(7)---'while'------>(8)*/
        /*10*/ {BgGr, {.G = gStatement}, st7, 21, 0},             /*(10)--statement--->(20)*/
        /*11*/ {BgSy, {(ul)zBegin}, NULL, 12, 15},                 /*(11)--'begin'----->(12)*/
        /*12*/ {BgGr, {.G = gStatement}, NULL, 13, 0},             /*(12)--statement--->(13)*/
        /*13*/ {BgSy, {.S = ';'}, NULL, 12, 14},                   /*(13)--'*'--------->(12)*/
        /*14*/ {BgSy, {(ul)zEnd}, NULL, 21, 0},                    /*(14)--'end'----->(15)*/
        /*15*/ {BgSy, {(ul)zCall}, NULL, 16, 17},                  /*(14)--'begin'----->(15)*/
        /*16*/ {BgMo, {.M = morphemeCodeIdentifier}, st8, 21, 0}, /*(15)--ident------->(20)*/
        /*17*/ {BgSy, {.S = '?'}, NULL, 18, 19},                   /*(16)--'?'--------->(17)*/
        /*18*/ {BgMo, {.M = morphemeCodeIdentifier}, st9, 21, 0}, /*(17)--ident------->(20)*/
        /*19*/ {BgSy, {.S = '!'}, NULL, 20, 21},                    /*(18)--'!'--------->(19)*/
        /*20*/ {BgGr, {.G = gExpression}, st10, 21, 0},            /*(19)--express----->(20)*/
        /*21*/ {BgEn, {.S = 0}, NULL, 0, 0}                        /*(E)---(ENDE)-----------*/
};

tBog gFactor[] =
    {
        /* 0*/ {BgMo, {(ul)morphemeCodeIdentifier}, fa2, 5, 1}, /*(0)---ident-------->(E)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgMo, {(ul)morphemeCodeNumber}, fa1, 5, 2},                                 /* +---number-------->(E)*/
        /* 2*/ {BgSy, {(ul)'('}, NULL, 3, 0},                                                /*(+)----'('--------->(3)*/
        /* 3*/ {BgGr, {.G = gExpression}, NULL, 4, 0},                                       /*(1)---express------>(4)*/
        /* 4*/ {BgSy, {(ul)')'}, NULL, 5, 0},                                                /*(0)----')'--------->(E)*/
        /* 5*/ {BgEn, {(ul)0}, NULL, 0, 0}                                                   /*(E)-------------(ENDE) */
};

tBog gCondition[] =
    {
        /* 0*/ {BgSy, {(ul)zOdd}, NULL, 1, 2},            /*(0)---'Odd'-------->(1)*/
        /* 1*/ {BgGr, {.G = gExpression}, co1, 10, 0},   /*(1)---express----->(10)*/
        /* 2*/ {BgGr, {.G = gExpression}, NULL, 3, 0},    /*(2)---express------>(3)*/
        /* 3*/ {BgSy, {(ul)'='}, set_cond_opp, 9, 4},             /*(3)----'='--------->(9)*/
        /* 4*/ {BgSy, {(ul)'#'}, set_cond_opp, 9, 5},             /*(4)----'#'--------->(9)*/
        /* 5*/ {BgSy, {(ul)'<'}, set_cond_opp, 9, 6},             /*(5)----'<'--------->(9)*/
        /* 6*/ {BgSy, {(ul)'>'}, set_cond_opp, 9, 7},             /*(6)----'>'--------->(9)*/
        /* 7*/ {BgSy, {(ul)zLessOrEqual}, set_cond_opp, 9, 8},    /*(7)----'<='-------->(9)*/
        /* 8*/ {BgSy, {(ul)zGreaterOrEqual}, set_cond_opp, 9, 0}, /*(8)----'>='-------->(9)*/
        /* 9*/ {BgGr, {.G = gExpression}, co8, 10, 0},   /*(9)---express----->(10)*/
        /*10*/ {BgEn, {(ul)0}, NULL, 0, 0}                /*(E)-------------(ENDE) */
};

int pars(tBog *pGraph)
{
    tBog *pBog = pGraph;
    char *bogenName;
    int succ = 0;
    if (Morph.morphemeCode == morphemeCodeEmpty) Lexer();
    if(pGraph == gProgramm) bogenName = "gProgramm";
    if(pGraph == gStatement) bogenName = "gStatement";
    if(pGraph == gBlock) bogenName = "gBlock";
    if(pGraph == gTerm) bogenName = "gTerm";
    if(pGraph == gFactor) bogenName = "gFactor";
    if(pGraph == gCondition) bogenName = "gCondition";
    if(pGraph == gExpression) bogenName = "gExpression";
    while (1)
    {
      //printf("in pars loop of %s %d %d\n", bogenName, pBog->iNext, pBog->iAlt);
        switch (pBog->BgD)
        {
        case BgNl:
            succ = 1;
            break;
        case BgSy:
            succ = (Morph.Value.symbol == pBog->BgX.S);
            break;
        case BgMo:
            succ = (Morph.morphemeCode == (tMorphemeCodes)pBog->BgX.M);
            break;
        case BgGr:
            succ = pars(pBog->BgX.G);
            break;
        case BgEn:
            /*
            if(pGraph == gProgramm) printf("return: gProgramm\n");
            if(pGraph == gStatement) printf("return: gStatement\n");
            if(pGraph == gBlock) printf("return: gBlock\n");
            if(pGraph == gTerm) printf("return: gTerm\n");
            if(pGraph == gFactor) printf("return: gFactor\n");
            if(pGraph == gCondition) printf("return: gCondition\n");
            if(pGraph == gExpression) printf("return: gExpression\n");
            */
            return 1; /* Ende erreichet - Erfolg */
        }
        if (succ && pBog->fx != NULL)
            succ = pBog->fx();
        if (!succ) /* Alternativbogen probieren */
            if (pBog->iAlt != 0)
                pBog = pGraph + pBog->iAlt;
            else {
                if(pGraph == gProgramm) printf("return fail: gProgramm\n");
                if(pGraph == gStatement) printf("return fail: gStatement\n");
                if(pGraph == gBlock) printf("return fail: gBlock\n");
                if(pGraph == gTerm) printf("return fail: gTerm\n");
                if(pGraph == gFactor) printf("return fail: gFactor\n");
                if(pGraph == gCondition) printf("return fail: gCondition\n");
                if(pGraph == gExpression) printf("return fail: gExpression\n");
                printf("Syntax ERROR at line: %d, column: %d\n", Morph.positionOfLine, Morph.positionOfColumn);
                exit(-1); // FAIL
            }
        else /* Morphem formal akzeptiert (eaten) */
        {
            if (pBog->BgD & BgSy || pBog->BgD & BgMo)
                Lexer();
            pBog = pGraph + pBog->iNext;
        }
    } /* while */
}