///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Circular FIFO queues
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
#ifndef FIFO_H_
#define FIFO_H_

#include "types.h"

/// FIFO state
#define DECL_FIFO_T(fifo, data_t) \
typedef struct _ ## fifo ## _t \
{ \
     volatile uint32_t read;                /* read position index */ \
     volatile uint32_t write;               /* write position index */ \
     volatile uint32_t size;                /* size of the FIFO */ \
     volatile data_t data[];                /* circular array of the data */ \
} fifo ## _t

/// \brief Get the current count of data elements in the FIFO
///
/// \param  fifo    Pointer to the FIFO structure
///
/// \return The current length
#define DECL_FIFO_COUNT(fifo) uint32_t fifo ## _count(fifo ## _t *fifo)

/// \brief Check if the FIFO is empty
///
/// \param  fifo    Pointer to the FIFO structure
///
/// \return True if it is empty and false if not
#define DECL_FIFO_EMPTY(fifo) bool fifo ## _empty(fifo ## _t *fifo)

/// \brief Read a 32-bit value from the FIFO.  If the FIFO is empty,
/// then the function will spin until something is written to it.
///
/// \param  fifo    Pointer to the FIFO structure
///
/// \return The 32-bit value from the FIFO 
#define DECL_FIFO_READ(fifo, data_t) data_t fifo ## _read(fifo ## _t* fifo)

/// \brief Peek at the next 32-bit value in the FIFO without removing
/// it.  If the FIFO is empty, then the function will spin until
/// something is written to it.
///
/// \param  fifo    Pointer to the FIFO structure
///
/// \return The 32-bit value from the FIFO 
#define DECL_FIFO_PEEK(fifo, data_t) data_t fifo ## _peek(fifo ## _t* fifo)

/// \brief Write a 32-bit value to the FIFO.  If the FIFO is full,
/// then the value is dropped.
///
/// \param  fifo    Pointer to the FIFO structure
#define DECL_FIFO_WRITE(fifo, data_t) void fifo ## _write(fifo ## _t* fifo, data_t value)

/// \brief Initialize the FIFO
///
/// \param  fifo    Pointer to the FIFO structure
/// \param  size    The size of the FIFO up to FIFO_SIZE_MAX
///
/// \return True if it is empty and false if not
#define DECL_FIFO_INIT(fifo) void fifo ## _init(fifo ## _t* fifo, uint32_t size)

#define DECL_FIFO(fifo, data_t) \
        DECL_FIFO_T(fifo,data_t); \
        DECL_FIFO_COUNT(fifo); \
        DECL_FIFO_EMPTY(fifo); \
        DECL_FIFO_READ(fifo,data_t); \
        DECL_FIFO_PEEK(fifo,data_t); \
        DECL_FIFO_WRITE(fifo,data_t); \
        DECL_FIFO_INIT(fifo);

#define FIFO(fifo, name, size) fifo ## _t name = { .data = { [0 ... size - 1] = 0 } }

DECL_FIFO(fifo32, uint32_t)
DECL_FIFO(fifo8, uint8_t)

#endif // FIFO_H_
