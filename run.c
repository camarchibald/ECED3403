// Created by Cameron Archibald B00893056 2024-21-06.
// run.c

#include "run.h"

#define SRC regfile[opreg.RC][opreg.SC].word
#define DST regfile[REG][opreg.D].word

int clock;

unsigned short imar;
unsigned short ictrl;
unsigned short imbr;

unsigned short dmar;
unsigned short dctrl;
unsigned short dmar;

unsigned short ir;
operands opreg;

int psw[PSWLEN];

void controlloop(void) {
    if(clock == 0) //At start of program, place MOV R0,R0 into instruction register to prevent incorrect decode
        ir = 0x4C00;

    int breakflag = 0;

    while(!breakflag) {
        if(clock % 2 == 0) {
            if(regfile[REG][PC].word == breakpoint + 2)
                break;
            F0();
            D0();
        } else {
            F1();
            E0();
            if(ir == 0x0000)
                breakflag = 1;
        }

        clock++;
    }
}

void F0(void) {
    imar = regfile[REG][PC].word;
    ictrl = 1;
    regfile[REG][PC].word+= 2;
}

void F1(void) {
    if(ictrl) {
        imbr = mem[IMEM].wordaddr[imar>>1].word;
        ir = imbr;
    }
    ictrl = 0;
}

void D0(void) {
    int instnum = decode(ir);
    assignoperands(instnum);
}

void E0(void) {
    switch(opreg.opcode) {
        case ADD:
        case ADDC:
            add(SRC, DST);
            break;
        case SUB:
        case SUBC:
            sub(SRC, DST);
            break;
        case DADD:
            memblock s = {.word = SRC}, d = {.word = DST};
            dadd(s, d);
            break;
        case CMP:
            cmp(SRC, DST);
            break;
        case XOR:
            xor(SRC,DST);
            break;
        case AND:
            and(SRC,DST);
            break;
        case OR:
            or(SRC,DST);
            break;
        case BIT:
            bit(SRC,DST);
            break;
        case BIC:
            bic(SRC,DST);
            break;
        case BIS:
            bis(SRC,DST);
            break;
        case MOV:
            mov(SRC);
            break;
        case SWAP:
            swap(SRC,DST);
            break;
        case SRA:
            sra(DST);
            break;
        case RRC:
            rrc(DST);
            break;
        case SWPB:
            swpb(DST);
            break;
        case SXT:
            sxt(DST);
            break;
        case MOVL:
        case MOVLZ:
        case MOVLS:
            movl(DST);
            break;
        case MOVH:
            movh(DST);
            break;
        default:
            //None
            break;
    }

    printf("%s executed at clock %d\n", instname_encoding[opreg.opcode][0], clock);
    clearopreg();
}

void clearopreg(void) {
    opreg.B = 0;
    opreg.D = 0;
    opreg.S = 0;
    opreg.RC = 0;
    opreg.SC = 0;
    opreg.WB = 0;
}