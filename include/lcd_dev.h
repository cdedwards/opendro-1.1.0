///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// LCD device driver
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
#ifndef LCD_DEV_H_
#define LCD_DEV_H_

#include "types.h"

typedef struct lcd_dev
{
    void (*clear)   (void);             ///< Clear the LCD display
    void (*on)      (void);             ///< Turn the LCD display on
    void (*off)     (void);             ///< Turn the LCD display off
    void (*update)  (uint8_t* buffer);  ///< Update the LCD frame buffer
    bool (*init)    (void);             ///< Initialize the LCD device
} lcd_dev_t;

#include "gdm12864.h"

#endif // LCD_H_
