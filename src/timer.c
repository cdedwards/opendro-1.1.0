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
#include "timer.h"
#include "board.h"
#include "irq.h"
#include "console.h"
#include "assert.h"
#include "trace.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////
static volatile uint32_t timer_ticks;
static uint32_t timer_piv;
static timer_node_t *head;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////
static void
timer_insert(timer_node_t* timer)
{
    timer_node_t*   iter;
    timer_node_t**  last;

    AT91F_PITDisableInt(AT91C_BASE_PITC);
    
    last = &head;
    iter = head;
    while (iter != NULL && iter->expire < timer->expire)
    {
        last = &iter->next;
        iter = iter->next;
    }
    
    *last = timer;
    timer->next = iter;

    AT91F_PITEnableInt(AT91C_BASE_PITC);
}

static void
timer_remove(timer_node_t *timer)
{
    timer_node_t*   iter;
    timer_node_t**  last;
    
    AT91F_PITDisableInt(AT91C_BASE_PITC);

    last = &head;
    iter = head;
    while (iter != NULL && iter != timer)
    {
        last = &iter->next;
        iter = iter->next;
    }
    
    if (iter != NULL)
        *last = timer->next;

    AT91F_PITEnableInt(AT91C_BASE_PITC);
}

static void
timer_process(void)
{
    timer_node_t*   timer;

    while (head != NULL && head->expire < timer_ticks)
    {
        timer = head;
        timer_remove(timer);
        if (timer->type == TIMER_ONE_SHOT)
            timer->ticks = 0;
        timer->callback(timer->user);

        if (timer->type == TIMER_RECURRING && timer->ticks != 0)
        {
            timer->expire += timer->ticks;
            timer_insert(timer);
        }
    }
}

static void
timer_isr(void)
{
    timer_ticks += (AT91C_BASE_PITC->PITC_PIVR >> 20);
    timer_process();

    // The console shares this interrupt so also call its ISR
    console_isr();
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////
void
timer_start(timer_node_t*       timer,
            uint32_t            ticks,
            timer_type_t        type,
            timer_callback_t    callback,
            void*               user)
{
    timer->expire = timer_ticks + ticks;
    timer->ticks = ticks;
    timer->type = type;
    timer->callback = callback;
    timer->user = user;
    timer_insert(timer);
}

void
timer_restart(timer_node_t*     timer,
              uint32_t          ticks)
{
    if (timer->ticks == 0)
    {
        timer->expire = timer_ticks + ticks;
        timer->ticks = ticks;
        timer_insert(timer);
    }
}

void
timer_stop(timer_node_t* timer)
{
    if (timer->ticks != 0)
    {
        timer_remove(timer);
        timer->ticks = 0;
    }
}

bool
timer_running(timer_node_t* timer)
{
    return (timer->ticks != 0);
}

uint32_t
timer_get(void)
{
    return timer_ticks;
}

void
timer_sleep(uint32_t usecs)
{
    uint32_t ticks;
    uint32_t counts;
    uint32_t roll;
    uint32_t delay;

    // Get the current PIT counts and timer ticks safely by handling
    // the possibility of rollover
    counts = (AT91C_BASE_PITC->PITC_PIIR & 0xfffff);
    ticks = timer_ticks;
    roll = (AT91C_BASE_PITC->PITC_PIIR & 0xfffff);
    if (roll < counts)
    {
        counts = roll;
        ticks = timer_ticks;
    }
        
    // Convert microseconds to PIT counts
    delay = (usecs * (MCLK / 1000000)) / 16;

    // Calculate the resume PIT counts and timer ticks
    counts += (delay % timer_piv);
    ticks += (delay / timer_piv);
    
    // Poll until PIT counts and timer ticks are passed
    while((timer_ticks < ticks) ||
          ((timer_ticks == ticks) &&
           (AT91C_BASE_PITC->PITC_PIIR & 0xfffff) < counts))
    {
    }
}

void
timer_delay(void)
{
    uint32_t counts;
    
    counts = (AT91C_BASE_PITC->PITC_PIIR & 0xfffff);
    while ((AT91C_BASE_PITC->PITC_PIIR & 0xfffff) == counts);
    counts = (counts + 1) & 0xfffff;
    while ((AT91C_BASE_PITC->PITC_PIIR & 0xfffff) == counts);
}

void
timer_init(void)
{
    // Calculate and save the period in ticks
    timer_piv = (TIMER_PERIOD * (MCLK / 1000000) + 8) / 16;
    
    // Initialize the PIT
    AT91C_BASE_PITC->PITC_PIMR = AT91C_PITC_PITEN | timer_piv;

    // Enable the PIT interrupt
    AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITIEN;

    irq_register(AT91C_ID_SYS, IRQ_PRIORITY_HIGH, IRQ_TYPE_LEVEL, timer_isr);
}
