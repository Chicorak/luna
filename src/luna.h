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

#ifndef LUNA_H
#define LUNA_H

typedef unsigned char byte_t;

enum opcode
{
    /* Upper Opcodes */
    NOP   = 0, /* No Instruction  */
    MOVB  = 1, /* Move BYTE  (1 byte)  */
    MOVW  = 2, /* Move WORD  (2 bytes) */
    MOVD  = 3, /* Move DWORD (4 bytes) */
    MOVQ  = 4, /* Move QWORD (8 bytes) */
    PUSHB = 5, /* Push 1 byte to h-stack */
    PUSHW = 6, /* Push 1 byte to h-stack */
    PUSHD = 7, /* Push 1 byte to h-stack */
    PUSHQ = 8, /* Push 1 byte to h-stack */
    PUSHR = 9, /* Push 1 byte to h-stack */
    POPV  = 10, /* Pop from h-stack */
    POPR  = 11, /* Pop from h-stack and store in register */
    JMP   = 12, /* Jump to address */
    JE    = 13, /* Jump to address if two values on stack are equal */
    JNE   = 14, /* Jump to address if two values on stack are not equal */
    CALL  = 15, /* Call subroutine at address */
    RET   = 16, /* Return from subroutine to address on call-stack */
    ADD   = 17, /* Add two values on h-stack and push result to h-stack */
    SUB   = 18, /* Subtract two values on h-stack and push result to h-stack */
    MUL   = 19, /* Multiply two values on h-stack and push result to h-stack */
    DIV   = 20, /* Divide two values on h-stack and push result to h-stack */
    INC   = 21, /* Increment register value by 1 */
    DEC   = 22, /* Increment register value by 1 */
    AND   = 23, /* Logical AND Instruction */
    OR    = 24, /* Logical OR Instruction */
    XOR   = 25, /* Logical XOR Instruction */
    TEST  = 26, /* Logical TEST Instruction */
    NOT   = 27, /* Logical NOT Instruction */
    INT   = 255, /* Call a VM interrupt/syscall */

    /* Lower Opcodes */
    MOV1  = 1, /* MOV: Dest (Register) Src (Value) */
    MOV2  = 2, /* MOV: Dest (Register) Src (Register) */
    MOV3  = 3, /* MOV: Dest (Register) Src (Value At Address) */
    MOV4  = 4, /* MOV: Dest (Address) Src (Value) */
    MOV5  = 5, /* MOV: Dest (Address) Src (Register) */
    MOV6  = 6, /* MOV: Dest (Address) Src (Value At Address) */
};

enum exception
{
    INVALID_OPCODE,
    UNSUPPORTED_OPCODE
};

struct luna_header
{
    int count, entry_point;
};

struct luna_vmcall
{
    int params;
    void (*vmcall)();
};

struct luna_rt
{
    void (*error)(int, char *);
    struct luna_vmcall vmcalls[2];
};

char *luna_compile(char *code);
byte_t *luna_assemble(char *code, int options);
int luna_execute(byte_t *program, struct luna_rt *rt);

#endif /* LUNA_H */
