// Created by Cameron Archibald B00893056 2024-05-06.
// debug.h - Declarations for debug functions

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include "memory.h"
#include "load.h"
#include "run.h"

void modifyregisters(const int regnum, unsigned short value);

void viewregisters(const int regnum);

void modifymem(int type, unsigned short address, unsigned char value);

void printmem(int start, int stop, int type);

void setbreakpoint(unsigned short address);

void printpsw(void);
#endif //DEBUG_H
