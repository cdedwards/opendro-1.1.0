///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Display object
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
#include "types.h"
#include "display.h"
#include "display_map.h"
#include "display_dev.h"
#include "drolim.h"
#include "assert.h"
#include "timer.h"
#include "trace.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

/// Number of times to toggle display for blink
#define BLINK_COUNT         5

/// Blink period
#define BLINK_PERIOD        (SEC_TICKS / 4)

/// Line state
typedef enum line_state
{
    LINE_STATE_ON,
    LINE_STATE_OFF,
    LINE_STATE_BLINK_ON,
    LINE_STATE_BLINK_OFF,
} line_state_t;

#define FOREACH_DEV(dev_itr) \
    for(dev_itr = 0; dev_itr < ARRAY_SIZE(display_map.devs); dev_itr++)

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

/// Timer to blink the reading line
static timer_node_t blink_timer[DISPLAY_LINE_MAX];

/// Number of times remaining to blink line
static uint8_t blink_count[DISPLAY_LINE_MAX];

/// State of each line
static line_state_t line_state[DISPLAY_LINE_MAX];

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static void
display_blink_timeout(void* user)
{
    uint8_t line = (uintptr_t) user;
    const display_dev_t *dev_ptr;

    ASSERT(line < DISPLAY_LINE_MAX);

    dev_ptr = display_map.reading[line];

    blink_count[line]--;
    if (line_state[line] == LINE_STATE_BLINK_OFF)
    {
        line_state[line] = LINE_STATE_BLINK_ON;
        dev_ptr->reading_on(display_map.line[line]);
    }
    else
    {
        line_state[line] = LINE_STATE_BLINK_OFF;
        dev_ptr->reading_off(display_map.line[line]);
    }

    if (blink_count[line] == 0)
    {
        line_state[line] = LINE_STATE_ON;
        timer_stop(&blink_timer[line]);
    }
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
display_reading_set(uint8_t   line,
                    double    number,
                    uint8_t   precision,
                    bool      mark)
{
    const display_dev_t *dev_ptr;

    ASSERT(line < DISPLAY_LINE_MAX);

    dev_ptr = display_map.reading[line];
    dev_ptr->reading_set(display_map.line[line], number, precision, mark);
}

const char*
display_reading_get(uint8_t line)
{
    const display_dev_t *dev_ptr;

    ASSERT(line < DISPLAY_LINE_MAX);

    dev_ptr = display_map.reading[line];
    return dev_ptr->reading_get(display_map.line[line]);
}

void
display_reading_on(uint8_t line)
{
    const display_dev_t *dev_ptr;

    ASSERT(line < DISPLAY_LINE_MAX);

    if (line_state[line] == LINE_STATE_OFF)
    {
        dev_ptr = display_map.reading[line];
        dev_ptr->reading_on(display_map.line[line]);
        line_state[line] = LINE_STATE_ON;
    }
}

void
display_reading_off(uint8_t line)
{
    const display_dev_t *dev_ptr;

    ASSERT(line < DISPLAY_LINE_MAX);

    if (line_state[line] == LINE_STATE_ON)
    {
        dev_ptr = display_map.reading[line];
        dev_ptr->reading_off(display_map.line[line]);
        line_state[line] = LINE_STATE_OFF;
    }
}

void
display_reading_blink(uint8_t line)
{
    ASSERT(line < DISPLAY_LINE_MAX);

    if (line_state[line] == LINE_STATE_ON)
    {
        display_reading_off(line);
        line_state[line] = LINE_STATE_BLINK_OFF;
        blink_count[line] = BLINK_COUNT;
        timer_start(&blink_timer[line], BLINK_PERIOD, TIMER_RECURRING,
                    display_blink_timeout, (void*) (uintptr_t) line);
    }

}

void
display_label_set(uint8_t       line,
                  const char*   format,
                  ...)
{
    va_list args;
    const display_dev_t *dev_ptr;

    ASSERT(line < DISPLAY_LINE_MAX);

    dev_ptr = display_map.reading[line];
    va_start(args, format);
    dev_ptr->label_set(display_map.line[line], format, args);
    va_end(args);
}

void
display_menu_printf(menu_t       menu,
                    const char*  format,
                    ...)
{
    va_list args;

    ASSERT(menu < MENU_MAX);

    va_start(args, format);
    display_map.menu->menu_printf(menu, format, args);
    va_end(args);
}

void
display_menu_puts(menu_t       menu,
                  const char*  string)
{
    ASSERT(menu < MENU_MAX);

    display_map.menu->menu_puts(menu, string);
}

const char*
display_menu_get(menu_t menu)
{
    const display_dev_t *dev_ptr;

    ASSERT(menu < MENU_MAX);

    dev_ptr = display_map.reading[menu];
    return dev_ptr->menu_get(menu);
}

void
display_menu_clear(void)
{
    display_map.menu->menu_clear();
}

bool
display_menu_state(void)
{
    return display_map.menu->menu_state();
}

void
display_status_set(status_t status,
                   ...)
{
    va_list args;

    ASSERT(status < STATUS_MAX);

    va_start(args, status);
    display_map.status->status_set(status, args);
    va_end(args);
}

void
display_indicator_set(indicator_t   indicator,
                      bool          state)
{
    ASSERT(indicator < INDICATOR_MAX);

    display_map.indicator->indicator_set(indicator, state);
}

bool
display_indicator_get(indicator_t   indicator)
{
    ASSERT(indicator < INDICATOR_MAX);

    return display_map.indicator->indicator_get(indicator);
}

void
display_on(void)
{
    uint8_t dev_itr;

    FOREACH_DEV(dev_itr)
    {
        display_map.devs[dev_itr]->on();
    }
}

void
display_off(void)
{
    uint8_t dev_itr;

    FOREACH_DEV(dev_itr)
    {
        display_map.devs[dev_itr]->off();
    }
}

void
display_save(void)
{
    uint8_t dev_itr;

    FOREACH_DEV(dev_itr)
    {
        display_map.devs[dev_itr]->save();
    }
}

void
display_setup(void)
{
    uint8_t dev_itr;

    FOREACH_DEV(dev_itr)
    {
        display_map.devs[dev_itr]->setup();
    }
}

void
display_init(void)
{
    uint8_t dev_itr;

    display_map_init();

    FOREACH_DEV(dev_itr)
    {
        display_map.devs[dev_itr]->init();
    }
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

#include <string.h>

static int
display_readset_lua(lua_State* L)
{
    uint32_t line;
    double number;
    uint8_t precision;
    bool mark;
    
    line = luaL_checkinteger(L, 1) - 1;
    if (line >= DISPLAY_LINE_MAX)
        return luaL_error(L, "invalid display line");
    number = luaL_checknumber(L, 2);
    precision = luaL_checkinteger(L, 3);
    mark = lua_toboolean(L, 4);

   display_reading_set(line, number, precision, mark);
 
    return 0;
}

static int
display_readget_lua(lua_State* L)
{
    uint32_t line;
    
    line = luaL_checkinteger(L, 1) - 1;
    if (line >= DISPLAY_LINE_MAX)
        return luaL_error(L, "invalid display line");

    lua_pushstring(L, display_reading_get(line));
 
    return 1;
}

static int
display_readblk_lua(lua_State* L)
{
    uint32_t line;
    
    line = luaL_checkinteger(L, 1) - 1;
    if (line >= DISPLAY_LINE_MAX)
        return luaL_error(L, "invalid display line");

    display_reading_blink(line);
 
    return 0;
}

static int
display_ind_lua(lua_State* L)
{
    int table;

    lua_createtable(L, 5, 0);
    table = lua_gettop(L);

    lua_pushboolean(L, display_indicator_get(INDICATOR_INCR));
    lua_rawseti(L, table, 1);
    lua_pushboolean(L, display_indicator_get(INDICATOR_MM));
    lua_rawseti(L, table, 2);
    lua_pushboolean(L, display_indicator_get(INDICATOR_ZERO));
    lua_rawseti(L, table, 3);
    lua_pushboolean(L, display_indicator_get(INDICATOR_SET));
    lua_rawseti(L, table, 4);
    lua_pushboolean(L, display_indicator_get(INDICATOR_FUNC));
    lua_rawseti(L, table, 5);

    return 1;
}

static int
display_on_lua(lua_State* L)
{
    display_on();
    return 0;
}
 
static int
display_off_lua(lua_State* L)
{
    display_off();
    return 0;
}

static int
display_menuset_lua(lua_State* L)
{
    menu_t  menu;
    const char* string;
    
    menu = luaL_checkinteger(L, 1) - 1;
    if (menu >= MENU_MAX)
        return luaL_error(L, "invalid menu position");

    string = luaL_checkstring(L, 2);

    display_menu_puts(menu, string);
    
    return 0;
}

static int
display_menuget_lua(lua_State* L)
{
    menu_t  menu;
    
    menu = luaL_checkinteger(L, 1) - 1;
    if (menu >= MENU_MAX)
        return luaL_error(L, "invalid menu position");

    lua_pushstring(L, display_menu_get(menu));

    return 1;
}

static int
display_menuclr_lua(lua_State* L)
{
    display_menu_clear();
    return 0;
}

static int
display_menustate_lua(lua_State* L)
{
    lua_pushboolean(L, display_menu_state());
    return 1;
}

static int
display_print_lua(lua_State* L)
{
    const char* string;
    char line[LED_COLUMN_MAX + 1];
    
    string = luaL_checkstring(L, 1);
    
    strlcpy(line, string, sizeof(line));
    display_menu_puts(MENU_TOP, line);
    string += strlen(line);
    
    strlcpy(line, string, sizeof(line));
    display_menu_puts(MENU_MIDDLE, line);
    string += strlen(line);

    strlcpy(line, string, sizeof(line));
    display_menu_puts(MENU_BOTTOM, line);

    return 0;
}

const LUA_REG_TYPE display_map_lua[] =
{
    { LSTRKEY("readset"), LFUNCVAL(display_readset_lua) },
    { LSTRKEY("readget"), LFUNCVAL(display_readget_lua) },
    { LSTRKEY("readblk"), LFUNCVAL(display_readblk_lua) },
    { LSTRKEY("ind"), LFUNCVAL(display_ind_lua) },
    { LSTRKEY("off"), LFUNCVAL(display_off_lua) },
    { LSTRKEY("on"), LFUNCVAL(display_on_lua) },
    { LSTRKEY("menuset"), LFUNCVAL(display_menuset_lua) },
    { LSTRKEY("menuget"), LFUNCVAL(display_menuget_lua) },
    { LSTRKEY("menuclr"), LFUNCVAL(display_menuclr_lua) },
    { LSTRKEY("menustate"), LFUNCVAL(display_menustate_lua) },
    { LSTRKEY("print"), LFUNCVAL(display_print_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
display_open_lua(lua_State* L)
{
    LREGISTER(L, "display", display_map_lua);
}
