///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Generic SD card interface
//
// Copyright (C) 2011 ShumaTech http://www.shumatech.com/
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
#ifndef SD_H_
#define SD_H_

#include <stdio.h>

#include "types.h"

void
sd_info(FILE* out);

bool
sd_card_ready(void);

bool
sd_card_wp(void);

uint32_t
sd_card_sectors(void);

const char*
sd_error(uint8_t response);

uint8_t
sd_cmd_r1(uint8_t cmd, uint32_t args);

uint8_t
sd_cmd_r2(uint8_t cmd, uint32_t args, uint8_t* value);

uint8_t
sd_cmd_r3(uint8_t cmd, uint32_t args, uint32_t* value);

bool
sd_cmd_read(uint8_t cmd, uint32_t args, uint8_t* block, uint32_t len);

bool
sd_cmd_write(uint8_t cmd, uint32_t args, const uint8_t* block, uint32_t len);

void
sd_init(void);

#endif // SPI_H_
