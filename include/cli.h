///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Command-line interface
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
#ifndef CLI_H_
#define CLI_H_

#include "types.h"

/// \brief Asynchronously notify the CLI that input is received
void
cli_rx(void);

/// \brief Save the CLI settings
void
cli_save(void);

/// \brief Initialize the CLI
void
cli_init(void);

#endif // CLI_H_
