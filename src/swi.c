///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Newlib libc SWI interface
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
#include <string.h>
#if PLATFORM_dro550
#include "fatfs/ff.h"
#endif
#include "swi.h"
#include "console.h"
#include "usb_cdc.h"
#include "fifo.h"
#include "gpio.h"
#include "trace.h"
#include "eeprom.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

#define FIFO_SIZE       128

FIFO(fifo8, stdin_fifo, FIFO_SIZE);

#if PLATFORM_dro550
static FATFS fatfs;
static FIL fil;
static DIR dir;
#endif

#define EEFILE_MAGIC_ADDR      (EEPROM_SIZE / 2)
#define EEFILE_MAGIC_VALUE     0xdeadc0de
#define EEFILE_SIZE_ADDR       (EEFILE_MAGIC_ADDR + 4)
#define EEFILE_DATA_ADDR       (EEFILE_SIZE_ADDR + 4)
#define EEFILE_DATA_SIZE       (EEPROM_SIZE - EEFILE_DATA_ADDR)

static int32_t eefile_pos = -1;
static uint32_t eefile_size;

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

#if PLATFORM_dro550

void
swi_mount(int32_t vol)
{
    f_mount(vol, &fatfs);
}

void
swi_umount(int32_t vol)
{
    f_mount(vol, NULL);
}

DIR*
swi_opendir(const char* path)
{
    FRESULT res;

    res = f_opendir(&dir, path);
    if (res == FR_OK)
        return &dir;
        
    return NULL;
}

struct dirent*
swi_readdir(DIR* dir)
{
    static struct dirent dent;
    FILINFO finfo;
    FRESULT res;
    
    res = f_readdir(dir, &finfo);
    if (res == FR_OK && finfo.fname[0] != '\0')
    {
        strlcpy(dent.d_name, finfo.fname, sizeof(dent.d_name));
        dent.d_size = finfo.fsize;
        
        dent.d_attrib = 0;
        if (finfo.fattrib & AM_DIR)
            dent.d_attrib |= ATTRIB_DIR;
        if (finfo.fattrib & AM_RDO)
            dent.d_attrib |= ATTRIB_RO;
        if (finfo.fattrib & AM_HID)
            dent.d_attrib |= ATTRIB_HID;
        if (finfo.fattrib & AM_SYS)
            dent.d_attrib |= ATTRIB_SYS;
        if (finfo.fattrib & AM_ARC)
            dent.d_attrib |= ATTRIB_ARC;

        dent.d_time.tm_sec = finfo.ftime & 0x1f;
        dent.d_time.tm_min = (finfo.ftime >> 5) & 0x2f;
        dent.d_time.tm_hour = (finfo.ftime >> 11) & 0x1f;
        
        dent.d_time.tm_mday = finfo.fdate & 0x1f;
        dent.d_time.tm_mon = (finfo.fdate >> 5) & 0xf;
        dent.d_time.tm_year = ((finfo.fdate >> 9) & 0x7f) + 80;
        
        dent.d_time.tm_wday = 0;
        dent.d_time.tm_yday = 0;
        dent.d_time.tm_isdst = 0;
        
        return &dent;
    }
        
    return NULL;
}

bool
swi_unlink(const char* path)
{
    return (f_unlink(path) == FR_OK);
}

bool
swi_mkdir(const char* path)
{
    return (f_mkdir(path) == FR_OK);
}

bool
swi_rename(const char* path_old, const char* path_new)
{
    return (f_rename(path_old, path_new) == FR_OK);
}

#endif

static bool
eefile_open(uint32_t flags)
{
    uint32_t magic;

    if (eeprom_read(EEFILE_MAGIC_ADDR, &magic, sizeof(magic)) != sizeof(magic))
        return false;
    
    if (magic == EEFILE_MAGIC_VALUE)
    {
        if (flags & SWI_FLAG_CREAT)
            eefile_size = 0;
        else
            if (eeprom_read(EEFILE_SIZE_ADDR, &eefile_size, sizeof(eefile_size)) != sizeof(eefile_size))
                return false;

        if (flags & SWI_FLAG_APPEND)
            eefile_pos = eefile_size;
        else
            eefile_pos = 0;
    }
    else 
    {
        magic = EEFILE_MAGIC_VALUE;
        if (eeprom_write(EEFILE_MAGIC_ADDR, &magic, sizeof(magic)) != sizeof(magic))
            return false;
        eefile_size = 0;
        eefile_pos = 0;
    }

    return true;
}

static int32_t
eefile_close(void)
{
    uint32_t last_size;
    
    if (eefile_pos == -1)
        return -1;

    eefile_pos = -1;
    
    if (eeprom_read(EEFILE_SIZE_ADDR, &last_size, sizeof(last_size)) != sizeof(last_size))
        return -1;
    
    if (last_size != eefile_size &&
        eeprom_write(EEFILE_SIZE_ADDR, &eefile_size, sizeof(eefile_size)) != sizeof(eefile_size))
        return -1;

    return 0;
}

static int32_t
eefile_read(void*       buffer,
            uint32_t    length)
{
    int32_t bytes;

    if (eefile_pos == -1)
        return -1;
    if (eefile_pos >= eefile_size)
        return 0;
    if (eefile_pos + length > eefile_size)
        length = eefile_size - eefile_pos;

    bytes = eeprom_read(EEFILE_DATA_ADDR + eefile_pos, buffer, length);
    if (bytes < 0)
        return bytes;
    
    eefile_pos += bytes;
    
    return bytes;
}

static int32_t
eefile_write(const void* buffer,
             uint32_t    length)
{
    int32_t bytes;

    if (eefile_pos == -1)
        return -1;
    if (eefile_pos >= EEFILE_DATA_SIZE)
        return -1;
    if (eefile_pos + length > EEFILE_DATA_SIZE)
        length = EEFILE_DATA_SIZE - eefile_pos;

    bytes = eeprom_write(EEFILE_DATA_ADDR + eefile_pos, buffer, length);
    if (bytes < 0)
        return bytes;

    eefile_pos += bytes;
    if (eefile_pos > eefile_size)
        eefile_size = eefile_pos;

    return bytes;
}

int32_t
swi_open(const char *path, uint32_t len, uint32_t flags)
{
    int32_t fd = -1;

    if (strcmp(path, ":tt") == 0)
    {
        if (flags & SWI_FLAG_CREAT)
            fd = SWI_STDOUT;
        else
            fd = SWI_STDIN;
    }
    else if (strcmp(path, ":err") == 0)
    {
        fd = SWI_STDERR;
    }
    else if (strcmp(path, ":eefile") == 0)
    {
        if (eefile_open(flags))
            fd = SWI_EEFILE;
    }
#if PLATFORM_dro550
    else
    {
        uint8_t ff_flags = FA_READ;
        FRESULT res;
        
        // Translate SWI flags to FatFs flags
        if (flags & SWI_FLAG_RDWR)
            ff_flags |= FA_WRITE;
        if (flags & SWI_FLAG_CREAT)
            ff_flags |= (FA_WRITE | FA_CREATE_ALWAYS);
        if (flags & SWI_FLAG_APPEND)
            ff_flags |= (FA_WRITE | FA_OPEN_ALWAYS);
            
        TRACE_DEBUG("f_open(%p %s 0x%x)\n", &fil, path, ff_flags);
        res = f_open(&fil, path, ff_flags);
        if (res == FR_OK)
            fd = SWI_FILE;
        TRACE_DEBUG("f_open=%d\n", res);
    }
#endif
    return fd;
}

int32_t
swi_close(int32_t fd)
{
    int32_t rc = -1;
    
    switch (fd)
    {
    case SWI_STDIN:
    case SWI_STDOUT:
    case SWI_STDERR:
        rc = 0;
        break;
    case SWI_EEFILE:
        rc = eefile_close();
        break;
#if PLATFORM_dro550
    case SWI_FILE:
    {
        FRESULT res;
        TRACE_DEBUG("f_close(%p)\n", fil);
        res = f_close(&fil);
        if (res == FR_OK)
            rc = 0;
        TRACE_DEBUG("f_close=%d\n", res);
        break;
    }
#endif
    }

    return rc;
}

static void
swi_write_fifo(fifo8_t* fifo, uint8_t* ptr, uint32_t len)
{
    static bool prev_cr;

    while (len > 0)
    {
        // Translate incoming carriage return into line feed
        if (*ptr == '\r')
        {
            fifo8_write(fifo, '\n');
            prev_cr = true;
        }
        // Write everything else except for a line feed following
        // a carriage return
        else if (!prev_cr || *ptr != '\n')
        {
            fifo8_write(fifo, *ptr);
            prev_cr = false;
        }

        ptr++;
        len--;
    }
}

static void
swi_write_dev(void (*write_fn)(uint8_t*, int32_t), uint8_t* ptr, uint32_t len)
{
    uint8_t* lf_pos;
    uint8_t* lf_last = ptr;
    uint32_t sublen;
    uint8_t cr_lf[] = { '\r', '\n' };

    // For each outgoing line feed, insert a carriage return before it
    while (len && (lf_pos = memchr(lf_last, '\n', len)) != NULL)
    {
        sublen = lf_pos - lf_last;
        if (sublen > 0)
            write_fn(lf_last, sublen);

        write_fn(cr_lf, sizeof(cr_lf));

        lf_last = lf_pos + 1;
        len -= sublen + 1;
    }

    if (len > 0)
        write_fn(lf_last, len);
}

int32_t
swi_write(int32_t fd, uint8_t* ptr, uint32_t len)
{
    int32_t rc = -1;
    
    switch (fd)
    {
    case SWI_STDIN:
        swi_write_fifo(&stdin_fifo, ptr, len);
        rc = 0;
        break;
    case SWI_STDOUT:
        swi_write_dev(console_write, ptr, len);
        swi_write_dev(usb_cdc_write, ptr, len);
        rc = 0;
        break;
    case SWI_STDERR:
        swi_write_dev(console_write, ptr, len);
        rc = 0;
        break;
    case SWI_EEFILE:
    {
        int32_t bytes;
        bytes = eefile_write(ptr, len);
        if (bytes >= 0)
            rc = (len - bytes);
        break;
    }
#if PLATFORM_dro550
    case SWI_FILE:
    {
        UINT bytes;
        FRESULT res;
        
        TRACE_DEBUG("f_write(%p %p %u)\n", &fil, ptr, len);
        res = f_write(&fil, ptr, len, &bytes);
        if (res == FR_OK)
            rc = (len - bytes);
        TRACE_DEBUG("f_write=%d %d\n", res, bytes);
        break;
    }
#endif
    }
    
    return rc;
}

int32_t
swi_read(int32_t fd, uint8_t* ptr, uint32_t len)
{
    int32_t rc = -1;
    
    switch (fd)
    {
    case SWI_STDIN:
        while (len > 0 && !fifo8_empty(&stdin_fifo))
        {
            *ptr++ = fifo8_read(&stdin_fifo);
            len--;
        }
        rc = len;
        break;
    case SWI_EEFILE:
    {
        int32_t bytes;
        bytes = eefile_read(ptr, len);
        if (bytes >= 0)
            rc = (len - bytes);
        break;
    }
#if PLATFORM_dro550
    case SWI_FILE:
    {
        UINT bytes;
        FRESULT res;

        TRACE_DEBUG("f_read(%p %p %u)\n", &fil, ptr, len);
        res = f_read(&fil, ptr, len, &bytes);
        if (res == FR_OK)
            rc = (len - bytes);
        TRACE_DEBUG("f_read=%d %d\n", res, bytes);
        break;
    }
#endif
    }

    return rc;
}

int32_t
swi_isr(int32_t reason,
        void*   args)
{
    int32_t result = -1;

    switch (reason)
    {
    case SWI_OPEN:
        result = swi_open(((char**) args)[0],
                          ((uint32_t*) args)[2],
                          ((uint32_t*) args)[1]);
        break;
    case SWI_CLOSE:
        result = swi_close(((int32_t*) args)[0]);
        break;
    case SWI_WRITE:
        result = swi_write(((int32_t*) args)[0],
                           ((uint8_t**) args)[1],
                           ((uint32_t*) args)[2]);
        break;
    case SWI_READ:
        result = swi_read(((int32_t*) args)[0],
                          ((uint8_t**) args)[1],
                          ((uint32_t*) args)[2]);
        break;
    default:
        break;
    }

    return result;
}

void
swi_init(void)
{
    // Call newlib to initialize standard handles
    extern void initialise_monitor_handles(void);
    initialise_monitor_handles();

    fifo8_init(&stdin_fifo, FIFO_SIZE);
}
