/* luna - a simple VM based programming language aimed at portability
 *
 * Copyright (c) 2021 Cleanware
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

struct luna_header
{
    int entry, count;
};

struct luna_rt
{
    int argc;
    char **argv;
    void (*error)(char *);
    int (*read)(int, void *, int);
    int (*write)(int, void *, int);
};

byte_t *luna_compile(char *code, int options);
int luna_execute(byte_t *program, struct luna_rt *rt);

#endif /* LUNA_H */
