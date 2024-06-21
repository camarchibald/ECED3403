// Created by Cameron Archibald B00893056 2024-05-06.
// memory.c - Declare memory elements IMEM, DMEM, regfile

#include "memory.h"

//Register file, column 0 are initial register values, column 1 are constants
memblock regfile[REGFILEWIDTH][REGFILELEN] = {{0,0,0,0,0,0,0,0},{0,1,2,4,8,16,32,-1}};

//IMEM + DMEM
memspace mem[2];

//Initialize breakpoint to end of program
unsigned short breakpoint = BYTESIZE - 1;