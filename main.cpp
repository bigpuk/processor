#include <stdio.h>

#include "assembler.h"
#include "processor.h"

int main()
{
    Assembler* data = {};
    stack* stk = {};
    FILE* code_data = fopen(file_name, "r"); 

    assembler(data, code_data, stk);
    
}