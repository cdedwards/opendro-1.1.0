///////////////////////////////////////////////////////////////////////////////
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
#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <stdio.h>
#include "keypad.h"
#include "axis.h"

/// State for pending function operations
typedef enum func_state
{
    STATE_NORMAL,                           ///< Normal i.e. no operation is pending
    STATE_BOLTHOLE,                         ///< Bolthole operation is pending
    STATE_GRID,                             ///< Grid pattern is pending
    STATE_RADIUS,                           ///< Radius operation is pending
    STATE_CALC,                             ///< Calculator is in operation
} func_state_t;

/// Edge compensation position
typedef enum edge
{
    EDGE_CENTER,                            ///< Center position
    EDGE_FRONT,                             ///< Front edge
    EDGE_REAR,                              ///< Rear edge
    EDGE_LEFT,                              ///< Left edge
    EDGE_RIGHT,                             ///< Right edge
} edge_t;

/// \brief  Get the cutting diameter using the current tool
///
/// \param  units           Units to return diameter in
///
/// \return The cutting diameter
double
function_get_cut_dia(axis_units_t   units);

/// \brief  Notify the functions that there is a new active machine
void
function_set_active(void);

/// \brief  Turn functions off
void
function_off(void);

/// \brief  Turn functions on
void
function_on(void);

/// \brief  Report a key press to the functions
///
/// \param  key             Key press
void
function_key(keypad_t   key);

/// \brief  Select a function
void
function_select(void);

/// \brief  Save all function configuration to non-volatile memory 
void
function_save(void);

/// \brief  Show the function state on the console
///
/// \param  out             File pointer to send output to
void
function_show(FILE* out);

/// \brief  Initialize the functions from non-volatile memory
void
function_init(void);

#endif // FUNCTION_H_
