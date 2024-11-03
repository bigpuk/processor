#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#include "assembler.h"
#include "processor.h"
#include "../../stack/include/INCLUDE.h"

#define NonArgCmd(enum) do {                            \
    asm_data->machine_code[machine_code_elem] = enum;   \
    machine_code_elem++;                                \
} while(0)

#define ArgCmd(command_len, enum) do {                                  \
    asm_data->machine_code[machine_code_elem] = enum;                   \
    machine_code_elem++;                                                \
                                                                        \
    asm_data->cmd_ptrs[asm_data->cmd_ptrs_num] += strlen(command_len);  \
                                                                        \
    sscanf(asm_data->cmd_ptrs[asm_data->cmd_ptrs_num], "%s", cmd);      \
                                                                        \
    int arg = -7;                                                       \
                                                                        \
    if(enum == PUSHR || enum == POPR) arg = (int) cmd[0] - '0';         \
    else arg = atoi(cmd);                                               \
                                                                        \
    asm_data->machine_code[machine_code_elem] = arg;                    \
    machine_code_elem++;                                                \
                                                                        \
    (asm_data->machine_code_size)++;                                    \
} while(0)

int read_user_code(char* cmd);

int assembler(Assembler* asm_data, FILE* code_data)
{
    assert(asm_data);
    assert(code_data);

    struct stat assembler_info = {};

    stat(file_name, &assembler_info);

    asm_data->code_buffer_size = assembler_info.st_size;    

    asm_data->code_buffer = (char*) calloc(asm_data->code_buffer_size + 1, sizeof(char));
    if(!asm_data->code_buffer)
    {
        printf("Out of memory!");

        return 1;
    }

    fread(asm_data->code_buffer, asm_data->code_buffer_size, sizeof(char), code_data);

    asm_data->cmd_ptrs_num = 0;

    for(size_t current_symbol = 0; current_symbol < asm_data->code_buffer_size; current_symbol++)
    {
        if(asm_data->code_buffer[current_symbol] == '\r' || asm_data->code_buffer[current_symbol] == '\n')
        {
            asm_data->code_buffer[current_symbol] = '\0';
        }
    }

    size_t beginning_check = 1;

    for(size_t current_elem = 0; current_elem < asm_data->code_buffer_size; current_elem++)
    {
        if(beginning_check)
        {
            (asm_data->cmd_ptrs_num)++;
        }

        if(((asm_data->code_buffer)[current_elem] == '\0') && ((asm_data->code_buffer)[current_elem + 1] != '\0'))
        {
            beginning_check = 1;
        }
        else beginning_check = 0;
    }

    strcat(asm_data->code_buffer, "\0");
    
    asm_data->cmd_ptrs = (char**) calloc(asm_data->cmd_ptrs_num, sizeof(char*));
    if(!asm_data->cmd_ptrs)
    {
        printf("ERROR!(cmd_ptrs)");
    }

    strcat(asm_data->code_buffer, "\0");

    beginning_check = 1;

    asm_data->cmd_ptrs_num = 0;

    for(size_t current_elem = 0; current_elem < asm_data->code_buffer_size; current_elem++)
    {
        if(beginning_check)
        {
            (asm_data->cmd_ptrs)[asm_data->cmd_ptrs_num] = ((asm_data->code_buffer) + current_elem);

            (asm_data->cmd_ptrs_num)++;
        }

        if(((asm_data->code_buffer)[current_elem] == '\0') && ((asm_data->code_buffer)[current_elem + 1] != '\0')) beginning_check = 1;
        else beginning_check = 0;
    }

    char* cmd = (char*) calloc(100, sizeof(char));// TODO insert into struct
    if(!cmd)
    {
        printf("ERROR!(cmd)");

        return 1;
    }

    asm_data->machine_code = (int*) calloc((size_t)10000, sizeof(int));
    if(!asm_data->machine_code)
    {
        printf("ERROR!(machine code)");

        return 1;
    }

    //asm_data->lables_array = (char**) calloc(100, 20*sizeof(char*));

    size_t machine_code_elem = 0;
    asm_data->cmd_ptrs_num = 0;
    
    //TODO метки
    //TODO дебаг джампов и всего
    //TODO функции

    while(1)  //TODO macro(антикопипаст)
    {
        sscanf(asm_data->cmd_ptrs[asm_data->cmd_ptrs_num], "%s", cmd);

        int cmd_type = read_user_code(cmd);
        
        if(cmd_type == PUSH)
        {
            ArgCmd("push", cmd_type);
        }
        else if(cmd_type == PUSHR)
        {
            ArgCmd("pushr", cmd_type);
        }
        else if(cmd_type == POPR)
        {
            ArgCmd("popr", cmd_type);
        }
        else if(cmd_type == JMP || cmd_type == JAE || cmd_type == JBE || cmd_type == JNE)
        {
            ArgCmd("xxx", cmd_type);
        }
        else if(cmd_type == JA || cmd_type == JB || cmd_type == JE)
        {
            ArgCmd("xx", cmd_type);
        }
        else if(cmd_type >= POP && cmd_type <= PROCDUMP)
        {
            NonArgCmd(cmd_type);
        }
        else if(cmd_type == HLT)
        {
            NonArgCmd(cmd_type);

            break;
        }
        else printf("Assembler ERROR: %d", cmd_type);

        (asm_data->cmd_ptrs_num)++;

        (asm_data->machine_code_size)++;
    }

    asm_data->machine_code_size = machine_code_elem;

    FILE* asm_bin = fopen(output_file_name, "w");

    for(machine_code_elem = 0; machine_code_elem < asm_data->machine_code_size; machine_code_elem++)
    {
        fprintf(asm_bin, "%d", asm_data->machine_code[machine_code_elem]);
    }
    

    return 0;
}

int read_user_code(char* cmd)
{
    if(strcmp(cmd, "hlt") == 0)       return HLT;
    if(strcmp(cmd, "push") == 0)      return PUSH;
    if(strcmp(cmd, "pop") == 0)       return POP;
    if(strcmp(cmd, "sub") == 0)       return SUB;
    if(strcmp(cmd, "add") == 0)       return ADD;
    if(strcmp(cmd, "mul") == 0)       return MUL;
    if(strcmp(cmd, "div") == 0)       return DIV;
    if(strcmp(cmd, "out") == 0)       return OUT;
    if(strcmp(cmd, "in") == 0)        return IN;
    if(strcmp(cmd, "sqrt") == 0)      return SQRT;
    if(strcmp(cmd, "sin") == 0)       return SIN;
    if(strcmp(cmd, "cos") == 0)       return COS;
    if(strcmp(cmd, "pushr") == 0)     return PUSHR;
    if(strcmp(cmd, "popr") == 0)      return POPR;
    if(strcmp(cmd, "jmp") == 0)       return JMP;
    if(strcmp(cmd, "ja") == 0)        return JA;
    if(strcmp(cmd, "jae") == 0)       return JAE;
    if(strcmp(cmd, "jb") == 0)        return JB;
    if(strcmp(cmd, "jbe") == 0)       return JBE;
    if(strcmp(cmd, "je") == 0)        return JE;
    if(strcmp(cmd, "jae") == 0)       return JAE;
    if(strcmp(cmd, "stackdump") == 0) return STACKDUMP;
    if(strcmp(cmd, "procdump") == 0)  return PROCDUMP;

    return -1337;
}