/*
	SPDX-License-Identifier: LGPL-3.0-only
    
    Zitatespucker: Library to spit out quotes (and relating information)
	Core (header)

    Copyright (C) 2024  Sembo Sadur <labmailssadur@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3,
	as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef ZITATESPUCKER_H
#define ZITATESPUCKER_H


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __STDC_VERSION__
	#if __STDC_VERSION__ < 199901L
		#error "Compiler doesn't seem C99-compliant."
	#endif
#else
	#error "Cannot determine C standard compliance."
#endif


/* Required headers */
#include "Zitatespucker_common.h"


/* json-c related things to read from .json files */
#ifdef ZITATESPUCKER_FEATURE_JSON_C
    #define ZITATESPUCKER_HAS_JSON
	#include "Zitatespucker_json-c.h"
#endif

/* jansson related things to read from .json files */
#if defined (ZITATESPUCKER_FEATURE_JANSSON) && !defined (ZITATESPUCKER_HAS_JSON)
    #define ZITATESPUCKER_HAS_JSON
	#include "Zitatespucker_jansson.h"
#endif

/* sqlite3 related things to read from SQL databases */
#ifdef ZITATESPUCKER_FEATURE_SQLITE
    #include "Zitatespucker_sqlite.h"
#endif


#ifdef __cplusplus
}
#endif


#endif
