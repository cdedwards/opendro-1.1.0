///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// LUA Modules
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
#ifndef LUAMODS_H_
#define LUAMODS_H_

LUALIB_API int (axis_open_lua) (lua_State *L);
LUALIB_API int (config_open_lua) (lua_State *L);
LUALIB_API int (display_open_lua) (lua_State *L);
LUALIB_API int (function_open_lua) (lua_State *L);
LUALIB_API int (keypad_open_lua) (lua_State *L);
LUALIB_API int (machine_open_lua) (lua_State *L);
LUALIB_API int (mem_open_lua) (lua_State *L);
LUALIB_API int (scale_open_lua) (lua_State *L);
LUALIB_API int (sd_open_lua) (lua_State *L);
LUALIB_API int (system_open_lua) (lua_State *L);
LUALIB_API int (trace_open_lua) (lua_State *L);
LUALIB_API int (luash_open_lua) (lua_State *L);

#define ROM(rom) _ROM(#rom, rom##_open_lua, rom##_map_lua)

#ifdef PLATFORM_dro550
#define LUA_PLATFORM_LIBS_ROM \
    ROM(axis) \
    ROM(config) \
    ROM(display) \
    ROM(function) \
    ROM(keypad) \
    ROM(luash) \
    ROM(machine) \
    ROM(mem) \
    ROM(scale) \
    ROM(sd) \
    ROM(system) \
    ROM(trace)
#else
#define LUA_PLATFORM_LIBS_ROM \
    ROM(axis) \
    ROM(display) \
    ROM(function) \
    ROM(keypad) \
    ROM(luash) \
    ROM(machine) \
    ROM(mem) \
    ROM(scale) \
    ROM(system) \
    ROM(trace)
#endif
#endif // LUAMODS_H_
