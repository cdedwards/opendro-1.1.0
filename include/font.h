///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Font metrics
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
#ifndef FONT_H_
#define FONT_H_

#include <stdint.h>

typedef struct font
{
    const uint8_t*  data;       ///< Constant pointer to font data
    uint8_t         width;      ///< Font width in pixels
    uint8_t         height;     ///< Font height in pixels
    uint8_t         bytes;      ///< Number of bytes in each line
    uint8_t         first;      ///< ASCII value for first character
    uint8_t         last;       ///< ASCII value for last character
} font_t;

#endif // FONT_H_
