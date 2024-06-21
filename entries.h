// Created by Cameron Archibald B00893056 2024-17-05.
// entries.h - definitions for entries.c

#ifndef ENTRIES_H
#define ENTRIES_H

#include "load.h"
#include "memory.h"

//Indices of the string that values start
#define RECTYPESTART 1
#define LENSTART 2
#define ADDRSTART 4
#define DATASTART 8



void readentry(FILE *infile);

entry parseentry(entry rec);

int assignrecordtype(char sx);

unsigned char hextobyte(char msnibble, char lsnibble);

unsigned char hextoint(char c);

entry recordvalid(entry rec);
#endif //ENTRIES_H
