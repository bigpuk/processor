#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "assembler.h"

struct SPU
{
    int* reg_array;

    size_t reg_elem_num;
};

int processor(Assembler* asm_data, SPU spu_data);

#endif