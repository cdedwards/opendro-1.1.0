/*/////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Linker script for AT91SAM7SE256
//
// Copyright (C) 2008-2010 ShumaTech http://www.shumatech.com/
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
/////////////////////////////////////////////////////////////////////////////*/
MEMORY 
{
	flash (rx)  : ORIGIN = 0x00100000, LENGTH = 256K
	sram  (!rx) : ORIGIN = 0x00200000, LENGTH = 32K
}

SECTIONS 
{
	. = ORIGIN(flash);
	
	.text : 
	{
		PROVIDE(stext = .);
		*(.reset)
		*(.text)
		*(.rodata)
		*(.rodata*)
		*(.glue_7)
		*(.glue_7t)
		PROVIDE(etext = .);
		PROVIDE(signature = .);
	} >flash

	__end_of_text__ = .;

	.data : 
	{
		__data_beg__ = .;
		__data_beg_src__ = __end_of_text__;
		*(.vectors)
		*(.data)
		*(.stext)
		__data_end__ = .;
	} >sram AT>flash

	.bss : 
	{
		__bss_beg__ = .;
		*(.bss)
	} >sram

	. = ALIGN(4);
	end = .;
	__bss_end__ = .;

    __stack_end__ = ORIGIN(sram) + LENGTH(sram) - 4;
}

