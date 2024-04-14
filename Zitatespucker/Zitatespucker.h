/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Core (header)
*/
#ifndef ZITATESPUCKER_H
#define ZITATESPUCKER_H


// TODO: STDC and STDC_VERSION check
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


// also: random function (RollDX?)
// "NOFEATURES" preproc flag to stop compilation when nothing is enabled?
#endif
