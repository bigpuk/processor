#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <math.h>

#include "assembler.h"
#include "processor.h"

int processor(Assembler* data, stack* stk)
{
    assert(data);

    size_t ip = 0;

    while(1)
    {
        switch(data->machine_code[ip])
        {
            case HLT: break;
            
            case PUSH:  {
                            ip++;

                            int arg = data->machine_code[ip];

                            stack_push(stk, arg);

                            break;
                        }
            
            case POP:   {
                            stack_pop(stk);

                            break;
                        }

            case SUB:   {
                            int substr_A = stack_pop(stk);
                            int substr_B = stack_pop(stk);

                            stack_push(stk, substr_B - substr_A);

                            break;
                        }

            case ADD:   {
                            stack_push(stk, stack_pop(stk) + stack_pop(stk));

                            break;
                        }

            case MUL:   {
                            stack_push(stk, stack_pop(stk) * stack_pop(stk));

                            break;
                        }

            case DIV:   {
                            int div_A = stack_pop(stk);
                            int div_B = stack_pop(stk);

                            stack_push(stk, div_B / div_A);

                            break;
                        }

            case OUT:   {
                            printf("%d", stack_pop(stk));

                            break;
                        }

            case IN:    {
                            int in_arg = 0;

                            scanf("%d", &in_arg);

                            stack_push(stk, in_arg);

                            break;
                        }

            case SQRT:  {
                            stack_push(stk, (int) sqrt(stack_pop(stk))); //TODO прочитать про фикс. точку

                            break;
                        }

            case SIN:   {
                            stack_push(stk, (int) sin(stack_pop(stk)));

                            break;
                        }

            case COS:   {
                            stack_push(stk, (int) cos(stack_pop(stk)));

                            break;
                        }

            case DUMP:  {
                            STACK_DUMP(stk);
                        }
            
            default:
                printf("ERROR: %d", data->machine_code[ip]);
        }
    }
}