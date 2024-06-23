//
// Created by Cameron Archibald B00893056 on 2024-05-29.
// decode.h - decode definitions, enums

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "memory.h"
#include "run.h"

//For instnameencoding
#define TEXT 0
#define VALUE 1

//Number in the ISA
#define NUMOFINSTRUCTIONS 40
//Max len of strings to print
#define INSTRUCTIONSTRINGLEN 7

typedef enum operandtypes {
    NONE = -1,
    RC_WB_SC_D,
    WB_S_D,
    S_D,
    WB_D,
    DST,
    B_D,
    O_WB_S_D
} operandtypes;

typedef enum bitlocations {
    RC = 7,
    WB = 6,
    SC = 3,
    S = 3,
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

extern char instname_encoding[NUMOFINSTRUCTIONS][2][INSTRUCTIONSTRINGLEN];

int decode(unsigned short instruction);

int ADD_SXT(unsigned short instruction);

void assignoperands(int opcode);

unsigned short mask(int position, int numofbits, unsigned short input);
#endif //DECODE_H
