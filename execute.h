// Created by Cameron Archibald B00893056 2024-21-06.
// execute.h - definitions for execution functions, updating psw

#ifndef EXECUTE_H
#define EXECUTE_H

#include "decode.h"

#define BIT7 1<<7
#define BIT15 1<<15
#define LOWBYTE 0x00FF
#define HIGHBYTE 0xFF00


enum {
    LOW = 0,
    HIGH = 1
};

//All execution functions
void add(unsigned short src, unsigned short dst);

void sub(unsigned short src, unsigned short dst);

void dadd(memblock src, memblock dst);
unsigned char dadd_byte(memblock src, memblock dst);

void cmp(unsigned short src, unsigned short dst);

void xor(unsigned short src, unsigned short dst);

void and(unsigned short src, unsigned short dst);

void or(unsigned short src, unsigned short dst);

void bit(unsigned short src, unsigned short dst);

void bic(unsigned short src, unsigned short dst);

void bis(unsigned short src, unsigned short dst);

void mov(unsigned short src);

void swap(unsigned short src, unsigned short dst);

void sra(unsigned short dst);

void rrc(unsigned short dst);

void swpb(unsigned short dst);

void sxt(unsigned short dst);

void movl(unsigned short dst);

void movh(unsigned short dst);


//Utility functions
void senddestination(unsigned short res, int destination);

void updatepswALL(unsigned short src, unsigned short dst, unsigned short res);

void updatepswNZ(unsigned short res);

#endif //EXECUTE_H
