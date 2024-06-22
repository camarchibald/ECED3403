// Created by Cameron Archibald B00893056 2024-17-05.
// load.h - definitions for load.c

#ifndef LOAD_H
#define LOAD_H

#include <stdio.h>
#include <stdbool.h>

#include "memory.h"
#include "run.h"

//How many bytes each type of value takes in the string
#define ADDRBYTES 2
#define LENBYTES 1
#define CHECKBYTES 1

//Number of bytes to print per line
#define PRINTSIZE 16

//Max len of string (30 + addrbytes + lenbytes + checkbytes + spare) * 2
#define MAXRECLEN 100

//Keep track of type of record
//Compatible with memtype enum as S1 writes to IMEM and S2 to DMEM
typedef enum recordtype {
    S0 = -1,
    S1 = 0,
    S2 = 1,
    S9 = 2,
    INVALID = -2
} recordtype;

//Information about an S record
typedef struct entry {
    //String read in from S record
    char string[MAXRECLEN];
    //Holds enum recordtype
    int recordtype;
    //Length of address + data + checksum
    unsigned char len;
    //Address to start writing to memory
    int address;
    //True if the line is valid
    bool checksum;
    //Holds data to be copied to memory
    unsigned char hexdata[MAXRECLEN];
} entry;

void loader(entry record);

#endif //LOAD_H
