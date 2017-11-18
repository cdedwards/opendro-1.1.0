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
#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>
#include "types.h"

/// Configuration version
typedef struct config_version
{
    uint8_t major;
    uint8_t minor;
    uint8_t maint;
    uint8_t pad;
} config_version_t;

/// Variable descriptor list node which describes how to perform setup on an object variable
typedef struct var_list
{
    /// Display name of the variable
    char    *name;
    /// Total number of instances
    uint8_t num_inst;
    // Function pointer called to display the variable value
    void    (*display)(uint8_t obj_inst, uint8_t var_inst);
    /// Function pointer called to change the variable value
    void    (*select)(uint8_t obj_inst, uint8_t var_inst);
} var_list_t;

/// Configuration object IDs for top-level objects 
typedef enum config_oid
{
    CONFIG_OID_END,             ///< Special value for end of config
    CONFIG_OID_MACHINE,         ///< Virutal machine object
    CONFIG_OID_AXIS,            ///< Axis object
    CONFIG_OID_SCALE,           ///< Scale object
    CONFIG_OID_DISPLAY,         ///< Display object
    CONFIG_OID_FUNCTION,        ///< Function settings
    CONFIG_OID_SYSTEM,          ///< System settings
    CONFIG_OID_KEYPAD,          ///< Keypad settings
    CONFIG_OID_TRACE,           ///< Trace logging
    CONFIG_OID_CLI,             ///< Command line interface
    CONFIG_OID_MAX,             ///< MUST be last
} config_oid_t;

/// \brief  Get the configuration version
///
/// \param  version Pointer to version store
///
/// \return true if version was retrieved, false if not
bool
config_get_version(config_version_t* version);

/// \brief  Set the configuration version
///
/// \param  version Pointer to version store
void
config_set_version(config_version_t* version);

/// \brief  Open a configuration object instance for reading.
///
/// \param  oid     Object identifier
/// \param  oindex  Object instance index
///
/// \return Returns true if object was successfully opened and false if
/// there was an error.
bool
config_obj_read(config_oid_t    oid,
                uint8_t         oindex);

/// \brief  Open a configuration object instance for reading and
/// writing.  If the object does not exist, then it is created.
///
/// \param  oid     Object identifier
/// \param  oindex  Object instance index
///
/// \return Returns true if object was successfully opened and false if
/// there was an error.
bool
config_obj_write(config_oid_t   oid,
                 uint8_t        oindex);

/// \brief  Close the currently opened object and write all updates.
void
config_obj_close(void);

/// \brief  Write a variable to an object.  If the variable does not exist,
/// then it is created if and only if the current object is newly created.
///
/// \param  vid         Variable identifier
/// \param  vindex      Variable instance index
/// \param  vid         Length of the variable value buffer
/// \param  value       Pointer to opaque variable value
///
/// \return Returns true if the variable was written or false if there
/// was an error.
bool
config_var_write(uint8_t        vid,
                 uint8_t        vindex,
                 uint16_t       length,
                 const void*    value);

/// \brief  Read a variable from an object.
///
/// \param  vid         Variable identifier
/// \param  vindex      Variable instance index
/// \param  vid         Length of the variable value buffer
/// \param  value       Pointer to opaque variable value
///
/// \return Returns true if the variable was read or false if there
/// was an error.
bool
config_var_read(uint8_t  vid,
                uint8_t  vindex,
                uint16_t length,
                void*    value);

/// \brief  Perform the setup for an object based on the passed list of variable descriptors
///
/// \param  obj_name    Object name text string
/// \param  num_objs    Number of object instances
/// \param  num_vars    Number of variables per object
/// \param  var_list    List of variable descriptors
///
/// \return Returns true if the setup is to be updated and false if not
bool
config_setup(const char*       obj_name,
             uint8_t           num_objs,
             uint8_t           num_vars,
             const var_list_t* var_list);

/// \brief  Erase all configuration data
void
config_erase(void);

#ifdef PLATFORM_dro550
/// \brief  Load configuration from a file
bool
config_load(FILE* in);

/// \brief  Save configuration to a file
bool
config_save(FILE* out);
#endif

/// \brief  Initialize the configuration from non-volatile memory
void
config_init(void);

#endif // CONFIG_H_
