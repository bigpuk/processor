#include <stdio.h>

#include "assembler.h"
#include "processor.h"
#include "proc_dump.h"

int main()
{
    Assembler asm_data = {};
    SPU spu_data = {};
    spu_data.stk = {};

    FILE* code_data = fopen(file_name, "r"); 

    stack_ctor(&(spu_data.stk), 16);

    assembler(&asm_data, code_data);
    processor(&asm_data, &spu_data);
}