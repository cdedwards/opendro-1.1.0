# OpenDRO Project
#
# Copyright (C) 2008-2010 ShumaTech http://www.shumatech.com/
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

MOD_SRC = axis.c config.c console.c crt0.s assert.c led.c display.c \
          edge.c eeprom.c fifo.c function.c interrupt.s irq.c keypad.c \
          machine.c main.c scale.c tach.c timer.c usb_cdc.c luash.c \
          trace.c swi.c wdt.c system.c mem.c

MOD_SRC_dro550 = sd.c diskio.c
MOD_SRC_dro550_lcd = lcd.c gdm12864.c font6x8.c font12x18.c

MOD_CFLAGS = -Isrc/lua -DLUA_OPTIMIZE_MEMORY=2

