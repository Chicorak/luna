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
    NOP   = 0b0000, /* No Instruction */
    MOVB  = 0b0001, /* Move BYTE  (1 byte)  */
    MOVW  = 0b0010, /* Move WORD  (2 bytes) */
    MOVD  = 0b0011, /* Move DWORD (4 bytes) */
    MOVQ  = 0b0100, /* Move QWORD (8 bytes) */
    PUSH  = 0b0101, /* Push to h-stack */
    POP   = 0b0110, /* Pop from h-stack */
    JMP   = 0b0111, /* Jump to address */
    CALL  = 0b1000, /* Call subroutine at address */
    RET   = 0b1001, /* Return from subroutine to address on call-stack */
    ADD   = 0b1010, /* Add two values on h-stack and push result to h-stack */
    SUB   = 0b1011, /* Subtract two values on h-stack and push result to h-stack */
    MUL   = 0b1100, /* Multiply two values on h-stack and push result to h-stack */
    DIV   = 0b1101, /* Divide two values on h-stack and push result to h-stack */
    MOD   = 0b1110, /* Divide two values on h-stack and push remainder result to h-stack */
    INT   = 0b1111, /* Call a VM interrupt/syscall */

    /* Lower Opcodes */
    MOV1  = 0b0001, /* MOV: Dest (Register) Src (Value) */
    MOV2  = 0b0011, /* MOV: Dest (Register) Src (Register) */
    MOV3  = 0b0010, /* MOV: Dest (Register) Src (Value At Address) */
    MOV4  = 0b0100, /* MOV: Dest (Address) Src (Value) */
    MOV5  = 0b0110, /* MOV: Dest (Address) Src (Register) */
    MOV6  = 0b0101, /* MOV: Dest (Address) Src (Value At Address) */
    PUSHB = 0b0001, /* PUSH: BYTE     (1 byte)  */
    PUSHW = 0b0010, /* PUSH: WORD     (2 bytes) */
    PUSHD = 0b0011, /* PUSH: DWORD    (4 bytes) */
    PUSHQ = 0b0100, /* PUSH: QWORD    (8 bytes) */
    PUSHR = 0b0101, /* PUSH: REGISTER (8 bytes) */
    POPV  = 0b0000, /* POP:  VOID     (0 bytes)  */
    POPR  = 0b0001, /* POP:  REGISTER (8 bytes)  */
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
