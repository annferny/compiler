/*
Gavrilova Anna
Matr.Nr: 53045
s85499
*/

#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "code.h"

void wr2ToCode(short x);
int openOFile(char* arg);
int closeOFile(void);
int code(tCode Code,...);
int CodeOut(void);
int write_consts2file();
void wr2ToCodeAtP(short x,char*pD);
#endif /* CODEGEN_H_ */
