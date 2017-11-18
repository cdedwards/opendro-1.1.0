///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Debug trace
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
#include <stdio.h>
#include <stdarg.h>
#include "config.h"
#include "assert.h"
#include "trace.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

/// System configuration variable IDs
typedef enum trace_vid
{
    VID_TRACE_LEVEL,          ///< Trace level
} trace_vid_t;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

static trace_level_t min_level = TRACE_LEVEL_INFO;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
trace_level(trace_level_t level)
{
    min_level = level;
    trace_save();
}

void
trace_printf(trace_level_t   level,
             const char*     format,
             ...)
{
    va_list args;

    ASSERT(level <= TRACE_LEVEL_FATAL);

    if (level >= min_level)
    {
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

void
trace_save(void)
{
    if (config_obj_write(CONFIG_OID_TRACE, 0))
    {
        config_var_write(VID_TRACE_LEVEL, 0, sizeof(min_level), &min_level);
        config_obj_close();
    }
}

void
trace_init(void)
{
    min_level = TRACE_LEVEL_INFO;
    if (config_obj_read(CONFIG_OID_TRACE, 0))
    {
        config_var_read(VID_TRACE_LEVEL, 0, sizeof(min_level), &min_level);
        config_obj_close();
    }

    freopen(":err", "w", stderr);
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

static int
trace_debug_lua(lua_State* L)
{
    trace_level(TRACE_LEVEL_DEBUG);
    return 0;
}

static int
trace_info_lua(lua_State* L)
{
    trace_level(TRACE_LEVEL_INFO);
    return 0;
}

static int
trace_warning_lua(lua_State* L)
{
    trace_level(TRACE_LEVEL_WARNING);
    return 0;
}

static int
trace_error_lua(lua_State* L)
{
    trace_level(TRACE_LEVEL_ERROR);
    return 0;
}

static int
trace_fatal_lua(lua_State* L)
{
    trace_level(TRACE_LEVEL_FATAL);
    return 0;
}

static int
trace_level_lua(lua_State* L)
{
    static const char* level_name[TRACE_LEVEL_MAX] =
    {
        [TRACE_LEVEL_DEBUG] = "debug",
        [TRACE_LEVEL_INFO] = "info",
        [TRACE_LEVEL_WARNING] = "warning",
        [TRACE_LEVEL_ERROR] = "error",
        [TRACE_LEVEL_FATAL] = "fatal",
    };

    lua_pushstring(L, level_name[min_level]);

    return 1;
}

const LUA_REG_TYPE trace_map_lua[] =
{
    { LSTRKEY("debug"), LFUNCVAL(trace_debug_lua) },
    { LSTRKEY("info"), LFUNCVAL(trace_info_lua) },
    { LSTRKEY("warning"), LFUNCVAL(trace_warning_lua) },
    { LSTRKEY("error"), LFUNCVAL(trace_error_lua) },
    { LSTRKEY("fatal"), LFUNCVAL(trace_fatal_lua) },
    { LSTRKEY("level"), LFUNCVAL(trace_level_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
trace_open_lua(lua_State* L)
{
    LREGISTER(L, "trace", trace_map_lua);
}

