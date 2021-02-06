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

    char *token;
    char **tokens = malloc(sizeof(char *));

    byte_t *bytes = malloc(sizeof(byte_t *));

    int bytes_index = 0;

    char modifiable[64];

    char sp[1024];

    strcpy(modifiable, code);

    int token_index = 0;

    
    token = strtok(modifiable, " ");

    while (token != NULL)
    {

        tokens[token_index] = token;

        token = strtok(NULL, " ");

        token_index++;
    }

    
    int i;

    for (i = 0; i < token_index; i++)
    {

        /* Get instruction if needed */
        size_t j = 0;
        char *instruction = malloc(sizeof(char *));

        char *sp_string = "sp";

        while (j != strlen(tokens[i]) - 1)
        {
            instruction[j] = tokens[i][j];
            j++;
        }

        /* mov */
        if (strcmp(instruction, "mov") == 0)
        {

            if (tokens[i + 1] != sp_string || tokens[i + 2] != sp_string)
            {
                bytes[bytes_index + 2] = atoi(tokens[i + 1]);
                bytes[bytes_index + 3] = atoi(tokens[i + 2]);
            }

            else
            {
	      
            }
            switch (tokens[i][strlen(tokens[i]) - 1])
            {

            case '1':

                bytes[bytes_index] = 1;

                bytes[bytes_index + 1] = 1;

                bytes_index += 3;

                break;

            case '2':

                bytes[bytes_index] = 1;

                bytes[bytes_index + 1] = 2;

                bytes_index += 3;

                break;

            case '3':

                bytes[bytes_index] = 1;

                bytes[bytes_index + 1] = 3;

                bytes_index += 3;

                break;

            case '4':

                bytes[bytes_index] = 1;

                bytes[bytes_index + 1] = 4;

                bytes_index += 3;

                break;

            case '5':

                bytes[bytes_index] = 1;

                bytes[bytes_index + 1] = 5;

                bytes_index += 3;

                break;

            case '6':

                bytes[bytes_index] = 1;

                bytes[bytes_index + 1] = 6;

                bytes_index += 3;

                break;

            default:
                printf("Invalid mov instruction");
                break;
            }
        }
    }

    return bytes;
}

int luna_execute(byte_t *program, struct luna_rt *rt)
{
    struct luna_header *header = (struct luna_header *)program;

    program += sizeof(struct luna_header);

    /* Program Counter */
    long pc = header->entry_point;

    int csp = 0; /* Call Stack Pointer */
    int hsp = 0; /* Hardware Stack Pointer */

    long cstack[64] = {0};                                      /* Call Stack */
    byte_t vstack[64] = {0};                                    /* Virtual Stack */
    struct hstack_value hstack[64] = {{.size = 8, .value = 0}}; /* Hardware Stack */

    /* General Purpose Registers */
    long registers[16];

    while (pc < header->count)
    {
        byte_t byte = program[pc];

        switch (byte)
        {
        case NOP:
        {
            break;
        }
        case MOVB:
        {
            byte_t lower = program[pc + 1];

            switch (lower)
            {
            case MOV1:
            {
                byte_t dest = program[pc + 2];
                byte_t src = program[pc + 3];

                pc += 3;

                registers[dest] = src;

                break;
            }
            case MOV2:
            {
                byte_t dest = program[pc + 2];
                byte_t src = program[pc + 3];

                pc += 3;

                registers[dest] = registers[src];

                break;
            }
            case MOV3:
            {
                byte_t dest = program[pc + 2];
                byte_t src = vstack[*((int *)&program[pc + 3])];

                pc += 6;

                registers[dest] = src;

                break;
            }
            case MOV4:
            {
                int dest = *((int *)&program[pc + 2]);
                byte_t src = program[pc + 5];

                pc += 6;

                vstack[dest] = src;

                break;
            }
            case MOV5:
            {
                int dest = *((int *)&program[pc + 2]);
                byte_t src = registers[pc + 5];

                pc += 6;

                vstack[dest] = src;

                break;
            }
            case MOV6:
            {
                int dest = *((int *)&program[pc + 2]);
                int src = *((int *)&program[pc + 5]);

                pc += 9;

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
            byte_t lower = program[pc + 1];

            switch (lower)
            {
            case MOV1:
            {
                byte_t dest = program[pc + 2];
                short src = *((short *)&program[pc + 3]);

                pc += 4;

                registers[dest] = src;

                break;
            }
            case MOV2:
            {
                byte_t dest = program[pc + 2];
                byte_t src = program[pc + 3];

                pc += 3;

                registers[dest] = registers[src];

                break;
            }
            case MOV3:
            {
                byte_t dest = program[pc + 2];
                short src = *((short *)&vstack[*((int *)&program[pc + 3])]);

                pc += 6;

                registers[dest] = src;

                break;
            }
            case MOV4:
            {
                int dest = *((int *)&program[pc + 2]);
                short src = *((short *)&program[pc + 5]);

                pc += 7;

                *((short *)&vstack[dest]) = src;

                break;
            }
            case MOV5:
            {
                int dest = *((int *)&program[pc + 3]);
                short src = (short)registers[pc + 5];

                pc += 6;

                *((short *)&vstack[dest]) = src;

                break;
            }
            case MOV6:
            {
                int dest = *((int *)&program[pc + 3]);
                int src = *((int *)&program[pc + 6]);

                pc += 9;

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
        case POPV:
        {
            hsp--;
            break;
        }
        case POPR:
        {
            registers[program[pc + 1]] = hstack[hsp].value;
            pc++;

            break;
        }
        case JMP:
        {
            pc = *((long *)&program[pc + 1]);

            continue;
        }
        case JE:
        {
            long val1 = hstack[hsp - 1].value;
            long val2 = hstack[hsp - 2].value;

            if (val1 == val2)
                pc = *((long *)&program[pc + 1]);
            else
                pc += 8;

            continue;
        }
        case JNE:
        {
            long val1 = hstack[hsp - 1].value;
            long val2 = hstack[hsp - 2].value;

            if (val1 != val2)
                pc = *((long *)&program[pc + 1]);
            else
                pc += 8;

            continue;
        }
        case CALL:
        {
            cstack[csp] = pc;
            csp++;

            pc = sizeof(struct luna_header) + *((long *)&program[pc + 2]);

            continue;
        }
        case RET:
        {
            if (csp == 0)
                goto exit;

            csp--;
            pc = cstack[csp];

            continue;
        }
        case ADD:
        {
            long val1 = hstack[hsp - 1].value;
            long val2 = hstack[hsp - 2].value;

            hsp -= 2;

            hstack[hsp].size = 8;
            hstack[hsp].value = val1 + val2;

            break;
        }
        case SUB:
        {
            long val1 = hstack[hsp - 1].value;
            long val2 = hstack[hsp - 2].value;

            hsp -= 2;

            hstack[hsp].size = 8;
            hstack[hsp].value = val1 - val2;

            break;
        }
        case MUL:
        {
            long val1 = hstack[hsp - 1].value;
            long val2 = hstack[hsp - 2].value;

            hsp -= 2;

            hstack[hsp].size = 8;
            hstack[hsp].value = val1 * val2;

            break;
        }
        case DIV:
        {
            long val1 = hstack[hsp - 1].value;
            long val2 = hstack[hsp - 2].value;

            hsp -= 2;

            hstack[hsp].size = 8;
            hstack[hsp].value = val1 / val2;

            break;
        }
        case MOD:
        {
            long val1 = hstack[hsp - 1].value;
            long val2 = hstack[hsp - 2].value;

            hsp -= 2;

            hstack[hsp].size = 8;
            hstack[hsp].value = val1 % val2;

            break;
        }
        default:
        {
            printf("0x%x\n", byte);
            rt->error(INVALID_OPCODE, "invalid opcode");
            break;
        }
        }

        pc++;
    }

exit:
    return hstack[hsp - 1].value;
}
