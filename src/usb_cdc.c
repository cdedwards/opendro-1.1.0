///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// USB CDC device class
//
// Copyright (C) 2009-2011 ShumaTech http://www.shumatech.com/
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
#include "usb_cdc.h"
#include "board.h"
#include "irq.h"
#include "trace.h"
#include "swi.h"
#include "usb/utility/led.h"
#include "usb/device/core/USBD.h"
#include "usb/device/cdc-serial/CDCDSerialDriver.h"

static uint8_t  rx_buffer[128];

// LEDS?  What LEDS?
unsigned char LED_Set(unsigned int led)
{
    return 0;
}

unsigned char LED_Clear(unsigned int led)
{
    return 0;
}

void usb_cdc_rx(void *pArg,
                unsigned char status,
                unsigned int transferred,
                unsigned int remaining)
{
    if (status == USBD_STATUS_SUCCESS)
    {
        swi_write(SWI_STDIN, rx_buffer, transferred);
        CDCDSerialDriver_Read(rx_buffer,
                              sizeof(rx_buffer),
                              (TransferCallback) usb_cdc_rx,
                              0);
    }
}

void
usb_cdc_write(uint8_t* ptr, int32_t len)
{
    if (USBD_GetState() >= USBD_STATE_CONFIGURED)
    {
        while (CDCDSerialDriver_Write(ptr, len, NULL, NULL) == USBD_STATUS_LOCKED);
    }
}

void
usb_cdc_init(void)
{
    CDCDSerialDriver_Initialize();
    USBD_Connect();
}

void USBDCallbacks_Initialized(void)
{
    // Register the interrupt handler
    irq_register(AT91C_ID_UDP, IRQ_PRIORITY_MEDIUM, IRQ_TYPE_LEVEL, USBD_InterruptHandler);
    AT91F_AIC_EnableIt(AT91C_BASE_AIC, AT91C_ID_UDP);
}

void USBDCallbacks_Reset(void)
{
    TRACE_DEBUG("%s\n",  __FUNCTION__);
}

void USBDCallbacks_Resumed(void)
{
    TRACE_DEBUG("%s\n",  __FUNCTION__);
}

void USBDCallbacks_Suspended(void)
{
    TRACE_DEBUG("\n%s\n",  __FUNCTION__);
}

void USBDDriverCallbacks_ConfigurationChanged(unsigned char cfgnum)
{
    TRACE_DEBUG("%s\n",  __FUNCTION__);
    CDCDSerialDriver_Read(rx_buffer,
                          sizeof(rx_buffer),
                          (TransferCallback) usb_cdc_rx,
                          0);
}
