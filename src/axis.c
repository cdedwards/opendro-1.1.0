///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Axis of measurement object
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
#include <math.h>
#include "axis.h"
#include "scale.h"
#include "display.h"
#include "keypad.h"
#include "drolim.h"
#include "config.h"
#include "function.h"
#include "assert.h"
#include "trace.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

/// Amount to adjust the precision for metric display
#define AXIS_MM_PRECISION   2

/// Object representing each axis in the DRO (ALWAYS uses inches as unit of measure)
typedef struct axis
{
    uint8_t     scale;                      ///< Scale object associated with this axis
    uint8_t     summing;                    ///< Scale summing into this axis
    uint8_t     precision;                  ///< Number of half-digits of display precision
    uint8_t     workspace;                  ///< Active workspace
    uint8_t     vector_axis;                ///< Axis to vector into this axis
    bool        reverse;                    ///< Reverse the axis polarity
    double      absolute[WORKSPACE_MAX];    ///< Absolute zeros
    double      incremental[WORKSPACE_MAX]; ///< Incremental zeros
    double      tool_offset;                ///< Tool offset amount
    double      vector_factor;              ///< Amount to vector into this axis
    double      scale_factor;               ///< Amount to factor scale into this axis
    double      lec_factor;                 ///< Linear error correction factor
} axis_t;

/// Configuration variable IDs 
typedef enum axis_vid
{
    VID_SCALE,          ///< Scale object associated with this axis 
    VID_SUMMING,        ///< Scale summing into this axis 
    VID_REVERSE,        ///< Reverse the axis polarity
    VID_PRECISION,      ///< Number of half-digits of display precision
    VID_WORKSPACE,      ///< Active workspace
    VID_ABSOLUTE,       ///< Absolute zeros
    VID_INCREMENTAL,    ///< Incremental zeros
    VID_LEC_FACTOR,     ///< Linear error correction factor
    VID_MAX             ///< MUST be last!!!
} axis_vid_t;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

/// Axis object array
static axis_t axis[AXIS_MAX];

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

// Convert from the axis internal units to the specified units
static double
axis_to_units(double        value,
              axis_units_t  units)
{
    if (units == AXIS_UNITS_MM)
        value *= INCH_TO_MM;
    
    return value;
}

// Convert from the specified units to the axis internal units
static double
axis_from_units(double          value,
                axis_units_t    units)
{
    if (units == AXIS_UNITS_MM)
        value *= MM_TO_INCH;
    
    return value;
}

// Show the selected scale
static void
axis_display_scale(uint8_t  obj_inst,
                   uint8_t  var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];

    display_menu_printf(MENU_BOTTOM, "%d", axis_ptr->scale + 1);
}

// Iterate through the scales
static void
axis_select_scale(uint8_t   obj_inst,
                  uint8_t   var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];

    axis_ptr->scale++;
    if (axis_ptr->scale == SCALE_MAX)
        axis_ptr->scale = 0;
}

// Show the summing scale
static void
axis_display_summing(uint8_t  obj_inst,
                     uint8_t  var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];

    if (axis_ptr->summing == SCALE_OFF)
        display_menu_puts(MENU_BOTTOM, "off");
    else
        display_menu_printf(MENU_BOTTOM, "scale%d", axis_ptr->summing + 1);
}

// Iterate through the scales
static void
axis_select_summing(uint8_t   obj_inst,
                    uint8_t   var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];

    if (axis_ptr->summing == SCALE_OFF)
        axis_ptr->summing = 0;
    else if (axis_ptr->summing == SCALE_MAX - 1)
        axis_ptr->summing = SCALE_OFF;
    else
        axis_ptr->summing++;
}

// Show the axis reverse setting
static void
axis_display_reverse(uint8_t    obj_inst,
                     uint8_t    var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];

    display_menu_puts(MENU_BOTTOM, axis_ptr->reverse ? "yes" : "no");
}

// Toggle the axis reverse setting
static void
axis_select_reverse(uint8_t obj_inst,
                    uint8_t var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];

    axis_ptr->reverse = (axis_ptr->reverse ? false : true);
}

// Show the display precision
static void
axis_display_precision(uint8_t  obj_inst,
                       uint8_t  var_inst)
{
    char*   string;
    axis_t* axis_ptr = &axis[obj_inst];

    if (axis_ptr->precision == PRECISION_FRACTION)
        string = "fract";
    else if (axis_ptr->precision == PRECISION_MAX)
        string = "high";
    else if (axis_ptr->precision == PRECISION_MAX - 1)
        string = "med";
    else
        string = "low";
        
    display_menu_puts(MENU_BOTTOM, string);
}

// Iterate through the display precisions
static void
axis_select_precision(uint8_t   obj_inst,
                      uint8_t   var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];

    if (axis_ptr->precision == PRECISION_FRACTION)
        axis_ptr->precision = PRECISION_MAX;
    else if (axis_ptr->precision == PRECISION_MAX)
        axis_ptr->precision = PRECISION_MAX - 1;
    else if (axis_ptr->precision == PRECISION_MAX - 1)
        axis_ptr->precision = PRECISION_MAX - 2;
    else
        axis_ptr->precision = PRECISION_FRACTION;
}

// Show the LEC factor
static void
axis_display_lec(uint8_t    obj_inst,
                 uint8_t    var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];
    int32_t number;
    
    number = (axis_ptr->lec_factor - 1.0) * 1000000.0 + 0.5;
    display_menu_printf(MENU_BOTTOM, "%"PRIi32, number);
}

// Enter the LEC factor via the keyboard
static void
axis_select_lec(uint8_t obj_inst,
                uint8_t var_inst)
{
    axis_t* axis_ptr = &axis[obj_inst];
    int32_t number;
    
    number = (axis_ptr->lec_factor - 1.0) * 1000000.0;
    if (keypad_get_number(MENU_BOTTOM, -9999, 9999, &number))
        axis_ptr->lec_factor = 1.0 + (number / 1000000.0);
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
axis_set_scale_factor(uint8_t   axis_num,
                      double    scale_factor)
{
    ASSERT(axis_num < AXIS_MAX);

    axis[axis_num].scale_factor = scale_factor;
}

double
axis_get_scale_factor(uint8_t   axis_num)
{
    ASSERT(axis_num < AXIS_MAX);

    return axis[axis_num].scale_factor;
}

void
axis_set_tool_offset(uint8_t        axis_num,
                     double         tool_offset,
                     axis_units_t   units)
{
    ASSERT(axis_num < AXIS_MAX);

    axis[axis_num].tool_offset = axis_from_units(tool_offset, units);
}

double
axis_get_tool_offset(uint8_t        axis_num,
                     axis_units_t   units)
{
    ASSERT(axis_num < AXIS_MAX);

    return axis_to_units(axis[axis_num].tool_offset, units);
}

void
axis_set_workspace(uint8_t  axis_num,
                   uint8_t  workspace)
{
    ASSERT(axis_num < AXIS_MAX);
    ASSERT(workspace < WORKSPACE_MAX);

    axis[axis_num].workspace = workspace;
}

void
axis_set_absolute(uint8_t       axis_num,
                  double        absolute,
                  axis_units_t  units)
{
    uint8_t workspace;
    axis_t* axis_ptr;
        
    ASSERT(axis_num < AXIS_MAX);

    axis_ptr = &axis[axis_num];
    workspace = axis_ptr->workspace;

    // Undo scale reversing
    if (axis_ptr->reverse)
        absolute = -absolute;

    // Undo scale factoring
    absolute /= axis_ptr->scale_factor;

    // Undo linear error correction
    absolute /= axis_ptr->lec_factor;

    axis_ptr->absolute[workspace] += axis_from_units(absolute, units);

    if (config_obj_write(CONFIG_OID_AXIS, axis_num))
    {
        config_var_write(VID_ABSOLUTE, workspace,
                         sizeof(axis_ptr->absolute[workspace]),
                         &axis_ptr->absolute[workspace]);                
        config_obj_close();
    }
}

void
axis_set_incremental(uint8_t        axis_num,
                     double         incremental,
                     axis_units_t   units)
{
    uint8_t workspace;
    axis_t* axis_ptr;
    
    ASSERT(axis_num < AXIS_MAX);

    axis_ptr = &axis[axis_num];
    workspace = axis_ptr->workspace;

    // Undo scale reversing
    if (axis_ptr->reverse)
        incremental = -incremental;

    // Undo scale factoring
    incremental /= axis_ptr->scale_factor;

    // Undo linear error correction
    incremental /= axis_ptr->lec_factor;

    axis_ptr->incremental[workspace] = axis_from_units(incremental, units);

    if (config_obj_write(CONFIG_OID_AXIS, axis_num))
    {
        config_var_write(VID_INCREMENTAL, workspace,
                         sizeof(axis_ptr->incremental[workspace]),
                         &axis_ptr->incremental[workspace]);             
        config_obj_close();
    }
}

double
axis_get_absolute(uint8_t       axis_num,
                  axis_units_t  units)
{
    uint8_t workspace;
    axis_t* axis_ptr;
    double  absolute;
    
    ASSERT(axis_num < AXIS_MAX);

    axis_ptr = &axis[axis_num];
    workspace = axis_ptr->workspace;

    absolute = axis_ptr->absolute[workspace] * axis_ptr->scale_factor;

    if (axis_ptr->reverse)
        absolute = -absolute;
    
    return axis_to_units(absolute, units);    
}

double
axis_get_incremental(uint8_t        axis_num,
                     axis_units_t   units)
{
    uint8_t workspace;
    axis_t* axis_ptr;
    double  incremental;
    
    ASSERT(axis_num < AXIS_MAX);

    axis_ptr = &axis[axis_num];
    workspace = axis_ptr->workspace;

    incremental = axis_ptr->incremental[workspace] * axis_ptr->scale_factor;    

    if (axis_ptr->reverse)
        incremental = -incremental;

    return axis_to_units(incremental, units);
}

uint8_t
axis_get_scale(uint8_t  axis_num)
{
    ASSERT(axis_num < AXIS_MAX);

    return axis[axis_num].scale;
}

double
axis_get_reading(uint8_t        axis_num,
                 axis_units_t   units,
                 axis_zero_t    zero)
{
    int32_t     count;
    uint32_t    cpi;
    double      reading;
    axis_t*     axis_ptr;

    ASSERT(axis_num < AXIS_MAX);

    axis_ptr = &axis[axis_num];

    // Calculate the reading from the scale associated to this axis 
    count = scale_get_count(axis_ptr->scale);
    cpi = scale_get_cpi(axis_ptr->scale);
    reading = (double) count / cpi;

    // Scale summing
    if (axis_ptr->summing < SCALE_MAX)
    {
        count = scale_get_count(axis_ptr->summing);
        cpi = scale_get_cpi(axis_ptr->summing);
        reading += (double) count / cpi;
    }
    
    // Add the vectored axis
    if (axis_ptr->vector_axis != AXIS_OFF)
        reading += axis_get_reading(axis_ptr->vector_axis, AXIS_UNITS_INCH, AXIS_ZERO_ABS) * axis_ptr->vector_factor;

    // Subtract the absolute zero
    reading -= axis_ptr->absolute[axis_ptr->workspace];
    
    // Subtract the incremental zero
    if (zero == AXIS_ZERO_INC)
        reading -= axis_ptr->incremental[axis_ptr->workspace];

    // Add the tool offset
    reading += axis_ptr->tool_offset;
    
    // Multiply by the linear error correction factor
    reading *= axis_ptr->lec_factor;

    // Multiply by the scale factor
    reading *= axis_ptr->scale_factor;

    // Adjust for reverse polarity
    if (axis_ptr->reverse)
        reading = -reading; 

    return axis_to_units(reading, units);
}

uint8_t
axis_get_precision(uint8_t      axis_num,
                   axis_units_t units)
{
    axis_t* axis_ptr;

    ASSERT(axis_num < AXIS_MAX);

    axis_ptr = &axis[axis_num];

    if (units == AXIS_UNITS_MM && axis_ptr->precision != PRECISION_FRACTION)
        return axis_ptr->precision - AXIS_MM_PRECISION;
    
    return axis_ptr->precision;
}

void
axis_set_vector(uint8_t    axis_num,
                uint8_t    vector_axis,
                double     vector_factor)
{
    axis_t* axis_ptr;

    ASSERT(axis_num < AXIS_MAX);

    axis_ptr = &axis[axis_num];
    axis_ptr->vector_axis = vector_axis;
    axis_ptr->vector_factor = vector_factor;
}

double
axis_convert_units(double value,
                   axis_units_t from_units,
                   axis_units_t to_units)
{
    if (from_units == to_units)
        return value;
        
    if (from_units == AXIS_UNITS_MM)
        value *= MM_TO_INCH;
    else
        value *= INCH_TO_MM;
        
    return value;
}

void
axis_save(void)
{
    axis_t* axis_ptr;
    uint8_t axis_num;
    uint8_t workspace_num;
    
    for (axis_num = 0; axis_num < AXIS_MAX; axis_num++)
    {
        axis_ptr = &axis[axis_num];
        
        if (config_obj_write(CONFIG_OID_AXIS, axis_num))
        {
            config_var_write(VID_SCALE, 0, sizeof(axis_ptr->scale), &axis_ptr->scale);
            config_var_write(VID_SUMMING, 0, sizeof(axis_ptr->summing), &axis_ptr->summing);
            config_var_write(VID_REVERSE, 0, sizeof(axis_ptr->reverse), &axis_ptr->reverse);
            config_var_write(VID_PRECISION, 0, sizeof(axis_ptr->precision), &axis_ptr->precision);
            config_var_write(VID_WORKSPACE, 0, sizeof(axis_ptr->workspace), &axis_ptr->workspace);
            config_var_write(VID_LEC_FACTOR, 0, sizeof(axis_ptr->lec_factor), &axis_ptr->lec_factor);
    
            for (workspace_num = 0; workspace_num < AXIS_MAX; workspace_num++)
            {
                config_var_write(VID_ABSOLUTE, workspace_num,
                                 sizeof(axis_ptr->absolute[workspace_num]),
                                 &axis_ptr->absolute[workspace_num]);
                    
                config_var_write(VID_INCREMENTAL, workspace_num,
                                 sizeof(axis_ptr->incremental[workspace_num]),
                                 &axis_ptr->incremental[workspace_num]);             
            }
            
            config_obj_close();
        }
    }
}

void
axis_setup(void)
{
    static const var_list_t var_list[] = {
        { "scale",  0,  axis_display_scale,     axis_select_scale },
        { "sumng",  0,  axis_display_summing,   axis_select_summing },
        { "revers", 0,  axis_display_reverse,   axis_select_reverse },
        { "precsn", 0,  axis_display_precision, axis_select_precision },
        { "lec",    0,  axis_display_lec,       axis_select_lec },
    };

    if (config_setup("axis%d", AXIS_MAX, ARRAY_SIZE(var_list), var_list))
        axis_save();
}

void
axis_show(FILE* out, uint8_t axis_num)
{
    axis_t* axis_ptr;
    uint8_t n;

    ASSERT(axis_num < AXIS_MAX);

#define FIELD_WIDTH     14

    axis_ptr = &axis[axis_num];
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Axis", axis_num);
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Scale", axis_ptr->scale);
    fprintf(out, "%-*s : ", FIELD_WIDTH, "Summing");
    if (axis_ptr->summing == SCALE_OFF)
        fprintf(out, "off\n");
    else
        fprintf(out, "%d\n", axis_ptr->summing);
    fprintf(out, "%-*s : %s\n", FIELD_WIDTH, "Reverse", axis_ptr->reverse ? "yes" : "no");
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Precision", axis_ptr->precision);
    fprintf(out, "%-*s : %d\n", FIELD_WIDTH, "Workspace", axis_ptr->workspace);
    fprintf(out, "%-*s :\n", FIELD_WIDTH, "Absolute");
    for (n = 0; n < WORKSPACE_MAX; n++)
        fprintf(out, "%*s : [%d] %f\n", FIELD_WIDTH, "", n, axis_ptr->absolute[n]);
    fprintf(out, "%-*s :\n", FIELD_WIDTH, "Incremental");
    for (n = 0; n < WORKSPACE_MAX; n++)
        fprintf(out, "%*s : [%d] %f\n", FIELD_WIDTH, "", n, axis_ptr->incremental[n]);
    fprintf(out, "%-*s : %f\n", FIELD_WIDTH, "Tool Offset", axis_ptr->tool_offset);
    fprintf(out, "%-*s : ", FIELD_WIDTH, "Vector Axis");
    if (axis_ptr->vector_axis == AXIS_OFF)
        fprintf(out, "off\n");
    else
        fprintf(out, "%d\n", axis_ptr->vector_axis);
    fprintf(out, "%-*s : %f\n", FIELD_WIDTH, "Vector Factor", axis_ptr->vector_factor);
    fprintf(out, "%-*s : %f\n", FIELD_WIDTH, "Scale Factor", axis_ptr->scale_factor);
    fprintf(out, "%-*s : %f\n", FIELD_WIDTH, "LEC Factor", axis_ptr->lec_factor);
}

void
axis_init(void)
{
    uint8_t axis_num;
    uint8_t workspace_num;
    axis_t* axis_ptr;
    
    for (axis_num = 0; axis_num < AXIS_MAX; axis_num++)
    {
        axis_ptr = &axis[axis_num];

        // Set non-zero default values
        axis_ptr->scale         = axis_num;
        axis_ptr->summing       = SCALE_OFF;
        axis_ptr->precision     = PRECISION_MAX;
        axis_ptr->vector_axis   = AXIS_OFF;
        axis_ptr->vector_factor = 1.0;
        axis_ptr->lec_factor    = 1.0;
        axis_ptr->scale_factor  = 1.0;
        
        if (config_obj_read(CONFIG_OID_AXIS, axis_num))
        {

            config_var_read(VID_SCALE, 0, sizeof(axis_ptr->scale), &axis_ptr->scale);
            if (axis_ptr->scale >= SCALE_MAX)
                axis_ptr->scale = SCALE_OFF;
            config_var_read(VID_SUMMING, 0, sizeof(axis_ptr->summing), &axis_ptr->summing);
            if (axis_ptr->summing >= SCALE_MAX)
                axis_ptr->summing = SCALE_OFF;
            config_var_read(VID_REVERSE, 0, sizeof(axis_ptr->reverse), &axis_ptr->reverse);
            config_var_read(VID_PRECISION, 0, sizeof(axis_ptr->precision), &axis_ptr->precision);
            if (axis_ptr->precision > PRECISION_MAX && axis_ptr->precision != PRECISION_FRACTION)
                axis_ptr->precision = PRECISION_MAX;
            config_var_read(VID_WORKSPACE, 0, sizeof(axis_ptr->workspace), &axis_ptr->workspace);
            if (axis_ptr->workspace >= WORKSPACE_MAX)
                axis_ptr->workspace = 0;
            config_var_read(VID_LEC_FACTOR, 0, sizeof(axis_ptr->lec_factor), &axis_ptr->lec_factor);
    
            for (workspace_num = 0; workspace_num < WORKSPACE_MAX; workspace_num++)
            {
                config_var_read(VID_ABSOLUTE, workspace_num,
                    sizeof(axis_ptr->absolute[workspace_num]),
                    &axis_ptr->absolute[workspace_num]);
                    
                config_var_read(VID_INCREMENTAL, workspace_num,
                    sizeof(axis_ptr->incremental[workspace_num]),
                    &axis_ptr->incremental[workspace_num]);
            }
            
            config_obj_close();
        }
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
#include "machine.h"

static int
axis_read_lua(lua_State* L)
{
    double          reading;
    uint32_t        axis;
    axis_units_t    units;
    axis_zero_t     zero;

    axis = machine_axis_by_label(luaL_checkstring(L, 1));
    if (axis >= AXIS_MAX)
        return luaL_error(L, "invalid axis");

    units = machine_get_units();
    zero = machine_get_zero();
    reading = axis_get_reading(axis, units, zero);

    lua_pushnumber(L, reading);

    return 1;
}

static int
axis_preseta_lua(lua_State* L)
{
    double          absolute;
    uint32_t        axis;
    axis_units_t    units;

    axis = machine_axis_by_label(luaL_checkstring(L, 1));
    if (axis >= AXIS_MAX)
        return luaL_error(L, "invalid axis");
    
    units = machine_get_units();
    absolute = luaL_checknumber(L, 2);

    axis_set_incremental(axis, absolute, units);
    
    return 0;
}

static int
axis_preseti_lua(lua_State* L)
{
    double          absolute;
    uint32_t        axis;
    axis_units_t    units;

    axis = machine_axis_by_label(luaL_checkstring(L, 1));
    if (axis >= AXIS_MAX)
        return luaL_error(L, "invalid axis");
    
    units = machine_get_units();
    absolute = luaL_checknumber(L, 2);

    absolute += axis_get_reading(axis, units, AXIS_ZERO_ABS);
                            
    axis_set_incremental(axis, absolute, units);
    
    return 0;
}
 
static int
axis_zeroa_lua(lua_State* L)
{
    double          absolute;
    uint32_t        axis;
    axis_units_t    units;

    axis = machine_axis_by_label(luaL_checkstring(L, 1));
    if (axis >= AXIS_MAX)
        return luaL_error(L, "invalid axis");
    
    units = machine_get_units();
    absolute = luaL_checknumber(L, 2);

    axis_set_absolute(axis, absolute, units);
    
    return 0;
}

static int
axis_zeroi_lua(lua_State* L)
{
    double          absolute;
    uint32_t        axis;
    axis_units_t    units;

    axis = machine_axis_by_label(luaL_checkstring(L, 1));
    if (axis >= AXIS_MAX)
        return luaL_error(L, "invalid axis");
    
    units = machine_get_units();
    absolute = axis_get_reading(axis, units, AXIS_ZERO_ABS);
    axis_set_incremental(axis, absolute, units);
    
    return 0;
}

static int
axis_save_lua(lua_State* L)
{
    axis_save();
    return 0;
}

static int
axis_show_lua(lua_State* L)
{
    uint32_t        axis;

    axis = machine_axis_by_label(luaL_checkstring(L, 1));
    if (axis >= AXIS_MAX)
        return luaL_error(L, "invalid axis");
    axis_show(luash_out(L), axis);
    return 0;
}

const LUA_REG_TYPE axis_map_lua[] =
{
    { LSTRKEY("read"), LFUNCVAL(axis_read_lua) },
    { LSTRKEY("preseti"), LFUNCVAL(axis_preseti_lua) },
    { LSTRKEY("preseta"), LFUNCVAL(axis_preseta_lua) },
    { LSTRKEY("zeroi"), LFUNCVAL(axis_zeroi_lua) },
    { LSTRKEY("zeroa"), LFUNCVAL(axis_zeroa_lua) },
    { LSTRKEY("save"), LFUNCVAL(axis_save_lua) },
    { LSTRKEY("show"), LFUNCVAL(axis_show_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
axis_open_lua(lua_State* L)
{
    LREGISTER(L, "axis", axis_map_lua);
}
