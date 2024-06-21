// Created by Cameron Archibald B00893056 2024-05-06.
// debug.c - Functions to view and modify memory and registers

#include "debug.h"

//Modify register contents
void modifyregisters(const int regnum, unsigned short value) {
    //If valid register
    if(regnum >= R0 && regnum <= R7)
        //Place value in register
        regfile[REG][regnum].word = value;
    else
        printf("Invalid register\n");
}

//View register contents
void viewregisters(const int regnum) {
    if(regnum >= 0 && regnum <= R7)
        //Print individual register
        printf("R%d: %04x\n", regnum, regfile[REG][regnum].word);
    else if(regnum == ALL)
        //Print all registers
        for (int i = 0; i < REGFILELEN; i++)
            printf("R%d: %04x\n", i, regfile[REG][i].word);
    else
        printf("Invalid register\n");
}

//Modify memory contents
void modifymem(int type, unsigned short address, unsigned short value) {
    //If address is valid and IMEM || DMEM
    if(address >= 0 && address < WORDSIZE && (type == IMEM || type == DMEM))
        //Place value in memory
        mem[type].wordaddr[address].word = value;
    else
        printf("Invalid entry\n");
}

//Set word address breakpoint
void setbreakpoint(unsigned short address) {
    //If address valid
    if(address >= 0 && address < WORDSIZE)
        breakpoint = address;
    else
        printf("Breakpoint out of bounds\n");
}


//Print from memory in 16 byte (or PRINTSIZE) increments
void printmem(int start, int stop, int type) {

    //Don't attempt to print if specified bounds outside of the memory size or not in IMEM or DMEM
    if (start < 0 || start > (DATASIZE) || stop < 0 || stop > (DATASIZE) || (type != IMEM && type != DMEM)) {
        printf("Invalid memory bounds\n");
        return;
    }

    unsigned char printchar;

    //Condition to begin on the next lowest multiple of 16
    if (start % PRINTSIZE != 0)
        start -= start % PRINTSIZE;
    //Condition to end on the next highest multiple of 16
    if (stop % PRINTSIZE != 0)
        stop += (PRINTSIZE - stop % PRINTSIZE);

    //Cycle through groups of 16
    for (int i = start; i < stop; i += PRINTSIZE) {
        printf("\n");
        //Print the starting addr of the given line byte (word)
        printf("%04x (%04x): ", i, i/2);

        //Print hex values for the group of 16
        for (int j = i; j < i + PRINTSIZE; j++)
            printf("%02x ", mem[type].byteaddr[j]);

        //Print ascii values for the group of 16
        for (int j = i; j < i + PRINTSIZE; j++) {
            printchar = mem[type].byteaddr[j];
            //If NOT one of the printable ascii values, print '.' (Extended ascii above 127 not desired)
            if (printchar < ' ' || printchar > 127)
                printchar = '.';
            printf("%c", printchar);
        }
    }
    printf("\n");
}

