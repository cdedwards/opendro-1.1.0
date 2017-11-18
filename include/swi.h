///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Newlib libc SWI interface
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
#ifndef SWI_H_
#define SWI_H_

#include "types.h"
#include <time.h>

// File descriptors
#define SWI_STDIN       0
#define SWI_STDOUT      1
#define SWI_STDERR      2
#define SWI_FILE        3
#define SWI_EEFILE      4

// Open flags
#define SWI_FLAG_RDWR   2
#define SWI_FLAG_CREAT  4
#define SWI_FLAG_APPEND 8

// Operations
#define SWI_OPEN        1
#define SWI_CLOSE       2
#define SWI_WRITE       5
#define SWI_READ        6

#ifndef _FATFS
#define DIR void
#endif

#define NAME_MAX        13

#define ATTRIB_DIR      0x1
#define ATTRIB_RO       0x2
#define ATTRIB_HID      0x4
#define ATTRIB_SYS      0x8
#define ATTRIB_ARC      0x10

struct dirent
{
    char            d_name[NAME_MAX];
    unsigned int    d_size;
    int             d_attrib;
    struct tm       d_time;
};

void
swi_mount(int32_t vol);

void
swi_umount(int32_t vol);

DIR*
swi_opendir(const char* path);

struct dirent*
swi_readdir(DIR* dir);

bool
swi_unlink(const char* path);

bool
swi_mkdir(const char* path);

bool
swi_rename(const char* path_old, const char* path_new);

int32_t
swi_open(const char *path, uint32_t len, uint32_t flags);

int32_t
swi_write(int32_t fd, uint8_t* ptr, uint32_t len);

int32_t
swi_read(int32_t fd, uint8_t* ptr, uint32_t len);

// \brief   This function handles the software interrupt generated from
// the newlib C library.
//
// \param   reason  newlib request code
// \param   args    array of arguments specific to the request
//
// \return  Depends on reason
int32_t
swi_isr(int32_t reason,
        void*   args);

void
swi_init(void);

#endif // SWI_H_
