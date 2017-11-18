// OpenDRO Project
//
// DRO functions
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
#include <math.h>
#include <string.h>
#include "types.h"
#include "function.h"
#include "keypad.h"
#include "scale.h"
#include "axis.h"
#include "machine.h"
#include "display.h"
#include "config.h"
#include "timer.h"
#include "assert.h"
#include "trace.h"
#include "luash.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

#define GROUP_MAX       5                   ///< Number of function groups
#define COMMAND_MAX     10                  ///< Number of commands per group
#define SPEEDKEY_MAX    10                  ///< Number of speedkeys

#define CALC_SIZE       4                   ///< RPN calculator stack size
#if CALC_SIZE < MENU_MAX
#error "Calculator stack size cannot be smaller than the menu size."
#endif
#define CALC_X          (CALC_SIZE - 1)     ///< RPN calculator stack X position
#define CALC_Y          (CALC_SIZE - 2)     ///< RPN calculator stack Y position
#define CALC_Z          (CALC_SIZE - 3)     ///< RPN calculator stack Z position
#define MEMORIES_MAX    10                  ///< RPN calculator number of memories

#define TOOL_NONE       INT8_MAX            ///< Special value for no tool offset

#define TO_RADIANS(angle)   ((angle) * PI / 180.0)

#define EDGE_DISPLAY_TIME   1500000         ///< How long to display the edge screen

/// Process function
typedef void (*process_t)(void);

/// Function command definition
typedef struct command
{
    char        *name;                      ///< Command name string
    process_t   process;                    /// Function pointer to command processor
} command_t;

/// Function group definition
typedef struct group
{
    char        *name;                      ///< Group name string
    command_t   command[COMMAND_MAX];       ///< Array of commands for the group
} group_t;

/// Tool definitions
typedef struct tool
{
    axis_units_t    units;                  ///< Units dimensions are defined in
    double          offset1;                ///< First dimension
    double          offset2;                ///< Second dimension
} tool_t;

/// Bolthole pattern parameters
typedef struct bolthole
{
    uint8_t         holes;                  ///< Number of holes
    axis_units_t    units;                  ///< Units the radius is defined in
    double          radius;                 ///< Radius of the circle
    double          start;                  ///< Starting position in degrees
    double          end;                    ///< Ending position in degrees
} bolthole_t;

/// Grid pattern parameters
typedef struct grid
{
    uint8_t         xholes;                 ///< Number of holes in the X dimension
    uint8_t         yholes;                 ///< Number of holes in the Y dimension
    axis_units_t    units;                  ///< Units the dimensions are defined in
    double          xdim;                   ///< X dimension of the grid
    double          ydim;                   ///< Y dimension of the grid
    double          angle;                  ///< Angle of the array in degrees
} grid_t;

/// Radius shape
typedef enum radius_shape
{
    RADIUS_SHAPE_CONVEX,                    ///< Convex radius shape
    RADIUS_SHAPE_CONCAVE,                   ///< Concave radius shape
    RADIUS_SHAPE_MAX,                       ///< MUST be last!!!
} radius_shape_t;

typedef struct radius
{
    uint8_t         axis;                   ///< Axis of radius
    uint8_t         steps;                  ///< Number of steps in the radius
    radius_shape_t  shape;                  ///< Radius shape
    axis_units_t    units;                  ///< Units the radius is defined in
    double          start;                  ///< Starting angle in degrees
    double          end;                    ///< Ending angle in degrees
    double          radius;                 ///< Radius of the circle
} radius_t;

typedef struct function
{
    int8_t          step;                   ///< Step count through the operation
    uint8_t         tool_num;               ///< Current tool number
    uint8_t         diameter_axis;          ///< Axis diameter is applied to
    uint8_t         bolthole_num;           ///< Current bolthole pattern number
    uint8_t         grid_num;               ///< Current grid pattern number
    uint8_t         radius_num;             ///< Current radius number
    int8_t          calc_op_num;            ///< current operation number
    uint8_t         workspace;              ///< Zero workspace
    bool            diameter;               ///< Diameter mode
    
    func_state_t    state;                  ///< State for pending operations
    edge_t          edge;                   ///< Edge compensation
    tool_t          tool[TOOL_MAX];         ///< Array of tool definitions
    bolthole_t      bolthole[BOLTHOLE_MAX]; ///< Array of bolthole pattern parameters
    grid_t          grid[GRID_MAX];         ///< Array of grid pattern parameters
    radius_t        radius[RADIUS_MAX];     ///< Array of radius parameters
    double          compound;               ///< Compound angle
    double          x_zero;                 ///< Saved X axis incremental position
    double          y_zero;                 ///< Saved Y axis incremental position
    double          z_zero;                 ///< Saved Z axis incremental position
    double          calc_stack[CALC_SIZE];  ///< RPN calculator stack
    double          calc_mem[MEMORIES_MAX]; ///< calculator memories
} function_t;

/// Configuration variable IDs 
typedef enum vid
{
    VID_EDGE,                               ///< Edge compensation
    VID_TOOL_NUM,                           ///< Current tool number
    VID_TOOL,                               ///< Tool definitions
    VID_COMPOUND,                           ///< Compound angle
    VID_DIAMETER,                           ///< Diameter mode
    VID_BOLTHOLE,                           ///< Bolthole pattern parameters
    VID_GRID,                               ///< Grid pattern parameters
    VID_WORKSPACE,                          ///< Zero workspace
    VID_MEMORY,                             ///< RPN memory
    VID_RADIUS,                             ///< Radius parameters
    VID_DIAMETER_AXIS,                      ///< Axis diameter is applied to
    VID_MAX                                 ///< MUST be last!!!
} vid_t;


/// Calculator operator functions
typedef enum calc_func
{
    CALC_MIN,                               ///< Memory in
    CALC_MOUT,                              ///< Memory out
    CALC_SIN,                               ///< Sin
    CALC_COS,                               ///< Cos
    CALC_TAN,                               ///< Tan
    CALC_SQRT,                              ///< Square root
    CALC_POW,                               ///< x ^ y
    CALC_PRESET,                            ///< Incremental preset axis 
    CALC_AXIS,                              ///< Load axis incremental pos into calc
    CALC_PI,                                ///< Push PI
    CALC_LENGTH,                            ///< Push Hypotemuse = sqrt(x^2 + y^2)
    CALC_FRAC,                              ///< Display result as fraction
    CALC_MAX                                ///< MUST be last!!!
} calc_func_t;


///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

/// Function settings
static function_t function;

// Forward declaration for the command processing functions
static void centerline(void);
static void diameter_set(void);
static void tachometer(void);
static void compound_set(void);
static void workspace_use(void);
static void workspace_set(void);
static void machine(void);
static void calc(void);
static void feed_rate(void);
static void tool_define(void);
static void tool_set(void);
static void bolthole_define(void);
static void bolthole_set(void);
static void grid_define(void);
static void grid_set(void);
static void radius_define(void);
static void radius_set(void);
static void power_off(void);
static void show_version(void);
static void send_fast(void);
static void erase(void);
#if PLATFORM_dro550
static void update(void);
static void save(void);
static void load(void);
#endif

/// Global function table per group
static const group_t mill_group[GROUP_MAX] =
{
    {
        .name = "positn",
        .command = {
            { .name = "tool",     .process = tool_set           },
            { .name = "c-line",   .process = centerline,        },
            { .name = "boltho",   .process = bolthole_set,      },
            { .name = "grid",     .process = grid_set,          },
            { .name = "radius",   .process = radius_set,        },
            { .name = "wspace",   .process = workspace_set,     },
        }
    },
    {
        .name = "status",
        .command = {
            { .name = "tach",     .process = tachometer,        },
            { .name = "feed",     .process = feed_rate,         },
            { .name = "calc",     .process = calc,              },
        }
    },
    {
        .name = "define",
        .command = {
            { .name = "tool",     .process = tool_define,       },
            { .name = "boltho",   .process = bolthole_define,   },
            { .name = "grid",     .process = grid_define,       },
            { .name = "radius",   .process = radius_define,     },
        }
    },
    {
        .name = "setup",
        .command = {
            { .name = "mach",     .process = machine_setup,     },
            { .name = "axis",     .process = axis_setup,        },
            { .name = "scale",    .process = scale_setup,       },
            { .name = "disply",   .process = display_setup,     },
            { .name = "keypad",   .process = keypad_setup,      },
        }
    },
    {
        .name = "sys",
        .command = {
            { .name = "off",      .process = power_off,         },
            { .name = "mach",     .process = machine,           },
            { .name = "sndfst",   .process = send_fast,         },
            { .name = "ver",      .process = show_version,      },
#if PLATFORM_dro550
            { .name = "update",   .process = update,            },
            { .name = "save",     .process = save,              },
            { .name = "load",     .process = load,              },
#endif
            { .name = "erase",    .process = erase,             },
        }
    },
};

static const group_t lathe_group[GROUP_MAX] =
{
    {
        .name = "positn",
        .command = {
            { .name = "tool",     .process = tool_set,          },
            { .name = "dia",      .process = diameter_set,      },
            { .name = "compnd",   .process = compound_set,      },
            { .name = "wspace",   .process = workspace_set,     },
        }
    },
    {
        .name = "status",
        .command = {
            { .name = "tach",     .process = tachometer,        },
            { .name = "feed",     .process = feed_rate,         },
            { .name = "calc",     .process = calc,              },
        }
    },
    {
        .name = "define",
        .command = {
            { .name = "tool",     .process = tool_define,       },
        }
    },
    {
        .name = "setup",
        .command = {
            { .name = "mach",     .process = machine_setup,     },
            { .name = "axis",     .process = axis_setup,        },
            { .name = "scale",    .process = scale_setup,       },
            { .name = "disply",   .process = display_setup,     },
            { .name = "keypad",   .process = keypad_setup,      },
        }
    },
    {
        .name = "sys",
        .command = {
            { .name = "off",      .process = power_off,         },
            { .name = "mach",     .process = machine,           },
            { .name = "sndfst",   .process = send_fast,         },
            { .name = "ver",      .process = show_version,      },
#if PLATFORM_dro550
            { .name = "update",   .process = update,            },
            { .name = "save",     .process = save,              },
            { .name = "load",     .process = load,              },
#endif
            { .name = "erase",    .process = erase,             },
        }
    },
};

static const process_t mill_speedkey[10] =
{
    axis_setup,
    centerline,
    calc,
    bolthole_define,
    bolthole_set,
    tool_define,
    tool_set,
    tachometer,
    feed_rate,
    power_off,
};

static const process_t lathe_speedkey[10] =
{
    axis_setup,
    diameter_set,
    calc,
    NULL,
    NULL,
    tool_define,
    tool_set,
    tachometer,
    feed_rate,
    power_off,
};

static const char *state_names[] = {
    [STATE_NORMAL]      = "normal",
    [STATE_BOLTHOLE]    = "bolthole",
    [STATE_GRID]        = "grid",
    [STATE_RADIUS]      = "radius",
};
static const char *unit_names[] = {
    [AXIS_UNITS_INCH]   = "inch",
    [AXIS_UNITS_MM]     = "mm",
};
static const char *edge_names[] = {
    [EDGE_CENTER]   = "center",
    [EDGE_LEFT]     = "left",
    [EDGE_RIGHT]    = "right",
    [EDGE_FRONT]    = "front",
    [EDGE_REAR]     = "rear",
};
static const char *shape_names[] = {
    [RADIUS_SHAPE_CONVEX]   = "convex",
    [RADIUS_SHAPE_CONCAVE]  = "concave",
};

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static bool
select_line(uint8_t *line_num)
{
    keypad_t key;
    
    do
    {
        key = keypad_get_press();
        if (key == KEYPAD_CLEAR)
            return false;
    } while (!keypad_display_line(key, line_num));
    
    return true;
}

// Process the edge compensation selection
static void
edge_use(void)
{
    tool_t*         tool_ptr;
    uint8_t         tool_num;
    double          x_offset = 0.0;
    double          y_offset = 0.0;
    double          z_offset = 0.0;
    axis_units_t    units = AXIS_UNITS_INCH;
    edge_t          edge;

    tool_num = function.tool_num;
    edge = function.edge;

    if (tool_num < TOOL_MAX)
    {
        tool_ptr = &function.tool[tool_num];
        units = tool_ptr->units;
        if (edge == EDGE_FRONT)
            y_offset = -tool_ptr->offset1;
        else if (edge == EDGE_REAR)
            y_offset = tool_ptr->offset1;
        else if (edge == EDGE_LEFT)
            x_offset = -tool_ptr->offset1;
        else if (edge == EDGE_RIGHT)
            x_offset = tool_ptr->offset1;
        z_offset = tool_ptr->offset2;

        display_status_set(STATUS_TOOL_NUM, tool_num + 1);
        display_status_set(STATUS_TOOL_OFFSET, tool_ptr->offset1);
        display_status_set(STATUS_TOOL_EDGE, edge);

        if (machine_get_prompt())
        {
            display_menu_puts(MENU_TOP, "tool");
            display_menu_printf(MENU_MIDDLE, "%d", tool_num + 1);
            display_menu_printf(MENU_BOTTOM, "%s", edge_names[edge]);

            timer_sleep(EDGE_DISPLAY_TIME);
        }
    }
    else
    {
        display_status_set(STATUS_TOOL_NUM, 0);
        display_status_set(STATUS_TOOL_OFFSET, 0.000);
        display_status_set(STATUS_TOOL_EDGE, EDGE_CENTER);
    }
    
    axis_set_tool_offset(MILL_X_AXIS, x_offset / 2.0, units);
    axis_set_tool_offset(MILL_Y_AXIS, y_offset / 2.0, units);
    axis_set_tool_offset(MILL_Z_AXIS, z_offset, units);

    TRACE_INFO("edge %d %f %f %f\n", edge, x_offset, y_offset, z_offset);
}

static void
edge_set(edge_t edge)
{
    function.edge = edge;

    edge_use();

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_EDGE, 0, sizeof(function.edge), &function.edge);
        config_obj_close();
    }
}

// Process the centerline definition
static void
centerline(void)
{
    keypad_t        key;
    uint8_t         line_num;
    uint8_t         axis_num;
    double          reading;
    axis_units_t    units;
    
    display_menu_puts(MENU_TOP, "c-line");
    display_menu_puts(MENU_MIDDLE, "select");
    display_menu_puts(MENU_BOTTOM, "axis");

    do
    {
        key = keypad_get_press();
        
        if (keypad_display_line(key, &line_num))
        {
            axis_num = machine_axis_by_line(line_num);
            units = machine_get_units();
            
            if (axis_num != AXIS_OFF)
            {
                if (machine_get_mode() == AXIS_ZERO_ABS)
                {
                    reading = axis_get_reading(axis_num, units, AXIS_ZERO_ABS) / 2.0;
                    axis_set_absolute(axis_num, reading, units);
                }   
                else
                {
                    reading = axis_get_reading(axis_num, units, AXIS_ZERO_ABS);
                    reading -= axis_get_reading(axis_num, units, AXIS_ZERO_INC) / 2.0;
                    axis_set_incremental(axis_num, reading, units);
                }

                TRACE_INFO("c-line %d %f\n", axis_num, reading);

                break;
            }   
        }
    } while (key != KEYPAD_CLEAR);
}

static void
diameter_use(void)
{
    double factor;

    if (function.diameter)
        factor = 2.0;
    else
        factor = 1.0;

    axis_set_scale_factor(function.diameter_axis, factor);

    display_status_set(STATUS_DIAMETER, factor);

    TRACE_INFO("diameter %d %f\n", function.diameter_axis, factor);
}

static void
diameter_set(void)
{
    keypad_t        key;
    uint8_t         line_num;
    uint8_t         axis_num;

    if (function.diameter)
    {
        function.diameter = false;
    }
    else
    {
        display_menu_puts(MENU_TOP, "diam");
        display_menu_puts(MENU_MIDDLE, "select");
        display_menu_puts(MENU_BOTTOM, "axis");

        do
        {
            key = keypad_get_press();

            if (keypad_display_line(key, &line_num))
            {
                function.diameter = true;
                axis_num = machine_axis_by_line(line_num);
                function.diameter_axis = axis_num;
                break;
            }
        } while (key != KEYPAD_CLEAR);
    }

    diameter_use();

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_DIAMETER, 0,
                         sizeof(function.diameter),
                         &function.diameter);
        config_var_write(VID_DIAMETER_AXIS, 0,
                         sizeof(function.diameter_axis),
                         &function.diameter_axis);
        config_obj_close();
    }
}

static void
tachometer(void)
{
    keypad_t    key;
    uint8_t     line_num;
    bool        surface;
    menu_t      menu;
    menu_nav_t  nav;

    machine_get_tach(&line_num, &surface);
    
    // If the tach is already being shown, then just disable it and return
    if (line_num < DISPLAY_LINE_MAX)
    {
        machine_set_tach(DISPLAY_LINE_MAX, 0);
        return;
    }

    // Get the display units for the tach       
    display_menu_puts(MENU_TOP, "tach");
    display_menu_puts(MENU_MIDDLE, "units");

    do
    {
        display_menu_puts(MENU_BOTTOM, surface ? "surfce" : "rpm");
        
        key = keypad_get_press();
        
        if (keypad_to_nav(key, &menu, &nav))
        {
            if (menu == MENU_BOTTOM)
                surface = (surface ? false : true);
        }
        else if (key == KEYPAD_CLEAR)
        {
            return;
        }
    } while (key != KEYPAD_ENTER);

    display_menu_puts(MENU_MIDDLE, "select");
    display_menu_puts(MENU_BOTTOM, "line");

    if (!select_line(&line_num))
        return;
    
    machine_set_tach(line_num, surface);
}

static void
feed_rate(void)
{
    uint8_t line_num;
    uint8_t axis_num;

    machine_get_feed(&line_num, &axis_num);
    if (line_num < DISPLAY_LINE_MAX)
    {
        machine_set_feed(DISPLAY_LINE_MAX, 0);
        return;
    }
    
    // Get the display units for the tach       
    display_menu_puts(MENU_TOP, "feed");
    display_menu_puts(MENU_MIDDLE, "select");
    display_menu_puts(MENU_BOTTOM, "axis");
    
    if (!select_line(&line_num))
        return;

    axis_num = machine_axis_by_line(line_num);

    display_menu_puts(MENU_BOTTOM, "line");
    
    if (!select_line(&line_num))
        return;
    
    machine_set_feed(line_num, axis_num);
}

static void
compound_use(void)
{
    double compound;
    
    compound = TO_RADIANS(function.compound);

    if (compound != 0.0)
    {
        axis_set_vector(LATHE_X_AXIS, LATHE_Z2_AXIS, sin(compound));
        axis_set_vector(LATHE_Z1_AXIS, LATHE_Z2_AXIS, cos(compound));
    }
    else
    {
        axis_set_vector(LATHE_X_AXIS, AXIS_OFF, 0.0);
        axis_set_vector(LATHE_Z1_AXIS, AXIS_OFF, 0.0);     
    }

    display_status_set(STATUS_COMPOUND, function.compound);

    TRACE_INFO("compound %f\n", compound);
}

static void
compound_set(void)
{
    double compound;
    
    display_menu_puts(MENU_TOP, "compnd");
    display_menu_puts(MENU_MIDDLE, "angle");

    compound = function.compound;
    if (!keypad_get_float(MENU_BOTTOM, &compound, NULL))
        return;

    function.compound = compound;

    compound_use();

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_COMPOUND, 0,
                         sizeof(function.compound),
                         &function.compound);
        config_obj_close();
    }
}

static void
workspace_use(void)
{
    uint8_t axis_num;
    uint8_t workspace;
    
    workspace = function.workspace;
    
    for (axis_num = 0; axis_num < AXIS_MAX; axis_num++)
        axis_set_workspace(axis_num, workspace);

    display_status_set(STATUS_WORKSPACE, workspace + 1);
        
    TRACE_INFO("workspace %d\n", workspace);
}

static void
workspace_set(void)
{
    int32_t workspace;

    display_menu_puts(MENU_TOP, "wspace");
    display_menu_puts(MENU_MIDDLE, "select");
    workspace = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, WORKSPACE_MAX, &workspace))
        return;

    workspace--;

    function.workspace = workspace;

    workspace_use();

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_WORKSPACE, 0,
                         sizeof(function.workspace),
                         &function.workspace);
        config_obj_close();
    }
}

static void
machine(void)
{
    int32_t new_machine;
    
    display_menu_puts(MENU_TOP, "mach");
    display_menu_puts(MENU_MIDDLE, "select");
    new_machine = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, MACHINE_MAX, &new_machine))
        return;

    new_machine--;
    
    machine_set_active(new_machine);
}

static void
calc_display(double* stack, bool funcActive, calc_func_t funcNum)
{
    display_menu_printf(MENU_TOP, "%*f", MENU_WIDTH, stack[CALC_Z]);
    display_menu_printf(MENU_MIDDLE, "%*f", MENU_WIDTH, stack[CALC_Y]);

    if (!funcActive)
        display_menu_printf(MENU_BOTTOM, "%*f", MENU_WIDTH, stack[CALC_X]);
    else
    {
        switch (funcNum)
        {
            case CALC_MIN:
                display_menu_puts(MENU_BOTTOM, "m in");
                break;
            case CALC_MOUT:
                display_menu_puts(MENU_BOTTOM, "m out");
                break;
            case CALC_SIN:
                display_menu_puts(MENU_BOTTOM, "sin");
                break;
            case CALC_COS:
                display_menu_puts(MENU_BOTTOM, "cos");
                break;
            case CALC_TAN:
                display_menu_puts(MENU_BOTTOM, "tan");
                break;
            case CALC_SQRT:
                display_menu_puts(MENU_BOTTOM, "sqrt");
                break;
            case CALC_POW:
                display_menu_puts(MENU_BOTTOM, "pow");
                break;
            case CALC_PI:
                display_menu_puts(MENU_BOTTOM, "pi");
                break;
            case CALC_PRESET:
                display_menu_puts(MENU_BOTTOM, "preset");
                break;
            case CALC_LENGTH:
                display_menu_puts(MENU_BOTTOM, "length");
                break;
            case CALC_FRAC:
                display_menu_puts(MENU_BOTTOM, "frac");
                break;
            case CALC_AXIS:
                display_menu_puts(MENU_BOTTOM, "axis");
                break;

            default:
                break;
        }
    }
}

static void
calc_push(double* stack)
{
    uint8_t pos;
    
    for (pos = 0; pos < CALC_SIZE - 1; pos++)
        stack[pos] = stack[pos + 1];
}

static void
calc_pop(double* stack)
{
    uint8_t pos;
   
    for (pos = CALC_SIZE - 1; pos > 0 ; pos--)
        stack[pos] = stack[pos - 1];
}

static void
calc_split_to_frac(double *value, int *integer, uint8_t *numerator, uint8_t * denominator)
{
    double  val;
    int     integ;
    uint8_t num, denom;
    bool    neg = false;
    
    if (*value < 0.0)
    {
        neg = true;
        val = -(*value);
    }
    else
        val = *value;
    
    integ = (uint32_t) val;
    val -= (double) integ;
    
    num = (uint8_t) (val * 64.0);
    denom = 64;
    
    while (num && !(num & 1))
    {
        num = num >> 1;
        denom = denom >> 1;
    }
    
    val -= (double) num / (double) denom;
    if (neg)
    {
        *value = -val;
        *integer = -integ;
    }
    else
    {
        *value = val;
        *integer = integ;
    }
    
    *numerator = num;
    *denominator = denom;
}

static bool
calc_query_axis(uint8_t *axis)
{
    keypad_t    key;
    uint8_t     line_num;
    
    display_menu_puts(MENU_BOTTOM, "-Axis-");
    
    key = keypad_get_press();

    if (keypad_display_line(key, &line_num))
    {
        *axis = machine_axis_by_line(line_num);
        return true;
    }
    return false;
}


static bool
calc_query_memloc(uint8_t *loc)
{
    keypad_t    key;
    
    display_menu_puts(MENU_BOTTOM, "-loc-");
    
    key = keypad_get_press();
    
    key -= KEYPAD_0;
    if (key <= MEMORIES_MAX)
    {
        *loc = key;
        return true;
    }
    
    return false;
}


static void
calc_do_operation(double *stack, calc_func_t func)
{
    double       value;
    int          integer;
    uint8_t      numerator;
    uint8_t      denominator;
    uint8_t      axis_num;
    uint8_t      mem_num;
    axis_units_t units;
    
    units = machine_get_units();
        
    switch (func)
    {
        case CALC_MIN:
            if (calc_query_memloc(&mem_num))
            {
                function.calc_mem[mem_num] = stack[CALC_X];
                
                if (config_obj_write(CONFIG_OID_FUNCTION, 0))
                {
                    config_var_write(VID_MEMORY, mem_num, sizeof(function.calc_mem[mem_num]), &function.calc_mem[mem_num]);
                    config_obj_close();
                }
            }
            break;
        case CALC_MOUT:
            if (calc_query_memloc(&mem_num))
            {
                calc_push(stack);
                stack[CALC_X] = function.calc_mem[mem_num];
            }
            break;
        case CALC_SIN:
            stack[CALC_X] = sin(TO_RADIANS(stack[CALC_X]));
            break;
        case CALC_COS:
            stack[CALC_X] = cos(TO_RADIANS(stack[CALC_X]));
            break;
        case CALC_TAN:
            stack[CALC_X] = tan(TO_RADIANS(stack[CALC_X]));
            break;
        case CALC_SQRT:
            stack[CALC_X] = sqrt(stack[CALC_X]);
            break;
        case CALC_POW:
            stack[CALC_Y] = pow(stack[CALC_Y], stack[CALC_X]);
            calc_pop(stack);
            break;
        case CALC_PI:
            calc_push(stack);
            stack[CALC_X] = PI;
            break;
        case CALC_PRESET:
            if (calc_query_axis(&axis_num))
            {
                value = axis_get_reading(axis_num,
                                         units,
                                         AXIS_ZERO_ABS);
                value += stack[CALC_X];
                axis_set_incremental(axis_num, value, units);
            }
            break;
        case CALC_LENGTH:
            stack[CALC_Y] = sqrt(stack[CALC_X] * stack[CALC_X] + stack[CALC_Y] * stack[CALC_Y]);
            calc_pop(stack);
            break;
        case CALC_FRAC:
            value = stack[CALC_X];
            calc_split_to_frac(&value, &integer, &numerator, &denominator);
            if (integer < 1000 && integer > -100)
               display_menu_printf(MENU_TOP, "%-3d%*d", integer, MENU_WIDTH - 3, numerator);
            else
                display_menu_printf(MENU_TOP, "OF %*d", MENU_WIDTH - 3, numerator);
            display_menu_printf(MENU_MIDDLE, "%*d", MENU_WIDTH, denominator);
            display_menu_printf(MENU_BOTTOM, "E%*f", MENU_WIDTH - 1, value);
            
            keypad_get_press();
            break;
        case CALC_AXIS:
            if (calc_query_axis(&axis_num))  
            {
                calc_push(stack);
                stack[CALC_X]= axis_get_reading(axis_num,
                                                units,
                                                AXIS_ZERO_INC);
            }
            break;

        default:
            break;
    }
}

static void
calc(void)
{
    uint8_t     digit;   
    double      value = 0.0;
    keypad_t    key;
    bool        done = false;
    double*     stack;
    uint8_t     pos;
    calc_func_t op_num;
    bool        op_active = false;
    
    stack = function.calc_stack;
    op_num = function.calc_op_num;

    display_status_set(STATUS_FUNC_STATE, STATE_CALC);

    while (!done)
    {
        calc_display(stack, op_active, op_num);
        
        key = keypad_peek_press();
        if ((keypad_to_digit(key, &digit) ||
            key == KEYPAD_DECIMAL) &&
            !op_active)
        {
            calc_push(stack);
            calc_display(stack, op_active, op_num);
            if (!keypad_get_float(MENU_BOTTOM, &value, &key))
            {
                calc_pop(stack);
                continue;
            }
            stack[CALC_X] = value;
        }
        else
        {
            key = keypad_get_press();
            if (key == KEYPAD_ENTER && !op_active)
                calc_push(stack);
        }

        // standard calc keys
        if (!op_active)
        {
            switch (key)
            {
            case KEYPAD_PRESET2:
            case KEYPAD_ZERO2:
                op_active = true;
                break;
            case KEYPAD_ZERO0:
                stack[CALC_Y] -= stack[CALC_X];
                calc_pop(stack);
                break;
            case KEYPAD_PRESET0:
                stack[CALC_Y] += stack[CALC_X];
                calc_pop(stack);
                break;
            case KEYPAD_ZERO1:
                stack[CALC_Y] /= stack[CALC_X];
                calc_pop(stack);
                break;
            case KEYPAD_PRESET1:
                stack[CALC_Y] *= stack[CALC_X];
                calc_pop(stack);
                break;
            case KEYPAD_PLUS_MINUS:
                stack[CALC_X] = -stack[CALC_X];
                break;
            case KEYPAD_FUNC:
                done = true;
                break;
            case KEYPAD_MM_INCH:
                value = stack[CALC_X];
                stack[CALC_X] = stack[CALC_Y];
                stack[CALC_Y] = value;
                break;
            case KEYPAD_CLEAR:
                calc_pop(stack);
                break;
            case KEYPAD_ABS_INCR:
                for (pos = 0; pos < CALC_SIZE; pos++)
                    stack[pos] = 0.0;
                break;
            default:
                break;
            }
        }
        // op mode key presses
        else
        {
            switch (key)
            {
            case KEYPAD_ENTER:
                calc_do_operation(stack, op_num);
                op_active = false;
                break;
            case KEYPAD_ZERO2:
                op_num++;
                if (op_num == CALC_MAX)
                    op_num = 0;
                break;
            case KEYPAD_PRESET2:
                if (!op_num)
                    op_num = CALC_MAX - 1;
                else
                    op_num--;
                break;
            case KEYPAD_FUNC:
                op_active = false;
                break;
            // speed keys    
            case KEYPAD_3:
                calc_do_operation(stack, CALC_PI);
                op_active = false;
                break;
            case KEYPAD_ZERO0:
                calc_do_operation(stack, CALC_MOUT);
                op_active = false;
                break;
            case KEYPAD_PRESET0:
                calc_do_operation(stack, CALC_MIN);
                op_active = false;
                break;
            case KEYPAD_ZERO1:
                calc_do_operation(stack, CALC_AXIS);
                op_active = false;
                break;
            case KEYPAD_PRESET1:
                calc_do_operation(stack, CALC_PRESET);
                op_active = false;
                break;
            case KEYPAD_7:
                calc_do_operation(stack, CALC_SIN);
                op_active = false;
                break;
            case KEYPAD_8:
                calc_do_operation(stack, CALC_COS);
                op_active = false;
                break;
            case KEYPAD_9:
                calc_do_operation(stack, CALC_TAN);
                op_active = false;
                break;
            case KEYPAD_DECIMAL:
                calc_do_operation(stack, CALC_FRAC);
                op_active = false;
                break;
            case KEYPAD_PLUS_MINUS:
                calc_do_operation(stack, CALC_LENGTH);
                op_active = false;
                break;
            case KEYPAD_4:
                calc_do_operation(stack, CALC_SQRT);
                op_active = false;
                break;
            case KEYPAD_5:
                calc_do_operation(stack, CALC_POW);
                op_active = false;
                break;
            default:
                break;
            }
        }
        TRACE_INFO("calc %f %f\n", stack[CALC_X], stack[CALC_Y]);
    }
    function.calc_op_num =  op_num;
    display_status_set(STATUS_FUNC_STATE, STATE_NORMAL);
}

static void
tool_use(void)
{
    tool_t*         tool_ptr;
    int32_t         tool_num;
    double          offset1 = 0.0;
    double          offset2 = 0.0;
    double          offset3 = 0.0;
    axis_units_t    units = AXIS_UNITS_INCH;

    tool_num = function.tool_num;

    // If the machine is a lathe, then get the tool offsets
    if (machine_get_type() == MACHINE_TYPE_LATHE)
    {
        if (tool_num < TOOL_MAX)
        {
            tool_ptr = &function.tool[tool_num];
            units = tool_ptr->units;
            offset1 = tool_ptr->offset1;
            offset2 = tool_ptr->offset2;
        }

        axis_set_tool_offset(LATHE_X_AXIS, offset1, units);
        axis_set_tool_offset(LATHE_Z1_AXIS, offset2, units);
        axis_set_tool_offset(LATHE_Z2_AXIS, offset3, units);

        display_status_set(STATUS_TOOL_NUM, tool_num == TOOL_NONE ? 0 : tool_num + 1);
        display_status_set(STATUS_TOOL_OFFSET, offset1);

        TRACE_INFO("tool %d %f %f %f\n", tool_num, offset1, offset2, offset3);
    }
    else if (machine_get_type() == MACHINE_TYPE_MILL)
    {
        edge_use();
    }
}

static void
tool_save(int32_t tool_num, double offset1, double offset2, axis_units_t units)
{
    tool_t* tool_ptr = &function.tool[tool_num];

    // Save the tool offset
    tool_ptr->offset1 = offset1;
    tool_ptr->offset2 = offset2;
    tool_ptr->units = units;

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_TOOL, tool_num, sizeof(*tool_ptr), tool_ptr);
        config_obj_close();
    }
    
    TRACE_INFO("tool define %d %f %f\n", tool_num, offset1, offset2);

    // If the current tool is modified, then reuse it
    if (tool_num == function.tool_num)
        tool_use();
}

static void
tool_define(void)
{
    int32_t tool_num;
    double  offset1;
    double  offset2;
    tool_t* tool_ptr;
    axis_units_t units;
    
    display_menu_puts(MENU_TOP, "tool");

    // Define the tool number 
    display_menu_puts(MENU_MIDDLE, "define");
    tool_num = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, TOOL_MAX, &tool_num))
        return;

    // Adjust to zero-based index
    tool_num--;
    tool_ptr = &function.tool[tool_num];

    // Get the machine units
    units = machine_get_units();
    
    // Get the first offset
    display_menu_puts(MENU_MIDDLE, "offst1");
    offset1 = axis_convert_units(tool_ptr->offset1, tool_ptr->units, units);
    if (!keypad_get_float(MENU_BOTTOM, &offset1, NULL))
        return;

    // Get the second offset
    display_menu_puts(MENU_MIDDLE, "offst2");
    offset2 = axis_convert_units(tool_ptr->offset2, tool_ptr->units, units);
    if (!keypad_get_float(MENU_BOTTOM, &offset2, NULL))
        return;
        
    // Save the tool offsets
    tool_save(tool_num, offset1, offset2, units);
}

static void
tool_set(void)
{
    int32_t tool_num;
    
    display_menu_puts(MENU_TOP, "tool");
    display_menu_puts(MENU_MIDDLE, "use");
    tool_num = -1;
    if (!keypad_get_number(MENU_BOTTOM, 0, TOOL_MAX, &tool_num))
        return;

    // Adjust to zero-based index
    tool_num--;

    if (tool_num < 0)
    {
        function.tool_num = TOOL_NONE;
    }
    else
    {
        // Make sure it is defined
        if (function.tool[tool_num].offset1 == 0.0 &&
            function.tool[tool_num].offset2 == 0.0)
        {
            display_menu_puts(MENU_BOTTOM, "undef");
            keypad_get_press();
            return;
        }
        function.tool_num = tool_num;
    }

    function.edge = EDGE_CENTER;

    tool_use();

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_EDGE, 0, sizeof(function.edge), &function.edge);
        config_var_write(VID_TOOL_NUM, 0, sizeof(function.tool_num), &function.tool_num);
        config_obj_close();
    }
}

static void
bolthole_define(void)
{
    int32_t     bolthole_num;
    int32_t     holes;
    double      radius;
    double      start;
    double      end;
    bolthole_t* bolthole_ptr;
    axis_units_t units;
    
    display_menu_puts(MENU_TOP, "boltho");

    display_menu_puts(MENU_MIDDLE, "define");
    bolthole_num = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, BOLTHOLE_MAX, &bolthole_num))
        return;

    // Adjust to zero-based index
    bolthole_num--;
    bolthole_ptr = &function.bolthole[bolthole_num];

    units = machine_get_units();
    
    display_menu_puts(MENU_MIDDLE, "holes");
    holes = bolthole_ptr->holes;
    if (!keypad_get_number(MENU_BOTTOM, 2, 99, &holes))
        return;

    display_menu_puts(MENU_MIDDLE, "radius");
    radius = axis_convert_units(bolthole_ptr->radius, bolthole_ptr->units, units);
    if (!keypad_get_float(MENU_BOTTOM, &radius, NULL))
        return;

    display_menu_puts(MENU_MIDDLE, "start");
    start = bolthole_ptr->start;
    if (!keypad_get_float(MENU_BOTTOM, &start, NULL))
        return;

    display_menu_puts(MENU_MIDDLE, "end");
    end = bolthole_ptr->end;
    if (!keypad_get_float(MENU_BOTTOM, &end, NULL))
        return;

    bolthole_ptr->holes = holes;
    bolthole_ptr->radius = fabs(radius);
    bolthole_ptr->start = fmod(start, 360.0);
    if (bolthole_ptr->start < 0.0)
        bolthole_ptr->start += 360.0;
    bolthole_ptr->end = end;
    if (bolthole_ptr->end <= bolthole_ptr->start)
        bolthole_ptr->end += 360.0;
    bolthole_ptr->units = units;

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_BOLTHOLE, bolthole_num, sizeof(*bolthole_ptr), bolthole_ptr);
        config_obj_close();
    }
    
    TRACE_INFO("bolthole define %d %d %f %f %f\n", bolthole_num,
         bolthole_ptr->holes, bolthole_ptr->radius, bolthole_ptr->start, bolthole_ptr->end);
}

static void
bolthole_done(void)
{
    uint32_t    bolthole_num;
    bolthole_t* bolthole_ptr;

    bolthole_num = function.bolthole_num;
    bolthole_ptr = &function.bolthole[bolthole_num];

    display_status_set(STATUS_FUNC_STATE, STATE_NORMAL);

    display_menu_puts(MENU_TOP, "bolt");
    display_menu_puts(MENU_MIDDLE, "hole");
    display_menu_puts(MENU_BOTTOM, "done");
    keypad_get_press();
    function.state = STATE_NORMAL;
    display_indicator_set(INDICATOR_FUNC, false);
    axis_set_incremental(MILL_X_AXIS, function.x_zero, bolthole_ptr->units);
    axis_set_incremental(MILL_Y_AXIS, function.y_zero, bolthole_ptr->units);
}

static void
bolthole_use(void)
{
    uint32_t    bolthole_num;
    int32_t     step;
    bolthole_t* bolthole_ptr;
    double      angle;
    double      x_zero;
    double      y_zero;
    
    display_menu_puts(MENU_TOP, "bolt");
    display_menu_puts(MENU_MIDDLE, "hole");

    bolthole_num = function.bolthole_num;
    bolthole_ptr = &function.bolthole[bolthole_num];
    step = function.step;
    
    if (function.step == bolthole_ptr->holes ||
        function.step < 0)
    {
        bolthole_done();
        return;
    }

    display_status_set(STATUS_FUNC_STATE, STATE_BOLTHOLE, bolthole_num + 1);
    display_status_set(STATUS_FUNC_STEP, step + 1);

    display_menu_printf(MENU_BOTTOM, "%"PRId32, step + 1);
    keypad_get_press();

    TRACE_INFO("bolthole %d step %d holes %d start %f end %f xzero %f yzero %f\n",
         bolthole_num, step, bolthole_ptr->holes, bolthole_ptr->start,
         bolthole_ptr->end, function.x_zero, function.y_zero);

    angle = (bolthole_ptr->end - bolthole_ptr->start);
    angle *= (double) step / bolthole_ptr->holes;
    angle += bolthole_ptr->start;
    angle = TO_RADIANS(angle);
    
    x_zero = function.x_zero;
    x_zero += bolthole_ptr->radius * sin(angle);

    y_zero = function.y_zero;
    y_zero += bolthole_ptr->radius * cos(angle);

    axis_set_incremental(MILL_X_AXIS, x_zero, bolthole_ptr->units);
    axis_set_incremental(MILL_Y_AXIS, y_zero, bolthole_ptr->units);

    machine_set_zero(AXIS_ZERO_INC);
    
    TRACE_INFO("bolthole %d %f %f %f", bolthole_num, angle, x_zero, y_zero);
}

static void
bolthole_set(void)
{
    int32_t         bolthole_num;
    axis_units_t    units;
    
    display_menu_puts(MENU_TOP, "boltho");

    display_menu_puts(MENU_MIDDLE, "use");
    bolthole_num = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, BOLTHOLE_MAX, &bolthole_num))
        return;

    // Adjust to zero-based index
    bolthole_num--;

    // Make sure it is defined
    if (function.bolthole[bolthole_num].radius == 0.0)
    {
        display_menu_puts(MENU_BOTTOM, "undef");
        keypad_get_press();
        return;
    }

    units = function.bolthole[bolthole_num].units;
    function.bolthole_num = bolthole_num;
    function.state = STATE_BOLTHOLE;
    function.step = 0;
    function.x_zero = axis_get_incremental(MILL_X_AXIS, units);
    function.y_zero = axis_get_incremental(MILL_Y_AXIS, units);
    bolthole_use();
}

static void
grid_define(void)
{
    int32_t grid_num;
    double  xdim;
    double  ydim;
    int32_t xholes;
    int32_t yholes;
    double  angle;
    grid_t* grid_ptr;
    axis_units_t units;
    
    display_menu_puts(MENU_TOP, "grid");

    display_menu_puts(MENU_MIDDLE, "define");
    grid_num = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, GRID_MAX, &grid_num))
        return;

    // Adjust to zero-based index
    grid_num--;
    grid_ptr = &function.grid[grid_num];

    units = machine_get_units();
    
    display_menu_puts(MENU_MIDDLE, "xdim");
    xdim = axis_convert_units(grid_ptr->xdim, grid_ptr->units, units);
    if (!keypad_get_float(MENU_BOTTOM, &xdim, NULL))
        return;

    display_menu_puts(MENU_MIDDLE, "ydim");
    ydim = axis_convert_units(grid_ptr->ydim, grid_ptr->units, units);
    if (!keypad_get_float(MENU_BOTTOM, &ydim, NULL))
        return;

    display_menu_puts(MENU_MIDDLE, "xholes");
    xholes = grid_ptr->xholes;
    if (!keypad_get_number(MENU_BOTTOM, 1, 99, &xholes))
        return;

    display_menu_puts(MENU_MIDDLE, "yholes");
    yholes = grid_ptr->yholes;
    if (!keypad_get_number(MENU_BOTTOM, 1, 99, &yholes))
        return;

    display_menu_puts(MENU_MIDDLE, "angle");
    angle = grid_ptr->angle;
    if (!keypad_get_float(MENU_BOTTOM, &angle, NULL))
        return;

    grid_ptr->xdim = xdim;
    grid_ptr->ydim = ydim;
    grid_ptr->xholes = xholes;
    grid_ptr->yholes = yholes;
    grid_ptr->angle = angle;
    grid_ptr->units = units;

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_GRID, grid_num, sizeof(*grid_ptr), grid_ptr);
        config_obj_close();
    }

    TRACE_INFO("grid define %d %f %f %d %d %f\n", grid_num, xdim, ydim, xholes, yholes, angle);
}

static void
grid_done(void)
{
    uint32_t    grid_num;
    grid_t*     grid_ptr;

    grid_num = function.grid_num;
    grid_ptr = &function.grid[grid_num];

    display_status_set(STATUS_FUNC_STATE, STATE_NORMAL);

    display_menu_puts(MENU_TOP, "grid");
    display_menu_puts(MENU_MIDDLE, "hole");
    display_menu_puts(MENU_BOTTOM, "done");
    keypad_get_press();
    function.state = STATE_NORMAL;
    display_indicator_set(INDICATOR_FUNC, false);
    axis_set_incremental(MILL_X_AXIS, function.x_zero, grid_ptr->units);
    axis_set_incremental(MILL_Y_AXIS, function.y_zero, grid_ptr->units);
}

static void
grid_use(void)
{
    uint32_t    grid_num;
    int32_t     step;
    grid_t*     grid_ptr;
    double      x_pos = 0.0;
    double      y_pos = 0.0;
    double      x_zero;
    double      y_zero;
    double      angle;
    
    display_menu_puts(MENU_TOP, "grid");
    display_menu_puts(MENU_MIDDLE, "hole");

    grid_num = function.grid_num;
    grid_ptr = &function.grid[grid_num];
    step = function.step;

    if (function.step == (grid_ptr->xholes * grid_ptr->yholes) ||
        function.step < 0)
    {
        grid_done();
        return;
    }

    display_status_set(STATUS_FUNC_STATE, STATE_GRID, grid_num + 1);
    display_status_set(STATUS_FUNC_STEP, step + 1);

    display_menu_printf(MENU_BOTTOM, "%"PRId32, step + 1);
    keypad_get_press();

    // Calculate the positions based on a zero angle
    x_pos = grid_ptr->xdim * (step % grid_ptr->xholes);
    y_pos = grid_ptr->ydim * (step / grid_ptr->xholes);

    // Apply the angle
    angle = TO_RADIANS(grid_ptr->angle);
    x_zero = (x_pos * cos(angle)) + (y_pos * sin(angle));
    TRACE_INFO("x_zero %f\n", x_zero);
    y_zero = (y_pos * cos(angle)) - (x_pos * sin(angle));
    TRACE_INFO("y_zero %f\n", y_zero);

    // Add in the saved incremental zeros
    x_zero += function.x_zero;
    y_zero += function.y_zero;

    // Set the new incremental zeros
    axis_set_incremental(MILL_X_AXIS, x_zero, grid_ptr->units);
    axis_set_incremental(MILL_Y_AXIS, y_zero, grid_ptr->units);
    machine_set_zero(AXIS_ZERO_INC);
    
    TRACE_INFO("grid %d %f %f\n", grid_num, x_zero, y_zero);
}

static void
grid_set(void)
{
    int32_t         grid_num;
    axis_units_t    units;
    
    display_menu_puts(MENU_TOP, "grid");

    display_menu_puts(MENU_MIDDLE, "use");
    grid_num = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, GRID_MAX, &grid_num))
        return;

    // Adjust to zero-based index
    grid_num--;

    // Make sure it is defined
    if (function.grid[grid_num].xdim == 0.0 &&
        function.grid[grid_num].ydim == 0.0)
    {
        display_menu_puts(MENU_BOTTOM, "undef");
        keypad_get_press();
        return;
    }

    units = function.grid[grid_num].units;
    function.grid_num = grid_num;  
    function.state = STATE_GRID;
    function.step = 0;
    function.x_zero = axis_get_incremental(MILL_X_AXIS, units);
    function.y_zero = axis_get_incremental(MILL_Y_AXIS, units);
    grid_use();
}

static void
radius_define(void)
{
    int32_t     radius_num;
    int32_t     steps;
    uint32_t    shape;
    uint32_t    axis;
    double      radius;
    double      start;
    double      end;
    radius_t*   radius_ptr;
    axis_units_t units;
    
    display_menu_puts(MENU_TOP, "radius");

    display_menu_puts(MENU_MIDDLE, "define");
    radius_num = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, RADIUS_MAX, &radius_num))
        return;

    // Adjust to zero-based index
    radius_num--;
    radius_ptr = &function.radius[radius_num];

    units = machine_get_units();
    
    display_menu_puts(MENU_MIDDLE, "length");
    radius = axis_convert_units(radius_ptr->radius, radius_ptr->units, units);
    if (!keypad_get_float(MENU_BOTTOM, &radius, NULL))
        return;

    display_menu_puts(MENU_MIDDLE, "shape");
    shape = radius_ptr->shape;
    if (!keypad_get_selection(MENU_BOTTOM, &shape, RADIUS_SHAPE_MAX,
            (const char*[]) { [RADIUS_SHAPE_CONVEX] = "convex",
                              [RADIUS_SHAPE_CONCAVE] = "concav" }))
        return;

    display_menu_puts(MENU_MIDDLE, "axis");
    axis = radius_ptr->axis;
    if (!keypad_get_selection(MENU_BOTTOM, &axis, 2,
            (const char*[]) { [MILL_X_AXIS] = "x axis",
                              [MILL_Y_AXIS] = "y axis" }))
        return;

    display_menu_puts(MENU_MIDDLE, "steps");
    steps = radius_ptr->steps;
    if (!keypad_get_number(MENU_BOTTOM, 2, 99, &steps))
        return;

    while (1)
    {
        display_menu_puts(MENU_MIDDLE, "start");
        start = radius_ptr->start;
        if (!keypad_get_float(MENU_BOTTOM, &start, NULL))
            return;

        // Normalize angle to 0 to 360 degrees
        start = fmod(start, 360.0);
        if (start < 0.0)
            start += 360.0;

        // For convex shape, shift angles above 270 down
        if (shape == RADIUS_SHAPE_CONVEX && start >= 270.0)
            start -= 360.0;

        // Make sure the angle is valid for the shape
        if (shape == RADIUS_SHAPE_CONVEX && (start >= -90.0 && start <= 90.0))
            break;
        if (shape == RADIUS_SHAPE_CONCAVE && (start <= 270.0 && start >= 90.0))
            break;
        display_menu_puts(MENU_MIDDLE, "invald");
        display_menu_puts(MENU_BOTTOM, "angle");
        keypad_get_press();
    }

    while (1)
    {
        display_menu_puts(MENU_MIDDLE, "end");
        end = radius_ptr->end;
        if (!keypad_get_float(MENU_BOTTOM, &end, NULL))
            return;

        // Normalize angle to 0 to 360 degrees
        end = fmod(end, 360.0);
        if (end < 0.0)
            end += 360.0;

        // For convex shape, shift angles above 270 down
        if (shape == RADIUS_SHAPE_CONVEX && end >= 270.0)
            end -= 360.0;

        // Make sure the angle is valid for the shape
        if (end > start)
        {
            if (shape == RADIUS_SHAPE_CONVEX && (end >= -90.0 && end <= 90.0))
                break;
            if (shape == RADIUS_SHAPE_CONCAVE && (end <= 270.0 && end >= 90.0))
                break;
        }
        display_menu_puts(MENU_MIDDLE, "invald");
        display_menu_puts(MENU_BOTTOM, "angle");
        keypad_get_press();
    }

    radius_ptr->radius = fabs(radius);
    radius_ptr->shape = shape;
    radius_ptr->axis = axis;
    radius_ptr->steps = steps;
    radius_ptr->start = start;
    radius_ptr->end = end;
    radius_ptr->units = units;

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        config_var_write(VID_RADIUS, radius_num, sizeof(*radius_ptr), radius_ptr);
        config_obj_close();
    }

    TRACE_INFO("radius define %d %f %d %d %d %f %f\n", radius_num,
            radius_ptr->radius, radius_ptr->shape, radius_ptr->axis,
            radius_ptr->steps,  radius_ptr->start, radius_ptr->end);
}

static void
radius_done(void)
{
    uint32_t    radius_num;
    radius_t*   radius_ptr;

    radius_num = function.radius_num;
    radius_ptr = &function.radius[radius_num];

    display_status_set(STATUS_FUNC_STATE, STATE_NORMAL);

    display_menu_puts(MENU_TOP, "radius");
    display_menu_puts(MENU_MIDDLE, "steps");
    display_menu_puts(MENU_BOTTOM, "done");
    keypad_get_press();
    function.state = STATE_NORMAL;
    display_indicator_set(INDICATOR_FUNC, false);
    axis_set_incremental(MILL_X_AXIS, function.x_zero, radius_ptr->units);
    axis_set_incremental(MILL_Y_AXIS, function.y_zero, radius_ptr->units);
    axis_set_incremental(MILL_Z_AXIS, function.z_zero, radius_ptr->units);
    edge_use();
}

static void
radius_use(void)
{
    uint32_t    radius_num;
    int32_t     step;
    radius_t*   radius_ptr;
    double      angle;
    double      axis_zero;
    double      axis_tool;
    double      z_zero;
    double      z_tool;
    double      tool_radius;
    double      tool_angle;
    uint8_t     tool_num;
    tool_t*     tool_ptr;

    display_menu_puts(MENU_TOP, "radius");
    display_menu_puts(MENU_MIDDLE, "step");

    radius_num = function.radius_num;
    radius_ptr = &function.radius[radius_num];
    step = function.step;

    // Check if we reached the last step or backed up past the first
    if (function.step == radius_ptr->steps ||
        function.step < 0)
    {
        radius_done();
        return;
    }

    TRACE_INFO("radius_num %d step %d steps %d\n",
         radius_num, step, radius_ptr->steps);
    TRACE_INFO("radius %f shape %d axis %d start %f end %f\n",
         radius_ptr->radius, radius_ptr->shape, radius_ptr->axis,
         radius_ptr->start, radius_ptr->end);

    // Display the step and wait for a key press
    display_status_set(STATUS_FUNC_STATE, STATE_RADIUS, radius_num + 1);
    display_status_set(STATUS_FUNC_STEP, step + 1);

    display_menu_printf(MENU_BOTTOM, "%"PRId32, step + 1);
    keypad_get_press();

    // Calculate the zero position on the radius
    angle = (radius_ptr->end - radius_ptr->start);
    angle *= (double) step / (radius_ptr->steps - 1.0);
    angle += radius_ptr->start;
    angle = TO_RADIANS(angle);

    if (radius_ptr->axis == MILL_X_AXIS)
        axis_zero = function.x_zero;
    else
        axis_zero = function.y_zero;
    axis_zero += radius_ptr->radius * sin(angle);

    z_zero = function.z_zero;
    z_zero += radius_ptr->radius * cos(angle);
    if (radius_ptr->shape == RADIUS_SHAPE_CONVEX)
        z_zero -= radius_ptr->radius;

    // Set the zero position as an incremental zero
    axis_set_incremental(radius_ptr->axis, axis_zero, radius_ptr->units);
    axis_set_incremental(MILL_Z_AXIS, z_zero, radius_ptr->units);

    // Calculate the point on the ball end tool that is tangent
    // to the radius circle
    machine_set_zero(AXIS_ZERO_INC);

    TRACE_INFO("angle %f axis_zero %f z_zero %f\n",
               angle, axis_zero, z_zero);

    tool_num = function.tool_num;
    ASSERT(tool_num < TOOL_MAX);
    tool_ptr = &function.tool[tool_num];

    tool_radius = tool_ptr->offset1 / 2.0;
    tool_angle = angle;
    if (radius_ptr->shape == RADIUS_SHAPE_CONVEX)
        tool_angle += TO_RADIANS(180.0);

    axis_tool = tool_radius * sin(tool_angle);
    z_tool = tool_ptr->offset2 + tool_radius + tool_radius * cos(tool_angle);

    // Set the tool tangent point as a tool offset
    axis_set_tool_offset(radius_ptr->axis, axis_tool, tool_ptr->units);
    axis_set_tool_offset(MILL_Z_AXIS, z_tool, tool_ptr->units);

    TRACE_INFO("tool_angle %f axis_tool %f z_tool %f\n",
               tool_angle, axis_tool, z_tool);
}

static void
radius_set(void)
{
    int32_t         radius_num;
    axis_units_t    units;

    // There must be a valid tool selected to use the radius function
    if (function.tool_num == TOOL_NONE)
    {
        display_menu_puts(MENU_TOP, "select");
        display_menu_puts(MENU_MIDDLE, "a tool");
        display_menu_puts(MENU_BOTTOM, "first");
        keypad_get_press();
        return;
    }

    display_menu_puts(MENU_TOP, "radius");

    display_menu_puts(MENU_MIDDLE, "use");
    radius_num = 0;
    if (!keypad_get_number(MENU_BOTTOM, 1, RADIUS_MAX, &radius_num))
        return;

    // Adjust to zero-based index
    radius_num--;

    // Make sure it is defined
    if (function.radius[radius_num].radius == 0.0)
    {
        display_menu_puts(MENU_BOTTOM, "undef");
        keypad_get_press();
        return;
    }

    units = function.radius[radius_num].units;
    function.radius_num = radius_num;
    function.state = STATE_RADIUS;
    function.step = 0;
    function.x_zero = axis_get_incremental(MILL_X_AXIS, units);
    function.y_zero = axis_get_incremental(MILL_Y_AXIS, units);
    function.z_zero = axis_get_incremental(MILL_Z_AXIS, units);
    radius_use();
}

static void
send_fast(void)
{
    scale_send_fast();
    
    TRACE_INFO("send fast\n");
}

static void
show_version(void)
{
    display_menu_puts(MENU_TOP, "opndro");
    display_menu_printf(MENU_MIDDLE, "%d.%d.%d", OPENDRO_MAJOR, OPENDRO_MINOR, OPENDRO_MAINT);
    display_menu_printf(MENU_BOTTOM, OPENDRO_HARDWARE);
    
    keypad_get_press();
}

static void
erase(void)
{
    display_menu_puts(MENU_TOP, "enter");
    display_menu_puts(MENU_MIDDLE, "if");
    display_menu_puts(MENU_BOTTOM, "sure");
    
    if (keypad_get_press() != KEYPAD_ENTER)
        return;
        
    config_erase();
        
    TRACE_INFO("erase\n");
}

#if PLATFORM_dro550
static void
update(void)
{
    display_menu_puts(MENU_TOP, "enter");
    display_menu_puts(MENU_MIDDLE, "if");
    display_menu_puts(MENU_BOTTOM, "sure");

    if (keypad_get_press() != KEYPAD_ENTER)
        return;

    // Clear GBNVM bit 2
    efc_command(AT91C_MC_FCMD_CLR_GP_NVM, 2);

    TRACE_INFO("update\n");
}

static void
save(void)
{
    FILE* file;
    
    file = fopen("config.bin", "w");
    if (!file)
    {
        display_menu_puts(MENU_TOP, "cannot");
        display_menu_puts(MENU_MIDDLE, "open");
        display_menu_puts(MENU_BOTTOM, "config");
        keypad_get_press();
        return;
    }
    
    config_save(file);
    
    fclose(file);
    
    display_menu_puts(MENU_TOP, "config");
    display_menu_puts(MENU_MIDDLE, "file");
    display_menu_puts(MENU_BOTTOM, "saved");

    keypad_get_press();
}

static void
load(void)
{
    FILE* file;
    
    display_menu_puts(MENU_TOP, "enter");
    display_menu_puts(MENU_MIDDLE, "if");
    display_menu_puts(MENU_BOTTOM, "sure");

    if (keypad_get_press() != KEYPAD_ENTER)
        return;
    
    file = fopen("config.bin", "r");
    if (!file)
    {
        display_menu_puts(MENU_TOP, "cannot");
        display_menu_puts(MENU_MIDDLE, "open");
        display_menu_puts(MENU_BOTTOM, "config");
        keypad_get_press();
        return;
    }
    
    config_load(file);

    fclose(file);
    
    reset();
}
#endif

static void
not_machine_type(machine_type_t type)
{
    display_menu_puts(MENU_TOP, "mach");
    display_menu_puts(MENU_MIDDLE, "not");
    display_menu_puts(MENU_BOTTOM, (type == MACHINE_TYPE_MILL) ? "mill" : "lathe");

    keypad_get_press();
}

static void
power_off(void)
{
    machine_off();
}

static void
user_func(void)
{
    keypad_t    key;
    uint8_t     digit;
    char        func[8];

    display_menu_puts(MENU_TOP, "func");
    display_menu_puts(MENU_MIDDLE, "user");
    display_menu_puts(MENU_BOTTOM, "0-9");
    key = keypad_get_press();
    if (keypad_to_digit(key, &digit))
    {
        snprintf(func, sizeof(func), "user%d()", digit);
        if (!luash_dostring(func))
        {
            display_menu_printf(MENU_TOP, "user%d", digit);
            display_menu_puts(MENU_MIDDLE, "lua");
            display_menu_puts(MENU_BOTTOM, "error");
            keypad_get_press();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

double
function_get_cut_dia(axis_units_t units)
{
    uint8_t tool_num;
    double  cut_dia;
    
    // Lathe cutting diameter is twice the absolute distance of the X axis
    if (machine_get_type() == MACHINE_TYPE_LATHE)
    {
        cut_dia = axis_get_reading(LATHE_X_AXIS, units, AXIS_ZERO_ABS);

        // Don't multiply by 2 if the reading is already a diameter
        if (!function.diameter)
            cut_dia *= 2.0;
    }
    // Mill cutting diameter is the size of the current cutting tool
    else
    {
        tool_num = function.tool_num;
        cut_dia = function.tool[tool_num].offset1;
        
        // Covert to diameter to the requested units
        if (function.tool[tool_num].units != units)
        {
            if (units == AXIS_UNITS_INCH)
                cut_dia *= MM_TO_INCH;
            else
                cut_dia *= INCH_TO_MM;  
        }
    }
    
    return cut_dia;
}

void
function_on(void)
{
    // Remind the user if edge compensation is active
    if (machine_get_type() == MACHINE_TYPE_MILL)
        edge_use();
}

void
function_off(void)
{
}

void
function_set_active(void)
{
    machine_type_t machine_type;

    machine_type = machine_get_type();
    
    tool_use();
    workspace_use();

    if (machine_type == MACHINE_TYPE_LATHE)
    {
        diameter_use();
        compound_use();
    }

    display_status_set(STATUS_MACHINE_NUM, machine_get_active());
    display_status_set(STATUS_MACHINE_TYPE, machine_type);
}

void
function_key(keypad_t key)
{
    edge_t edge;

    ASSERT(key < KEYPAD_MAX);
    
    display_on();

    switch (function.state)
    {
    case STATE_BOLTHOLE:
        if (machine_get_type() == MACHINE_TYPE_MILL)
        {
            if (key == KEYPAD_ENTER)
                function.step++;
            else if (key == KEYPAD_CLEAR)
                function.step--;
            else
                return;
                
            bolthole_use();
        }
        break;
    case STATE_GRID:
        if (machine_get_type() == MACHINE_TYPE_MILL)
        {
            if (key == KEYPAD_ENTER)
                function.step++;
            else if (key == KEYPAD_CLEAR)
                function.step--;
            else
                return;
                
            grid_use();
        }
        break;
    case STATE_RADIUS:
        if (machine_get_type() == MACHINE_TYPE_MILL)
        {
            if (key == KEYPAD_ENTER)
                function.step++;
            else if (key == KEYPAD_CLEAR)
                function.step--;
            else
                return;

            radius_use();
        }
        break;
    default:
    case STATE_NORMAL:
        if (machine_get_type() == MACHINE_TYPE_MILL)
        {
            if (key == KEYPAD_5)
                edge = EDGE_CENTER;
            else if (key == KEYPAD_2)
                edge = EDGE_FRONT;
            else if (key == KEYPAD_8)
                edge = EDGE_REAR;
            else if (key == KEYPAD_4)
                edge = EDGE_LEFT;
            else if (key == KEYPAD_6)
                edge = EDGE_RIGHT;
            else
                return; 
        
            edge_set(edge);
        }
        break;
    }
}

void
function_select(void)
{
    const group_t*  group_ptr = NULL;
    const command_t* command_ptr = NULL;
    int8_t          command_num = 0;
    int8_t          group_num = 0;
    uint8_t         digit;
    bool            done = false;
    keypad_t        key;
    machine_type_t  machine_type;

    // Stop performing a function if one is currently in progress
    if (function.state != STATE_NORMAL)
    {
        if (function.state == STATE_BOLTHOLE)
            bolthole_done();
        else if (function.state == STATE_GRID)
            grid_done();
        else if (function.state == STATE_RADIUS)
            radius_done();

        display_status_set(STATUS_FUNC_STATE, STATE_NORMAL);

        function.state = STATE_NORMAL;
        return;
    }
    
    display_on();
    display_indicator_set(INDICATOR_FUNC, true);
    display_menu_puts(MENU_TOP, "func");
    
    machine_type = machine_get_type();

    while (!done)
    {
        if (machine_type == MACHINE_TYPE_MILL)
            group_ptr = &mill_group[group_num];
        else
            group_ptr = &lathe_group[group_num];

        display_menu_puts(MENU_MIDDLE, group_ptr->name);
        
        command_ptr = &group_ptr->command[command_num];
        display_menu_puts(MENU_BOTTOM, command_ptr->name);

        key = keypad_get_press();
        switch (key)
        {
        case KEYPAD_ENTER:
            command_ptr->process();
            done = true;
            break;

        case KEYPAD_NEXT_MIDDLE:
            group_num++;
            if (group_num == GROUP_MAX)
                group_num = 0;
            command_num = 0;
            break;

        case KEYPAD_PREV_MIDDLE:
            group_num--;
            if (group_num < 0)
                group_num = GROUP_MAX - 1;
            command_num = 0;
            break;

        case KEYPAD_NEXT_BOTTOM:
            command_num++;
            if (command_num == COMMAND_MAX ||
                group_ptr->command[command_num].name == 0)
                command_num = 0;
            break;

        case KEYPAD_PREV_BOTTOM:
            command_num--;
            if (command_num < 0) {
                command_num = COMMAND_MAX - 1;
                while (group_ptr->command[command_num].name == 0)
                    command_num--;
            }
            break;

        case KEYPAD_MM_INCH:
            machine_display_toggle();
            done = true;
            break;

        case KEYPAD_ABS_INCR:
            send_fast();
            done = true;
            break;

        case KEYPAD_DECIMAL:
            if (machine_type == MACHINE_TYPE_LATHE)
                compound_set();
            else
                not_machine_type(MACHINE_TYPE_LATHE);
            done = true;
            break;

        case KEYPAD_PLUS_MINUS:
            workspace_set();
            done = true;
            break;

        case KEYPAD_CLEAR:
            done = true;
            break;

        case KEYPAD_FUNC:
            user_func();
            done = true;
            break;

        default:
            if (keypad_to_digit(key, &digit))
            {
                if (machine_type == MACHINE_TYPE_MILL)
                {
                    if (mill_speedkey[digit])
                        mill_speedkey[digit]();
                    else
                        not_machine_type(MACHINE_TYPE_MILL);
                }
                else
                {
                    if (lathe_speedkey[digit])
                        lathe_speedkey[digit]();
                    else
                        not_machine_type(MACHINE_TYPE_LATHE);
                }
                done = true;
            }
            break;
        }
    }

    // Turn off the function indicator if not in progress
    if (function.state == STATE_NORMAL)
        display_indicator_set(INDICATOR_FUNC, false);
}

void
function_save(void)
{
    uint8_t         tool_num;
    uint8_t         bolthole_num;
    uint8_t         grid_num;
    uint8_t         mem_num;
    uint8_t         radius_num;

    if (config_obj_write(CONFIG_OID_FUNCTION, 0))
    {
        for (tool_num = 0; tool_num < TOOL_MAX; tool_num++)
        {
            config_var_write(VID_TOOL, tool_num,
                             sizeof(function.tool[tool_num]),
                             &function.tool[tool_num]);
        }

        config_var_write(VID_EDGE, 0,
                         sizeof(function.edge),
                         &function.edge);
        config_var_write(VID_TOOL_NUM, 0,
                         sizeof(function.tool_num),
                         &function.tool_num);

        for (bolthole_num = 0; bolthole_num < BOLTHOLE_MAX; bolthole_num++)
        {
            config_var_write(VID_BOLTHOLE, bolthole_num,
                             sizeof(function.bolthole[bolthole_num]),
                             &function.bolthole[bolthole_num]);
        }

        for (grid_num = 0; grid_num < GRID_MAX; grid_num++)
        {
            config_var_write(VID_GRID, grid_num,
                             sizeof(function.grid[grid_num]),
                             &function.grid[grid_num]);
        }

        for (mem_num = 0; mem_num < MEMORIES_MAX; mem_num++)
        {
            config_var_write(VID_MEMORY, mem_num,
                             sizeof(function.calc_mem[mem_num]),
                             &function.calc_mem[mem_num]);
        }

        for (radius_num = 0; radius_num < RADIUS_MAX; radius_num++)
        {
            config_var_write(VID_RADIUS, radius_num,
                             sizeof(function.radius[radius_num]),
                             &function.radius[radius_num]);
        }

        config_var_write(VID_TOOL_NUM, 0,
                         sizeof(function.tool_num),
                         &function.tool_num);
        config_var_write(VID_DIAMETER, 0,
                         sizeof(function.diameter),
                         &function.diameter);
        config_var_write(VID_COMPOUND, 0,
                         sizeof(function.compound),
                         &function.compound);
        config_var_write(VID_WORKSPACE, 0,
                         sizeof(function.workspace),
                         &function.workspace);

        config_obj_close();
    }
}

void
function_show(FILE* out)
{
    uint8_t     n;

#define FIELD_WIDTH     12

    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "State", state_names[function.state]);
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Step", function.step);
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Tool Num", function.tool_num);
    fprintf(out, "%-*s :\n", FIELD_WIDTH, "Tools");
    for (n = 0; n < TOOL_MAX; n++)
    {
        if (function.tool[n].offset1 == 0.0 &&
            function.tool[n].offset2 == 0.0)
        {
            fprintf(out, "%-*s : [%d] undef\n", FIELD_WIDTH, "", n);
        }
        else
        {
            fprintf(out, "%-*s : [%d] %f %f %s\n", FIELD_WIDTH, "", n,
                   function.tool[n].offset1,
                   function.tool[n].offset2,
                   unit_names[function.tool[n].units]);
        }
    }
    if (machine_get_type() == MACHINE_TYPE_MILL)
    {
        fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Edge", edge_names[function.edge]);
        fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Bolthole Num", function.bolthole_num);
        fprintf(out, "%-*s :\n", FIELD_WIDTH, "Boltholes");
        for (n = 0; n < BOLTHOLE_MAX; n++)
        {
            if (function.bolthole[n].radius == 0.0)
            {
                fprintf(out, "%-*s : [%d] undef\n", FIELD_WIDTH, "", n);
            }
            else
            {
                fprintf(out, "%-*s : [%d] %d %f %f %f %s\n", FIELD_WIDTH, "", n,
                       function.bolthole[n].holes,
                       function.bolthole[n].radius,
                       function.bolthole[n].start,
                       function.bolthole[n].end,
                       unit_names[function.bolthole[n].units]);
            }
        }
        fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Grid Num", function.grid_num);
        fprintf(out, "%-*s :\n", FIELD_WIDTH, "Grids");
        for (n = 0; n < GRID_MAX; n++)
        {
            if (function.grid[n].xdim == 0.0 &&
                function.grid[n].ydim == 0.0)
            {
                fprintf(out, "%-*s : [%d] undef\n", FIELD_WIDTH, "", n);
            }
            else
            {
                fprintf(out, "%-*s : [%d] %f %f %d %d %f %s\n", FIELD_WIDTH, "", n,
                       function.grid[n].xdim,
                       function.grid[n].ydim,
                       function.grid[n].xholes,
                       function.grid[n].yholes,
                       function.grid[n].angle,
                       unit_names[function.grid[n].units]);
            }
        }
        fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Radius Num", function.radius_num);
        fprintf(out, "%-*s :\n", FIELD_WIDTH, "Radii");
        for (n = 0; n < RADIUS_MAX; n++)
        {
            if (function.radius[n].radius == 0.0)
            {
                fprintf(out, "%-*s : [%d] undef\n", FIELD_WIDTH, "", n);
            }
            else
            {
                fprintf(out, "%-*s : [%d] %f %s %s %d %f %f %s\n", FIELD_WIDTH, "", n,
                       function.radius[n].radius,
                       shape_names[function.radius[n].shape],
                       machine_axis_get_label(function.radius[n].axis),
                       function.radius[n].steps,
                       function.radius[n].start,
                       function.radius[n].end,
                       unit_names[function.radius[n].units]);
            }
        }
        fprintf(out, "%-*s : %f\n", FIELD_WIDTH, "X Zero Save", function.x_zero);
        fprintf(out, "%-*s : %f\n", FIELD_WIDTH, "Y Zero Save", function.y_zero);
    }
    else
    {
        fprintf(out, "%-*s : %f\n", FIELD_WIDTH, "Compound", function.compound);
        fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Diameter", function.diameter ? "on" : "off");
        fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Diam Axis", function.diameter_axis);
    }
    fprintf(out, "%-*s :\n", FIELD_WIDTH, "Calc Stack");
    for (n = 0; n < CALC_SIZE; n++)
    {
        fprintf(out, "%-*s : [%d] %f\n", FIELD_WIDTH, "", n, function.calc_stack[n]);
    }
}

void
function_init(void)
{
    uint8_t         tool_num;
    uint8_t         bolthole_num;
    uint8_t         grid_num;
    uint8_t         mem_num;
    uint8_t         radius_num;

    // Set non-zero default values
    function.tool_num = TOOL_NONE;

    if (config_obj_read(CONFIG_OID_FUNCTION, 0))
    {
        for (tool_num = 0; tool_num < TOOL_MAX; tool_num++)
        {
            config_var_read(VID_TOOL, tool_num,
                            sizeof(function.tool[tool_num]),
                            &function.tool[tool_num]);
        }

        config_var_read(VID_EDGE, 0,
                        sizeof(function.edge),
                        &function.edge);

        config_var_read(VID_TOOL_NUM, 0,
                        sizeof(function.tool_num),
                        &function.tool_num);
        if (function.tool_num >= TOOL_MAX)
            function.tool_num = TOOL_NONE;

        for (bolthole_num = 0; bolthole_num < BOLTHOLE_MAX; bolthole_num++)
        {
            config_var_read(VID_BOLTHOLE, bolthole_num,
                            sizeof(function.bolthole[bolthole_num]),
                            &function.bolthole[bolthole_num]);
        }

        for (grid_num = 0; grid_num < GRID_MAX; grid_num++)
        {
            config_var_read(VID_GRID, grid_num,
                            sizeof(function.grid[grid_num]),
                            &function.grid[grid_num]);
        }

        for (mem_num = 0; mem_num < MEMORIES_MAX; mem_num++)
        {
            config_var_read(VID_MEMORY, mem_num,
                            sizeof(function.calc_mem[mem_num]),
                            &function.calc_mem[mem_num]);
        }

        for (radius_num = 0; radius_num < RADIUS_MAX; radius_num++)
        {
            config_var_read(VID_RADIUS, radius_num,
                            sizeof(function.radius[radius_num]),
                            &function.radius[radius_num]);
        }

        config_var_read(VID_DIAMETER, 0,
                        sizeof(function.diameter),
                        &function.diameter);
        config_var_read(VID_DIAMETER_AXIS, 0,
                        sizeof(function.diameter_axis),
                        &function.diameter_axis);
        config_var_read(VID_COMPOUND, 0,
                        sizeof(function.compound),
                        &function.compound);

        config_var_read(VID_WORKSPACE, 0,
                        sizeof(function.workspace),
                        &function.workspace);
        if (function.workspace >= WORKSPACE_MAX)
            function.workspace = 0;


        config_obj_close();
    }

    display_status_set(STATUS_WORKSPACE, 1);
    display_status_set(STATUS_FUNC_STATE, STATE_NORMAL);

    // Set the active machine to update our dynamic state
    function_set_active();
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
function_run_lua(lua_State* L)
{
    const char*      group_name;
    const char*      command_name;
    const group_t*   group_ptr;
    const command_t* command_ptr;
    int8_t           command_num;
    int8_t           group_num;

    group_name = luaL_checkstring(L, 1);
    command_name = luaL_checkstring(L, 2);

    if (function.state != STATE_NORMAL)
        return luaL_error(L, "function already running");
   
    if (machine_get_type() == MACHINE_TYPE_MILL)
        group_ptr = mill_group;
    else
        group_ptr = lathe_group;

    for (group_num = 0; group_num < GROUP_MAX; group_num++)
    {
        if (group_ptr[group_num].name &&
            strcasecmp(group_ptr[group_num].name, group_name) == 0)
            break;
    }
    if (group_num == GROUP_MAX)
        return luaL_error(L, "group not found");

    for (command_num = 0; command_num < COMMAND_MAX; command_num++)
    {
        command_ptr = &group_ptr[group_num].command[command_num];
        if (command_ptr->name &&
            strcasecmp(command_ptr->name, command_name) == 0)
            break;
    }
    if (command_num == COMMAND_MAX)
        return luaL_error(L, "command not found");

    display_indicator_set(INDICATOR_FUNC, true);

    command_ptr->process();

    if (function.state == STATE_NORMAL)
        display_indicator_set(INDICATOR_FUNC, false);

    return 0;
}

static int
function_tool_lua(lua_State* L)
{
    int tool_num;

    if (lua_gettop(L) == 0)
    {
        if (function.tool_num == TOOL_NONE)
            lua_pushinteger(L, 0);
        else
            lua_pushinteger(L, function.tool_num + 1);
        return 1;
    }

    tool_num = luaL_checkinteger(L, 1);
    if (tool_num == 0)
    {
        tool_num = TOOL_NONE;
    }
    else
    {
        tool_num--;
        if (tool_num < 0 || tool_num >= TOOL_MAX)
            return luaL_error(L, "invalid tool");

        // Disallow setting undefined tool as active
        if (function.tool[tool_num].offset1 == 0.0 &&
            function.tool[tool_num].offset2 == 0.0)
             return luaL_error(L, "undefined tool");
    }

    function.tool_num = tool_num;
    tool_use();

    return 0;
}

static int
function_edge_lua(lua_State* L)
{
    const char* name;
    int edge;

    if (lua_gettop(L) == 0)
    {
        lua_pushstring(L, edge_names[function.edge]);
        return 1;
    }

    name = luaL_checkstring(L, 1);
    for (edge = 0; edge < ARRAY_SIZE(edge_names); edge++)
        if (strcasecmp(name, edge_names[edge]) == 0)
            break;

    if (edge == ARRAY_SIZE(edge_names))
        return luaL_error(L, "invalid edge");

    function.edge = edge;
    edge_use();

    return 0;
}

static int
function_diameter_lua(lua_State* L)
{
    int axis;

    if (lua_gettop(L) == 0)
    {
        lua_pushboolean(L, function.diameter);
        lua_pushinteger(L, function.diameter_axis + 1);
        return 2;
    }

    if (function.diameter)
    {
        function.diameter = false;
        diameter_use();
        return 0;
    }

    axis = machine_axis_by_label(luaL_checkstring(L, 1));
    if (axis >= AXIS_MAX)
        return luaL_error(L, "invalid axis");

    function.diameter = true;
    function.diameter_axis = axis;    
    diameter_use();

    return 0;
}

static int
function_compound_lua(lua_State* L)
{
    double compound;

    if (lua_gettop(L) == 0)
    {
        lua_pushnumber(L, function.compound);
        return 1;
    }

    compound = luaL_checknumber(L, 1);
    function.compound = compound;
    compound_use();
    
    return 0;
}

static int
function_workspace_lua(lua_State* L)
{
    int workspace;

    if (lua_gettop(L) == 0)
    {
        lua_pushinteger(L, function.workspace + 1);
        return 1;
    }

    workspace = luaL_checkinteger(L, 1) - 1;
    if (workspace >= WORKSPACE_MAX)
        return luaL_error(L, "invalid workspace");

    function.workspace = workspace;
    workspace_use();
    
    return 0;
}

static int
function_tach_lua(lua_State* L)
{
    uint8_t line;
    bool surface;
    
    machine_get_tach(&line, &surface);
    if (line < DISPLAY_LINE_MAX)
    {
        machine_set_tach(DISPLAY_LINE_MAX, 0);
        return 0;
    }
    
    line = luaL_checkinteger(L, 1) - 1;
    if (line >= DISPLAY_LINE_MAX)
        return luaL_error(L, "invalid line");

    surface = lua_toboolean(L, 2);
    
    machine_set_tach(line, surface);
    
    return 0;
}

static int
function_feed_lua(lua_State* L)
{
    uint8_t axis;
    uint8_t line;

    machine_get_feed(&line, &axis);
    if (line < DISPLAY_LINE_MAX)
    {
        machine_set_feed(DISPLAY_LINE_MAX, 0);
        return 0;
    }

    line = luaL_checkinteger(L, 1) - 1;
    if (line >= DISPLAY_LINE_MAX)
        return luaL_error(L, "invalid line");

    axis = machine_axis_by_label(luaL_checkstring(L, 2));
    if (axis >= AXIS_MAX)
        return luaL_error(L, "invalid axis");
        
    machine_set_feed(line, axis);
    
    return 0;
}

static int
function_tooldef_lua(lua_State* L)
{
    int tool_num;
    double offset1;
    double offset2;
    axis_units_t units;

    tool_num = luaL_checkinteger(L, 1) - 1;
    if ((tool_num < 0) || (tool_num >= TOOL_MAX))
        return luaL_error(L, "invalid tool number");
        
    if (lua_gettop(L) == 1)
    {
        if (function.tool[tool_num].offset1 == 0.0 &&
            function.tool[tool_num].offset2 == 0.0)
        {
            return 0;
        }
        else
        {
            lua_pushnumber(L, function.tool[tool_num].offset1);
            lua_pushnumber(L, function.tool[tool_num].offset2);
            lua_pushstring(L, unit_names[function.tool[tool_num].units]);
            return 3;
        }
    }

    // Get the machine units
    units = machine_get_units();

    offset1 = luaL_checknumber(L, 2);
    offset2 = luaL_checknumber(L, 3);

    // Save the tool offsets
    tool_save(tool_num, offset1, offset2, units);

    // Reset active tool if this tool def has been reset and was the active tool
    if ((offset1==0) && (offset2==0) && (function.tool_num == tool_num))
    {
        function.tool_num = TOOL_NONE;
        tool_use();
    }

    return 0;
}

static int
function_save_lua(lua_State* L)
{
    function_save();
    return 0;
}

static int
function_show_lua(lua_State* L)
{
    function_show(luash_out(L));
    return 0;
}

const LUA_REG_TYPE function_map_lua[] =
{
    { LSTRKEY("run"), LFUNCVAL(function_run_lua) },
    { LSTRKEY("tool"), LFUNCVAL(function_tool_lua) },
    { LSTRKEY("edge"), LFUNCVAL(function_edge_lua) },
    { LSTRKEY("diameter"), LFUNCVAL(function_diameter_lua) },
    { LSTRKEY("compound"), LFUNCVAL(function_compound_lua) },
    { LSTRKEY("workspace"), LFUNCVAL(function_workspace_lua) },
    { LSTRKEY("tach"), LFUNCVAL(function_tach_lua) },
    { LSTRKEY("feed"), LFUNCVAL(function_feed_lua) },
    { LSTRKEY("tooldef"), LFUNCVAL(function_tooldef_lua) },
    { LSTRKEY("save"), LFUNCVAL(function_save_lua) },
    { LSTRKEY("show"), LFUNCVAL(function_show_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
function_open_lua(lua_State* L)
{
    LREGISTER(L, "func", function_map_lua);
}

