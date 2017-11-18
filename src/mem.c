///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Memory Lua commands
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
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "types.h"
#include "mem.h"
#include "trace.h"
#include "errno.h"

uint8_t* __heap_end__ = &__bss_end__;

void* _sbrk_r(void *reent, ptrdiff_t incr)
{
    uint8_t* area_start;
    uint8_t* area_end;
    
    area_start = __heap_end__;
    area_end = __heap_end__ + incr;
    if (area_end > __stack_top__)
    {
        TRACE_WARNING("heap attempt to overrun stack\n");
        errno = ENOMEM;
        return (void*) -1;
    }
    
    __heap_end__ = area_end;
    
    return area_start;
}

uint32_t memfree(void)
{
    struct mallinfo info;
    
    info = mallinfo();
    
    return(&__stack_end__ - &__bss_end__ - info.uordblks - __STACK_SIZE__);
}

///////////////////////////////////////////////////////////////////////////////
// L U A
///////////////////////////////////////////////////////////////////////////////

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lrotable.h"
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
#include "luash.h"
#include "lstate.h"

static int
mem_rb_lua(lua_State* L)
{
    uintptr_t addr = luaL_checknumber(L, 1);
    lua_pushnumber(L, *(uint8_t*)addr);
    return 1;
}

static int
mem_rw_lua(lua_State* L)
{
    uintptr_t addr = luaL_checknumber(L, 1);
    lua_pushnumber(L, *(uint32_t*)addr);
    return 1;
}

static int
mem_wb_lua(lua_State* L)
{
    uintptr_t addr = luaL_checknumber(L, 1);
    uint8_t value = luaL_checknumber(L, 2);
    *(uint8_t*)addr = value;
    return 0;
}

static int
mem_ww_lua(lua_State* L)
{
    uintptr_t addr = luaL_checknumber(L, 1);
    uint32_t value = luaL_checknumber(L, 2);
    *(uint32_t*)addr = value;
    return 0;
}

static int
mem_db_lua(lua_State* L)
{
    uintptr_t addr = luaL_checknumber(L, 1);
    uint32_t len = luaL_checknumber(L, 2);
    FILE* out;

    // Retrieve the output file pointer
    out = luash_out(L);

    fprintf(out, "          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
    while (len > 0)
    {
        fprintf(out, "%08"PRIx32, addr & 0xfffffff0);
        if (addr & 0xf)
            fprintf(out, "%*s", 3 * (int)(addr & 0xf), "");
        do
        {
            fprintf(out, " %02"PRIx8, *((uint8_t*)addr));
            addr++;
            len--;
        } while (addr & 0xf && len > 0);
        fprintf(out, "\n");
    }

    return 0;
}

static int
mem_dw_lua(lua_State* L)
{
    uintptr_t addr = luaL_checknumber(L, 1);
    uint32_t len = luaL_checknumber(L, 2);
    FILE* out;

    // Retrieve the output file pointer
    out = luash_out(L);

    addr &= 0xfffffffc;
    fprintf(out, "                0        4        8        c\n");
    while (len > 0)
    {
        fprintf(out, "%08"PRIx32, addr & 0xfffffff0);
        if (addr & 0xf)
            fprintf(out, "%*s", 3 * (int)(addr & 0xf), "");
        do
        {
            fprintf(out, " %08"PRIx32, *((uint32_t*)addr));
            addr += 4;
            len--;
        } while (addr & 0xf && len > 0);
        fprintf(out, "\n");
    }

    return 0;
}

static int
mem_show_lua(lua_State* L)
{
    struct mallinfo info;
    FILE* out;
    global_State *g = G(L);
    uint8_t* stack_max;
    
    // Retrieve the output file pointer
    out = luash_out(L);

    info = mallinfo();
    
    for (stack_max = &__stack_end__ - __STACK_SIZE__;
         stack_max < &__stack_end__;
         stack_max++)
    {
        if (*stack_max != 0x5a)
            break;
    }
    
#define FIELD_WIDTH 13
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Heap reserved", info.arena);
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Heap used", info.uordblks);
    fprintf(out, "%-*s : %p\n", FIELD_WIDTH, "Heap end", __heap_end__);
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Stack size", __STACK_SIZE__);
    fprintf(out, "%-*s : %ld\n", FIELD_WIDTH, "Stack used", &__stack_end__ - __stack_top__);
    fprintf(out, "%-*s : %ld\n", FIELD_WIDTH, "Stack max", &__stack_end__ - stack_max);
    fprintf(out, "%-*s : %p\n", FIELD_WIDTH, "Stack top", __stack_top__);
    fprintf(out, "%-*s : %ld\n", FIELD_WIDTH, "Lua limit", g->memlimit);
    fprintf(out, "%-*s : %ld\n", FIELD_WIDTH, "Lua used", g->totalbytes);
    fprintf(out, "%-*s : %ld\n", FIELD_WIDTH, "Lua free", g->memlimit - g->totalbytes);

    return 0;
}

const LUA_REG_TYPE mem_map_lua[] =
{
    { LSTRKEY("rb"), LFUNCVAL(mem_rb_lua) },
    { LSTRKEY("rw"), LFUNCVAL(mem_rw_lua) },
    { LSTRKEY("wb"), LFUNCVAL(mem_wb_lua) },
    { LSTRKEY("ww"), LFUNCVAL(mem_ww_lua) },
    { LSTRKEY("db"), LFUNCVAL(mem_db_lua) },
    { LSTRKEY("dw"), LFUNCVAL(mem_dw_lua) },
    { LSTRKEY("show"), LFUNCVAL(mem_show_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
mem_open_lua(lua_State* L)
{
    LREGISTER(L, "mem", mem_map_lua);
}
