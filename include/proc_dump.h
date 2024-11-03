#ifndef PROC_DUMP_H
#define PROC_DUMP_H

#include "assembler.h"
#include "processor.h"

int proc_dump(Assembler* asm_data, SPU* spu_data);

#define PROC_DUMP(arg1, arg2) do   \
{                           \
    proc_dump(arg1, arg2);         \
} while(0)

#endif