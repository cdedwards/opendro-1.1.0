///////////////////////////////////////////////////////////////////////////////
// OpenDRO Project
//
// Debug tracing
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
#ifndef TRACE_H_
#define TRACE_H_

/// Trace levels
typedef enum {
    TRACE_LEVEL_DEBUG,
    TRACE_LEVEL_INFO,
    TRACE_LEVEL_WARNING,
    TRACE_LEVEL_ERROR,
    TRACE_LEVEL_FATAL,
    TRACE_LEVEL_MAX // MUST BE LAST!!!
} trace_level_t;


#define TRACE_FATAL(...)        trace_printf(TRACE_LEVEL_FATAL,     "<F> " __VA_ARGS__)
#define TRACE_ERROR(...)        trace_printf(TRACE_LEVEL_ERROR,     "<E> " __VA_ARGS__)
#define TRACE_WARNING(...)      trace_printf(TRACE_LEVEL_WARNING,   "<W> " __VA_ARGS__)
#ifdef DEBUG
#define TRACE_INFO(...)         trace_printf(TRACE_LEVEL_INFO,      "<I> " __VA_ARGS__)
#define TRACE_DEBUG(...)        trace_printf(TRACE_LEVEL_DEBUG,     "<D> " __VA_ARGS__)
#else
#define TRACE_INFO(...)
#define TRACE_DEBUG(...)
#endif

#define TRACE_FATAL_WP(...)     trace_printf(TRACE_LEVEL_FATAL,     __VA_ARGS__)
#define TRACE_ERROR_WP(...)     trace_printf(TRACE_LEVEL_ERROR,     __VA_ARGS__)
#define TRACE_WARNING_WP(...)   trace_printf(TRACE_LEVEL_WARNING,   __VA_ARGS__)
#ifdef DEBUG
#define TRACE_INFO_WP(...)      trace_printf(TRACE_LEVEL_INFO,      __VA_ARGS__)
#define TRACE_DEBUG_WP(...)     trace_printf(TRACE_LEVEL_DEBUG,     __VA_ARGS__)
#else
#define TRACE_INFO_WP(...)
#define TRACE_DEBUG_WP(...)
#endif

/// \brief  Display a trace message of a certain priority
///
/// \param  level   Priority level
/// \param  format  Printf-style format string
/// \param  ...     Variable parameters for format string
void
trace_printf(trace_level_t   level,
             const char*     format,
             ...);

/// \brief  Set the trace display priority level
///
/// \param  level   Priority level
void
trace_level(trace_level_t level);

/// \brief  Save trace settings
void
trace_save(void);

/// \brief  Initialize the trace
void
trace_init(void);

#endif // TRACE_H_
