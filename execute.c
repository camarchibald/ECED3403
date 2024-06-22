// Created by Cameron Archibald B00893056 2024-21-06.
// execute.c

#include "execute.h"

#define UPDATEZ(res) ((res == 0) ? (psw[Z] = 1) : (psw[Z] = 0))

int carryarr[2][2][2] = {0,0,1,0,1,0,1,1};
int varr[2][2][2] = {0,1,0,0,0,0,1,0};


void add(unsigned short src, unsigned short dst) {
    unsigned short res = src + dst;
    if(opreg.opcode == ADDC)
        res += psw[C];
    senddestination(res, opreg.D);
    updatepswALL(src,dst,res);
}

void sub(unsigned short src, unsigned short dst) {
    unsigned short res;
    if(opreg.WB == 0)
        res = dst + ~(src);
    else
        res = (dst & 0x00FF) + (~(src) & 0x00FF);

    if(opreg.opcode == SUBC)
        res += psw[C];
    else
        res += 1;

    senddestination(res, opreg.D);

    updatepswALL(~src,dst,res);
}

void dadd(memblock src, memblock dst) {
    unsigned char lowbyte = dadd_byte(src, dst);
    unsigned char highbyte = 0;

    if(opreg.WB == 0) {
        src.word = (src.word>>8);
        dst.word = (dst.word>>8);
        highbyte = dadd_byte(src, dst);
    }

    unsigned short result = lowbyte + highbyte * (BYTESIZE);
    senddestination(result, opreg.D);

    //Carry handled by dadd_byte()
    UPDATEZ(result);
}

unsigned char dadd_byte(memblock src, memblock dst) {
    unsigned char lownibble = 0, highnibble = 0;

    lownibble = src.nibbles.n0 + dst.nibbles.n0 + psw[C];

    if(lownibble >= 10) {
        lownibble -= 10;
        highnibble = 1;
    }

    highnibble += src.nibbles.n1 + dst.nibbles.n1;

    if(highnibble >= 10) {
        highnibble -= 10;
        psw[C] = 1;
    } else {
        psw[C] = 0;
    }

    return lownibble + highnibble * (NIBBLESIZE);
}

void cmp(unsigned short src, unsigned short dst) {
    unsigned short res;

    if(opreg.WB == 0)
        res = dst + ~(src) + 1;
    else
        res = (dst & 0x00FF) + (~(src) & 0x00FF) + 1;

    updatepswALL(~src,dst,res);
}

void xor(unsigned short src, unsigned short dst) {
    unsigned short res = dst ^ src;
    senddestination(res, opreg.D);
    updatepswNZ(res);
}

void and(unsigned short src, unsigned short dst) {
    unsigned short res = dst & src;
    senddestination(res, opreg.D);
    updatepswNZ(res);
}

void or(unsigned short src, unsigned short dst) {
    unsigned short res = dst | src;
    senddestination(res, opreg.D);
    updatepswNZ(res);
}

void bit(unsigned short src, unsigned short dst) {
    unsigned short res = dst & (1<<src);
    updatepswNZ(res);
}

void bic(unsigned short src, unsigned short dst) {
    unsigned short res = dst & ~(1<<src);
    senddestination(res, opreg.D);

    updatepswNZ(res);
}

void bis(unsigned short src, unsigned short dst) {
    unsigned short res = dst | (1<<src);
    senddestination(res, opreg.D);

    updatepswNZ(res);
}

void mov(unsigned short src) {
    senddestination(src, opreg.D);
}

void swap(unsigned short src, unsigned short dst) {
    senddestination(src, opreg.D);
    senddestination(dst, opreg.SC);
}

void sra(unsigned short dst) {
    dst = dst>>1;
    senddestination(dst,opreg.D);
    updatepswNZ(dst);
}

void rrc(unsigned short dst) {
    int tempcarry = mask(0,1,dst);
    dst = dst>>1;
    if(opreg.WB == 1)
        dst = (dst & ~(BIT7)) + (psw[C] * (BIT7));
    else
        dst = (dst & ~(BIT15)) + (psw[C] * (BIT15));

    psw[C] = tempcarry;
    senddestination(dst, opreg.D);
    updatepswNZ(dst);
}

void swpb(unsigned short dst) {
    unsigned char templow = dst & 0x00FF;
    dst = dst>>8;
    dst += templow * BYTESIZE;
    senddestination(dst, opreg.D);
}

void sxt(unsigned short dst) {
    int sign = mask(7,1,dst);
    if(sign)
        dst |= 0xFF00;
    else
        dst &= 0x00FF;
    senddestination(dst, opreg.D);
    updatepswNZ(dst);
}

void movl(unsigned short dst) {
    //Place byte in result
    unsigned short result = opreg.B;

    switch(opreg.opcode) {
        case MOVL:
            result += (dst & 0xFF00);
            break;
        case MOVLZ:
            //do nothing as upper byte of result already zero
            break;
        case MOVLS:
            result += 0xFF00;
            break;
    }
    senddestination(result, opreg.D);
}

void movh(unsigned short dst) {
    //Keep low byte, place B in high byte
    dst = (dst & 0x00FF) + (opreg.B * BYTESIZE);
    senddestination(dst, opreg.D);
}

void senddestination(unsigned short res, int destination) {
    if(opreg.WB == 0)
        regfile[REG][destination].word = res;
    else
        regfile[REG][destination].word = (regfile[REG][destination].word & 0xFF00) + (res & 0x00FF);
}

void updatepswALL(unsigned short src, unsigned short dst, unsigned short res) {
    updatepswNZ(res);

    int msbitposition;
    (opreg.WB == 1) ? (msbitposition = 7) : (msbitposition = 15);

    int msbsrc = mask(msbitposition,1,src);
    int msbdst = mask(msbitposition,1,dst);
    int msbres = mask(msbitposition,1,res);

    psw[C] = carryarr[msbsrc][msbdst][msbres];

    psw[V] = varr[msbsrc][msbdst][msbres];
}

void updatepswNZ(unsigned short res) {
    int msbitposition;
    (opreg.WB == 1) ? (msbitposition = 7) : (msbitposition = 15);

    //Get MSbit of result for negative flag
    psw[N] = mask(msbitposition, 1, res);

    //If byte operation, zero the high byte of result (only want if the relevent byte is zero)
    if(opreg.WB == 1)
        res &= 0x00FF;

    UPDATEZ(res);
}