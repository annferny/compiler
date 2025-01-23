#include <string.h>
#include <ctype.h>
#include "lex.h"

FILE *pointedFile; /* Quellfile 				*/
static tMorph MorphInit; /* Morphem   				*/
tMorph Morph;
static int actualInputSymbol; /* Aktuelles Eingabezeichen 		*/
static int actualStateOfAutomat; /* Aktueller Zustand des Automaten 	*/
static char vBuf[128 + 1], *pBuf; /* Ausgabepuffer */
static int line=1, column=0; // Zeile und Spalte
static int endFlag; /* Flag 				*/

char *keyWords[] = {"BEGIN", "CALL", "CONST", "DO", "END", "IF", "ODD", "PROCEDURE", "THEN", "VAR", "WHILE"};

/*---- Initialisierung der lexiaklischen Analyse ----*/
FILE *initLexer(char *fname) {
    pointedFile = fopen(fname, "r+t");
    if (pointedFile)
        actualInputSymbol = fgetc(pointedFile);
    Morph.morphemeCode = morphemeCodeEmpty;
    return pointedFile;
}

/* Zeichenklassenvektor */

static char characterClassVector[128] =
        /*     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F      */
        /*---------------------------------------------------------*/
        {
            /* 0*/ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, /* 0*/
            /*10*/ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, /*10*/
            /*20*/ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*20*/
            /*30*/ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 5, 4, 6, 0, /*30*/
            /*40*/ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /*40*/
            /*50*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, /*50*/
            /*60*/ 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /*60*/
            /*70*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 7 /*70*/
        };

/* Automatentabelle */

static entry automatTable[][8] =
{
    {
        {9, ifWriteReadEnd}, {1, ifWriteRead}, {2, ifWriteAsCapitalLetters}, {3, ifWriteRead}, {9, ifWriteReadEnd},
        {4, ifWriteRead}, {5, ifWriteRead}, {0, ifRead}
    },
    {{9, ifEnd}, {1, ifWriteRead}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}},
    {
        {9, ifEnd}, {2, ifWriteRead}, {2, ifWriteAsCapitalLetters}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd},
        {9, ifEnd}
    },
    {{9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {6, ifWriteRead}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}},
    {{9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {7, ifWriteRead}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}},
    {{9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {8, ifWriteRead}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}},
    {{9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}},
    {{9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}},
    {{9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}, {9, ifEnd}},
};

/* Ausgabefunktionen des Automaten */
static void read(void);

static void end(void);

static void writeAsCapitalLetters(void);

static void writeRead(void);

static void writeReadEnd(void);

typedef void (*FunctionPointer)(void);

static FunctionPointer vectorPointersToFunctions[] = {read, end, writeAsCapitalLetters, writeRead, writeReadEnd};

/*---- Lexikalische Analyse ----*/
tMorph *Lexer(void) {
    actualStateOfAutomat = 0;
    Morph = MorphInit;
    pBuf = vBuf;
    endFlag = 0;
    do {
        // printf("in lex: %c ", X);
        // printf("akt. Zustand: %d ", Z);

        // TODO: delete after parser addition
        if (actualInputSymbol == EOF && actualStateOfAutomat == 0) {
            Morph.morphemeCode = morphemeCodeSymbol;
            Morph.Value.symbol = -1;
            return &Morph;
        }

        /* Berechnung des Folgezustands */
        const int nextState = automatTable[actualStateOfAutomat][characterClassVector[actualInputSymbol & 0x7f]].state;
        /* Ausfuehrung der Aktion (Ausgabefunktion */
        vectorPointersToFunctions[automatTable[actualStateOfAutomat][characterClassVector[actualInputSymbol & 0x7f]].
            action]();
        /* Automat schaltet */
        actualStateOfAutomat = nextState;
        // printf("neuer Zustand: %d\n", Z);
    } while (endFlag == 0);
    return &Morph;
}

/*---- lesen ----*/
static void read(void) {
    actualInputSymbol = fgetc(pointedFile);
    if (actualInputSymbol == '\n')
        line++, column = 0;
    else
        column++;
}

/*---- schreiben als Grossbuchstabe, lesen ----*/
static void writeAsCapitalLetters(void) {
    *pBuf = (char) toupper(actualInputSymbol); // oder *Buf=(char)X&0xdf;
    *(++pBuf) = 0;
    read();
}

/*---- schreiben, lesen ----*/
static void writeRead(void) {
    *pBuf = (char) actualInputSymbol;
    *(++pBuf) = 0;
    read();
}

/*---- schreiben, lesen, beenden ----*/
static void writeReadEnd(void) {
    writeRead();
    end();
}

static void end() {
    int i;
    printf("Token: %s\n",vBuf);
    const size_t numKeywords = sizeof(keyWords) / sizeof(keyWords[0]);
    printf("Morph %s\n", vBuf);
    Morph.positionOfColumn = column;
    Morph.positionOfLine = line;
    switch (actualStateOfAutomat) {
        /* Symbol */
        case 0: // other symbols
        case 3: // :
        case 4: // <
        case 5: // >
            Morph.Value.symbol = vBuf[0];
            Morph.morphemeCode = morphemeCodeSymbol;
            break;
        /* Zahl */
        case 1:
            Morph.Value.number = atol(vBuf);
            Morph.morphemeCode = morphemeCodeNumber;
            break;
        /* Ident */
        case 2:
            for (i = 0; i < numKeywords; i++) {
                if (strcmp(vBuf, keyWords[i]) == 0) {
                    break;
                }
            }
            if (i < numKeywords) {
                Morph.Value.symbol = zBegin + i;
                Morph.morphemeCode = morphemeCodeSymbol;
            } else {
                Morph.Value.string = vBuf;
                Morph.morphemeCode = morphemeCodeIdentifier;
            }
            break;
        /* Ergibtzeichen */
        case 6:
            Morph.Value.symbol = (long) zResult;
            Morph.morphemeCode = morphemeCodeSymbol;
            break;
        /* KleinerGleich */
        case 7:
            Morph.Value.symbol = (long) zLessOrEqual;
            Morph.morphemeCode = morphemeCodeSymbol;
            break;
        /* GroesserGleich */
        case 8:
            Morph.Value.symbol = (long) zGreaterOrEqual;
            Morph.morphemeCode = morphemeCodeSymbol;
            break;

        default:
            printf("Unknown value %d\n", actualStateOfAutomat);
            break;
    }
    endFlag = 1; // entfällt bei Variante mit Zustand zEnd
}
