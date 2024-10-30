#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#include "assembler.h"
#include "..\..\stack\include\INCLUDE.h"

#define NonArgCmd(command) do {     \
    command();                      \
} while(0)

#define ArgCmd(command, arg) do {   \
    command(arg);                   \
} while(0)

int read_user_code(char* cmd);

int assembler(Assembler* data, FILE* code_data, stack* stk)
{
    assert(data);
    assert(code_data);
    assert(stk);

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
    
    while(1)  //TODO macro
    {
        sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

        int cmd_num = read_user_code(cmd);
        
        if(cmd_num == PUSH)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;

            data->cmd_ptrs[data->cmd_ptrs_num] += strlen("push");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);
            
            int arg = atoi(cmd);

            printf("%d", arg);

            data->machine_code[machine_code_elem] = arg;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_num == PUSHR)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;

            data->code_buffer += strlen("pushr");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

            int reg_num = (int) cmd[0] - '0';

            data->machine_code[machine_code_elem] = reg_num;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_num == POPR)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;

            data->code_buffer += strlen("popr");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

            int reg_num = (int) cmd[0] - '0';

            data->machine_code[machine_code_elem] = reg_num;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_num == JMP || cmd_num == JAE || cmd_num == JBE || cmd_num == JNE)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;

            data->code_buffer += strlen("jmp");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

            int ip_num = (int) cmd[0] - '0';

            data->machine_code[machine_code_elem] = ip_num;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_num == JA || cmd_num == JB || cmd_num == JE)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;

            data->code_buffer += strlen("ja");

            sscanf(data->cmd_ptrs[data->cmd_ptrs_num], "%s", cmd);

            int ip_num = (int) cmd[0] - '0';

            data->machine_code[machine_code_elem] = ip_num;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_num >= POP && cmd_num <= PROCDUMP)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;
        }
        else if(cmd_num == HLT)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;

            break;
        }
        else printf("ERROR: %d", cmd_num);

        (data->cmd_ptrs_num)++;

        (data->machine_code_size)++;
    }

    return 0;
}

int read_user_code(char* cmd)
{
    if(strcmp(cmd, "hlt"))       return HLT;
    if(strcmp(cmd, "push"))      return PUSH;
    if(strcmp(cmd, "pop"))       return POP;
    if(strcmp(cmd, "sub"))       return SUB;
    if(strcmp(cmd, "add"))       return ADD;
    if(strcmp(cmd, "mul"))       return MUL;
    if(strcmp(cmd, "div"))       return DIV;
    if(strcmp(cmd, "out"))       return OUT;
    if(strcmp(cmd, "in"))        return IN;
    if(strcmp(cmd, "sqrt"))      return SQRT;
    if(strcmp(cmd, "sin"))       return SIN;
    if(strcmp(cmd, "cos"))       return COS;
    if(strcmp(cmd, "pushr"))     return PUSHR;
    if(strcmp(cmd, "popr"))      return POPR;
    if(strcmp(cmd, "jmp"))       return JMP;
    if(strcmp(cmd, "ja"))        return JA;
    if(strcmp(cmd, "jae"))       return JAE;
    if(strcmp(cmd, "jb"))        return JB;
    if(strcmp(cmd, "jbe"))       return JBE;
    if(strcmp(cmd, "je"))        return JE;
    if(strcmp(cmd, "jae"))       return JAE;
    if(strcmp(cmd, "stackdump")) return STACKDUMP;
    if(strcmp(cmd, "procdump"))  return PROCDUMP;

    return -1337;
}