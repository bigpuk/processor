#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

struct code_info
{
    int* code;
    size_t code_size;
    
    size_t funcsion_has_arg;
    int current_function;
};

struct Register
{
    int* reg_array;

    size_t reg_elem_num;
};

enum cmd
{
    HLT  = -1,
    PUSH =  1,
    POP  =  2,
    SUB  =  3,
    ADD  =  4,
    MUL  =  5,
    DIV  =  6,
    OUT  =  7,
    IN   =  8,
    SQRT =  9,
    SIN  = 10,
    COS  = 11,
    DUMP = 12
};

// struct cmd
// {
//     int hlt  = HLT;
//     int push = PUSH;
//     int pop  = POP;
//     int sub  = SUB;
//     int add  = ADD;
//     int mul  = MUL;
//     int div  = DIV;
//     int out  = OUT;
//     int in   = IN;  //vvod+push
//     int sqrt = SQRT;
//     int sin  = SIN;
//     int cos  = COS;
//     int dump = DUMP;
// };

#define file_name "code.txt"


int assembler(code_info* data, FILE* code_data, stack* stk, Register* reg);
#endif