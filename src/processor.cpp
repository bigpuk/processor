#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <math.h>

#include "assembler.h"
#include "processor.h"
#include "proc_dump.h"

int processor(Assembler* data, stack* stk)
{
    assert(data);

    while(1)
    {
        switch(data->machine_code[data->ip])
        {
            case HLT:       {
                                return 0;
                            }
            
            case PUSH:      {
                                (data->ip)++;

                                int arg = data->machine_code[data->ip];

                                stack_push(stk, arg);

                                break;
                            }
            
            case POP:       {
                                stack_pop(stk);

                                break;
                            }

            case SUB:       {
                                int substr_A = stack_pop(stk);
                                int substr_B = stack_pop(stk);

                                stack_push(stk, substr_B - substr_A);

                                break;
                            }

            case ADD:       {
                                stack_push(stk, stack_pop(stk) + stack_pop(stk));

                                break;
                            }

            case MUL:       {
                                stack_push(stk, stack_pop(stk) * stack_pop(stk));

                                break;
                            }

            case DIV:       {
                                int div_A = stack_pop(stk);
                                int div_B = stack_pop(stk);

                                stack_push(stk, div_B / div_A);

                                break;
                            }

            case OUT:       {
                                printf("%d", stack_pop(stk));

                                break;
                            }

            case IN:        {
                                int in_arg = 0;

                                scanf("%d", &in_arg);

                                stack_push(stk, in_arg);

                                break;
                            }

            case SQRT:      {
                                stack_push(stk, (int) sqrt(stack_pop(stk))); //TODO прочитать про фикс. точку

                                break;
                            }

            case SIN:       {
                                stack_push(stk, (int) sin(stack_pop(stk)));

                                break;
                            }

            case COS:       {
                                stack_push(stk, (int) cos(stack_pop(stk)));

                                break;
                            }

            case PUSHR:     {
                                (data->ip)++;

                                int arg = data->reg_array[data->machine_code[data->ip]];

                                stack_push(stk, arg);

                                break;
                            }

            case POPR :     {
                                (data->ip)++;

                                data->reg_array[data->machine_code[data->ip]] = stack_pop(stk);

                                break;
                            }

            case JMP:       {
                                (data->ip)++;

                                size_t jump_ptr = data->machine_code[data->ip];

                                (data->ip) = jump_ptr - 1;

                                break;
                            }

            case JA:        {
                                int compare_A = stack_pop(stk);
                                int compare_B = stack_pop(stk);

                                (data->ip)++;

                                if(compare_B < compare_A)
                                {
                                    size_t jump_ptr = data->machine_code[data->ip];

                                    (data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case JAE:       {
                                int compare_A = stack_pop(stk);
                                int compare_B = stack_pop(stk);

                                (data->ip)++;

                                if(compare_B <= compare_A)
                                {
                                    size_t jump_ptr = data->machine_code[data->ip];

                                    (data->ip) = jump_ptr - 1;
                                }

                                break;
                            }
            
            case JB:        {
                                int compare_A = stack_pop(stk);
                                int compare_B = stack_pop(stk);
                                
                                (data->ip)++;

                                if(compare_B > compare_A)
                                {
                                    size_t jump_ptr = data->machine_code[data->ip];

                                    (data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case JBE:       {
                                int compare_A = stack_pop(stk);
                                int compare_B = stack_pop(stk);

                                (data->ip)++;

                                if(compare_B >= compare_A)
                                {
                                    size_t jump_ptr = data->machine_code[data->ip];

                                    (data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case JE:        {
                                int compare_A = stack_pop(stk);
                                int compare_B = stack_pop(stk);

                                (data->ip)++;

                                if(compare_B == compare_A)
                                {
                                    size_t jump_ptr = data->machine_code[data->ip];

                                    (data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case JNE:       {
                                int compare_A = stack_pop(stk);
                                int compare_B = stack_pop(stk);

                                (data->ip)++;

                                if(compare_B != compare_A)
                                {
                                    size_t jump_ptr = data->machine_code[data->ip];

                                    (data->ip) = jump_ptr - 1;
                                }

                                break;
                            }

            case STACKDUMP: {
                                STACK_DUMP(stk);

                                break;
                            }

            case PROCDUMP:  {
                                PROC_DUMP(data);

                                break;
                            }
            
            default:
                printf("ERROR: %d", data->machine_code[data->ip]);
        }
        (data->ip)++;
    }
}