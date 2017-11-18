///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// A 32-bit, circular FIFO queue
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
#include "fifo.h"
#include "assert.h"
#include "trace.h"

#define DEFUN_FIFO_COUNT(fifo) \
DECL_FIFO_COUNT(fifo) \
{ \
    uint32_t write = fifo->write; \
    uint32_t read = fifo->read; \
    \
    if (read <= write) \
        return write - read; \
    \
    return (write + fifo->size - read); \
}

#define DEFUN_FIFO_EMPTY(fifo) \
DECL_FIFO_EMPTY(fifo) \
{ \
    return (fifo->read == fifo->write); \
}

#define DEFUN_FIFO_READ(fifo, data_t) \
DECL_FIFO_READ(fifo, data_t) \
{ \
    data_t value; \
    uint32_t read; \
    \
    while (fifo->read == fifo->write); \
    \
    value = fifo->data[fifo->read]; \
    read = fifo->read + 1; \
    if (read == fifo->size) \
        read = 0; \
    fifo->read = read; \
    \
    return value; \
}

#define DEFUN_FIFO_PEEK(fifo, data_t) \
DECL_FIFO_PEEK(fifo, data_t) \
{ \
    while (fifo->read == fifo->write); \
    \
    return fifo->data[fifo->read]; \
}

#define DEFUN_FIFO_WRITE(fifo, data_t) \
DECL_FIFO_WRITE(fifo, data_t) \
{ \
    uint32_t write; \
    \
    write = fifo->write + 1; \
    if (write == fifo->size) \
        write = 0; \
    \
    if (write != fifo->read) \
    { \
        fifo->data[fifo->write] = value; \
        fifo->write = write; \
    } \
}

#define DEFUN_FIFO_INIT(fifo) \
DECL_FIFO_INIT(fifo) \
{ \
    fifo->read = 0; \
    fifo->write = 0; \
    fifo->size = size; \
}

#define DEFUN_FIFO(fifo, data_t) \
        DEFUN_FIFO_COUNT(fifo) \
        DEFUN_FIFO_EMPTY(fifo) \
        DEFUN_FIFO_READ(fifo, data_t) \
        DEFUN_FIFO_PEEK(fifo, data_t) \
        DEFUN_FIFO_WRITE(fifo, data_t) \
        DEFUN_FIFO_INIT(fifo)

DEFUN_FIFO(fifo32, uint32_t)
DEFUN_FIFO(fifo8, uint8_t)

