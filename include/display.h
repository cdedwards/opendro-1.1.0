///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// DRO display object
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
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdarg.h>
#include "types.h"
#include "gpio.h"

/// Three-line menu display
typedef enum menu
{
    MENU_TOP,               ///< Top line in the menu
    MENU_MIDDLE,            ///< Middle line in the menu
    MENU_BOTTOM,            ///< Bottom line in the menu
    MENU_MAX,               ///< MUST BE LAST!!!
} menu_t;

/// Binary indicators on the display
typedef enum indicator
{
    INDICATOR_INCR = IND_LED_INCR,  ///< Incremental mode
    INDICATOR_MM = IND_LED_MM,      ///< Metric mode
    INDICATOR_ZERO = IND_LED_ZERO,  ///< Zero axis
    INDICATOR_SET = IND_LED_SET,    ///< Set value
    INDICATOR_FUNC = IND_LED_FUNC,  ///< Function active
    INDICATOR_MAX = IND_LED_MAX,    ///< MUST BE LAST!!!
} indicator_t;

typedef enum status
{
    STATUS_MACHINE_NUM,     ///<
    STATUS_MACHINE_TYPE,    ///<
    STATUS_FUNC_STATE,      ///<
    STATUS_FUNC_STEP,       ///<
    STATUS_WORKSPACE,       ///<
    STATUS_COMPOUND,        ///<
    STATUS_DIAMETER,        ///<
    STATUS_TOOL_NUM,        ///<
    STATUS_TOOL_OFFSET,     ///<
    STATUS_TOOL_EDGE,       ///<
    STATUS_MAX,             ///< MUST BE LAST!!!
} status_t;

/// \brief  Display a floating point number on a reading line
///
/// \param  line        Line to display on (0..DISPLAY_LINE_MAX-1)
/// \param  number      Floating point number to display
/// \param  precision   Number of decimal places to display in half digits
///                     (i.e. passing 5 results in 2.5 decimal places)
///                     Fraction output if set to PRECISION_FRACTION
/// \param  mark        Mark the line with a visual indicator
void
display_reading_set(uint8_t   line,
                    double    number,
                    uint8_t   precision,
                    bool      mark);

/// \brief  Get the text on a reading line
///
/// \param  line        Line to get (0..DISPLAY_LINE_MAX-1)
///
/// \return A pointer to a nil-terminated string of the line's text
const char *
display_reading_get(uint8_t line);

/// \brief  Blink a reading line
///
/// \param  line        Line to blink on (0..DISPLAY_LINE_MAX-1)
void
display_reading_blink(uint8_t line);

/// \brief  Turn a reading line on so that it displays
///
/// \param  line        Line to clear (0..DISPLAY_LINE_MAX-1)
void
display_reading_on(uint8_t line);

/// \brief  Turn a reading line on so that it displays
///
/// \param  line        Line to clear (0..DISPLAY_LINE_MAX-1)
void
display_reading_off(uint8_t line);

/// \brief  Display a line label
///
/// \param  line        Line to label (0..DISPLAY_LINE_MAX-1)
/// \param  format      Printf-style format string
/// \param  ...         Variable argument list
void
display_label_set(uint8_t       line,
                  const char*   format,
                  ...)
    __attribute__ ((format (printf, 2, 3)));

/// \brief  Display text on the menu with printf-style formatting
///
/// \param  menu        Menu position
/// \param  format      Printf-style format string
/// \param  ...         Variable argument list
void
display_menu_printf(menu_t       menu,
                    const char*  format,
                    ...)
    __attribute__ ((format (printf, 2, 3)));

/// \brief  Display a string on the menu
///
/// \param  menu        Menu position
/// \param  text        Null-terminated text string to display
void
display_menu_puts(menu_t       menu,
                  const char*  text);

/// \brief  Get the text on a reading line
///
/// \param  menu        Menu position
///
/// \return A pointer to a nil-terminated string of the menu's text
const char *
display_menu_get(menu_t menu);

/// \brief  Clear the menu display
void
display_menu_clear(void);

/// \brief Return the menu display state
///
/// \return true if menu is displayed, false if hidden
bool
display_menu_state(void);

/// \brief  Set the status field with printf-style formatting
///
/// \param  status      Status field identifier
/// \param  ...         Variable argument list
void
display_status_set(status_t status,
                   ...);

/// \brief  Set the display state of a binary indicator
///
/// \param  indicator   Indicator to set
/// \param  state       New binary state 
void
display_indicator_set(indicator_t   indicator,
                      bool          on);

/// \brief  Get the display state of a binary indicator
///
/// \param  indicator   Indicator to get
///
/// \return Binary state
bool
display_indicator_get(indicator_t   indicator);

/// \brief  Turn the display off but keep processing the display
void
display_off(void);

/// \brief  Turn the display on if it was off before
void
display_on(void);

/// \brief  Save all display configuration to non-volatile memory 
void
display_save(void);

/// \brief  Enter display setup for a particular machine 
void 
display_setup(void);

/// \brief  Initialize the display object from non-volatile memory 
void
display_init(void);

#endif // DISPLAY_H_
