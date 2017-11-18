///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Graphic LCD device driver for GDM12864 (2 x KS0108B)
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
#ifndef GDM12864_H_
#define GDM12864_H_

#include "lcd_dev.h"

#define KS0108B_WIDTH           64
#define KS0108B_HEIGHT          64
#define KS0108B_CHIPS           2

#define GDM12864_DISPLAY_WIDTH  (KS0108B_WIDTH * KS0108B_CHIPS)
#define GDM12864_DISPLAY_HEIGHT (KS0108B_HEIGHT)

#define LCD_DISPLAY_WIDTH       GDM12864_DISPLAY_WIDTH
#define LCD_DISPLAY_HEIGHT      GDM12864_DISPLAY_HEIGHT

extern const lcd_dev_t lcd_dev;

#endif // GDM12864_H_
