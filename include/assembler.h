#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#include "../../stack/include/INCLUDE.h"

struct Assembler
{
    int* machine_code;
    size_t ip;
    char* code_buffer;
    char** cmd_ptrs;
    char** lables_array;

    size_t code_buffer_size;
    size_t machine_code_size;
    size_t cmd_ptrs_num;
    
    int current_function;
};

enum cmd
{
    HLT       = -1,
    PUSH      =  1,
    POP       =  2,
    SUB       =  3,
    ADD       =  4,
    MUL       =  5,
    DIV       =  6,
    OUT       =  7,
    IN        =  8,
    SQRT      =  9,
    SIN       = 10,
    COS       = 11,
    PUSHR     = 12,
    POPR      = 13,
    JMP       = 14,
    JA        = 15,
    JAE       = 16,
    JB        = 17,
    JBE       = 18,
    JE        = 19,
    JNE       = 20,
    STACKDUMP = 21,
    PROCDUMP  = 22
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
#define output_file_name "asm_bin.txt"

#define bug printf("8=====>");

int assembler(Assembler* data, FILE* code_data);

#endif
