#include <stdio.h>

#include "proc_dump.h"

int proc_dump(Assembler* asm_data, SPU* spu_data)
{
    printf("--------PROCESSOR INFO--------\n");

    printf("code buffer: pointer: %p, size: %d\n", asm_data->code_buffer, asm_data->code_buffer_size);
    printf("machine code: pointer: %p, size: %d\n", asm_data->machine_code, asm_data->machine_code_size);
    printf("line pointers: pointer: %p, size: %d", asm_data->cmd_ptrs, asm_data->cmd_ptrs_num);
    printf("register array pointer: %p\nip: %p\n", spu_data->reg_array, asm_data->ip);

    printf("------------------------------\n\n");
}
