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
#ifndef USB_H_
#define USB_H_

#include "types.h"

void
usb_cdc_write(uint8_t* ptr, int32_t len);

/// \brief  Initialize the USB CDC interface
void
usb_cdc_init(void);

#endif // USB_H_
