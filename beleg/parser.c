#include "parser.h"
#include<stdlib.h>

typedef unsigned long ul;

tBog gFactor[] =
{
        /* 0*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 5, 1}, /*(0)---ident--->(E)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgMo, {(ul)morphemeCodeNumber},     NULL, 5, 2}, /* +---number--->(E)*/
        /* 2*/ {BgSy, {(ul)'('},                    NULL, 3, 0}, /*(+)----'('---->(3)*/
        /* 3*/ {BgGr, {(ul)gExpr},                  NULL, 4, 0}, /*(1)---express->(4)*/
        /* 4*/ {BgSy, {(ul)')'},                    NULL, 5, 0}, /*(0)----')'---->(E)*/
        /* 5*/ {BgEn, {(ul)0},                      NULL, 0, 0}  /*(E)--------(ENDE) */
};

tBog gBlock[] =
{
    /* 0*/ {BgSy, {(ul)zConst},                 NULL, 1, 6}, /*(0)---'CONST'-->(1)*/ // NULL steht jetzt für zukunftige Funktionen
    /* 1*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 2, 0}, /*(1)---ident---->(2)*/
    /* 2*/ {BgSy, {(ul)'='},                    NULL, 3, 0}, /*(2)----'='----->(3)*/
    /* 3*/ {BgMo, {(ul)morphemeCodeNumber},     NULL, 4, 0}, /*(3)---number--->(4)*/
    /* 4*/ {BgSy, {(ul)','},                    NULL, 1, 5}, /*(4)----','----->(1)*/
    /* 5*/ {BgSy, {(ul)';'},                    NULL, 6, 0}, /*(5)----';'----->(6)*/
    /* 6*/ {BgSy, {(ul)zVar},                   NULL, 7,10}, /*(6)---'Var'---->(7)*/
    /* 7*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 8, 0}, /*(7)---ident---->(8)*/
    /* 8*/ {BgSy, {(ul)','},                    NULL, 7, 9}, /*(8)----','----->(7)*/
    /* 9*/ {BgSy, {(ul)';'},                    NULL,10, 0}, /*(9)----';'----->(10)*/
    /*10*/ {BgSy, {(ul)zProcedure},             NULL,11,15}, /*(10)--'Var'---->(11)*/
    /*11*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL,12, 0}, /*(11)---ident--->(12)*/
    /*12*/ {BgSy, {(ul)';'},                    NULL,13, 0}, /*(12)----';'---->(13)*/
    /*13*/ {BgGr, {(ul)gBlock},                 NULL,14, 0}, /*(13)---block--->(14)*/
    /* 9*/ {BgSy, {(ul)';'},                    NULL,10, 0}, /*(9)----';'----->(10)*/

};

tBog gProgramm[] =
{
        /* 0*/ {BgGr, {(ul)gBlock}, NULL, 1, 0}, /*(0)---block--->(1)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgSy, {(ul)'.'},    NULL, 2, 0}, /*(1)---'.'--->(E)*/
        /* 2*/ {BgEn, {(ul)0},      NULL, 0, 0}  /*(E)--------(ENDE) */
};

