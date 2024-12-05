#include "parser.h"
#include <stdlib.h>

typedef unsigned long ul;
extern tMorph Morph;

tBog gStatement[], gBlock[], gTerm[], gFactor[], gCondition[], gProgramm[];

tBog gProgramm[] =
    {
        /* 0*/ {BgGr, {.G = gBlock}, NULL, 1, 0}, /*(0)---block--->(1)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgSy, {(ul)'.'}, NULL, 2, 0},                            /*(1)---'.'--->(E)*/
        /* 2*/ {BgEn, {(ul)0}, NULL, 0, 0}                               /*(E)--------(ENDE) */
};

tBog gBlock[] =
    {
        /* 0*/ {BgSy, {(ul)zConst}, NULL, 1, 6}, /*(0)---'CONST'-->(1)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 2, 0},         /*(1)---ident---->(2)*/
        /* 2*/ {BgSy, {(ul)'='}, NULL, 3, 0},                            /*(2)----'='----->(3)*/
        /* 3*/ {BgMo, {(ul)morphemeCodeNumber}, NULL, 4, 0},             /*(3)---number--->(4)*/
        /* 4*/ {BgSy, {(ul)','}, NULL, 1, 5},                            /*(4)----','----->(1)*/
        /* 5*/ {BgSy, {(ul)';'}, NULL, 6, 0},                            /*(5)----';'----->(6)*/
        /* 6*/ {BgSy, {(ul)zVar}, NULL, 7, 10},                          /*(6)---'Var'---->(7)*/
        /* 7*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 8, 0},         /*(7)---ident---->(8)*/
        /* 8*/ {BgSy, {(ul)','}, NULL, 7, 9},                            /*(8)----','----->(7)*/
        /* 9*/ {BgSy, {(ul)';'}, NULL, 10, 0},                           /*(9)----';'----->(10)*/
        /*10*/ {BgSy, {(ul)zProcedure}, NULL, 11, 15},                   /*(10)--'Var'---->(11)*/
        /*11*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 12, 0},        /*(11)---ident--->(12)*/
        /*12*/ {BgSy, {(ul)';'}, NULL, 13, 0},                           /*(12)----';'---->(13)*/
        /*13*/ {BgGr, {.G = gBlock}, NULL, 14, 0},                       /*(13)---block--->(14)*/
        /*14*/ {BgSy, {(ul)';'}, NULL, 10, 0},                           /*(14)----';'---->(10)*/
        /*15*/ {BgGr, {.G = gStatement}, NULL, 16, 0},                   /*(15)---block--->(16)*/
        /*16*/ {BgEn, {(ul)0}, NULL, 0, 0}                               /*(E)--------(ENDE) */
};

tBog gExpression[] =
    {
        /* 0*/ {BgSy, {(ul)'-'}, NULL, 1, 2},    /*(0)----'-'----->(1)*/
        /* 1*/ {BgGr, {.G = gTerm}, NULL, 3, 0}, /*(1)---term----->(3)*/
        /* 2*/ {BgGr, {.G = gTerm}, NULL, 3, 0}, /*(2)---term----->(3)*/
        /* 3*/ {BgNl, {.S = 0}, NULL, 4, 0},     /*(3)---Nil------>(4)*/
        /* 4*/ {BgSy, {.S = '+'}, NULL, 5, 6},   /*(4)----'+'----->(5)*/
        /* 5*/ {BgGr, {.G = gTerm}, NULL, 3, 0}, /*(5)---term----->(3)*/
        /* 6*/ {BgSy, {.S = '-'}, NULL, 7, 8},   /*(6)----'-'----->(7)*/
        /* 7*/ {BgGr, {.G = gTerm}, NULL, 3, 0}, /*(7)---term----->(3)*/
        /* 8*/ {BgEn, {.S = 0}, NULL, 0, 0}      /*(E)----------(ENDE)*/
};

tBog gTerm[] =
    {
        /* 0*/ {BgGr, {.G = gFactor}, NULL, 1, 0}, /*(0)---factor--->(1)*/
        /* 1*/ {BgNl, {.S = 0}, NULL, 2, 0},       /*(1)---Nil------>(2)*/
        /* 2*/ {BgSy, {.S = '*'}, NULL, 3, 4},     /*(2)----'*'----->(3)*/
        /* 3*/ {BgGr, {.G = gFactor}, NULL, 1, 0}, /*(3)---factor--->(1)*/
        /* 4*/ {BgSy, {.S = '/'}, NULL, 5, 6},     /*(4)----'*'----->(5)*/
        /* 5*/ {BgGr, {.G = gFactor}, NULL, 1, 0}, /*(5)---factor--->(1)*/
        /* 6*/ {BgEn, {.S = 0}, NULL, 0, 0}        /*(E)----------(ENDE)*/
};

tBog gStatement[] =
    {
        /* 0*/ {BgMo, {.M = morphemeCodeIdentifier}, NULL, 1, 3},  /*(0)---ident-------->(1)*/
        /* 1*/ {BgSy, {(ul)zResult}, NULL, 2, 0},                  /*(1)---':='--------->(2)*/
        /* 2*/ {BgGr, {.G = gExpression}, NULL, 21, 0},            /*(2)---express----->(20)*/
        /* 3*/ {BgSy, {(ul)zIf}, NULL, 4, 7},                      /*(3)---'if'--------->(4)*/
        /* 4*/ {BgGr, {.G = gCondition}, NULL, 5, 0},              /*(4)---condition---->(5)*/
        /* 5*/ {BgSy, {(ul)zThen}, NULL, 6, 0},                    /*(5)---'then'------->(6)*/
        /* 6*/ {BgGr, {.G = gStatement}, NULL, 21, 0},             /*(6)---statement--->(20)*/
        /* 7*/ {BgSy, {(ul)zWhile}, NULL, 8, 11},                  /*(7)---'while'------>(8)*/
        /* 8*/ {BgGr, {.G = gCondition}, NULL, 9, 0},              /*(8)---condition---->(9)*/
        /* 9*/ {BgSy, {(ul)zDo}, NULL, 10, 0},                      /*(7)---'while'------>(8)*/
        /*10*/ {BgGr, {.G = gStatement}, NULL, 21, 0},             /*(10)--statement--->(20)*/
        /*11*/ {BgSy, {(ul)zBegin}, NULL, 12, 14},                 /*(11)--'begin'----->(12)*/
        /*12*/ {BgGr, {.G = gStatement}, NULL, 13, 0},             /*(12)--statement--->(13)*/
        /*13*/ {BgSy, {.S = ';'}, NULL, 12, 21},                   /*(13)--'*'--------->(12)*/

        /*14*/ {BgSy, {(ul)zEnd}, NULL, 21, 0},                    /*(14)--'end'----->(15)*/  

        /*15*/ {BgSy, {(ul)zCall}, NULL, 16, 17},                  /*(14)--'begin'----->(15)*/
        /*16*/ {BgMo, {.M = morphemeCodeIdentifier}, NULL, 21, 0}, /*(15)--ident------->(20)*/
        /*17*/ {BgSy, {.S = '?'}, NULL, 18, 19},                   /*(16)--'?'--------->(17)*/
        /*18*/ {BgMo, {.M = morphemeCodeIdentifier}, NULL, 21, 0}, /*(17)--ident------->(20)*/
        /*19*/ {BgSy, {.S = '!'}, NULL, 20, 0},                    /*(18)--'!'--------->(19)*/
        /*20*/ {BgGr, {.G = gExpression}, NULL, 21, 0},            /*(19)--express----->(20)*/
        /*21*/ {BgEn, {.S = 0}, NULL, 0, 0}                        /*(E)---(ENDE)-----------*/
};

tBog gFactor[] =
    {
        /* 0*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 5, 1}, /*(0)---ident-------->(E)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgMo, {(ul)morphemeCodeNumber}, NULL, 5, 2},                                 /* +---number-------->(E)*/
        /* 2*/ {BgSy, {(ul)'('}, NULL, 3, 0},                                                /*(+)----'('--------->(3)*/
        /* 3*/ {BgGr, {.G = gExpression}, NULL, 4, 0},                                       /*(1)---express------>(4)*/
        /* 4*/ {BgSy, {(ul)')'}, NULL, 5, 0},                                                /*(0)----')'--------->(E)*/
        /* 5*/ {BgEn, {(ul)0}, NULL, 0, 0}                                                   /*(E)-------------(ENDE) */
};

tBog gCondition[] =
    {
        /* 0*/ {BgSy, {(ul)zOdd}, NULL, 1, 2},            /*(0)---'Odd'-------->(1)*/
        /* 1*/ {BgGr, {.G = gExpression}, NULL, 10, 0},   /*(1)---express----->(10)*/
        /* 2*/ {BgGr, {.G = gExpression}, NULL, 3, 0},    /*(2)---express------>(3)*/
        /* 3*/ {BgSy, {(ul)'='}, NULL, 9, 4},             /*(3)----'='--------->(9)*/
        /* 4*/ {BgSy, {(ul)'#'}, NULL, 9, 5},             /*(4)----'#'--------->(9)*/
        /* 5*/ {BgSy, {(ul)'<'}, NULL, 9, 6},             /*(5)----'<'--------->(9)*/
        /* 6*/ {BgSy, {(ul)'>'}, NULL, 9, 7},             /*(6)----'>'--------->(9)*/
        /* 7*/ {BgSy, {(ul)zLessOrEqual}, NULL, 9, 8},    /*(7)----'<='-------->(9)*/
        /* 8*/ {BgSy, {(ul)zGreaterOrEqual}, NULL, 9, 0}, /*(8)----'>='-------->(9)*/
        /* 9*/ {BgGr, {.G = gExpression}, NULL, 10, 0},   /*(9)---express----->(10)*/
        /*10*/ {BgEn, {(ul)0}, NULL, 0, 0}                /*(E)-------------(ENDE) */
};

int pars(tBog *pGraph)
{
    tBog *pBog = pGraph;
    int succ = 0;
    if (Morph.morphemeCode == morphemeCodeEmpty)
        Lexer();
    printf("pars call\n");  
    if(pGraph == gProgramm) printf("gProgramm\n");
    if(pGraph == gStatement) printf("gStatement\n");
    if(pGraph == gBlock) printf("gBlock\n");
    if(pGraph == gTerm) printf("gTerm\n");
    if(pGraph == gFactor) printf("gFactor\n");
    if(pGraph == gCondition) printf("gCondition\n");
    if(pGraph == gExpression) printf("gExpression\n");
    while (1)
    {
      printf("in pars loop %d %d\n", pBog->iNext, pBog->iAlt);  
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
            if(pGraph == gProgramm) printf("gProgramm\n");
            if(pGraph == gStatement) printf("gStatement\n");
            if(pGraph == gBlock) printf("gBlock\n");
            if(pGraph == gTerm) printf("gTerm\n");
            if(pGraph == gFactor) printf("gFactor\n");
            if(pGraph == gCondition) printf("gCondition\n");
            if(pGraph == gExpression) printf("gExpression\n");
            printf("return\n");
            return 1; /* Ende erreichet - Erfolg */
        }
        if (succ && pBog->fx != NULL)
            succ = pBog->fx();
        if (!succ) /* Alternativbogen probieren */
            if (pBog->iAlt != 0)
                pBog = pGraph + pBog->iAlt;
            else {
                if(pGraph == gProgramm) printf("gProgramm\n");
                if(pGraph == gStatement) printf("gStatement\n");
                if(pGraph == gBlock) printf("gBlock\n");
                if(pGraph == gTerm) printf("gTerm\n");
                if(pGraph == gFactor) printf("gFactor\n");
                if(pGraph == gCondition) printf("gCondition\n");
                if(pGraph == gExpression) printf("gExpression\n");
                printf("return fail\n");
                return 0; // FAIL
            }
        else /* Morphem formal akzeptiert (eaten) */
        {
            if (pBog->BgD & BgSy || pBog->BgD & BgMo)
                Lexer();
            pBog = pGraph + pBog->iNext;
        }
        
    } /* while */
}