///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Board-level hardware definitions
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
#ifndef BOARD_H_
#define BOARD_H_

#if PLATFORM_dro375
#  include "dro375/board.h"
#elif PLATFORM_dpu550
#  include "dpu550/board.h"
#elif PLATFORM_dro550
#  include "dro550/board.h"
#else
#  error "No supported platform defined"
#endif

#endif // BOARD_H_
