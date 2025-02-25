/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#ifndef PARSER_H
#define PARSER_H

#include "lex.h"

typedef enum BOGEN_DESC
{
    BgNl = 0, // NIL
    BgSy = 1, // Symbol
    BgMo = 2, // Morphem
    BgGr = 4, // Graph
    BgEn = 8,  // Graphende (altenativ 0 oder -1 als Folgebogen)
} tBg;

typedef struct BOGEN
{
    tBg BgD;  // Bogentyp (Nil, Symbol, Name/Zahl, Graph)
    union BGX // Bogenbeschreibung
    {
        unsigned long X; // für Initialisierung notwendig
        int S;           // Symbol (Ascii oder Wortsymbolcode aus enum)
        tMorphemeCodes M;           // Morphemtyp (Zahl oder Bezeichner)
        struct BOGEN *G; // Verweis auf Graph
    } BgX;
    int (*fx)(void); // Funktionspointer (Funktion, wenn Bogen akzeptiert)
    int iNext;       // Folgebogen, wenn Bogen akzeptiert
    int iAlt;        // Alternativbogen, wenn Bogen nicht akzeptiert
    // oder 0 oder -1, wenn es keinen
    // Alternativbogen gibt.
} tBog;

int pars(tBog* pGraph);

#endif