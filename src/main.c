///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Main program entry
//
// Copyright (C) 2008-2012 ShumaTech http://www.shumatech.com/
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
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "board.h"
#include "gpio.h"
#include "scale.h"
#include "keypad.h"
#include "axis.h"
#include "timer.h"
#include "assert.h"
#include "trace.h"
#include "display.h"
#include "config.h"
#include "console.h"
#include "machine.h"
#include "tach.h"
#include "edge.h"
#include "function.h"
#include "interrupt.h"
#include "usb_cdc.h"
#include "luash.h"
#include "wdt.h"
#include "sd.h"

int
main(int    argc,
     char*  argv[])
{
    config_version_t version;

    // Console informational notice
    console_init();
    printf("\nOpenDRO %s %d.%d.%d\n", OPENDRO_HARDWARE, OPENDRO_MAJOR, OPENDRO_MINOR, OPENDRO_MAINT);
    printf("Copyright (C) 2008-2012 ShumaTech\n");
    printf("This program comes with ABSOLUTELY NO WARRANTY.  This is free software,\n");
    printf("and you are welcome to redistribute it under certain conditions.\n");

    // Initialize system objects
    gpio_init();
    timer_init();
    config_init();
    trace_init();
    usb_cdc_init();
#if PLATFORM_dro550
    sd_init();
#endif
    
    // Initialize DRO objects
    keypad_init();
    scale_init();
    display_init();
    axis_init();
    machine_init();
    tach_init();
    edge_init();
    function_init();

    // Update the config if the version changed
    if (!config_get_version(&version) ||
        version.major != OPENDRO_MAJOR ||
        version.minor != OPENDRO_MINOR ||
        version.maint != OPENDRO_MAINT)
    {
        TRACE_WARNING("updating config from %d.%d.%d\n", version.major, version.minor, version.maint);
        config_erase();
        
        keypad_save();
        display_save();
        scale_save();
        axis_save();
        machine_save();
        function_save();
        trace_save();
        
        version.major = OPENDRO_MAJOR;
        version.minor = OPENDRO_MINOR;
        version.maint = OPENDRO_MAINT;
        config_set_version(&version);
    }

    // Initialize the Lua shell
    luash_init();

    // Initialize the watchdog timer
    wdt_init();

    while (1)
    {    
        machine_process();
    }
    
    return 0;
}

