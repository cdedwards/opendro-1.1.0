///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Display device driver
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
#ifndef DISPLAY_DEV_H_
#define DISPLAY_DEV_H_

#include "types.h"
#include "display.h"

typedef struct display_dev
{
    void
    (*reading_set)(uint8_t   line,
                   double    number,
                   uint8_t   precision,
                   bool      mark);

    const char*
    (*reading_get)(uint8_t line);

    void
    (*reading_on)(uint8_t line);

    void
    (*reading_off)(uint8_t line);

    void
    (*label_set)(uint8_t       line,
                 const char*   format,
                 va_list       args);

    void
    (*menu_printf)(menu_t       menu,
                   const char*  format,
                   va_list      args);

    void
    (*menu_puts)(menu_t       menu,
                 const char*  text);

    const char*
    (*menu_get)(menu_t menu);

    void
    (*menu_clear)(void);

    bool
    (*menu_state)(void);

    void
    (*status_set)(status_t status,
                  va_list  args);

    void
    (*indicator_set)(indicator_t   indicator,
                     bool          on);

    bool
    (*indicator_get)(indicator_t   indicator);

    void
    (*off)(void);

    void
    (*on)(void);

    void
    (*save)(void);

    void
    (*setup)(void);

    void
    (*init)(void);
} display_dev_t;

#endif // DISPLAY_DEV_H_
