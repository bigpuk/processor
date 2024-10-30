#include <stdio.h>

#include "assembler.h"
#include "processor.h"
#include "proc_dump.h"

int main()
{
    Assembler data = {};
    stack stk = {};

    FILE* code_data = fopen(file_name, "r"); 

    stack_ctor(&stk, 16);

    assembler(&data, code_data, &stk);
    processor(&data, &stk);
}