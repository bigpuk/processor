#include <stdio.h>

#include "proc_dump.h"

int proc_dump(Assembler* data)
{
    printf("--------PROCESSOR INFO--------\n");
    printf("code buffer: pointer: %p, size: %d\n", data->code_buffer, data->code_buffer_size);
    printf("machine code: pointer: %p, size: %d\n", data->machine_code, data->machine_code_size);
    printf("line pointers: pointer: %p, size: %d", data->cmd_ptrs, data->cmd_ptrs_num);
    printf("register array pointer: %p\nip: %p\n", data->reg_array, data->ip);

    printf("------------------------------\n\n");
}
