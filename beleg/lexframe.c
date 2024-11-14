#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"

static FILE *pIF;        /* Quellfile 				*/
static tMorph MorphInit; /* Morphem   				*/
tMorph Morph;
static int X;                     /* Aktuelles Eingabezeichen 		*/
static int Z;                     /* Aktueller Zustand des Automaten 	*/
static char vBuf[128 + 1], *pBuf; /* Ausgabepuffer */
static int line, col;             // Zeile und Spalte
static int Ende;                  /* Flag 				*/

char *keyWords[] = {"BEGIN", "CALL", "CONST", "DO", "END"};

/*---- Initialisierung der lexiaklischen Analyse ----*/
int initLex(char *fname)
{
    pIF = fopen(fname, "r+t");
    if (pIF)
        X = fgetc(pIF);
    Morph.MC = mcEmpty;
    return (int)pIF;
}

/* Zeichenklassenvektor */

static char vZKl[128] =
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
        /*70*/ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 7  /*70*/
};

/* Automatentabelle */

static entry vSMatrix[][8] =
    {
        {{9, ifslb}, {1, ifsl}, {2, ifgl}, {3, ifsl}, {9, ifslb}, {4, ifsl}, {5, ifsl}, {0, ifl}},
        {{9, ifb}, {1, ifsl}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}},
        {{9, ifb}, {2, ifsl}, {2, ifgl}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}},
        {{9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {6, ifsl}, {9, ifb}, {9, ifb}, {9, ifb}},
        {{9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {7, ifsl}, {9, ifb}, {9, ifb}, {9, ifb}},
        {{9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {8, ifsl}, {9, ifb}, {9, ifb}, {9, ifb}},
        {{9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}},
        {{9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}},
        {{9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}, {9, ifb}},
};

/* Ausgabefunktionen des Automaten */
static void fl(void);

static void fb(void);

static void fgl(void);

static void fsl(void);

static void fslb(void);

typedef void (*FX)(void);

static FX vfx[] = {fl, fb, fgl, fsl, fslb};

/*---- Lexikalische Analyse ----*/
tMorph *Lex(void)
{

    int Zn;
    Z = 0;
    Morph = MorphInit;
    pBuf = vBuf;
    Ende = 0;
    do
    {
        // printf("in lex: %c ", X);
        // printf("akt. Zustand: %d ", Z);

        // TODO: delete after parser addition
        if (X == EOF && Z == 0)
        {
            Morph.MC = mcSymb;
            Morph.Val.Symb = -1;
            return &Morph;
        }

        /* Berechnung des Folgezustands */
        Zn = vSMatrix[Z][vZKl[X & 0x7f]].zustand;
        /* Ausfuehrung der Aktion (Ausgabefunktion */
        vfx[(vSMatrix[Z][vZKl[X & 0x7f]]).aktion]();
        /* Automat schaltet */
        Z = Zn;
        // printf("neuer Zustand: %d\n", Z);
    } while (Ende == 0);
    return &Morph;
}

/*---- lesen ----*/
static void fl(void)
{
    X = fgetc(pIF);
    if (X == '\n')
        line++, col = 0;
    else
        col++;
}

/*---- schreiben als Grossbuchstabe, lesen ----*/
static void fgl(void)
{
    *pBuf = (char)toupper(X); // oder *Buf=(char)X&0xdf;
    *(++pBuf) = 0;
    fl();
}

/*---- schreiben, lesen ----*/
static void fsl(void)
{
    *pBuf = (char)X;
    *(++pBuf) = 0;
    fl();
}

/*---- schreiben, lesen, beenden ----*/
static void fslb(void)
{
    fsl();
    fb();
}

static void fb()
{
    int i, j;
    switch (Z)
    {
    case 0:
        Morph.Val.Symb = vBuf[0];
        Morph.MC = mcSymb;
        break;
    /* Zahl */
    case 1:
        Morph.Val.Num = atol(vBuf);
        Morph.MC = mcNum;
        break;
    /* Ident */
    case 2:
        int i = 0;
        for (i; i < sizeof(keyWords) / sizeof(keyWords[0]); i++)
            if (strcmp(vBuf, keyWords[i]) == 0)
                break;
        if (i < sizeof(keyWords) / sizeof(keyWords[0]))
        {
            Morph.Val.Symb = zBGN + i;
            Morph.MC = mcSymb;
        }
        else
        {
            Morph.Val.pStr = vBuf;
            Morph.MC = mcIdent;
        }
        break;
    /* : */
    case 3:
        Morph.Val.Symb = vBuf[0];
        Morph.MC = mcIdent;
        break;
    case 4: // <
    case 5: // >
    }
    Ende = 1; // entfällt bei Variante mit Zustand zEnd
}
