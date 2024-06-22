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

int psw[4];

void controlloop(void) {
    if(clock == 0) //At start of program, place MOV R0,R0 into instruction register to prevent incorrect decode
        ir = 0x4C00;

    while(ir != 0x0000) {
        if(clock % 2 == 0) {
            if(regfile[REG][PC].word == breakpoint)
                break;
            F0();
            D0();
        } else {
            F1();
            E0();
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
    printf("%s\n", instname_encoding[instnum][0]);
    assignoperands(instnum);
}

void E0(void) {
    switch(opreg.opcode) {
        case ADD:
        case ADDC:
            add(SRC, DST);
            break;
        case SUB:
            break;
        case SUBC:
            break;
        case DADD:
            break;
        case CMP:
            break;
        case XOR:
            break;
        case AND:
            break;
        case OR:
            break;
        case BIT:
            break;
        case BIC:
            break;
        case BIS:
            break;
        case MOV:
            break;
        case SWAP:
            break;
        case SRA:
            break;
        case RRC:
            break;
        case SWPB:
            break;
        case SXT:
            break;
        case MOVL:
            break;
        case MOVLZ:
            break;
        case MOVLS:
            break;
        case MOVH:
            break;
        default:
            printf("%s Not included in A2\n", instname_encoding[opreg.opcode][0]);
        break;
    }


}

void clearopreg(void) {
    opreg.B = 0;
    opreg.D = 0;
    opreg.S = 0;
    opreg.RC = 0;
    opreg.SC = 0;
    opreg.WB = 0;
}