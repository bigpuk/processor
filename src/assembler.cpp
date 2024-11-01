#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#include "assembler.h"
#include "processor.h"
#include "..\..\stack\include\INCLUDE.h"

#define NonArgCmd(command) do {     \
    command();                      \
} while(0)

#define ArgCmd(command, arg) do {   \
    command(arg);                   \
} while(0)

int read_user_code(char* cmd);

int assembler(Assembler* data, FILE* code_data)
{
    assert(data);
    assert(code_data);

    struct stat assembler_info = {};

    stat(file_name, &assembler_info);

    data->code_buffer_size = assembler_info.st_size;    

    data->code_buffer = (char*) calloc(data->code_buffer_size + 1, sizeof(char));
    if(!data->code_buffer)
    {
        printf("Out of memory!");

        return 1;
    }

    fread(data->code_buffer, data->code_buffer_size, sizeof(char), code_data);

    data->cmd_ptrs_num = 0;

    for(size_t current_symbol = 0; current_symbol < data->code_buffer_size; current_symbol++)
    {
        if(data->code_buffer[current_symbol] == '\r' || data->code_buffer[current_symbol] == '\n')
        {
            data->code_buffer[current_symbol] = '\0';
        }
    }

    size_t beginning_check = 1;

    for(size_t current_elem = 0; current_elem < data->code_buffer_size; current_elem++)
    {
        if(beginning_check)
        {
            (data->cmd_ptrs_num)++;
        }

        if(((data->code_buffer)[current_elem] == '\0') && ((data->code_buffer)[current_elem + 1] != '\0'))
        {
            beginning_check = 1;
        }
        else beginning_check = 0;
    }

    strcat(data->code_buffer, "\0");
    
    data->cmd_ptrs = (char**) calloc(data->cmd_ptrs_num, sizeof(char*));
    if(!data->cmd_ptrs)
    {
        printf("ERROR!(cmd_ptrs)");
    }

    strcat(data->code_buffer, "\0");

    beginning_check = 1;

    data->cmd_ptrs_num = 0;

    for(size_t current_elem = 0; current_elem < data->code_buffer_size; current_elem++)
    {
        if(beginning_check)
        {
            (data->cmd_ptrs)[data->cmd_ptrs_num] = ((data->code_buffer) + current_elem);

            (data->cmd_ptrs_num)++;
        }

        if(((data->code_buffer)[current_elem] == '\0') && ((data->code_buffer)[current_elem + 1] != '\0')) beginning_check = 1;
        else beginning_check = 0;
    }

    char* cmd = (char*) calloc(100, sizeof(char));
    if(!cmd)
    {
        printf("ERROR!(cmd)");

        return 1;
    }

    data->machine_code = (int*) calloc((size_t)10000, sizeof(int));
    if(!data->machine_code)
    {
        printf("ERROR!(machine code)");

        return 1;
    }

    //data->lables_array = (char**) calloc(100, 20*sizeof(char*));

    size_t machine_code_elem = 0;

    data->cmd_ptrs_num = 0;
    
    //TODO метки
    //TODO дебаг джампов и всего
    //TODO функции

    while(1)  //TODO macro(антикопипаст)
    {
        sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

        int cmd_type = read_user_code(cmd);
        
        if(cmd_type == PUSH)
        {
            data->machine_code[machine_code_elem] = cmd_type;
            machine_code_elem++;

            data->cmd_ptrs[data->cmd_ptrs_num] += strlen("push");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);
            
            int arg = atoi(cmd);

            data->machine_code[machine_code_elem] = arg;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_type == PUSHR)
        {
            data->machine_code[machine_code_elem] = cmd_type;
            machine_code_elem++;

            data->code_buffer += strlen("pushr");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

            int reg_num = (int) cmd[0] - '0';

            data->machine_code[machine_code_elem] = reg_num;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_type == POPR)
        {
            data->machine_code[machine_code_elem] = cmd_type;
            machine_code_elem++;

            data->code_buffer += strlen("popr");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

            int reg_num = (int) cmd[0] - '0';

            data->machine_code[machine_code_elem] = reg_num;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_type == JMP || cmd_type == JAE || cmd_type == JBE || cmd_type == JNE)
        {
            data->machine_code[machine_code_elem] = cmd_type;
            machine_code_elem++;

            data->cmd_ptrs[data->cmd_ptrs_num] += strlen("jmp");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

            int ip_num = atoi(cmd);

            data->machine_code[machine_code_elem] = ip_num;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_type == JA || cmd_type == JB || cmd_type == JE)
        {
            data->machine_code[machine_code_elem] = cmd_type;
            machine_code_elem++;

            data->cmd_ptrs[data->cmd_ptrs_num] += strlen("ja");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

            int ip_num = atoi(cmd);

            data->machine_code[machine_code_elem] = ip_num;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_type >= POP && cmd_type <= PROCDUMP)
        {
            data->machine_code[machine_code_elem] = cmd_type;
            machine_code_elem++;
        }
        else if(cmd_type == HLT)
        {
            data->machine_code[machine_code_elem] = cmd_type;
            machine_code_elem++;

            break;
        }
        else printf("Assembler ERROR: %d", cmd_type);

        (data->cmd_ptrs_num)++;

        (data->machine_code_size)++;
    }

    data->machine_code_size = machine_code_elem;

    FILE* asm_bin = fopen(output_file_name, "w");

    for(machine_code_elem = 0; machine_code_elem < data->machine_code_size; machine_code_elem++)
    {
        fprintf(asm_bin, "%d", data->machine_code[machine_code_elem]);
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