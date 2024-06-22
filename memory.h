// Created by Cameron Archibald B00893056 2024-05-06.
// memory.h - Defs, enums, structs, union relating to memory

#ifndef MEMORY_H
#define MEMORY_H

#define REGFILELEN 8
#define REGFILEWIDTH 2

#define DATASIZE 1<<16
#define WORDSIZE 1<<15
#define BYTESIZE 1<<8
#define NIBBLESIZE 1<<4

//To access specific column of memory
typedef enum memtype {
    IMEM = 0,
    DMEM
} memtype;

typedef enum reg_const {
    REG = 0,
    CONST = 1
} reg_const;

typedef enum reg {
    R0 = 0,
    R1 = 1,
    R2 = 2,
    R3 = 3,
    R4 = 4,
    R5 = 5,
    LR = 5,
    R6 = 6,
    SP = 6,
    R7 = 7,
    PC = 7,
    ALL = -1
} reg;

typedef struct nibbles {
    unsigned short n0 : 4;
    unsigned short n1 : 4;
    unsigned short n2 : 4;
    unsigned short n3 : 4;
} nibbles;

//Access a word as the word or a pair of bytes
typedef union memblock {
    unsigned short word;
    unsigned char byte[2];
    nibbles nibbles;
} memblock;

//IMEM or DMEM, can be accessed by either wordaddr (then get .word or .byte[0|1]) or byteaddr
typedef union memspace {
    memblock wordaddr[WORDSIZE];
    unsigned char byteaddr[DATASIZE];
} memspace;

//Register file
extern memblock regfile[REGFILEWIDTH][REGFILELEN];

//IMEM + DMEM
extern memspace mem[2];

//Addr of breakpoint
extern unsigned short breakpoint;

#endif //MEMORY_H
