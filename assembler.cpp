#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#include "assembler.h"
#include "..\stack\include\INCLUDE.h"

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

    struct stat cmd_info = {};

    stat(file_name, &cmd_info);

    data->code_buffer_size = cmd_info.st_size;    

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

    for(size_t current_line = 0; current_line < data->code_buffer_size; current_line++)
    {
        if(beginning_check)
        {
            (data->cmd_ptrs_num)++;
        }

        if(((data->code_buffer)[current_line] == '\0') && ((data->code_buffer)[current_line + 1] != '\0'))
        {
            beginning_check = 1;
        }
        else beginning_check = 0;
    }

    data->cmd_prts = (char**) calloc(data->cmd_ptrs_num, sizeof(char *));

    strcat(data->code_buffer, "\0");

    char* cmd = (char*) calloc(100, sizeof(char));

    data->machine_code = (int *) calloc(10000, sizeof(int));

    size_t machine_code_elem = 0;

    size_t ptr_num = 0;

    while(1)
    {
        sscanf(data->code_buffer, "%s", cmd);

        int cmd_num = read_user_code(cmd);
        
        // switch(cmd_num)
        // {
        //     case HLT: break;
            
        //     case PUSH:  {
        //                     data->machine_code[machine_code_elem] = PUSH;
        //                     machine_code_elem++;

        //                     data->code_buffer += strlen("push");

        //                     sscanf(data->cmd_prts[ptr_num], "%s", cmd);
                            
        //                     int arg = atoi(cmd);

        //                     data->machine_code[machine_code_elem] = arg;
        //                     machine_code_elem++;

        //                     break;
        //                 }
            
        //     case POP:   {
        //                     data->machine_code[machine_code_elem] = POP;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case SUB:   {
        //                     data->machine_code[machine_code_elem] = SUB;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case ADD:   {
        //                     data->machine_code[machine_code_elem] = ADD;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case MUL:   {
        //                     data->machine_code[machine_code_elem] = MUL;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case DIV:   {
        //                     data->machine_code[machine_code_elem] = DIV;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case OUT:   {
        //                     data->machine_code[machine_code_elem] = DIV;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case IN:    {
        //                     data->machine_code[machine_code_elem] = DIV;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case SQRT:  {
        //                     data->machine_code[machine_code_elem] = DIV;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case SIN:   {
        //                     data->machine_code[machine_code_elem] = DIV;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case COS:   {
        //                     data->machine_code[machine_code_elem] = DIV;
        //                     machine_code_elem++;

        //                     break;
        //                 }

        //     case DUMP:  {
        //                     STACK_DUMP(stk);
        //                 }
            
        //     default:
        //         break;
        // }

        if(cmd_num == PUSH)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;

            data->code_buffer += strlen("push");

            sscanf(data->cmd_prts[ptr_num], "%s", cmd);
            
            int arg = atoi(cmd);

            data->machine_code[machine_code_elem] = arg;
            machine_code_elem++;

            (data->machine_code_size)++;
        }
        else if(cmd_num >= POP && cmd_num <= DUMP)
        {
            data->machine_code[machine_code_elem] = cmd_num;
            machine_code_elem++;
        }
        else if(cmd_num == HLT)
        {
            break;//TODO (- 1)
        }
        else printf("ERROR: %d", cmd_num);

        (data->machine_code_size)++;
    }

    return -1337;
}

int read_user_code(char* cmd)
{
    if(strcmp(cmd, "hlt"))  return HLT;
    if(strcmp(cmd, "push")) return PUSH;
    if(strcmp(cmd, "pop"))  return POP;
    if(strcmp(cmd, "sub"))  return SUB;
    if(strcmp(cmd, "add"))  return ADD;
    if(strcmp(cmd, "mul"))  return MUL;
    if(strcmp(cmd, "div"))  return DIV;
    if(strcmp(cmd, "out"))  return OUT;
    if(strcmp(cmd, "in"))   return IN;
    if(strcmp(cmd, "sqrt")) return SQRT;
    if(strcmp(cmd, "sin"))  return SIN;
    if(strcmp(cmd, "cos"))  return COS;
    if(strcmp(cmd, "dump")) return DUMP;

    return -1337;
}
