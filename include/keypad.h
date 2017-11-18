///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Keypad object
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
#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "types.h"
#include "display.h"

/// Keypad keys
typedef enum keypad
{
    KEYPAD_0,
    KEYPAD_1,
    KEYPAD_2,
    KEYPAD_3,
    KEYPAD_4,
    KEYPAD_5,
    KEYPAD_6,
    KEYPAD_7,
    KEYPAD_8,
    KEYPAD_9,
    KEYPAD_PLUS_MINUS,
    KEYPAD_DECIMAL,
    KEYPAD_FUNC,
    KEYPAD_MM_INCH,
    KEYPAD_ABS_INCR,
    KEYPAD_CLEAR,
    KEYPAD_ENTER,
    KEYPAD_ZERO0,
    KEYPAD_PRESET0,
    KEYPAD_ZERO1,
    KEYPAD_PRESET1,
    KEYPAD_ZERO2,
    KEYPAD_PRESET2,
    KEYPAD_UNKNOWN,
#ifdef CONFIG_lcd
    KEYPAD_ZERO3,
    KEYPAD_PRESET3,
    KEYPAD_ZERO4,
    KEYPAD_PRESET4,
#endif
    KEYPAD_MAX,             ///< MUST be last non-initialized value!!!

    // Virtual menu navigation keys
    KEYPAD_PREV_TOP = KEYPAD_PRESET0,
    KEYPAD_NEXT_TOP = KEYPAD_ZERO0,
    KEYPAD_PREV_MIDDLE = KEYPAD_PRESET1,
    KEYPAD_NEXT_MIDDLE = KEYPAD_ZERO1,
    KEYPAD_PREV_BOTTOM = KEYPAD_PRESET2,
    KEYPAD_NEXT_BOTTOM = KEYPAD_ZERO2,
} keypad_t;

/// Menu navigation
typedef enum menu_nav
{
    MENU_NAV_NEXT,          ///< Navigate to next
    MENU_NAV_PREV,          ///< Navigate to previous
    MENU_NAV_MAX,           ///< MUST be last!!!
} menu_nav_t;

// \brief   Logically map a preset or zero key press to a display line. 
// \param   key         key press to convert
//          line_num    OUT display line number key press maps to
// \return  true if key is a preset or zero and false if not
bool
keypad_display_line(keypad_t    key,
                    uint8_t*    line_num);

// \brief   Map a numeric key press to its numerical value.
// \param   key         key press to convert
//          digit       OUT display line number key press maps to
// \return  true if key is numeric and false if not
bool
keypad_to_digit(keypad_t    key,
                uint8_t*    digit);

// \brief   Get an integer number from the operator that is between the
// given minimum and maximum values.
// \param   menu        display menu line to show the number on
//          min_value   minimum valid number, inclusive
//          max_value   maximum valid number, inclusive
//          number      OUT entered number, unmodified if not valid
// \return  true if number is valid and false if not
bool
keypad_get_number(menu_t    menu,
                  int32_t   min_value,
                  int32_t   max_value,
                  int32_t*  number);

// \brief   Get a floating point number from the operator.
// \param   menu        display menu line to show the number on
//          number      OUT entered number, unmodified if not valid
//          stop_key    OUT if supplied, stop input on any non-numeric
//                          key and return it
// \return  true if number is valid and false if not
bool
keypad_get_float(menu_t     menu,
                 double*    number,
                 keypad_t*  stop_key);

// \brief   Map a key press to its menu navigation and line
// \param   key         key press to convert
//          menu        OUT menu line
//          nav         OUT menu navigation
// \return  true if key is a navigation key
bool
keypad_to_nav(keypad_t    key,
              menu_t*     menu,
              menu_nav_t* nav);

// \brief   Present a series of selections and allow a choice between them
// \param   menu        display menu line to show the selections on
//          selection   OUT selection index, unmodified if not valid
//          num         number of selections
//          strings     array of selection strings
// \return  true if number is valid and false if not
bool
keypad_get_selection(menu_t         menu,
                     uint32_t*      selection,
                     uint32_t       max,
                     const char*    strings[]);

// \brief   Process the keypad matrix for valid key presses.
void
keypad_scan(void);

// \brief   Check if the key press FIFO is empty.
// \return  true if the FIFO is empty, false if not
bool
keypad_empty(void);

// \brief   Flush the key press FIFO.
void
keypad_flush(void);

// \brief   Get the next key press off of the FIFO.
// \return  Next key pressed 
keypad_t
keypad_get_press(void);

// \brief   Peek at the next key press without removing it from the FIFO.
// \return  Next key pressed
keypad_t
keypad_peek_press(void);

// \brief   Add a keypad press to the FIFO.
// \param   key     The keypad press to add
void
keypad_add_press(keypad_t key);

// \brief   Keypad number to string mapping
// \param   key         key press to map
// \return  String representing key or NULL if key is invalid
const char *
keypad_to_string(keypad_t    key);

/// \brief  Save all keypad configuration to non-volatile memory
void
keypad_save(void);

/// \brief  Enter keypad setup for a particular machine
void
keypad_setup(void);

/// \brief  Initialize the keypad object from non-volatile memory
void
keypad_init(void);

#endif // KEYPAD_H_
