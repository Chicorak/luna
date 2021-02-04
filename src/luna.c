/* luna - a simple VM based programming language aimed at portability
 *
 * Copyright (c) 2021 Fossbay
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#include "luna.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hstack_value
{
    int size;
    long value;
};

char *luna_compile(char *code)
{
    int index = 0;
    char storage[1024];

    int token_index = 0;
    char **tokens = malloc(sizeof(char *));

    int i, j;
    for (i = 0; code[i] != 0; i++)
    {
        char c = code[i];

        switch (c)
        {
            case '!':
            case '#':
            case '%':
            case '^':
            case '&':
            case '*':
            case '(':
            case ')':
            case '-':
            case '+':
            case '=':
            case '{':
            case '}':
            case '[':
            case ']':
            case ':':
            case ';':
            case '\"':
            case '\'':
            case '<':
            case '>':
            case ',':
            case '.':
            case '?':
            case '|':
            case '\\':
            {
                break;
            }
        }

        storage[index] = c;
        storage[index + 1] = 0;
        index++;
    }
}

byte_t *luna_assemble(char *code, int options)
{
}

int luna_execute(byte_t *program, struct luna_rt *rt)
{
    struct luna_header *header = (struct luna_header *)program;

    program += sizeof(struct luna_header);

    /* Program Counter */
    long pc = header->entry_point;

    int csp = 0; /* Call Stack Pointer */
    int hsp = 0; /* Hardware Stack Pointer */

    long cstack[65536]; /* Call Stack */
    byte_t vstack[65536]; /* Virtual Stack */
    struct hstack_value hstack[65536]; /* Hardware Stack */

    /* General Purpose Registers */
    long registers[16];

    while (pc < header->count)
    {
        byte_t byte = program[pc];

        printf("0x%x\n", byte);

        byte_t upper = byte >> 4;
        byte_t lower = byte & 0x0F;

        switch (upper)
        {
            case NOP:
            {
                break;
            }
            case MOVB:
            {
                switch (lower)
                {
                    case MOV1:
                    {
                        byte_t dest = program[pc + 1];
                        byte_t src = program[pc + 2];

                        pc += 2;

                        registers[dest] = src;

                        break;
                    }
                    case MOV2:
                    {
                        byte_t dest = program[pc + 1];
                        byte_t src = program[pc + 2];

                        pc += 2;

                        registers[dest] = registers[src];

                        break;
                    }
                    case MOV3:
                    {
                        byte_t dest = program[pc + 1];
                        byte_t src = vstack[*((int *)&program[pc + 2])];

                        pc += 5;

                        registers[dest] = src;

                        break;
                    }
                    case MOV4:
                    {
                        int dest = *((int *)&program[pc + 1]);
                        byte_t src = program[pc + 4];

                        pc += 5;

                        vstack[dest] = src;

                        break;
                    }
                    case MOV5:
                    {
                        int dest = *((int *)&program[pc + 1]);
                        byte_t src = registers[pc + 4];

                        pc += 5;

                        vstack[dest] = src;

                        break;
                    }
                    case MOV6:
                    {
                        int dest = *((int *)&program[pc + 1]);
                        int src = *((int *)&program[pc + 4]);

                        pc += 8;

                        vstack[dest] = vstack[src];

                        break;
                    }
                    default:
                    {
                        rt->error(UNSUPPORTED_OPCODE, "unsupported movw type");
                        break;
                    }
                }

                break;
            }
            case MOVW:
            {
                switch (lower)
                {
                    case MOV1:
                    {
                        byte_t dest = program[pc + 1];
                        short src = *((short *)&program[pc + 2]);

                        pc += 3;

                        registers[dest] = src;

                        break;
                    }
                    case MOV2:
                    {
                        byte_t dest = program[pc + 1];
                        byte_t src = program[pc + 2];

                        pc += 2;

                        registers[dest] = registers[src];

                        break;
                    }
                    case MOV3:
                    {
                        byte_t dest = program[pc + 1];
                        short src = *((short *)&vstack[*((int *)&program[pc + 2])]);

                        pc += 5;

                        registers[dest] = src;

                        break;
                    }
                    case MOV4:
                    {
                        int dest = *((int *)&program[pc + 1]);
                        short src = *((short *)&program[pc + 4]);

                        pc += 6;

                        *((short *)&vstack[dest]) = src;

                        break;
                    }
                    case MOV5:
                    {
                        int dest = *((int *)&program[pc + 1]);
                        short src = (short)registers[pc + 4];

                        pc += 5;

                        *((short *)&vstack[dest]) = src;

                        break;
                    }
                    case MOV6:
                    {
                        int dest = *((int *)&program[pc + 1]);
                        int src = *((int *)&program[pc + 4]);

                        pc += 8;

                        *((short *)&vstack[dest]) = *((short *)&vstack[src]);

                        break;
                    }
                    default:
                    {
                        rt->error(UNSUPPORTED_OPCODE, "unsupported movw type");
                        break;
                    }
                }

                break;
            }
            case PUSH:
            {
                switch (lower)
                {
                    case PUSHB:
                    {
                        hstack[hsp].size = 1;
                        hstack[hsp].value = program[pc + 1];

                        hsp++;
                        pc++;

                        break;
                    }
                    case PUSHW:
                    {
                        hstack[hsp].size = 2;
                        hstack[hsp].value = *((short *)&program[pc + 1]);

                        hsp += 1;
                        pc += 2;

                        break;
                    }
                    case PUSHD:
                    {
                        hstack[hsp].size = 4;
                        hstack[hsp].value = *((int *)&program[pc + 1]);

                        hsp += 1;
                        pc += 4;

                        break;
                    }
                    case PUSHQ:
                    {
                        hstack[hsp].size = 8;
                        hstack[hsp].value = *((long *)&program[pc + 1]);

                        hsp += 1;
                        pc += 8;

                        break;
                    }
                    case PUSHR:
                    {
                        hstack[hsp].size = 8;
                        hstack[hsp].value = registers[program[pc + 1]];

                        hsp++;
                        pc++;

                        break;
                    }
                    default:
                    {
                        rt->error(UNSUPPORTED_OPCODE, "unsupported push type");
                        break;
                    }
                }

                break;
            }
            case POP:
            {
                hsp--;

                if (lower == POPR)
                {
                    registers[program[pc + 1]] = hstack[hsp].value;
                }

                break;
            }
            case JMP:
            {
                pc = *((long *)&program[pc + 1]);

                continue;
            }
            case CALL:
            {
                cstack[csp] = pc;
                csp++;

                pc = sizeof(struct luna_header) + *((long *)&program[pc + 1]);

                continue;
            }
            case RET:
            {
                csp--;
                pc = cstack[csp];

                continue;
            }
            default:
            {
                rt->error(INVALID_OPCODE, "invalid opcode");
                break;
            }
        }

        pc++;
    }

    return hstack[hsp - 1].value;
}
