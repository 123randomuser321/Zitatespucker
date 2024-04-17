/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Core

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


/* check for version information, should be passed by make */
#if !defined (ZITATESPUCKER_VERSION_MAJOR) || !defined (ZITATESPUCKER_VERSION_MINOR) || !defined (ZITATESPUCKER_VERSION_PATCH)
	#error "Version information undefined, maybe a Makefile error?"
#endif


/* Standard headers */
#include <stdlib.h>


/* Internal headers */
#include "Zitatespucker/Zitatespucker_common.h"


/* Common functions */

void ZitatespuckerGetVersion(uint8_t *major, uint8_t *minor, uint8_t *patch) {

	if (major != NULL)
		*major = ZITATESPUCKER_VERSION_MAJOR;
	
	if (minor != NULL)
		*minor = ZITATESPUCKER_VERSION_MINOR;
	
	if (patch != NULL)
		*patch = ZITATESPUCKER_VERSION_PATCH;
	
	return;

}

void ZitatespuckerZitatInit(ZitatespuckerZitat *ZitatToInit) {

	ZitatToInit->author = NULL;
	ZitatToInit->zitat = NULL;
	ZitatToInit->day = 0;
	ZitatToInit->month = 0;
	ZitatToInit->year = 0;
	ZitatToInit->annodomini = false;
	ZitatToInit->nextZitat = NULL;

	return;

}

void ZitatespuckerZitatFree(ZitatespuckerZitat *ZitatToFree) {

	if (ZitatToFree->author != NULL)
		free((void *) ZitatToFree->author);
	
	if (ZitatToFree->zitat != NULL)
		free((void *) ZitatToFree->zitat);
	
	if (ZitatToFree->nextZitat != NULL)
		ZitatespuckerZitatFree(ZitatToFree->nextZitat);
	
	free((void *) ZitatToFree);

	return;

}
