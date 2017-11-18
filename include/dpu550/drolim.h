///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Size limits on objects and other components
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
#ifndef DPU550_DROLIM_H_
#define DPU550_DROLIM_H_

#ifndef PLATFORM_dpu550
#  error "DPU-550 platform is not defined"
#endif

#if CONFIG_base
#define AXIS_MAX            5   ///< Number of axes
#elif CONFIG_lite
#define AXIS_MAX            3   ///< Number of axes
#endif
#define WORKSPACE_MAX       10  ///< Number of workspaces
#if CONFIG_base
#  define SCALE_MAX         5   ///< Total number of scales
#elif CONFIG_lite
#  define SCALE_MAX         3   ///< Total number of scales
#endif
#define SCALE_IN_MAX        2   ///< Number of inputs per scale
#define FILTER_MAX          32  ///< Maximum size of scale filter
#define LED_COLUMN_MAX      6   ///< Number of columns in the LED display
#define LED_LINE_MAX        3   ///< Number of lines in the LED display
#  define DISPLAY_LINE_MAX  LED_LINE_MAX ///< Number reading display lines
#define MENU_WIDTH          6   ///< Number of characters in menu
#define KEYPAD_ROW_MAX      5   ///< Number of rows in the keypad
#define KEYPAD_COLUMN_MAX   5   ///< Number of columns in the keypad
#define MACHINE_MAX         2   ///< Total number of virtual machines
#define TOOL_MAX            16  ///< Total number of tools
#if CONFIG_base
#  define TACH_MAX          2   ///< Total number of tachometers
#  define EDGE_MAX          2   ///< Total number of edge finders
#elif CONFIG_lite
#  define TACH_MAX          2   ///< Total number of tachometers
#  define EDGE_MAX          1   ///< Total number of edge finders
#endif
#define PRECISION_MAX       8   ///< Floating point precision (half-digits)
#define BOLTHOLE_MAX        10  ///< Total number of bolthole circles
#define GRID_MAX            10  ///< Total number of hole grids
#define RADIUS_MAX          10  ///< Total number of radii
#define DISPLAY_DEV_MAX     1   ///< Number of display devices

#endif // DPU550_DROLIM_H_
