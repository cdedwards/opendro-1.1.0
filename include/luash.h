///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Lua interative shell
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
#ifndef LUASH_H_
#define LUASH_H_

#include <stdio.h>

#include "lua.h"
#include "types.h"

bool
luash_cat(const char* file);

bool
luash_pipe(const char* file);

// \brief Return the output file used by a Lua state
//
// \param L    Pointer to the lua state
//
// \return Output file pointer
FILE*
luash_out(lua_State* L);

// \brief Load and run a Lua file
//
// \param file  File path to load and run
//
// \return true if run is successful, false if not
bool
luash_dofile(const char* file);

// \brief Load and run a Lua string
//
// \param string    String to load and run
//
// \return true if run is successful, false if not
bool
luash_dostring(const char* string);

// \brief Process the Lua shell
void
luash_process(void);

// \brief Initialize the Lua shell
void
luash_init(void);

#endif // LUASH_H_
