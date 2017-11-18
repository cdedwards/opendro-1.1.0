///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Timer subsystem
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
#ifndef TIMER_H_
#define TIMER_H_

#include "types.h"
#include "drolim.h"

#define SEC_TICKS       (1000000 / TIMER_PERIOD)
#define MSEC_TICKS      (1000 / TIMER_PERIOD)

typedef void (*timer_callback_t)(void* user);

typedef enum timer_type
{
    TIMER_ONE_SHOT,
    TIMER_RECURRING
} timer_type_t;

typedef struct timer_node
{
    uint32_t            expire;
    uint32_t            ticks;
    timer_type_t        type;
    timer_callback_t    callback;
    void*               user;
    struct timer_node*  next;
} timer_node_t;

typedef timer_node_t* timer_handle_t;

/// \brief  Start a timer and call a function when it expires
///
/// \param  timer       Pointer to the timer node to store timer state in
/// \param  ticks       Number of ticks until expiration
/// \param  type        Timer type (one-shot or recurring)
/// \param  callback    Function to call when the timer expires
/// \param  user        User pointer returned to callback
void
timer_start(timer_node_t*       timer,
            uint32_t            ticks,
            timer_type_t        type,
            timer_callback_t    callback,
            void*               user);

/// \brief  Restart a stopped timer
///
/// \param  timer       Pointer to the timer node to restart
/// \param  ticks       Number of ticks until expiration
void
timer_restart(timer_node_t* timer,
              uint32_t      ticks);

/// \brief  Stop a timer that is currently running
///
/// \param  timer       Pointer to the timer node to stop
void
timer_stop(timer_node_t* timer);

/// \brief  Check if a timer is runnning
///
/// \return true if the timer is running, false if not
bool
timer_running(timer_node_t* timer);

/// \brief  Initalize the timer subsystem
void
timer_init(void);

/// \brief  Put the caller to sleep for a number of microseconds
///
/// \param  usecs   How long to sleep in microseconds
void
timer_sleep(uint32_t usecs);

/// \brief  Delay the caller for about one microsecond
void
timer_delay(void);

/// \brief  Get the current time in ticks
///
/// \return Current time in ticks
uint32_t
timer_get(void);

#endif /*TIMER_H_*/
