#ifndef PROC_DUMP_H
#define PROC_DUMP_H

#include "assembler.h"

int proc_dump(Assembler* data);

#define PROC_DUMP(arg) do   \
{                           \
    proc_dump(arg);         \
} while(0)

#endif