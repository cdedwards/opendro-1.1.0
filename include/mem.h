///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Memory
//
// Copyright (C) 2011 ShumaTech http://www.shumatech.com/
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
#ifndef MEM_H_
#define MEM_H_

#include "types.h"

#define __STACK_SIZE__ (5 * 1024)

extern uint32_t __data_beg__;
extern uint8_t __bss_end__;
extern uint8_t __stack_end__;
extern uint8_t* __heap_end__;
register uint8_t* __stack_top__ asm("sp");

uint32_t memfree(void);

#endif // MEM_H_
