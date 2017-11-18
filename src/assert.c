///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Debug assertions
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
#include <stdarg.h>
#include "types.h"
#include "display.h"
#include "trace.h"
#include "assert.h"

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

void
assert(const char*    function,
       int            line,
       const char*    condition,
       ...)
{
    const char *msg;
    va_list args;

    display_menu_puts(MENU_TOP, "assert");
    display_menu_puts(MENU_MIDDLE, function);
    display_menu_printf(MENU_BOTTOM, "%d", line);

    va_start(args, condition);
    msg = va_arg(args, const char *);
    va_end(args);

    if (!msg)
        msg = condition;

    TRACE_FATAL("assertion failed: %s:%d - %s\n", function, line, msg);

    while(1);
}
