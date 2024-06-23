// Created by Cameron Archibald B00893056 on 2024-05-29.
// decode.c - decode functions, place operands in opreg

#include "decode.h"

//Stores information about the name of instruction and type of printing it requires
//Access array using the enumerated instruction name (instname)
char instname_encoding[NUMOFINSTRUCTIONS][2][INSTRUCTIONSTRINGLEN] = {
    {"BL", NONE}, {"BEQ", NONE}, {"BNE", NONE}, {"BC", NONE},
    {"BNC", NONE}, {"BN", NONE}, {"BGE", NONE}, {"BLT", NONE},
    {"BRA", NONE}, {"ADD", RC_WB_SC_D}, {"ADDC", RC_WB_SC_D}, {"SUB", RC_WB_SC_D},
    {"SUBC", RC_WB_SC_D}, {"DADD", RC_WB_SC_D}, {"CMP", RC_WB_SC_D}, {"XOR", RC_WB_SC_D},
    {"AND", RC_WB_SC_D}, {"OR", RC_WB_SC_D}, {"BIT", RC_WB_SC_D}, {"BIC", RC_WB_SC_D},
    {"BIS", RC_WB_SC_D}, {"MOV", WB_S_D}, {"SWAP", S_D}, {"SRA", WB_D},
    {"RRC", WB_D}, {"SWPB", DST}, {"SXT", DST}, {"SETPRI", NONE},
    {"SVC", NONE}, {"SETCC", NONE}, {"CLRCC", NONE}, {"CEX", NONE},
    {"LD", NONE}, {"ST", NONE}, {"MOVL", B_D}, {"MOVLZ", B_D},
    {"MOVLS", B_D}, {"MOVH", B_D}, {"LDR", NONE}, {"STR", NONE}
};

//Stores encoded values for registers and constants. Access using bits S/C or D.
char rc_encoding[8][2][INSTRUCTIONSTRINGLEN] = {
    {"R0", 0}, {"R1", 1}, {"R2", 2}, {"R3", 4}, {"R4", 8}, {"R5/LR", 16}, {"R6/SP", 32}, {"R7/PC", -1}
};

//Handle initial decoding layer
int decode(unsigned short instruction) {
    //Corresponds to the number in the ISA list
    int instnum = -1;

    //Check first 3 bits, #010 ADD-ST, #011 MOVL-MOVH
    switch (mask(13, 3, instruction)) {
        case 0b010: //ADD-ST

            if (mask(11, 2, instruction) == 0b11) {
                //LD/ST (bit12,11 == 11)
                instnum = LD + mask(10, 1, instruction);
            } else if (!(mask(12, 1, instruction) == 1 || mask(7, 5, instruction) == 0b11011)) {
                //NOT [CEX (bit12 != 0) or SETPRI-CLRCC (bit11,10,9,8,7 != 0b11011)] hence ADD-SXT
                instnum = ADD_SXT(instruction);
            }
            break;

        case 0b011: //MOVL-MOVH
            //MOVL + offset of bit12,11 (00,01,10,11)
            instnum = MOVL + mask(11, 2, instruction);
            break;

        case 0b100: //LDR/STR
        case 0b101:
        case 0b110:
        case 0b111:
            //LDR + offset of bit14
            instnum = LDR + mask(14, 1, instruction);
            break;

        default: //Not included in assignment 2
            break;
    }
    return instnum;
}

//Decode the ADD-SXT section
int ADD_SXT(unsigned short instruction) {
    //Return instruction number
    int instnum = -1;

    if (mask(10, 2, instruction) != 0b11) {
        //ADD-BIS (bit11,10 != 11)

        //ADD + offset of bit11,10,9,8
        instnum = ADD + mask(8, 4, instruction);
    } else {
        //MOV-SXT (bit11,10 == 11)
        if (mask(8, 1, instruction) == 0b0) {
            //MOV-SWAP (bit8 == 0)

            //MOV + offset of bit7
            instnum = MOV + mask(7, 1, instruction);
        } else {
            //SRA-SXT (bit8 == 1)

            //Offset from SRA hard coded from bit5,4,3
            switch (mask(3, 3, instruction)) {
                case 0b000:
                    instnum = SRA; //SRA
                    break;
                case 0b001:
                    instnum = SRA + 1; //RRC
                    break;
                case 0b011:
                    instnum = SRA + 2; //SWPB
                    break;
                case 0b100:
                    instnum = SRA + 3; //SXT
                    break;
                default:
                    break;
            }
        }
    }
    return instnum;
}

//Place operands in opreg
void assignoperands(int opcode) {
    //Place opcode in opreg
    opreg.opcode = opcode;

    //Get the type of instruction
    int insttype = instname_encoding[opcode][VALUE][0];

    //Place values in opreg based on type
    if (insttype == RC_WB_SC_D) {
        opreg.RC = mask(RC, 1, ir);
        opreg.SC = mask(SC, 3, ir);
    }

    if (insttype == RC_WB_SC_D || insttype == WB_S_D || insttype == WB_D)
        opreg.WB = mask(WB, 1, ir);

    if (insttype == WB_S_D)
        opreg.SC = mask(SC, 3, ir);

    if (insttype == B_D)
        opreg.B = mask(B, 8, ir);

    opreg.D = mask(D, 3, ir);
}

//Return only desired bits
unsigned short mask(int position, int numofbits, unsigned short input) {
    //Position = rightmost bit that we want
    //Numofbits = how many bits to the left of that we want to keep

    //Shift bits to the very right
    input = input >> position;

    //Starting from the left of the bits we want to keep up to the very left, zero all the bits
    for (int i = numofbits; i < 16; i++)
        input = input & ~(1 << i);
    return input;
}
