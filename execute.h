// Created by Cameron Archibald B00893056 2024-21-06.
// execute.c

#ifndef EXECUTE_H
#define EXECUTE_H

#include "decode.h"

void add(unsigned short src, unsigned short dst);
void senddestination(unsigned short res);
void updatepsw(unsigned short src, unsigned short dst, unsigned short res);

#endif //EXECUTE_H
