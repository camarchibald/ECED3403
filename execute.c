// Created by Cameron Archibald B00893056 2024-21-06.
// execute.c

#include "execute.h"




void add(unsigned short src, unsigned short dst) {
    unsigned short res = src + dst;
    if(opreg.opcode == ADDC)
        res += psw[C];
    senddestination(res);
}

void sub(unsigned short src, unsigned short dst) {
    //2's complement, also think about byte vs word since that will affect things
}

void senddestination(unsigned short res) {
    if(opreg.WB == 0)
        regfile[REG][opreg.D].word = res;
    else
        regfile[REG][opreg.D].word = (regfile[REG][opreg.D].word & 0xFF00) + (res & 0x00FF);
}

void updatepsw(unsigned short src, unsigned short dst, unsigned short res) {
    //Get MSbit of result for negative flag
    psw[N] = mask(15, 1, res);

    //If result is zero, zero flag is 1
    (res == 0) ? psw[Z] = 1 : psw[Z] = 0;

    //V and C, later
}