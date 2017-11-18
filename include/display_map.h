///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Display device map
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
#ifndef DISPLAY_MAP_H_
#define DISPLAY_MAP_H_

#include "types.h"
#include "drolim.h"
#include "display_dev.h"

/// Map the display functions to the device that controls them
typedef struct display_map
{
    uint8_t line[DISPLAY_LINE_MAX];                 ///<
    const display_dev_t* reading[DISPLAY_LINE_MAX]; ///<
    const display_dev_t* menu;                      ///<
    const display_dev_t* status;                    ///<
    const display_dev_t* indicator;                 ///<
    const display_dev_t* devs[DISPLAY_DEV_MAX];     ///<
} display_map_t;

extern display_map_t display_map;

void
display_map_init(void);

#endif // DISPLAY_MAP_H_
