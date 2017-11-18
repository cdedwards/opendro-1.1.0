///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Watchdog timer
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
#include "wdt.h"
#include "board.h"
#include "timer.h"

static timer_node_t wd_timer;

static void
wdt_reset(void* user)
{
    AT91C_BASE_WDTC->WDTC_WDCR = (0xa5 << 24) | AT91C_WDTC_WDRSTT;
}

void
wdt_init(void)
{
    wdt_reset(NULL);
    
    // ATMEL ERRATA - "WDT: The Watchdog Timer May Lock the Device in a Reset State"
    // The WDD field must be set to 0xfff and the WDV field must be set to a special
    // value.  The values below sets the watchdog to about 3 seconds.
    AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDBGHLT | (0xfff << 16) | AT91C_WDTC_WDRPROC | AT91C_WDTC_WDRSTEN | 0x2ff;

    timer_start(&wd_timer, SEC_TICKS, TIMER_RECURRING, wdt_reset, NULL);
}
