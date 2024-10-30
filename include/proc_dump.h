#ifndef PROC_DUMP_H
#define PROC_DUMP_H

#include "assembler.h"
#include "processor.h"

int proc_dump(Assembler* data);

#define PROC_DUMP(arg) do   \
{                           \
    proc_dump(arg);         \
} while(0)

#endif