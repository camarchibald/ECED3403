// Created by Cameron Archibald B00893056 2024-21-06.
// run.c - pipelining and F0,F1 stages. D0 calls to decode.c, E0 calls to execute.c
//       - registers, psw

#include "run.h"

#define SRC regfile[opreg.RC][opreg.SC].word
#define DST regfile[REG][opreg.D].word

//Global registers used by the cpu
int clock;

int psw[PSWLEN];

unsigned short imar;
unsigned short ictrl;
unsigned short imbr;

unsigned short dmar;
unsigned short dctrl;
unsigned short dmar;

unsigned short ir;
//Holds operands after the decode stage
operands opreg;

//Main pipeline control
void controlloop(void) {
    //At start of program, place MOV R0,R0 into instruction register to prevent incorrect decode
    if (clock == 0)
        ir = 0x4C00;

    int breakflag = 0;

    while (!breakflag) {
        if (clock % 2 == 0) {
            //Check for breakpoint (breaking at PC = breakpoint + 2 allows instruction at PC - 2 to execute)
            if (regfile[REG][PC].word == breakpoint + 2)
                break;
            F0();
            D0();
        } else {
            F1();
            E0();
            printpsw();
            //Once ir contains instruction 0000 (after E0 for the preceeding instruction), break
            if (ir == 0x0000)
                breakflag = 1;
        }
        clock++;
    }
}

//F0 stage
void F0(void) {
    //Place PC in imar, set ictrl indicating a read is desired, increment PC
    imar = regfile[REG][PC].word;
    ictrl = 1;
    regfile[REG][PC].word += 2;
}

//F1 stage
void F1(void) {
    //If read intended, place in imbr the value from imem as described in imar, then place in ir
    //imar contains pc as byte address, >>1 to get word addr
    if (ictrl) {
        imbr = mem[IMEM].wordaddr[imar >> 1].word;
        ir = imbr;
        ictrl = 0;
    }
}

//D0 stage (decode.c)
void D0(void) {
    //Call to decode to determine the opcode, then place operands in opreg
    assignoperands(decode(ir));
}

//E0 stage (execute.c)
void E0(void) {
    //Call execute functions based on opcodes
    //Pass in SRC and/or DST which are macros to fetch the value contained in src/dst registers
    switch (opreg.opcode) {
        case ADD:
        case ADDC:
            add(SRC, DST);
            break;
        case SUB:
        case SUBC:
            sub(SRC, DST);
            break;
        case DADD:
            //Create memblock struct to pass into dadd, enables isolating nibbles
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

    //Execution finished
    printf("%s executed at clock %d\n", instname_encoding[opreg.opcode][0], clock);
    //Clear operands from opreg
    clearopreg();
}

//Clear operands from opreg
void clearopreg(void) {
    opreg.B = 0;
    opreg.D = 0;
    opreg.S = 0;
    opreg.RC = 0;
    opreg.SC = 0;
    opreg.WB = 0;
}