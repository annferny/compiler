#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "code.h"

void wr2ToCode(short x);
int openOFile(char* arg);
int closeOFile(void);
int code(tCode Code,...);
int CodeOut(void);

#endif /* CODEGEN_H_ */
