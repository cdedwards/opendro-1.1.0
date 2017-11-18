///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Size limits on objects and other components
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
#ifndef DROLIM_H_
#define DROLIM_H_

#if PLATFORM_dro375
#  include "dro375/drolim.h"
#elif PLATFORM_dpu550
#  include "dpu550/drolim.h"
#elif PLATFORM_dro550
#  include "dro550/drolim.h"
#else
#  error "No platform defined"
#endif

#if (AXIS_MAX > SCALE_MAX)
#error "You must have at least as many scales as axes"
#endif

#if (DISPLAY_LINE_MAX > AXIS_MAX)
#error "You must have at least as many axes as reading lines"
#endif

#endif // DROLIM_H_
