///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Disk IO function for FatFs
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

#include "fatfs/diskio.h"
#include "sd.h"
#include "trace.h"

#define SECTOR_SIZE 512

DSTATUS disk_initialize(BYTE disk)
{
    return disk_status(disk);
}

DSTATUS disk_status(BYTE disk)
{
    DSTATUS status = 0;
    
    if (!sd_card_ready())
        status |= STA_NOINIT;

    if (sd_card_wp())
        status |= STA_PROTECT;

    return status;
}

DRESULT disk_read(BYTE disk, BYTE* buffer, DWORD number, BYTE count)
{
    TRACE_DEBUG("disk_read(%p, %d, %d)\n", buffer, number, count);
    if (!sd_card_ready())
        return RES_NOTRDY;
    
    while (count--)
    {
        if (!sd_cmd_read(17, number++ * SECTOR_SIZE, buffer, SECTOR_SIZE))
        {
            TRACE_DEBUG("disk_read error\n");
            return RES_ERROR;
        }
        buffer += SECTOR_SIZE;
    }
    
    return RES_OK;
}

#if _READONLY == 0
DRESULT disk_write(BYTE disk, const BYTE* buffer, DWORD number, BYTE count)
{
    TRACE_DEBUG("disk_write(%p, %d, %d)\n", buffer, number, count);
    if (!sd_card_ready())
        return RES_NOTRDY;
    
    while (count--)
    {
        if (!sd_cmd_write(24, number++ * SECTOR_SIZE, buffer, SECTOR_SIZE))
        {
            TRACE_DEBUG("disk_write error\n");
            return RES_ERROR;
        }
        buffer += SECTOR_SIZE;
    }

    return RES_OK;
}
#endif

DRESULT disk_ioctl(BYTE disk, BYTE command, void* buffer)
{
    DRESULT result;
    DWORD* sectors = buffer;
    
    switch (command)
    {
    case CTRL_SYNC:
        result = RES_OK;
        break;
    case GET_SECTOR_COUNT:
        *sectors = sd_card_sectors();
        result = RES_OK;
        break;
    default:
        result = RES_PARERR;
        break;
    }
    
    return result;
}

#if !_FS_READONLY
DWORD get_fattime(void)
{
    return ((2010 - 1980) << 25) |
           (8 << 21) |
           (12 << 16) |
           (12 << 11) |
           (22 << 5);
}
#endif
