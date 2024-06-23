// Created by Cameron Archibald B00893056 2024-21-06.
// run.h - definitions for running, registers, psw

#ifndef RUN_H
#define RUN_H

#include "memory.h"
#include "decode.h"
#include "execute.h"

#define PSWLEN 5

//Access psw array
typedef enum pswcodes {
    C = 0,
    Z,
    N,
    SLP,
    V
} pswcodes;

//Values in opreg
typedef struct operands {
    int opcode;
    unsigned int RC: 1;
    unsigned int WB: 1;
    unsigned int SC: 3;
    unsigned int S: 3;
    unsigned int D: 3;
    unsigned int B: 8;
} operands;

//Registers
extern int clock;

extern int psw[PSWLEN];

extern unsigned short imar;
extern unsigned short ictrl;
extern unsigned short imbr;

extern unsigned short dmar;
extern unsigned short dctrl;
extern unsigned short dmar;

extern unsigned short ir;
extern operands opreg;

void controlloop(void);

void F0(void);

void F1(void);

void D0(void);

void E0(void);

void clearopreg(void);
#endif //RUN_H
