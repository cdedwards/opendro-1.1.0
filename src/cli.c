///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Command line interface
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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "drolim.h"
#include "types.h"
#include "board.h"
#include "cli.h"
#include "config.h"
#include "irq.h"
#include "trace.h"
#include "display.h"
#include "machine.h"
#include "function.h"
#include "axis.h"
#include "scale.h"
#include "keypad.h"
#include "usb_cdc.h"
#include "swi.h"
#if PLATFORM_dro550
#include "sd.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

#define METHODS_MAX             10
#define CLI_SESSIONS            2

typedef struct _cli_session_t
{
    FILE*       in;
    FILE*       out;
    char        buffer[80];
    uint32_t    pos;
    bool        echo;
} cli_session_t;

typedef void (*process_t)(cli_session_t* session_ptr, uint32_t argc, char* argv[]);

typedef struct _cli_method_t {
    const char*     name;
    process_t       process;
    uint32_t        args;
} cli_method_t;

typedef struct _cli_obj_t {
    const char*     name;
    cli_method_t    methods[METHODS_MAX];
} cli_obj_t;

/// Configuration variable IDs
typedef enum cli_vid
{
    VID_CLI_ECHO,             ///< CLI echo
} cli_vid_t;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

static cli_session_t    con_session;

static void cli_system_echo(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_system_trace(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_system_reset(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_system_version(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_system_show(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
#if PLATFORM_dro550
static void cli_system_load(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_system_save(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
#endif

static void cli_display_readset(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_readget(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_readblk(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_ind(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_off(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_on(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_menuset(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_menuget(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_menuclr(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_display_menustate(cli_session_t* session_ptr, uint32_t argc, char* argv[]);


static void cli_machine_units(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_machine_zero(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_machine_on(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_machine_off(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_machine_show(cli_session_t* session_ptr, uint32_t argc, char* argv[]);

static void cli_function_show(cli_session_t* session_ptr, uint32_t argc, char* argv[]);

static void cli_axis_read(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_axis_abs(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_axis_incr(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_axis_show(cli_session_t* session_ptr, uint32_t argc, char* argv[]);

static void cli_scale_count(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_scale_show(cli_session_t* session_ptr, uint32_t argc, char* argv[]);

static void cli_keypad_press(cli_session_t* session_ptr, uint32_t argc, char* argv[]);

#if PLATFORM_dro550
static void cli_sd_show(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_sd_ls(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_sd_cat(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
static void cli_sd_hd(cli_session_t* session_ptr, uint32_t argc, char* argv[]);
#endif

static cli_obj_t objects[] = {
    // System object
    {
        .name     = "system",
        .methods  = {
            { .name = "trace",      .process = cli_system_trace,        .args = 1 },
            { .name = "echo",       .process = cli_system_echo,         .args = 1 },
            { .name = "reset",      .process = cli_system_reset,        .args = 0 },
            { .name = "version",    .process = cli_system_version,      .args = 0 },
            { .name = "show",       .process = cli_system_show,         .args = 0 },
#if PLATFORM_dro550
            { .name = "load",       .process = cli_system_load,         .args = 1 },
            { .name = "save",       .process = cli_system_save,         .args = 1 },
#endif
        }
    },
    // Display object
    {
        .name     = "display",
        .methods  = {
            { .name = "readset",    .process = cli_display_readset,     .args = 4 },
            { .name = "readget",    .process = cli_display_readget,     .args = 1 },
            { .name = "readblk",    .process = cli_display_readblk,     .args = 1 },
            { .name = "ind",        .process = cli_display_ind,         .args = 0 },
            { .name = "off",        .process = cli_display_off,         .args = 0 },
            { .name = "on",         .process = cli_display_on,          .args = 0 },
            { .name = "menuset",    .process = cli_display_menuset,     .args = 2 },
            { .name = "menuget",    .process = cli_display_menuget,     .args = 1 },
            { .name = "menuclr",    .process = cli_display_menuclr,     .args = 0 },
            { .name = "menustate",  .process = cli_display_menustate,   .args = 0 },
        }
    },
    // Machine object
    {
        .name     = "machine",
        .methods  = {
            { .name = "units",      .process = cli_machine_units,       .args = 1 },
            { .name = "zero",       .process = cli_machine_zero,        .args = 1 },
            { .name = "on",         .process = cli_machine_on,          .args = 0 },
            { .name = "off",        .process = cli_machine_off,         .args = 0 },
            { .name = "show",       .process = cli_machine_show,        .args = 0 },
        }
    },
    // Machine functions
    {
        .name     = "function",
        .methods  = {
            { .name = "show",       .process = cli_function_show,       .args = 0 },
        }
    },
    // Axis object
    {
        .name     = "axis",
        .methods  = {
            { .name = "read",       .process = cli_axis_read,           .args = 1 },
            { .name = "abs",        .process = cli_axis_abs,            .args = 2 },
            { .name = "incr",       .process = cli_axis_incr,           .args = 2 },
            { .name = "show",       .process = cli_axis_show,           .args = 1 },
        }
    },
    // Scale object
    {
        .name     = "scale",
        .methods  = {
            { .name = "count",      .process = cli_scale_count,         .args = 1 },
            { .name = "show",       .process = cli_scale_show,          .args = 1 },
        }
    },
    // Keypad object
    {
        .name     = "keypad",
        .methods  = {
            { .name = "press",      .process = cli_keypad_press,        .args = 1 },
        }
    },
#if PLATFORM_dro550
    // SD card
    {
        .name     = "sd",
        .methods  = {
            { .name = "show",       .process = cli_sd_show,             .args = 0 },
            { .name = "ls",         .process = cli_sd_ls,               .args = 1 },
            { .name = "cat",        .process = cli_sd_cat,              .args = 1 },
            { .name = "hd",         .process = cli_sd_hd,               .args = 1 },
        }
    },
#endif
};

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static void
cli_help(cli_session_t* session_ptr)
{
    uint32_t    obj_itr;
    uint32_t    method_itr;

    for (obj_itr = 0; obj_itr < ARRAY_SIZE(objects); obj_itr++) {
        for (method_itr = 0; method_itr < ARRAY_SIZE(objects[obj_itr].methods); method_itr++) {
            if (objects[obj_itr].methods[method_itr].name != NULL) {
                fprintf(session_ptr->out, "%s %s <%"PRIu32" args>\n", objects[obj_itr].name,
                        objects[obj_itr].methods[method_itr].name,
                        objects[obj_itr].methods[method_itr].args);
            }
        }
    }
}

static void
cli_system_echo(cli_session_t* session_ptr,
                uint32_t       argc,
                char*          argv[])
{
    if (strcasecmp(argv[2], "off") == 0) {
        session_ptr->echo = false;
        cli_save();
    } else if (strcasecmp(argv[2], "on") == 0) {
        session_ptr->echo = true;
        cli_save();
    } else {
        fprintf(session_ptr->out, "echo must be 'off' or 'on'\n");
    }
}

static void
cli_system_trace(cli_session_t*  session_ptr,
                 uint32_t        argc,
                 char*           argv[])
{
    trace_level_t level;

    if (strcasecmp(argv[2], "debug") == 0) {
        level = TRACE_LEVEL_DEBUG;
    } else if (strcasecmp(argv[2], "info") == 0) {
        level = TRACE_LEVEL_INFO;
    } else if (strcasecmp(argv[2], "warning") == 0) {
        level = TRACE_LEVEL_WARNING;
    } else if (strcasecmp(argv[2], "error") == 0) {
        level = TRACE_LEVEL_ERROR;
    } else if (strcasecmp(argv[2], "fatal") == 0) {
        level = TRACE_LEVEL_FATAL;
    } else {
        fprintf(session_ptr->out, "level must be 'debug', 'info', 'warning', 'error', or 'fatal'\n");
        return;
    }

    trace_level(level);
}

static void
cli_system_reset(cli_session_t* session_ptr,
                  uint32_t       argc,
                  char*          argv[])
{
    reset();
}

static void
cli_system_version(cli_session_t* session_ptr,
                   uint32_t       argc,
                   char*          argv[])
{
    fprintf(session_ptr->out, "OpenDRO %s %s\n", OPENDRO_HARDWARE, OPENDRO_RELEASE);

}

static void
cli_system_show(cli_session_t* session_ptr,
                uint32_t       argc,
                char*          argv[])
{
    uint8_t     machine;
    uint32_t    i;

    machine = machine_get_active();
    machine_show(session_ptr->out, machine);
    function_show(session_ptr->out, machine);
    for (i = 0; i < AXIS_MAX; i++)
        axis_show(session_ptr->out, machine, i);
    for (i = 0; i < SCALE_MAX; i++)
        scale_show(session_ptr->out, i);
}

#if PLATFORM_dro550
void
cli_system_load(cli_session_t* session_ptr,
                uint32_t       argc,
                char*          argv[])
{
    FILE* file;
    
    file = fopen(argv[2], "r");
    if (!file)
    {
        fprintf(session_ptr->out, "unable to open file\n");
        return;
    }
    
    config_load(file);

    fclose(file);
    
    // Reset the system
    reset();
}

void
cli_system_save(cli_session_t* session_ptr,
                uint32_t       argc,
                char*          argv[])
{
    FILE* file;
    
    file = fopen(argv[2], "w");
    if (!file)
    {
        fprintf(session_ptr->out, "unable to open file\n");
        return;
    }
    
    config_save(file);
    
    fclose(file);
}
#endif

static void
cli_display_readset(cli_session_t* session_ptr,
                    uint32_t       argc,
                    char*          argv[])
{
    uint32_t    line;
    double      number;
    uint8_t     precision;
    bool        mark;

    line = atoi(argv[2]);
    number = atof(argv[3]);
    precision = atoi(argv[4]);
    mark = atoi(argv[5]);

    if (line >= DISPLAY_LINE_MAX) {
        fprintf(session_ptr->out, "invalid display line\n");
        return;
    }

    display_reading_set(line, number, precision, mark);
}

static void
cli_display_readget(cli_session_t* session_ptr,
                    uint32_t       argc,
                    char*          argv[])
{
    uint32_t line;

    line = atoi(argv[2]);
    if (line >= DISPLAY_LINE_MAX) {
        fprintf(session_ptr->out, "invalid display line\n");
        return;
    }
    fprintf(session_ptr->out, "%s\n", display_reading_get(line));
}

static void
cli_display_readblk(cli_session_t* session_ptr,
                    uint32_t       argc,
                    char*          argv[])
{
    uint32_t    line;

    line = atoi(argv[2]);

    if (line >= DISPLAY_LINE_MAX) {
        fprintf(session_ptr->out, "invalid display line\n");
        return;
    }

    display_reading_blink(line);
}

static void
cli_display_ind(cli_session_t*  session_ptr,
                uint32_t        argc,
                char*           argv[])
{
    fprintf(session_ptr->out, "%c %c %c %c %c\n",
        display_indicator_get(INDICATOR_INCR) ? '1' : '0',
        display_indicator_get(INDICATOR_MM)   ? '1' : '0',
        display_indicator_get(INDICATOR_ZERO) ? '1' : '0',
        display_indicator_get(INDICATOR_SET)  ? '1' : '0',
        display_indicator_get(INDICATOR_FUNC) ? '1' : '0');
}

static void
cli_display_off(cli_session_t*  session_ptr,
                uint32_t        argc,
                char*           argv[])
{
    display_off();
}

static void
cli_display_on(cli_session_t*   session_ptr,
               uint32_t         argc,
               char*            argv[])
{
    display_on();
}

static void
cli_display_menuset(cli_session_t*   session_ptr,
                    uint32_t         argc,
                    char*            argv[])
{
    menu_t menu;

    menu = atoi(argv[2]);
    if (menu >= MENU_MAX) {
        fprintf(session_ptr->out, "invalid menu\n");
        return;
    }

    display_menu_puts(menu, argv[3]);
}

static void
cli_display_menuget(cli_session_t*   session_ptr,
                    uint32_t         argc,
                    char*            argv[])
{
    menu_t menu;

    menu = atoi(argv[2]);
    if (menu >= MENU_MAX) {
        fprintf(session_ptr->out, "invalid menu\n");
        return;
    }

    fprintf(session_ptr->out, "%s\n", display_menu_get(menu));
}

static void
cli_display_menuclr(cli_session_t*    session_ptr,
                    uint32_t          argc,
                    char*             argv[])
{
    display_menu_clear();
}

static void
cli_display_menustate(cli_session_t*    session_ptr,
                      uint32_t          argc,
                      char*             argv[])
{
    if (display_menu_state())
        fprintf(session_ptr->out, "on\n");
    else
        fprintf(session_ptr->out, "off\n");
}

static void
cli_machine_units(cli_session_t*    session_ptr,
                  uint32_t          argc,
                  char*             argv[])
{
    uint8_t machine;

    machine = machine_get_active();
    if (strcasecmp(argv[2], "mm") == 0) {
        machine_set_units(machine, AXIS_UNITS_MM);
    } else if (strcasecmp(argv[2], "inch") == 0) {
        machine_set_units(machine, AXIS_UNITS_INCH);
    } else {
        fprintf(session_ptr->out, "units must be 'mm' or 'inch'\n");
    }
}

static void
cli_machine_zero(cli_session_t* session_ptr,
                 uint32_t       argc,
                 char*          argv[])
{
    uint8_t machine;

    machine = machine_get_active();
    if (strcasecmp(argv[2], "abs") == 0) {
        machine_set_zero(machine, AXIS_ZERO_ABS);
    } else if (strcasecmp(argv[2], "incr") == 0) {
        machine_set_zero(machine, AXIS_ZERO_INC);
    } else {
        fprintf(session_ptr->out, "zero must be 'abs' or 'incr'\n");
    }
}

static void
cli_machine_on(cli_session_t*    session_ptr,
                  uint32_t          argc,
                  char*             argv[])
{
    machine_on();
}

static void
cli_machine_off(cli_session_t* session_ptr,
                 uint32_t       argc,
                 char*          argv[])
{
    machine_off();
}

static void
cli_machine_show(cli_session_t* session_ptr,
                 uint32_t       argc,
                 char*          argv[])
{
    uint8_t machine;

    machine = machine_get_active();
    machine_show(session_ptr->out, machine);
}

static void
cli_function_show(cli_session_t* session_ptr,
                  uint32_t       argc,
                  char*          argv[])
{
    uint8_t machine;

    machine = machine_get_active();
    function_show(session_ptr->out, machine);
}

static void
cli_axis_read(cli_session_t*    session_ptr,
              uint32_t          argc,
              char*             argv[])
{
    double          reading;
    uint8_t         machine;
    uint32_t        axis;
    axis_units_t    units;
    axis_zero_t     zero;

    machine = machine_get_active();
    axis = machine_axis_by_label(machine, argv[2]);
    if (axis >= AXIS_MAX) {
        fprintf(session_ptr->out, "invalid axis number\n");
        return;
    }

    units = machine_get_units(machine);
    zero = machine_get_zero(machine);
    reading = axis_get_reading(machine, axis, units, zero);

    fprintf(session_ptr->out, "%f\n", reading);
}

static void
cli_axis_abs(cli_session_t* session_ptr,
             uint32_t       argc,
             char*          argv[])
{
    double          absolute;
    uint8_t         machine;
    uint32_t        axis;
    axis_units_t    units;

    machine = machine_get_active();
    axis = machine_axis_by_label(machine, argv[2]);
    if (axis >= AXIS_MAX) {
        fprintf(session_ptr->out, "invalid axis\n");
        return;
    }
    
    units = machine_get_units(machine);
    absolute = atof(argv[3]);

    axis_set_incremental(machine, axis, absolute, units);
}

static void
cli_axis_incr(cli_session_t*    session_ptr,
              uint32_t          argc,
              char*             argv[])
{
    double          absolute;
    uint8_t         machine;
    uint32_t        axis;
    axis_units_t    units;

    machine = machine_get_active();
    axis = machine_axis_by_label(machine, argv[2]);
    if (axis >= AXIS_MAX) {
        fprintf(session_ptr->out, "invalid axis number\n");
        return;
    }
    
    units = machine_get_units(machine);
    absolute = atof(argv[3]);

    absolute += axis_get_reading(machine,
                                 axis,
                                 units,
                                 AXIS_ZERO_ABS);
                            
    axis_set_incremental(machine, axis, absolute, units);
}

static void
cli_axis_show(cli_session_t*    session_ptr,
              uint32_t          argc,
              char*             argv[])
{
    uint8_t         machine;
    uint32_t        axis;

    machine = machine_get_active();
    axis = machine_axis_by_label(machine, argv[2]);
    if (axis >= AXIS_MAX) {
        fprintf(session_ptr->out, "invalid axis number\n");
        return;
    }

    axis_show(session_ptr->out, machine, axis);
}

static void
cli_scale_count(cli_session_t*  session_ptr,
                uint32_t        argc,
                char*           argv[])
{
    uint32_t        scale;

    scale = atoi(argv[2]);
    if (scale >= SCALE_MAX) {
        fprintf(session_ptr->out, "invalid scale number\n");
        return;
    }

    fprintf(session_ptr->out, "%"PRId32"\n", scale_get_count(scale));
}

static void
cli_scale_show(cli_session_t*  session_ptr,
               uint32_t        argc,
               char*           argv[])
{
    uint32_t        scale;

    scale = atoi(argv[2]);
    if (scale >= SCALE_MAX) {
        fprintf(session_ptr->out, "invalid scale number\n");
        return;
    }

    scale_show(session_ptr->out, scale);
}

static void
cli_keypad_press(cli_session_t* session_ptr,
                 uint32_t       argc,
                 char*          argv[])
{
    keypad_t key;

    for (key = 0; key < KEYPAD_MAX; key++)
        if (strcasecmp(argv[2], keypad_to_string(key)) == 0)
            break;

    if (key == KEYPAD_MAX) {
        fprintf(session_ptr->out, "invalid keypad key\n");
        return;
    }

    keypad_add_press(key);
}

#if PLATFORM_dro550
static void
cli_sd_show(cli_session_t*  session_ptr,
            uint32_t        argc,
            char*           argv[])
{
    sd_show(session_ptr->out);
}

static void
cli_sd_ls(cli_session_t*  session_ptr,
          uint32_t        argc,
          char*           argv[])
{
    DIR* dir;
    struct dirent* dent;
    
    dir = swi_opendir(argv[2]);
    if (!dir)
    {
        fprintf(session_ptr->out, "unable to open directory\n");
        return;
    }
    
    while ((dent = swi_readdir(dir)))
    {
        fprintf(session_ptr->out, "%c%c%c%c%c %10u %04d-%02d-%02d %02d:%02d %s\n",
                (dent->d_attrib & ATTRIB_DIR) ? 'd' : '-',
                (dent->d_attrib & ATTRIB_RO) ? 'r' : '-',
                (dent->d_attrib & ATTRIB_HID) ? 'h' : '-',
                (dent->d_attrib & ATTRIB_SYS) ? 's' : '-',
                (dent->d_attrib & ATTRIB_ARC) ? 'a' : '-',
                dent->d_size, dent->d_time.tm_year + 1900, dent->d_time.tm_mon,
                dent->d_time.tm_mday, dent->d_time.tm_hour, dent->d_time.tm_min,
                dent->d_name);
    }
}

static void
cli_sd_cat(cli_session_t*  session_ptr,
           uint32_t        argc,
           char*           argv[])
{
    FILE* file;
    size_t len;
    uint8_t buffer[512];
    
    file = fopen(argv[2], "r");
    if (!file)
    {
        fprintf(session_ptr->out, "unable to open file\n");
        return;
    }
    
    while ((len = fread(buffer, 1, sizeof(buffer), file)) > 0)
        fwrite(buffer, 1, len, session_ptr->out);
        
    fclose(file);
}

static void
cli_sd_hd(cli_session_t*  session_ptr,
           uint32_t        argc,
           char*           argv[])
{
    FILE* file;
    size_t len;
    uint8_t buffer[512];
    int32_t addr;
    int32_t offset;
    
    file = fopen(argv[2], "r");
    if (!file)
    {
        fprintf(session_ptr->out, "unable to open file\n");
        return;
    }
    
    addr = 0;
    while ((len = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        for (offset = 0; offset < len; offset++)
        {
            if (addr % 16 == 0)
                fprintf(session_ptr->out, "%08"PRIx32" ", addr);
            fprintf(session_ptr->out, "%02x%s", buffer[offset],
                    (addr % 16 == 15) ? "\n" : " ");
            addr++;
        }
    }
    if (addr % 16 == 0)
        fprintf(session_ptr->out, "\n");
        
    fclose(file);
}

#endif

static uint32_t
cli_split(cli_session_t*    session_ptr,
          char*             argv[],
          uint32_t          size)
{
    uint32_t argc = 0;
    uint32_t pos;

    for (pos = 0; pos < session_ptr->pos && argc < size; pos++)
    {
        // Skip whitespace
        while (isspace(session_ptr->buffer[pos]) && pos < session_ptr->pos) {
            pos++;
        }

        if (pos < session_ptr->pos) {
            argv[argc++] = &session_ptr->buffer[pos];

            // Skip non-whitespace
            while (!isspace(session_ptr->buffer[pos]) && pos < session_ptr->pos) {
                pos++;
            }

            session_ptr->buffer[pos] = '\0';
        }
    }

    return argc;
}

static void
cli_process(cli_session_t* session_ptr)
{
    uint32_t    obj_itr;
    uint32_t    method_itr;
    uint32_t    argc;
    char        *argv[10];
    bool        found;
    int32_t     c;

    while ((c = fgetc(session_ptr->in)) != EOF)
    {
        if (session_ptr->echo && c != '\b') {
            fputc(c, session_ptr->out);
        }

        if (c == '\n')
        {
            argc = cli_split(session_ptr, argv, ARRAY_SIZE(argv));
            found = false;
            if (argc >= 2)
            {
                for (obj_itr = 0; obj_itr < ARRAY_SIZE(objects); obj_itr++)
                {
                    if (strcasecmp(argv[0], objects[obj_itr].name) == 0)
                    {
                        for (method_itr = 0; method_itr < ARRAY_SIZE(objects[obj_itr].methods); method_itr++)
                        {
                            if (strcasecmp(argv[1], objects[obj_itr].methods[method_itr].name) == 0)
                            {
                                found = true;
                                if (argc != objects[obj_itr].methods[method_itr].args + 2)
                                    fprintf(session_ptr->out, "!ERROR: wrong number of arguments\n");
                                else
                                    objects[obj_itr].methods[method_itr].process(session_ptr, argc, argv);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            else if (argc == 1 &&
                     (strcasecmp(argv[0], "help") == 0 ||
                      strcasecmp(argv[0], "?") == 0))
            {
                cli_help(session_ptr);
                found = true;
            }
            if (!found && argc >= 1)
                fprintf(session_ptr->out, "!ERROR: unknown command\n");
            session_ptr->pos = 0;
            if (session_ptr->echo)
                fprintf(session_ptr->out, "$ ");
        }
        else if (c == '\b')
        {
            if (session_ptr->pos > 0)
            {
                if (session_ptr->echo)
                    fprintf(session_ptr->out, "\b \b");
                session_ptr->pos--;
            }
        }
        else
        {
            if (session_ptr->pos < sizeof(session_ptr->buffer) - 1)
                session_ptr->buffer[session_ptr->pos++] = c;
        }
    }

    clearerr(session_ptr->in);
}

static void
cli_isr(void)
{
    cli_process(&con_session);
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
cli_rx(void)
{
    irq_trigger(IRQ_CLI);
}

void
cli_save(void)
{
    if (config_obj_write(CONFIG_OID_CLI, 0))
    {
        config_var_write(VID_CLI_ECHO, 0, sizeof(con_session.echo), &con_session.echo);
        config_obj_close();
    }
}

void
cli_init(void)
{
    con_session.in = stdin;
    con_session.out = stdout;
    con_session.echo = true;

    if (config_obj_read(CONFIG_OID_CLI, 0))
    {
        config_var_read(VID_CLI_ECHO, 0, sizeof(con_session.echo), &con_session.echo);
        config_obj_close();
    }

    irq_register(IRQ_CLI, IRQ_PRIORITY_LOW, IRQ_TYPE_EDGE, cli_isr);
}

