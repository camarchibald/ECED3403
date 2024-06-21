// Created by Cameron Archibald B00893056 2024-17-05.
// main.c - handles general user input

#include "main.h"

int main(void) {
    char inchar = '0';
    char filename[20];
    FILE *infile;
    printf("XM-23p loader - Cameron Archibald B00893056\n");
    listoptions();

    //Allow user to select options
    while (inchar != 'x') {
        printf("\nOption: ");
        scanf(" %c", &inchar);

        switch (inchar) {
            case 'd':
                //Call decoder
                instructionloop();
                break;
            case '?':
                //View different options
                listoptions();
                break;
            case 'l':
                //Loader
                printf("Enter file to open: ");
                scanf("%s", filename);
                //If fopen != NULL (file exists), call readentry
                if ((infile = fopen(filename, "r"))) {
                    readentry(infile);
                    fclose(infile);
                } else {
                    printf("No such file exists\n");
                }
                break;
            case 'v':
                //View registers
                printf("Enter -1 to view all registers, or 0-7 for a specific register:\n");
                int reg;
                scanf("%d", &reg);
                viewregisters(reg);
                break;
            case 'r':
                //Modify registers
                printf("Enter 0-7 to select register and hex value:\n");
                int regnum, value;
                scanf("%d %x", &regnum, &value);
                modifyregisters(regnum, value);
                break;
            case 'm':
                //View memory
                printf("Enter lower bound (byte), upper bound (byte), and 0 for IMEM or 1 for DMEM:\n");
                int lower, upper, type;
                scanf("%x %x %d", &lower, &upper, &type);
                printmem(lower, upper, type);
                break;
            case 'c':
                //Modify memory
                printf("Enter word address (hex), value (hex), and memory space (IMEM 0 or DMEM 1)\n");
                int addr, val, space;
                scanf("%x %x %d", &addr, &val, &space);
                modifymem(space, addr, val);
                break;
            case 'b':
                //Set breakpoint
                printf("Enter breakpoint word address (hex)\n");
                int address;
                scanf("%x", &address);
                setbreakpoint(address);
                break;
            case 'x':
                //Exit the program
                printf("Exiting program\n");
                break;
            default:
                //Invalid char
                printf("Invalid selection\n");
                break;
        }
    }
    return 0;
}

//Display options to choose from
void listoptions(void) {
    printf("\n? - lists the options (below)\n"
        "l - load\n"
        "d - decode starting from address in S9 record\n"
        "v - view registers\n"
        "r - modify registers\n"
        "m - view memory\n"
        "c - modify memory\n"
        "b - add breakpoint\n"
        "x - exit\n");
}
