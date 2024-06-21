//
// Created by Cameron Archibald B00893056 on 2024-05-29.
// Decode definitions, enums (Lab 2)

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "memory.h"

#define TEXT 0
#define VALUE 1

typedef enum printtypes {
    NONE = -1,
    RC_WB_SC_D,
    WB_S_D,
    S_D,
    WB_D,
    DST,
    B_D
} printtypes;

typedef enum bitlocations {
    RC = 7,
    WB = 6,
    SC = 3,
    D = 0,
    B = 3
} bitlocations;

typedef enum instname {
    BL = 0,
    BEQ,
    BNE,
    BC,
    BNC,
    BN,
    BGE,
    BLT,
    BRA,
    ADD,
    ADDC,
    SUB,
    SUBC,
    DADD,
    CMP,
    XOR,
    AND,
    OR,
    BIT,
    BIC,
    BIS,
    MOV,
    SWAP,
    SRA,
    RRC,
    SWPB,
    SXT,
    SETPRI,
    SVC,
    SETCC,
    CLRCC,
    CEX,
    LD,
    ST,
    MOVL,
    MOVLZ,
    MOVLS,
    MOVH,
    LDR,
    STR
}instname;

void instructionloop();

void decode(unsigned short instruction, unsigned short address);

int ADD_SXT(unsigned short instruction);

int MOVL_MOVH(unsigned short instruction);

void printinstruction(unsigned short instruction, unsigned short address, int type);

unsigned short mask(int position, int numofbits, unsigned short input);
#endif //DECODE_H
