/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "list.h"
#include "lex.h"
#include "parser.h"
#include "structs.h"
#include "namelist.h"
#include "CodeGen.h"

tProcedure *pMainPr;

extern tProcedure *currProcedure;
extern List *constList;
extern List *pSOprtr; /* Operatorenkeller fr Ausdruck   */
extern List *pSLabl; /* Keller fuer Label               */
extern int IdxCnst; /* Zaehler fuer Konstantenindex    */
extern tMorph Morph;
extern char *vCode; /* Pointer auf dynamischen Bereich fuer Code */
extern char*  pCode;    /* Pointer auf aktuelle Position             */
// extern int iCode; /* Pointer auf aktuelle Position             */
extern short numProc; /* Zaehler fuer Prozeduren                   */
extern int LenCode; /* Laenge des Codeausgabebereiches           */
FILE *destFile;

/* Weitere Funktionen ... */

/*--------------------------*/

/*--------------------------*/
void wr2ToCode(short x) {
    *pCode++=(unsigned char)(x & 0xff);
    *pCode++=(unsigned char)(x >> 8);
}

void wr2ToCodeAtP(short x,char*pD) {
    * pD   =(unsigned char)(x & 0xff);
    *(pD+1)=(unsigned char)(x >> 8);
}

int code(tCode Code,...)
{
    va_list ap;
    short sarg;

    if (pCode-vCode+MAX_LEN_OF_CODE>=LenCode)
    {
        char* xCode=realloc(vCode,(LenCode+=1024));
        if (xCode==NULL) printf("Memory allocation error\n");
        pCode=xCode+(pCode-vCode);
        vCode=xCode;
    }
    *pCode++=(char)Code;
    va_start(ap,Code);
    switch (Code)
    {
        /* Befehle mit 3 Parametern */
        case entryProc:
            sarg=va_arg(ap,int);
            wr2ToCode(sarg);
        /* Befehle mit 2 Parametern */
        case puValVrGlob:
        case puAdrVrGlob:
               sarg=va_arg(ap,int);
        wr2ToCode(sarg);
        /* Befehle mit 1 Parameter */
        case puValVrMain:
        case puAdrVrMain:
        case puValVrLocl:
        case puAdrVrLocl:
        case puConst:
        case jmp :
        case jnot:
        case call:
                sarg=va_arg(ap,int); /* Prozedurnummer               */
        wr2ToCode(sarg);
        break;

        /* keine Parameter */
        case putStrg: /*
            if ((int)(pCode-vCode+strlen(Morph.Val.pStr)+1)>=LenCode)
            {
                char* xCode=realloc(vCode,(LenCode+=1024));
                if (xCode==NULL) Error(ENoMem);
                pCode=xCode+(pCode-vCode);
                vCode=xCode;
            }
        strcpy(pCode,Morph.Val.pStr);
        pCode+=strlen(pCode)+1;
        break; */
        default     : break;
    }
    va_end  (ap);
    return OK;
}

int CodeOut(void) {
    unsigned short Len=(unsigned short)(pCode-vCode);
    wr2ToCodeAtP((short)Len,vCode+1);
    wr2ToCodeAtP((short)currProcedure->lengthVar,vCode+5);
    if (Len==fwrite(vCode,sizeof(char),Len,destFile)) return OK;
    else                                            return FAIL;
}

/*------------------------------*/
/* Oeffnen/Schliessen des Codeausgabefiles */
int openOFile(char *arg) {
    long i = 0L;
    char vName[128 + 1];

    strcpy(vName, arg);
    if (strstr(vName, ".pl0") == NULL) strcat(vName, ".cl0");
    else *(strchr(vName, '.') + 1) = 'c';

    if ((destFile = fopen(vName, "wb")) != NULL) {
        fwrite(&i, sizeof(int64_t), 1, destFile);
        return OK;
    } else return FAIL;
}

int closeOFile(void) {

    unsigned short sys = 8;
    fseek(destFile, 0,SEEK_SET);
    //wr2ToCodeAtP(numProc,vBuf2);
    if (fwrite(&numProc, 2, 1, destFile) == 1);
    else return FAIL;
    if (fwrite(&sys, 2, 1, destFile) == 1);
    else return FAIL;
}

int write_consts2file()
{
    int len=sizeof(int)* constList->listLength;
    if (len==fwrite(constList,len,constList->listLength,destFile))
        return OK;
    else
        return FAIL;
}
