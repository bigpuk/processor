#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <math.h>

#include "assembler.h"
#include "processor.h"
#include "proc_dump.h"

// TODO macro

int processor(Assembler* asm_data, SPU* spu_data)
{
    assert(asm_data);
    assert(spu_data);

    spu_data->reg_array = (int*) calloc((size_t) 26, sizeof(int));
    if(!spu_data->reg_array)
    {
        printf("Out of memory!");

        return 1;
    }

    while(1)
    {
        switch(asm_data->machine_code[asm_data->ip])
        {
            case HLT:       {
                                return 0;
                            }
            
            case PUSH:      {
                                (asm_data->ip)++;

                                int arg = asm_data->machine_code[asm_data->ip];

                                stack_push(&(spu_data->stk), arg);

                                break;
                            }
            
            case POP:       {
                                stack_pop(&(spu_data->stk));

                                break;
                            }

            case SUB:       {
                                int substr_A = stack_pop(&(spu_data->stk));
                                int substr_B = stack_pop(&(spu_data->stk));

                                stack_push(&(spu_data->stk), substr_B - substr_A);

                                break;
                            }

            case ADD:       {
                                stack_push(&(spu_data->stk), stack_pop(&(spu_data->stk)) + stack_pop(&(spu_data->stk)));

                                break;
                            }

            case MUL:       {
                                stack_push(&(spu_data->stk), stack_pop(&(spu_data->stk)) * stack_pop(&(spu_data->stk)));

                                break;
                            }

            case DIV:       {
                                int div_A = stack_pop(&(spu_data->stk));
                                int div_B = stack_pop(&(spu_data->stk));

                                stack_push(&(spu_data->stk), div_B / div_A);

                                break;
                            }

            case OUT:       {
                                printf("%d", stack_pop(&(spu_data->stk)));

                                break;
                            }

            case IN:        {
                                int in_arg = 0;

                                scanf("%d", &in_arg);

                                stack_push(&(spu_data->stk), in_arg);

                                break;
                            }

            case SQRT:      {
                                stack_push(&(spu_data->stk), (int) sqrt(stack_pop(&(spu_data->stk)))); //TODO прочитать про фикс. точку

                                break;
                            }

            case SIN:       {
                                stack_push(&(spu_data->stk), (int) sin(stack_pop(&(spu_data->stk))));

                                break;
                            }

            case COS:       {
                                stack_push(&(spu_data->stk), (int) cos(stack_pop(&(spu_data->stk))));

                                break;
                            }

            case PUSHR:     {
                                (asm_data->ip)++;

                                int arg = spu_data->reg_array[asm_data->machine_code[asm_data->ip]];

                                stack_push(&(spu_data->stk), arg);

                                break;
                            }

            case POPR :     {
                                (asm_data->ip)++;

                                spu_data->reg_array[asm_data->machine_code[asm_data->ip]] = stack_pop(&(spu_data->stk));

                                break;
                            }

            case JMP:       {
                                (asm_data->ip)++;

                                size_t jump_ptr = asm_data->machine_code[asm_data->ip];

                                (asm_data->ip) = jump_ptr - 1;

                                break;
                            }

            case JA:        {
                                int compare_A = stack_pop(&(spu_data->stk));
                                int compare_B = stack_pop(&(spu_data->stk));

                                (asm_data->ip)++;

                                if(compare_B < compare_A)
                                {
                                    size_t jump_ptr = asm_data->machine_code[asm_data->ip];

                                    (asm_data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case JAE:       {
                                int compare_A = stack_pop(&(spu_data->stk));
                                int compare_B = stack_pop(&(spu_data->stk));

                                (asm_data->ip)++;

                                if(compare_B <= compare_A)
                                {
                                    size_t jump_ptr = asm_data->machine_code[asm_data->ip];

                                    (asm_data->ip) = jump_ptr - 1;
                                }

                                break;
                            }
            
            case JB:        {
                                int compare_A = stack_pop(&(spu_data->stk));
                                int compare_B = stack_pop(&(spu_data->stk));
                                
                                (asm_data->ip)++;

                                if(compare_B > compare_A)
                                {
                                    size_t jump_ptr = asm_data->machine_code[asm_data->ip];

                                    (asm_data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case JBE:       {
                                int compare_A = stack_pop(&(spu_data->stk));
                                int compare_B = stack_pop(&(spu_data->stk));

                                (asm_data->ip)++;

                                if(compare_B >= compare_A)
                                {
                                    size_t jump_ptr = asm_data->machine_code[asm_data->ip];

                                    (asm_data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case JE:        {
                                int compare_A = stack_pop(&(spu_data->stk));
                                int compare_B = stack_pop(&(spu_data->stk));

                                (asm_data->ip)++;

                                if(compare_B == compare_A)
                                {
                                    size_t jump_ptr = asm_data->machine_code[asm_data->ip];

                                    (asm_data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case JNE:       {
                                int compare_A = stack_pop(&(spu_data->stk));
                                int compare_B = stack_pop(&(spu_data->stk));

                                (asm_data->ip)++;

                                if(compare_B != compare_A)
                                {
                                    size_t jump_ptr = asm_data->machine_code[asm_data->ip];

                                    (asm_data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case STACKDUMP: {
                                STACK_DUMP(&(spu_data->stk));

                                break;
                            }

            case PROCDUMP:  {
                                PROC_DUMP(asm_data, spu_data);

                                break;
                            }
            
            default:
                printf("Processor ERROR: %d", asm_data->machine_code[asm_data->ip]);
        }
        (asm_data->ip)++;
    }
}