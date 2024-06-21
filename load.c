// Created by Cameron Archibald B00893056 2024-17-05.
// load.c - handles writing valid records to memory

#include "load.h"

//Take action based on the type of record
void loader(entry record) {
    switch (record.recordtype) {
        case S0:
            //Print data (filename) to user
            printf("Source filename: ");
            for (int i = 0; i < record.len - (ADDRBYTES + CHECKBYTES); i++)
                printf("%c", record.hexdata[i]);
            printf("\n");
            break;
        case S1:
        case S2:
            //Load into memory. mem[address][S1|S2], where S1 and S2 are enum for index 0 or 1 of the array (either IMEM or DMEM)
            //Load into the address specified by record.address + the offset
            //Only copy data bytes (len - # of address bytes - # of checksum bytes)
            for (int offset = 0; offset < record.len - (ADDRBYTES + CHECKBYTES); offset++) {
                mem[record.recordtype].byteaddr[record.address + offset] = record.hexdata[offset];
            }

            break;
        case S9:
            //End of .xme file, only reaches if no errors found, print to user
            printf("File read - no errors detected. Starting address: %x\n\n", record.address);
            //Assign starting WORD address, from BYTE address supplied by S9
            regfile[REG][PC].word = record.address;
            break;
        default:
            break;
    }
}


