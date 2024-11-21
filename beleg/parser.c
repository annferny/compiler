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

tBog gProgramm[] =
    {
        /* 0*/ {BgGr, {(ul)gBlock}, NULL, 1, 0}, /*(0)---block--->(1)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgSy, {(ul)'.'},    NULL, 2, 0}, /*(1)---'.'--->(E)*/
        /* 2*/ {BgEn, {(ul)0},      NULL, 0, 0}  /*(E)--------(ENDE) */
};

tBog gBlock[] =
    {
        /* 0*/ {BgSy, {(ul)zConst},                 NULL, 1, 6}, /*(0)---'CONST'-->(1)*/ // NULL steht jetzt für zukunftige Funktionen
        /* 1*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 2, 0}, /*(1)---ident---->(2)*/
        /* 2*/ {BgSy, {(ul)'='},                    NULL, 3, 0}, /*(2)----'='----->(3)*/
        /* 3*/ {BgMo, {(ul)morphemeCodeNumber},     NULL, 4, 0}, /*(3)---number--->(4)*/
        /* 4*/ {BgSy, {(ul)','},                    NULL, 1, 5}, /*(4)----','----->(1)*/
        /* 5*/ {BgSy, {(ul)';'},                    NULL, 6, 0}, /*(5)----';'----->(6)*/
        /* 6*/ {BgSy, {(ul)zVar},                   NULL, 7,11}, /*(6)---'Var'---->(7)*/
        /* 7*/ {BgMo, {(ul)morphemeCodeIdentifier}, NULL, 8, 0}, /*(7)---ident---->(8)*/
        ...
};