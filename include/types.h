///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Types and basic defines
//
// Copyright (C) 2008-2011 ShumaTech http://www.shumatech.com/
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////////
#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define ABS(X)   ((X) < 0 ? -(X) : (X))

#ifndef NULL
#define NULL 0
#endif

#define PI              3.1415926535

#define BITS_PER_BYTE   8

static inline uint32_t
atomic_swap(volatile uint32_t*  ptr,
            uint32_t            value)
{
    uint32_t prev;
    
    __asm__ __volatile__ (
        "adr     r3, 1f\n"
        "bx      r3\n"
        ".ALIGN\n"
        ".ARM\n"
        "1:\n"
        "swp     %0, %1, [%2]\n"
        "adr     r3, 2f + 1\n"
        "bx      r3\n"
        ".THUMB\n"
        "2:\n"
        : "=&r" (prev)
        : "r" (value), "r" (ptr)
        : "r3", "memory", "cc");
    
    return prev;
}

#endif /*TYPES_H_*/
