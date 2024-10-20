#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "assembler.h"

int read_user_code(char* cmd);

int assembler(code_info* data, FILE* code_data, stack* stk, Register* reg)
{
    while(1)
    {
        char cmd[1000];

        fscanf(code_data, cmd);

        switch(read_user_code(cmd))
        {
            case HLT: break;
            
            case PUSH:  {
                            int arg = 0;

                            fscanf(code_data, (const char*) arg)

                            stack_push(stk, arg);
                        }
            case POP:   {
                            reg->reg_array[reg->reg_elem_num] = stack_pop(stk);

                            (reg->reg_elem_num)++;
                        }
            
            default:
                break;
        }
    }
}

int read_user_code(char* cmd)
{
    if(strcmp(cmd, "hlt")) return HLT;
    if(strcmp(cmd, "push")) return PUSH;
    if(strcmp(cmd, "pop")) return POP;
    if(strcmp(cmd, "sub")) return SUB;
    if(strcmp(cmd, "add")) return ADD;
    if(strcmp(cmd, "mul")) return MUL;
    if(strcmp(cmd, "div")) return DIV;
    if(strcmp(cmd, "out")) return OUT;
    if(strcmp(cmd, "in")) return IN;
    if(strcmp(cmd, "sqrt")) return SQRT;
    if(strcmp(cmd, "sin")) return SIN;
    if(strcmp(cmd, "cos")) return COS;
    if(strcmp(cmd, "dump")) return DUMP;
}