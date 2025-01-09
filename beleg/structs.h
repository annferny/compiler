/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <stdint.h>

/*--------------------------------------------------------------------------------------*/

// Struct of one element of the list
struct connectElements;
typedef struct connectElements connector;

// Struct of a circle list
struct structList;
typedef struct structList List;

typedef enum {KzIdentifier,KzProcedure,KzConst,KzVar,KzLable} tKennzeichen; // typedef enum tKZ{KzBez,KzPrc,KzConst,KzVar,KzLabl}tKz;

/*--------------------------------------------------------------------------------------*/

typedef struct
{
    tKennzeichen kennzeichen; // tKz Kz; 
    short indexProcedure; // idxProc;
    void* pointerObject; //pObj;
    char* pointerName; // pName;
} tIdentifier; // tBez

typedef struct tPROCEDURE //tPROC
{
    tKennzeichen kennzeichen;		/* Kennzeichen tKz    Kz; */
    short  indexProcedure;	/* Prozedurindex short  IdxProc; */
    struct tPROCEDURE* pointerParent;	/* Zeiger auf umgebende Prozedur  struct tPROC*pParent;*/
    List* pListIdentifier;    	/* Namensliste  tList *pLBez;   */
    int lengthVar;
} tProcedure; //tProc;


typedef struct tCONST
{
    tKennzeichen kennzeichen; //tKz  Kz;		/* Kennzeichen */
    long value; //long Val;		/* Wert der Konstanten*/
    //int index; //int  Idx;		/* Index im Konstantenblock */ ---- ignoriert, da eine Liste ohne Indexen genutzt wird
} tConst;

typedef struct tVAR
{
    tKennzeichen kennzeichen; //tKz Kz;		/* Kennzeichen */
    int relativeAddress; //int Dspl;		/* Relativadresse der Variablen */
} tVar;

typedef struct tLABL
{
    tKennzeichen kennzeichen; //tKz Kz;		/* Kennzeichen */
    long indexJump; //long iJmp;		/* Pointer in Codeausgabebereich */
} tLabl;

// Struct of one element of the list
typedef struct connectElements
{
    struct connectElements *next; // pointer on the next element
    struct connectElements *previous; // pointer on the previous element
    void *pointedItem; // pointer on data
} connector;

// Struct of a circle list
typedef struct structList
{
    connector head;
    connector*currentPosition;
    int listLength;
} List;

#endif /* STRUCTS_H_ */

