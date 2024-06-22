// Created by Cameron Archibald B00893056 2024-21-06.
// run.h

#ifndef RUN_H
#define RUN_H

#include "memory.h"
#include "decode.h"
#include "execute.h"

#define PSWLEN 5

typedef enum pswcodes {
    C = 0,
    Z,
    N,
    SLP,
    V
} pswcodes;

typedef struct operands {
    int opcode;
    unsigned int RC : 1;
    unsigned int WB : 1;
    unsigned int SC : 3;
    unsigned int S: 3;
    unsigned int D: 3;
    unsigned int B: 8;
} operands;

extern int clock;

extern unsigned short imar;
extern unsigned short ictrl;
extern unsigned short imbr;

extern unsigned short dmar;
extern unsigned short dctrl;
extern unsigned short dmar;

extern unsigned short ir;
extern operands opreg;

extern int psw[4];

void controlloop(void);

void F0(void);
void F1(void);
void D0(void);
void E0(void);

void clearopreg(void);
#endif //RUN_H
