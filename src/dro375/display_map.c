///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// DRO-375 display device driver
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

#include "display_map.h"
#include "led.h"
#include "lcd.h"

///////////////////////////////////////////////////////////////////////////////
// G L O B A L   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

display_map_t display_map;

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
display_map_init(void)
{
    display_map.line[0]     = 0;
    display_map.line[1]     = 1;
    display_map.line[2]     = 2;

    display_map.reading[0]  = &led_display_dev;
    display_map.reading[1]  = &led_display_dev;
    display_map.reading[2]  = &led_display_dev;

    display_map.menu        = &led_display_dev;
    display_map.status      = &led_display_dev;
    display_map.indicator   = &led_display_dev;

    display_map.devs[0]     = &led_display_dev;
 }
