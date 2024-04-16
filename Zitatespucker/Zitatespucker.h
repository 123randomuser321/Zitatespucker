/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Core (header)

    Copyright (C) 2024  Sembo Sadur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef ZITATESPUCKER_H
#define ZITATESPUCKER_H


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
	#include "Zitatespucker_json-c.h"
#endif

// TODO:
// Tests (for error cases etc)
// "NOFEATURES" preproc flag to stop compilation when nothing is enabled?
// UIs: Nuklear (also test on Windows); Win32
// SQL (https://www.sqlite.org/)
#endif
