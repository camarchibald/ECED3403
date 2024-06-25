// Created by Cameron Archibald B00893056 2024-21-06.
// execute.c - functions to execute instructions and update psw

#include "execute.h"

//Update the psw zero based on the result being zero
#define UPDATEZ(res) ((res == 0) ? (psw[Z] = 1) : (psw[Z] = 0))

//C/V based on MSbit of src,dst,res
int carryarr[2][2][2] = {0, 0, 1, 0, 1, 0, 1, 1};
int varr[2][2][2] = {0, 1, 0, 0, 0, 0, 1, 0};

//Instructions take in the value stored at the src,dst registers
//Operations are performed, and if applicable senddestination() places items back in register
//Senddestination() takes into account w/b, hence often the full word result is passed
//psw is updated, if applicable

void add(unsigned short src, unsigned short dst) {
    unsigned short res = src + dst;

    //Add carry
    if (opreg.opcode == ADDC)
        res += psw[C];

    senddestination(res, opreg.D);
    updatepswALL(src, dst, res);
}

void sub(unsigned short src, unsigned short dst) {
    unsigned short res;

    //If byte, get only the lower byte
    if (opreg.WB == 0)
        res = dst + ~(src);
    else
        res = (dst & LOWBYTE) + (~(src) & LOWBYTE);

    //Carry in, otherwise add 1 to perform two's complement
    if (opreg.opcode == SUBC)
        res += psw[C];
    else
        res += 1;

    senddestination(res, opreg.D);

    //Complement of src used in C,V arrays
    updatepswALL(~src, dst, res);
}

void dadd(memblock src, memblock dst) {
    //Perform lower byte decimal addition
    unsigned char lowbyte = dadd_byte(src, dst);
    unsigned char highbyte = 0;

    //If word desired, shift the high byte to the low byte and perform decimal addition
    //Since C is considered by dadd_byte, C is carried in to the high byte
    if (opreg.WB == 0) {
        src.word = (src.word >> 8);
        dst.word = (dst.word >> 8);
        highbyte = dadd_byte(src, dst);
    }

    //Form result from low byte to high byte
    unsigned short result = lowbyte + highbyte * (BYTESIZE);

    senddestination(result, opreg.D);

    //Carry handled by dadd_byte(), update Z only
    UPDATEZ(result);
}

unsigned char dadd_byte(memblock src, memblock dst) {
    //Decimal addition on the low two nibbles of src,dst

    unsigned char lownibble = 0, highnibble = 0;

    //Add low + carry in
    lownibble = src.nibbles.n0 + dst.nibbles.n0 + psw[C];

    //If exceeds decimal limit, carry out
    if (lownibble >= 10) {
        lownibble -= 10;
        highnibble = 1;
    }

    //Add high
    highnibble += src.nibbles.n1 + dst.nibbles.n1;

    //If exceeds decimal limit, carry out
    if (highnibble >= 10) {
        highnibble -= 10;
        psw[C] = 1;
    } else {
        psw[C] = 0;
    }

    //Result is formed from low and high nibbles
    return lownibble + highnibble * (NIBBLESIZE);
}

void cmp(unsigned short src, unsigned short dst) {
    unsigned short res;

    //If applicable, mask lower byte
    if (opreg.WB == 0)
        res = dst + ~(src) + 1;
    else
        res = (dst & LOWBYTE) + (~(src) & LOWBYTE) + 1;

    //Don't place in destination

    //Complement of src used for C,V
    updatepswALL(~src, dst, res);
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
    //Test bit indicated by src register
    unsigned short res = dst & (1 << src);

    updatepswNZ(res);
}

void bic(unsigned short src, unsigned short dst) {
    unsigned short res = dst & ~(1 << src);

    senddestination(res, opreg.D);
    updatepswNZ(res);
}

void bis(unsigned short src, unsigned short dst) {
    unsigned short res = dst | (1 << src);

    senddestination(res, opreg.D);
    updatepswNZ(res);
}

void mov(unsigned short src) {
    //Place src in dst
    senddestination(src, opreg.D);
}

void swap(unsigned short src, unsigned short dst) {
    //Place src in dst, dst in src
    senddestination(src, opreg.D);
    senddestination(dst, opreg.SC);
}

void sra(unsigned short dst) {
    //Save msbit
    int tempmsb = mask(15, 1, dst);
    dst = dst >> 1;

    //Clear either bit 7 or bit15 based on W/B, then place the saved msbit in that
    if (opreg.WB == 1)
        dst = (dst & ~(BIT7)) + (tempmsb * (BIT7));
    else
        dst = (dst & ~(BIT15)) + (tempmsb * (BIT15));

    senddestination(dst, opreg.D);
    updatepswNZ(dst);
}

void rrc(unsigned short dst) {
    //Get low bit that will be shifted out
    int tempcarry = mask(0, 1, dst);
    dst = dst >> 1;

    //Clear either bit 7 or bit15 based on W/B, then place carry in that bit
    if (opreg.WB == 1)
        dst = (dst & ~(BIT7)) + (psw[C] * (BIT7));
    else
        dst = (dst & ~(BIT15)) + (psw[C] * (BIT15));

    //Place the shifted out bit in carry
    psw[C] = tempcarry;

    senddestination(dst, opreg.D);
    updatepswNZ(dst);
}

void swpb(unsigned short dst) {
    //Save low byte, shift high to low, place low in high
    unsigned char templow = dst & 0x00FF;
    dst = dst >> 8;
    dst += templow * BYTESIZE;

    senddestination(dst, opreg.D);
}

void sxt(unsigned short dst) {
    //Get sign, extend the sign into the high byte
    int sign = mask(7, 1, dst);
    if (sign)
        dst |= 0xFF00;
    else
        dst &= 0x00FF;

    senddestination(dst, opreg.D);
    updatepswNZ(dst);
}

void movl(unsigned short dst) {
    //Place byte in result
    unsigned short result = opreg.B;

    //Modify upper
    switch (opreg.opcode) {
        case MOVL:
            //Keep high byte of dst in result
            result += (dst & HIGHBYTE);
            break;
        case MOVLZ:
            //do nothing as upper byte of result already zero
            break;
        case MOVLS:
            //Set high byte of result
            result += 0xFF00;
            break;
        default:
            break;
    }

    senddestination(result, opreg.D);
}

void movh(unsigned short dst) {
    //Keep low byte, place B in high byte
    dst = (dst & LOWBYTE) + (opreg.B * BYTESIZE);

    senddestination(dst, opreg.D);
}

void senddestination(unsigned short res, int destination) {
    //Place res in the register indicated by destination (opreg.D or opreg.S)

    //If byte, keep high byte of destination and place result in low byte
    if (opreg.WB == 0)
        regfile[REG][destination].word = res;
    else
        regfile[REG][destination].word = (regfile[REG][destination].word & HIGHBYTE) + (res & LOWBYTE);
}

//Update C,V,N,Z
void updatepswALL(unsigned short src, unsigned short dst, unsigned short res) {
    //Update N,Z
    updatepswNZ(res);

    //If byte, msbit is position 7, else msbit 15
    int msbitposition;
    (opreg.WB == 1) ? (msbitposition = 7) : (msbitposition = 15);

    //Get most significant bits of src,dst,res
    int msbsrc = mask(msbitposition, 1, src);
    int msbdst = mask(msbitposition, 1, dst);
    int msbres = mask(msbitposition, 1, res);

    //Update C,V based on array
    psw[C] = carryarr[msbsrc][msbdst][msbres];
    psw[V] = varr[msbsrc][msbdst][msbres];
}

//Update only N,Z
void updatepswNZ(unsigned short res) {
    //If byte, msbit is position 7, else msbit 15
    int msbitposition;
    (opreg.WB == 1) ? (msbitposition = 7) : (msbitposition = 15);

    //Get MSbit of result for negative flag
    psw[N] = mask(msbitposition, 1, res);

    //If byte operation, zero the high byte of result (only want to know if the relevent byte is zero)
    if (opreg.WB == 1)
        res &= LOWBYTE;

    //Update Z
    UPDATEZ(res);
}