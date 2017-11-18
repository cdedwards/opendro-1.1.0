///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// System Lua commands
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

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lrotable.h"
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"

#include "board.h"
#include "drolim.h"
#include "trace.h"
#include "machine.h"
#include "function.h"
#include "scale.h"
#include "axis.h"
#include "luash.h"

static int
system_reset(lua_State* L)
{
    reset();
    return 0;
}

static int
system_version(lua_State* L)
{
    char version[10];

    snprintf(version, sizeof(version), "%d.%d.%d",
             OPENDRO_MAJOR, OPENDRO_MINOR, OPENDRO_MAINT);
    lua_pushstring(L, version);
    return 1;
}

static int
system_hardware(lua_State* L)
{
    lua_pushstring(L, OPENDRO_HARDWARE);
    return 1;
}

static int
system_silent(lua_State* L)
{
    machine_all_prompt(!lua_toboolean(L, 1));

    return 0;
}

static int
system_show(lua_State* L)
{
    uint32_t    i;
    FILE*       out;

    // Retrieve the output file pointer
    out = luash_out(L);

    machine_show(out);
    function_show(out);
    for (i = 0; i < AXIS_MAX; i++)
        axis_show(out, i);
    for (i = 0; i < SCALE_MAX; i++)
        scale_show(out, i);
    return 0;
}

const LUA_REG_TYPE system_map_lua[] =
{
    { LSTRKEY("reset"), LFUNCVAL(system_reset) },
    { LSTRKEY("version"), LFUNCVAL(system_version) },
    { LSTRKEY("hardware"), LFUNCVAL(system_hardware) },
    { LSTRKEY("silent"), LFUNCVAL(system_silent) },
    { LSTRKEY("show"), LFUNCVAL(system_show) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
system_open_lua(lua_State* L)
{
    LREGISTER(L, "system", system_map_lua);
}
