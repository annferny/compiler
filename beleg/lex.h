/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/


#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>

// Funktionsindex *0x10, bzw. *16
typedef enum T_Fx
{
    ifRead,
    ifEnd,
    ifWriteAsCapitalLetters,
    ifWriteRead,
    ifWriteReadEnd
} tFunctionsIndex;

/* Morphemcodes */
typedef enum T_MC
{
    morphemeCodeEmpty,
    morphemeCodeSymbol,
    morphemeCodeNumber,
    morphemeCodeIdentifier,
    morphemeCodeString
} tMorphemeCodes;

typedef enum T_ZS
{
    zNIL,
    zResult = 128,   // :=
    zLessOrEqual,    // <=
    zGreaterOrEqual, // >=
    zBegin,
    zCall,
    zConst,
    zDo,
    zEnd,
    zIf,
    zOdd,
    zProcedure,
    zThen,
    zVar,
    zWhile
} tSpecialSymbols;

typedef struct
{
    tMorphemeCodes morphemeCode;      /* Morphemcode */
    int positionOfLine; /* Zeile       */
    int positionOfColumn;  /* Spalte      */
    union VAL
    {
        long number;
        char *string;
        int symbol;
    } Value;
    int morphemeLength; /* Morphemlänge*/
} tMorph;

typedef struct {
    int state;
    tFunctionsIndex action;
} entry;

FILE* initLexer(char *fname);

tMorph *Lexer(void);

#endif