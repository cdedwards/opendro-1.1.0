///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Newlib libc SWI interface
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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "legc.h"

#include "luash.h"
#include "types.h"
#include "trace.h"
#include "irq.h"
#include "mem.h"

#define LINE_MAX        128
#define HISTORY_MAX     10

#define START_OF_LINE   0x1
#define CURSOR_LEFT     0x2
#define EOT             0x4
#define END_OF_LINE     0x5
#define CURSOR_RIGHT    0x6
#define BELL            0x7
#define BACKSPACE       0x8
#define HORIZONTAL_TAB  0x9
#define DELETE_TO_END   0xb
#define CURSOR_DOWN     0xe
#define CURSOR_UP       0x10
#define DELETE_LINE     0x15
#define ESCAPE_CODE     0x1b
#define DELETE          0x7f
#define CSI             "\x1b["

#define LUASH_EXEC_FILE ":eefile"

typedef struct luash
{
    char        line[LINE_MAX];
    bool        incomplete;
    bool        busy;
    bool        automate;
    bool        refresh;
    bool        clear;
    lua_State*  state;
    FILE*       in;
    FILE*       out;
    char*       history[HISTORY_MAX];
    uint32_t    eoh;
    uint32_t    eol;
    uint32_t    cursor;
    uint32_t    escape;
    int32_t     hpos;
} luash_t;

static luash_t lsh;

static void
luash_eval(void)
{
    int status;
    lua_State* L = lsh.state;

    // Clear the stack if this is a new chunk
    if (!lsh.incomplete)
        lua_settop(L, 0);

    // Push the line onto the stack
    lua_pushlstring(L, lsh.line, lsh.eol);
    if (lsh.incomplete)
    {
        lua_pushliteral(L, "\n");
        lua_insert(L, -2);
        lua_concat(L, 3);
    }
    
    // Try to load the buffer
    status = luaL_loadbuffer(L, lua_tostring(L, -1), lua_strlen(L, -1), "=luash");
    
    // Check for an incomplete line
    if (status == LUA_ERRSYNTAX)
    {
        size_t lmsg;
        const char *msg = lua_tolstring(L, -1, &lmsg);
        lmsg -= sizeof(LUA_QL("<eof>")) - 1;
        if (strcmp(msg + lmsg, LUA_QL("<eof>")) == 0)
        {
            lua_pop(L, 1);
            lsh.incomplete = true;
            return;
        }
    }

    if (status == 0)
    {
        status = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (status != 0)
        {
            fprintf(lsh.out, "%s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    else
    {
        fprintf(lsh.out, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    // End of chunk reached
    lsh.incomplete = false;
}

static const char*
luash_completion_next(uint32_t partial)
{
    const char* key;
    lua_State* L = lsh.state;

    while (lua_next(L, -2) != 0)
    {
        lua_pushvalue(L, -2);
        key = lua_tostring(L, -1);
        lua_pop(L, 2);
        if (key[0] != '_' && strncmp(&lsh.line[partial], key, lsh.eol - partial) == 0)
            return key;
    }
    
    // Search the read-only table
    lua_getfield(L, -1, "_R");
    if (lua_isrotable(L, -1))
    {
        lua_pushnil(L);
        return luash_completion_next(partial);
    }
    
    return NULL;
}

static uint32_t
luash_prev_sep(uint32_t pos)
{
    int32_t c;
    
    while (pos > 0)
    {
        c = lsh.line[pos - 1];
        if (!isalnum(c) && c != '_')
            return pos;
        pos--;
    }
    return pos;
}

static void
luash_completion(void)
{
    int top;
    const char* match;
    lua_State* L = lsh.state;
    size_t len;
    uint32_t partial;
    uint32_t table;

    // Back up to the previous seperator
    partial = luash_prev_sep(lsh.eol);
    
    // Save the stack
    top = lua_gettop(L);
    
    // If the separator is a dot, then check if the
    // name before the dot is a table otherwise the
    // search is done on the global environment.
    if (partial > 0 && lsh.line[partial - 1] == '.')
    {
        table = luash_prev_sep(partial - 1);
        lsh.line[partial - 1] = '\0';
        lua_getglobal(L, &lsh.line[table]);
        lsh.line[partial - 1] = '.';
        if (!lua_istable(L, -1) && !lua_isrotable(L, -1))
        {
            lua_pop(L, 1);
            lua_getglobal(L, "_G");
        }
    }
    else
    {
        lua_getglobal(L, "_G");
    }
    
    // Find the first matching table key
    lua_pushnil(L);
    match = luash_completion_next(partial);
    
    // Find next key else beep
    if (match)
    {
        char first[LINE_MAX];
        
        // Save the first table key
        strlcpy(first, match, sizeof(first));
        
        // Look for the next matching table key
        match = luash_completion_next(partial);
        
        // If found, then beep and print out all remaining matches.
        // Else, insert the first table key into the line.
        if (match)
        {
            fprintf(lsh.out, "%c\n%s %s ", BELL, first, match);
            while ((match = luash_completion_next(partial)))
                fprintf(lsh.out, "%s ", match);
            fputc('\n', lsh.out);
            fflush(lsh.out);
        }
        else
        {
            lsh.line[partial] = '\0';
            len = strlcat(lsh.line, first, sizeof(lsh.line));
            lsh.eol = MIN(len, sizeof(lsh.line));
            lsh.cursor = lsh.eol;
        }
    }
    else
    {
        fputc(BELL, lsh.out);
    }

    // Restore the stack
    lua_settop(L, top);
}

static void
luash_prompt(const char* prompt)
{
    size_t len;

    // Move cursor to the left edge
    fprintf(lsh.out, CSI "G");

    // Display the prompt
    fprintf(lsh.out, "%s", prompt);

    // Display the line
    lsh.line[lsh.eol] = '\0';
    fputs(lsh.line, lsh.out);

    // Delete to end
    fprintf(lsh.out, CSI "K");

    // Move cursor
    if (lsh.cursor != lsh.eol)
    {
        len = strlen(prompt);
        fprintf(lsh.out, CSI "G" CSI "%"PRIu32"C", lsh.cursor + len);
    }
    
    // Flush the output
    fflush(lsh.out);
}

static void
luash_history_prev(void)
{
    int32_t hpos = lsh.hpos;

    // Decrement the history position
    hpos--;
    if (hpos < 0)
        hpos = HISTORY_MAX - 1;

    // Beep if at the end of history
    if (!lsh.history[hpos])
    {
        fputc(BELL, lsh.out);
        return;
    }

    // Copy the history into the line
    strlcpy(lsh.line, lsh.history[hpos], LINE_MAX);
    lsh.hpos = hpos;
    lsh.eol = lsh.cursor = strlen(lsh.line);
}

static void
luash_history_next(void)
{
    int32_t hpos = lsh.hpos;

    // Beep if at the end of history
    if (!lsh.history[hpos])
    {
        fputc(BELL, lsh.out);
        return;
    }
    
    // Increment the history position
    hpos++;
    if (hpos == HISTORY_MAX)
        hpos = 0;

    // Leave history so clear the line
    if (!lsh.history[hpos])
    {
        lsh.eol = lsh.cursor = 0;
        lsh.hpos = lsh.eoh;
        return;
    }

    // Copy the history into the line
    strlcpy(lsh.line, lsh.history[hpos], LINE_MAX);
    lsh.hpos = hpos;
    lsh.eol = lsh.cursor = strlen(lsh.line);
}

static void
luash_history_add(void)
{
    // Don't add empty lines
    if (lsh.eol == 0)
        return;
  
    // Don't add lines if automate mode
    if (lsh.automate)
       return;

    // Duplicate the line and save in history
    lsh.history[lsh.eoh++] = strdup(lsh.line);
    if (lsh.eoh == HISTORY_MAX)
        lsh.eoh = 0;

    // Delete an entry if history is full
    if (lsh.history[lsh.eoh])
    {
        free(lsh.history[lsh.eoh]);
        lsh.history[lsh.eoh] = NULL;
    }
    
    // Reset history position at end
    lsh.hpos = lsh.eoh;
}

static void
luash_escape(int32_t c)
{
    lsh.refresh = true;
    
    if (lsh.escape == 1)
    {
        if (c == '[')
            lsh.escape = 2;
        else
            lsh.escape = 0;
        lsh.refresh = false;
    }
    else if (lsh.escape == 2)
    {
        lsh.escape = 0;
        switch (c)
        {
        // Up
        case 'A':
            luash_history_prev();
            break;
        // Down
        case 'B':
            luash_history_next();
            break;
        // Right
        case 'C':
            if (lsh.cursor < lsh.eol)
                lsh.cursor++;
            break;
        // Left
        case 'D':
            if (lsh.cursor > 0)
                lsh.cursor--;
            break;
        default:
            if (c >= '1' && c <= '9')
            {
                lsh.escape = 3;
                lsh.line[lsh.eol] = c;
            }
            lsh.refresh = false;
            break;
        }
    }
    else
    {
        lsh.escape = 0;
        if (c == '~')
        {
            switch (lsh.line[lsh.eol])
            {
            // Home
            case '1':
            case '7':
                lsh.cursor = 0;
                break;
            // Delete
            case '3':
                if (lsh.cursor < lsh.eol)
                {
                    memmove(&lsh.line[lsh.cursor],
                            &lsh.line[lsh.cursor + 1],
                            lsh.eol - lsh.cursor - 1);
                    lsh.eol--;
                }
                break;
            // End
            case '4':
            case '8':
                lsh.cursor = lsh.eol;
                break;
            default:
                lsh.refresh = false;
                break;
            }
        }
    }
}

static void
luash_control(int32_t c)
{
    lsh.refresh = true;
    
    switch (c)
    {
    case START_OF_LINE:
        lsh.cursor = 0;
        break;
    case CURSOR_LEFT:
        if (lsh.cursor > 0)
            lsh.cursor--;
        break;
    case END_OF_LINE:
        lsh.cursor = lsh.eol;
        break;
    case CURSOR_RIGHT:
        if (lsh.cursor < lsh.eol)
            lsh.cursor++;
        break;
    case BACKSPACE:
    case DELETE:
        if (lsh.cursor > 0)
        {
            memmove(&lsh.line[lsh.cursor - 1],
                    &lsh.line[lsh.cursor],
                    lsh.eol - lsh.cursor);
            lsh.eol--;
            lsh.cursor--;
        }
        break;
    case HORIZONTAL_TAB:
        luash_completion();
        break;
    case DELETE_TO_END:
        lsh.eol = lsh.cursor;
        break;
    case CURSOR_DOWN:
        luash_history_next();
        break;
    case CURSOR_UP:
        luash_history_prev();
        break;
    case DELETE_LINE:
        lsh.eol = lsh.cursor = 0;
        break;
    case ESCAPE_CODE:
        lsh.escape = 1;
        lsh.refresh = false;
        break;
    default:
        lsh.refresh = false;
        break;
    }
}

static int
luash_rawline(void)
{
    int c;

    while ((c = fgetc(lsh.in)) != EOF)
    {
        if (c == '\n')
        {
            lsh.line[lsh.eol] = '\0';
            lsh.clear = true;
            return lsh.eol;
        }

        if (c == EOT && lsh.eol == 0)
        {
            return -2;
        }

        if (isprint(c))
        {
            if (lsh.eol < LINE_MAX - 1)
            {
                lsh.line[lsh.eol++] = c;
            }
        }
    }

    // Clear the EOF error
    clearerr(lsh.in);

    return -1;
}

static int
luash_readline(const char* prompt)
{
    int32_t c;

    // Clear the line
    if (lsh.clear)
    {
        lsh.eol = lsh.cursor = 0;
        lsh.clear = false;
    }

    // Read raw line if automate mode
    if (lsh.automate)
        return luash_rawline();

    // Refresh the prompt
    if (lsh.refresh)
    {
        luash_prompt(prompt);
        lsh.refresh = false;
    }

    while ((c = fgetc(lsh.in)) != EOF)
    {
        // If currently escaped, continue processing
        if (lsh.escape)
        {
            luash_escape(c);
        }
        // Process a printable character
        else if (isprint(c))
        {
            if (lsh.eol < LINE_MAX - 1)
            {
                // Shift line if not at the end
                if (lsh.cursor < lsh.eol)
                {
                    memmove(&lsh.line[lsh.cursor + 1],
                            &lsh.line[lsh.cursor],
                            lsh.eol - lsh.cursor);
                    lsh.eol++;
                    lsh.refresh = true;
                }
                
                // Save character
                lsh.line[lsh.cursor++] = c;

                // Update end of line
                if (lsh.cursor > lsh.eol)
                    lsh.eol = lsh.cursor;
                    
                // Echo output character
                fputc(c, lsh.out);
            }
            else
            {
                lsh.refresh = true;
            }
        }
        // Return on line feed
        else if (c == '\n')
        {
            fputc('\n', lsh.out);
            lsh.line[lsh.eol] = '\0';
            lsh.refresh = true;
            lsh.clear = true;
            
            return lsh.eol;
        }
        // Return error on EOT
        else if (c == EOT && lsh.eol == 0)
        {
            fputc('\n', lsh.out);
            lsh.refresh = true;
            
            return -2;
        }
        // Else it is a control code
        else
        {
            luash_control(c);
        }
    }

    // Clear the EOF error
    clearerr(lsh.in);

    return -1;
}

static bool
luash_create(FILE* in, FILE* out)
{
    ptrdiff_t memlimit;
    
    lua_State* L = lua_open();
    if (L == NULL)
        return false;

    global_State *g = G(L);

    // Open the core Lua libs
    luaL_openlibs(L);

    // Configure the emergency garbage collector
    memlimit = g->totalbytes + memfree();
    legc_set_mode(L, EGC_ALWAYS | EGC_ON_MEM_LIMIT | EGC_ON_ALLOC_FAILURE, memlimit);

    // Store the out FILE pointer in the registry
    lua_pushlightuserdata(L, stdout); // key
    lua_pushlightuserdata(L, out);
    lua_settable(L, LUA_REGISTRYINDEX);
    
    lsh.state = L;
    lsh.in = in;
    lsh.out = out;
    lsh.incomplete = false;
    lsh.busy = false;
    lsh.automate = false;
    lsh.refresh = true;
    lsh.clear = true;

    return true;
}

bool
luash_dofile(const char* file)
{
    int status;

    if (lsh.busy)
        return false;

    lsh.busy = true;

    status = luaL_dofile(lsh.state, file);
    if (status != 0)
        fprintf(lsh.out, "%s\n", lua_tostring(lsh.state, -1));

    lsh.busy = false;

    return (status == 0);
}

bool
luash_dostring(const char* string)
{
    int status;

    if (lsh.busy)
        return false;

    lsh.busy = true;

    status = luaL_dostring(lsh.state, string);
    if (status != 0)
        fprintf(lsh.out, "%s\n", lua_tostring(lsh.state, -1));

    lsh.busy = false;

    return (status == 0);
}

void
luash_process(void)
{
    if (lsh.busy)
        return;

    lsh.busy = true;

    if (luash_readline(lsh.incomplete ? "OpenDRO>> " : "OpenDRO> ") > 0)
    {
        luash_history_add();
        luash_eval();
    }

    lsh.busy = false;
}

bool
luash_cat(const char* file)
{
    FILE* pipe;
    uint8_t buffer[128];
    int len;
    
    pipe = fopen(file, "r");
    if (!pipe)
        return false;
        
    while ((len = fread(buffer, 1, sizeof(buffer), pipe)) > 0)
    {
        fwrite(buffer, 1, len, lsh.out);
    }
    fputs("\n", lsh.out);
    
    fclose(pipe);
    
    return true;
}

bool
luash_pipe(const char* file)
{
    FILE* pipe;
    int len;
    
    pipe = fopen(file, "w");
    if (!pipe)
        return false;

    if (!lsh.automate)
        fputs("Press Ctrl-D to end\n", lsh.out);

    while ((len = luash_readline("> ")) != -2)
    {
        if (len >= 0)
        {
            fputs(lsh.line, pipe);
            fputs("\n", pipe);
        }
    }

    fclose(pipe);
    
    return true;
}

FILE*
luash_out(lua_State* L)
{
    // Retrieve the out FILE pointer from the registry
    lua_pushlightuserdata(L, stdout); // key
    lua_gettable(L, LUA_REGISTRYINDEX);
    return lua_touserdata(L, -1);
}

void
luash_init(void)
{
    if (!luash_create(stdin, stdout))
    {
        TRACE_ERROR("Unable to open Lua shell\n");
        return;
    }

    // Run the exec file
    luash_dofile(LUASH_EXEC_FILE);
}

///////////////////////////////////////////////////////////////////////////////
// L U A
///////////////////////////////////////////////////////////////////////////////

#include "lrotable.h"
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"

static int
luash_automate_lua(lua_State* L)
{
    lsh.automate = lua_toboolean(L, 1);
    return 0;
}

static int
luash_execrd_lua(lua_State* L)
{
    if (!luash_cat(LUASH_EXEC_FILE))
        return luaL_error(L, "unable to open");

    return 0;
}

static int
luash_execwr_lua(lua_State* L)
{
    if (!luash_pipe(LUASH_EXEC_FILE))
        return luaL_error(L, "unable to open");

    return 0;
}

static int
luash_history_lua(lua_State* L)
{
    int hpos = lsh.eoh;

    if (--hpos < 0)
        hpos = HISTORY_MAX - 1;

    while (lsh.history[hpos])
    {
        fprintf(luash_out(L), "%s\n", lsh.history[hpos]);
        if (--hpos < 0)
            hpos = HISTORY_MAX - 1;
    }

    return 0;
}

const LUA_REG_TYPE luash_map_lua[] =
{
    { LSTRKEY("automate"), LFUNCVAL(luash_automate_lua) },
    { LSTRKEY("execrd"), LFUNCVAL(luash_execrd_lua) },
    { LSTRKEY("execwr"), LFUNCVAL(luash_execwr_lua) },
    { LSTRKEY("history"), LFUNCVAL(luash_history_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
luash_open_lua(lua_State* L)
{
    LREGISTER(L, "luash", luash_map_lua);
}

