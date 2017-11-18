///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Non-volatile configuration
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
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "drolim.h"
#include "eeprom.h"
#include "machine.h"
#include "keypad.h"
#include "display.h"
#include "assert.h"
#include "trace.h"

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   D E C L A R A T I O N S
///////////////////////////////////////////////////////////////////////////////

/// Magic value
#define CONFIG_MAGIC_VALUE      0xfeedbabe
#define CONFIG_MAGIC_ADDR       0
#define CONFIG_MAGIC_SIZE       4

/// Configuration version
#define CONFIG_VERSION_ADDR     (CONFIG_MAGIC_ADDR + CONFIG_MAGIC_SIZE)
#define CONFIG_VERSION_SIZE     4

/// Start of configuration address
#define CONFIG_START_ADDR       (CONFIG_VERSION_ADDR + CONFIG_VERSION_SIZE)

/// End of configuration address
#define CONFIG_END_ADDR         (EEPROM_SIZE / 2)

/// Maximum number of objects supported
#define CONFIG_OBJ_MAX          32

/// Inital allocation size of a new object's value buffer
#define CONFIG_OBJ_ALLOC        256

/// All configuration objects and variables are stored in tlv format (tag-length-value)
typedef struct tlv
{
    uint8_t     id;             ///< Unique object/variable identifier  
    uint8_t     index;          ///< Object/variable instance index
    uint16_t    length;         ///< Total length of value
    uint8_t     value[];        ///< Flexible array containing value
} tlv_t;

/// Directory entry for objects
typedef struct dirent
{
    tlv_t       tlv;            ///< TLV of the object without a value
    uint16_t    addr;           ///< Address of the object's TLV
} dirent_t;

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   V A R I A B L E S
///////////////////////////////////////////////////////////////////////////////

static dirent_t*    obj_dirent;                 ///< Open object directory entry
static uint8_t*     obj_value;                  ///< Open object value buffer
static bool         obj_write;                  ///< Object is open for writing
static uint32_t     obj_alloc;                  ///< Allocation size of new object value
static dirent_t     dir_obj[CONFIG_OBJ_MAX];    ///< Top-level object directory
static uint32_t     dir_end;                    ///< Ending index of directory

///////////////////////////////////////////////////////////////////////////////
// P R I V A T E   F U N C T I O N S
///////////////////////////////////////////////////////////////////////////////

static int32_t
config_read(uint32_t    address,
            void*       buffer,
            uint32_t    length)
{
    if (address >= CONFIG_END_ADDR)
        return -1;
    if (address + length > CONFIG_END_ADDR)
        length = CONFIG_END_ADDR - address;
        
    return eeprom_read(address, buffer, length);
}

static int32_t
config_write(uint32_t       address,
             const void*    buffer,
             uint32_t       length)
{
   if (address >= CONFIG_END_ADDR)
        return -1;
     if (address + length > CONFIG_END_ADDR)
        length = CONFIG_END_ADDR - address;

    return eeprom_write(address, buffer, length);
}

static bool
config_is_valid(void)
{
    uint32_t magic;

    if (config_read(CONFIG_MAGIC_ADDR, &magic, CONFIG_MAGIC_SIZE) != CONFIG_MAGIC_SIZE ||
        magic != CONFIG_MAGIC_VALUE)
    {
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////
// P U B L I C   A P I
///////////////////////////////////////////////////////////////////////////////

bool
config_obj_read(config_oid_t    oid,
                uint8_t         oindex)
{
    uint32_t dir_idx;
    dirent_t* dirent;

    ASSERT(oid < CONFIG_OID_MAX);
    ASSERT(obj_dirent == NULL);
    ASSERT(obj_value == NULL);

    TRACE_DEBUG("%s %d.%d\n", __FUNCTION__, oid, oindex);

    // Find the object in the directory
    for (dir_idx = 0; dir_idx < dir_end; dir_idx++)
    {
        dirent = &dir_obj[dir_idx];

        // Stop if the end of the directory is reached
        if (dirent->tlv.id == CONFIG_OID_END)
            break;

        // Stop if the OID is found
        if (dirent->tlv.id == oid && dirent->tlv.index == oindex)
            break;
    }

    // Error if the end of directory was reached
    if (dir_idx >= dir_end)
        return false;

    // Allocate space for the object value
    obj_value = malloc(dirent->tlv.length);
    if (obj_value == NULL)
    {
        TRACE_WARNING("Cannot allocate object value\n");
        return false;
    }

    // Read in the object value
    if (config_read(dirent->addr + sizeof(tlv_t),
                    obj_value,
                    dirent->tlv.length) != dirent->tlv.length)
    {
        free(obj_value);
        return false;
    }

    obj_dirent = dirent;
    obj_write = false;

    return true;
}

bool
config_obj_write(config_oid_t   oid,
                 uint8_t        oindex)
{
    ASSERT(oid < CONFIG_OID_MAX);
    ASSERT(obj_dirent == NULL);
    ASSERT(obj_value == NULL);

    TRACE_DEBUG("%s %d.%d\n", __FUNCTION__, oid, oindex);

    // Open for reading first
    if (config_obj_read(oid, oindex))
    {
        obj_alloc = 0;
        obj_write = true;
        return true;
    }

    // Error if the directory is full
    if (dir_end >= ARRAY_SIZE(dir_obj))
        return false;

    // This is a new object so allocate space for the value
    obj_alloc = CONFIG_OBJ_ALLOC;
    obj_value = malloc(obj_alloc);
    if (obj_value == NULL)
    {
        TRACE_WARNING("Cannot allocate object value\n");
        return false;
    }

    // Add the new object to the directory
    obj_dirent = &dir_obj[dir_end];
    obj_dirent->tlv.id = oid;
    obj_dirent->tlv.index = oindex;
    obj_dirent->tlv.length = 0;
    if (dir_end == 0)
        obj_dirent->addr = CONFIG_START_ADDR;
    else
        obj_dirent->addr = dir_obj[dir_end - 1].addr +
            sizeof(tlv_t) + dir_obj[dir_end - 1].tlv.length;
    dir_end++;

    obj_write = true;

    return true;
}

void
config_obj_close(void)
{
    uint32_t obj_addr;
    tlv_t end_tlv = { .id = CONFIG_OID_END };

    ASSERT(obj_dirent != NULL);
    ASSERT(obj_value != NULL);

    TRACE_DEBUG("%s\n", __FUNCTION__);

    // Write the object if opened for write
    if (obj_write)
    {
        obj_addr = obj_dirent->addr;
        config_write(obj_addr, &obj_dirent->tlv, sizeof(tlv_t));
        obj_addr += sizeof(tlv_t);
        config_write(obj_addr, obj_value, obj_dirent->tlv.length);

        // Write an end TLV if this object was newly allocated
        if (obj_alloc > 0)
        {
            obj_addr += obj_dirent->tlv.length;
            config_write(obj_addr, &end_tlv, sizeof(end_tlv));
        }
    }

    obj_dirent = NULL;
    free(obj_value);
    obj_value = NULL;
    obj_alloc = 0;
}

bool 
config_var_write(uint8_t       vid,
                 uint8_t       vindex,
                 uint16_t      length,
                 const void*   value)
{
    uint32_t    var_addr;
    tlv_t       var_tlv;

    ASSERT(obj_dirent != NULL);
    ASSERT(obj_value != NULL);

    TRACE_DEBUG("%s %d.%d len %d\n", __FUNCTION__, vid, vindex, length);

    // Make sure the object is opened write
    if (!obj_write)
        return false;

    // Replace the value if the var exists
    for (var_addr = 0;
         var_addr < obj_dirent->tlv.length;
         var_addr += var_tlv.length + sizeof(tlv_t))
    {
        memcpy(&var_tlv, &obj_value[var_addr], sizeof(var_tlv));
        if (var_tlv.id == vid &&
            var_tlv.index == vindex &&
            var_tlv.length == length)
        {
            memcpy(&obj_value[var_addr + sizeof(tlv_t)], value, length);
            return true;
        }
    }

    // Return an error if the object is not new
    if (obj_alloc == 0)
        return false;

    // Make sure the value is large enough
    while (obj_alloc < obj_dirent->tlv.length + length)
    {
        obj_alloc <<= 2;
        obj_value = realloc(obj_value, obj_alloc);
        if (obj_value == NULL)
        {
            TRACE_WARNING("Cannot allocate object value\n");
            return false;
        }
    }

    var_tlv.id = vid;
    var_tlv.index = vindex;
    var_tlv.length = length;

    memcpy(&obj_value[var_addr], &var_tlv, sizeof(tlv_t));
    obj_dirent->tlv.length += sizeof(tlv_t);

    memcpy(&obj_value[var_addr + sizeof(tlv_t)], value, length);
    obj_dirent->tlv.length += length;

    return true;
}

bool
config_var_read(uint8_t  vid,
                uint8_t  vindex,
                uint16_t length,
                void*    value)
{
    uint32_t var_addr;
    tlv_t var_tlv;

    ASSERT(obj_dirent != NULL);
    ASSERT(obj_value != NULL);

    TRACE_DEBUG("%s %d.%d len %d\n", __FUNCTION__, vid, vindex, length);

    for (var_addr = 0;
         var_addr < obj_dirent->tlv.length;
         var_addr += var_tlv.length + sizeof(tlv_t))
    {
        memcpy(&var_tlv, &obj_value[var_addr], sizeof(var_tlv));
        if (var_tlv.id == vid &&
            var_tlv.index == vindex &&
            var_tlv.length == length)
        {
            memcpy(value, &obj_value[var_addr + sizeof(tlv_t)], length);
            return true;
        }
    }

    return false;
}

bool
config_setup(const char*       obj_name,
             uint8_t           num_objs,
             uint8_t           num_vars,
             const var_list_t* var_list)
{
    int8_t      obj_inst = 0;
    int8_t      var_num = 0;
    int8_t      var_inst = 0;
    keypad_t    key;

    display_menu_clear();
    
    do
    {
        display_menu_printf(MENU_TOP, obj_name, obj_inst + 1);
        display_menu_printf(MENU_MIDDLE, var_list[var_num].name, var_inst + 1);
        var_list[var_num].display(obj_inst, var_inst);

        key = keypad_get_press();
        
        if (key == KEYPAD_CLEAR)
        {
            return false;
        }
        else if (key == KEYPAD_NEXT_TOP)
        {
            obj_inst++;
            if (obj_inst == num_objs)
                obj_inst = 0;
        }
        else if (key == KEYPAD_PREV_TOP)
        {
            obj_inst--;
            if (obj_inst < 0)
                obj_inst = num_objs - 1;
        }
        else if (key == KEYPAD_NEXT_MIDDLE)
        {
            var_inst++;
            if (var_inst >= var_list[var_num].num_inst)
            {
                var_num++;
                if (var_num == num_vars)
                    var_num = 0;
                var_inst = 0;
            }
        }
        else if (key == KEYPAD_PREV_MIDDLE)
        {
            var_inst--;
            if (var_inst < 0)
            {
                var_num--;
                if (var_num < 0)
                    var_num = num_vars - 1;
                var_inst = var_list[var_num].num_inst - 1;
            }
        }
        else if (key == KEYPAD_PREV_BOTTOM ||
                 key == KEYPAD_NEXT_BOTTOM)
        {
            var_list[var_num].select(obj_inst, var_inst);
        }
    } while (key != KEYPAD_ENTER);

    return true;
}

void
config_erase(void)
{
    uint32_t zero = 0;
    
    // Erase the magic value
    config_write(CONFIG_MAGIC_ADDR, &zero, CONFIG_MAGIC_SIZE);

    // Erase the version number
    config_write(CONFIG_VERSION_ADDR, &zero, CONFIG_VERSION_SIZE);
}

#ifdef PLATFORM_dro550
bool
config_load(FILE* in)
{
    uint8_t buffer[128];
    uint32_t len;
    uint32_t addr;
    
    for (addr = 0; addr < CONFIG_END_ADDR; addr += len)
    {
        len = fread(buffer, 1, sizeof(buffer), in);
        if (len <= 0)
        {
            TRACE_WARNING("config_load error\n");
            break;
        }
        config_write(addr, buffer, len);
    }
    
    return (len > 0);
}

bool
config_save(FILE* out)
{
    uint8_t buffer[128];
    uint32_t len;
    uint32_t addr;
    
    for (addr = 0; addr < CONFIG_END_ADDR; addr += len)
    {
        len = config_read(addr, buffer, sizeof(buffer));
        if (len <= 0)
        {
            TRACE_WARNING("config_save error\n");
            break;
        }
        fwrite(buffer, 1, len, out);
    }
    
    return (len > 0);
}
#endif

bool
config_get_version(config_version_t* version)
{
    // Check the magic value and read in the version
    if (!config_is_valid() ||
        config_read(CONFIG_VERSION_ADDR, version, CONFIG_VERSION_SIZE) != CONFIG_VERSION_SIZE)
    {
        return false;
    }

    return true;
}

void
config_set_version(config_version_t* version)
{
    uint32_t magic = CONFIG_MAGIC_VALUE;

    // Write the magic value
    config_write(CONFIG_MAGIC_ADDR, &magic, CONFIG_MAGIC_SIZE);

    // Write the configuration version
    config_write(CONFIG_VERSION_ADDR, version, CONFIG_VERSION_SIZE);
}

void
config_init(void)
{
    uint32_t obj_addr;
    dirent_t* dirent;

    eeprom_init();

    // Make sure the config is valid
    if (!config_is_valid())
    {
        TRACE_WARNING("config is invalid\n");
        return;
    }
    
    // Read the top level objects into the directory
    for (obj_addr = CONFIG_START_ADDR;
         obj_addr < CONFIG_END_ADDR;
         obj_addr += dirent->tlv.length)
    {
        if(dir_end >= ARRAY_SIZE(dir_obj))
        {
            TRACE_WARNING("Config object dir full\n");
            break;
        }

        // Read the object tlv into the directory
        dirent = &dir_obj[dir_end];
        config_read(obj_addr, &dirent->tlv, sizeof(tlv_t));
        dirent->addr = obj_addr;
        obj_addr += sizeof(tlv_t);

        // Stop if the end of the config is reached
        if (dirent->tlv.id == CONFIG_OID_END || dirent->tlv.id >= CONFIG_OID_MAX)
            break;

        // Advance the end of directory
        dir_end++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// L U A
///////////////////////////////////////////////////////////////////////////////

#ifdef PLATFORM_dro550

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lrotable.h"
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
#include "luash.h"

static int
config_load_lua(lua_State* L)
{
    FILE* file;
    
    file = fopen(luaL_checkstring(L, 1), "r");
    if (!file)
        return luaL_error(L, "unable to open file");
    if (!config_load(file))
    {
        fclose(file);
        return luaL_error(L, "error loading config");
    }
    fclose(file);
    
    // Reset the system
    reset();
    
    return 0;
}

static int
config_save_lua(lua_State* L)
{
    FILE* file;
    
    file = fopen(luaL_checkstring(L, 1), "w");
    if (!file)
        return luaL_error(L, "unable to open file");
    if (!config_save(file))
    {
        fclose(file);
        return luaL_error(L, "error saving config");
    }
    fclose(file);
    
    return 0;
}

const LUA_REG_TYPE config_map_lua[] =
{
    { LSTRKEY("load"), LFUNCVAL(config_load_lua) },
    { LSTRKEY("save"), LFUNCVAL(config_save_lua) },
    { LNILKEY, LNILVAL }
};

LUALIB_API int
config_open_lua(lua_State* L)
{
    LREGISTER(L, "config", config_map_lua);
}

#endif
