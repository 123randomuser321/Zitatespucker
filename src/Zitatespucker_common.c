/*
	SPDX-License-Identifier: LGPL-3.0-only
	
	Zitatespucker: Library to spit out quotes (and relating information)
	Core

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


/* check for version information, should be passed by make */
#if !defined (ZITATESPUCKER_VERSION_MAJOR) || !defined (ZITATESPUCKER_VERSION_MINOR) || !defined (ZITATESPUCKER_VERSION_PATCH)
	#error "Version information undefined, maybe a Makefile error?"
#endif


/* Standard headers */
#include <stdlib.h>


/* Internal headers */
#include "../Zitatespucker/Zitatespucker_common.h"


/* Common functions */

void ZitatespuckerGetVersion(uint8_t *major, uint8_t *minor, uint8_t *patch)
{
	if (major != NULL)
		*major = ZITATESPUCKER_VERSION_MAJOR;
	
	if (minor != NULL)
		*minor = ZITATESPUCKER_VERSION_MINOR;
	
	if (patch != NULL)
		*patch = ZITATESPUCKER_VERSION_PATCH;
	
	return;
}

void ZitatespuckerZitatInit(ZitatespuckerZitat *ZitatToInit)
{
	ZitatToInit->author = NULL;
	ZitatToInit->zitat = NULL;
	ZitatToInit->comment = NULL;
	ZitatToInit->day = 0;
	ZitatToInit->month = 0;
	ZitatToInit->year = 0;
	ZitatToInit->annodomini = false;
	ZitatToInit->nextZitat = NULL;
	ZitatToInit->prevZitat = NULL;

	return;
}

void ZitatespuckerZitatFree(ZitatespuckerZitat *ZitatToFree)
{
	if (ZitatToFree == NULL)
		return;
	
	if (ZitatToFree->prevZitat != NULL)
		ZitatespuckerZitatFreePrevOnly(ZitatToFree->prevZitat);
	
	ZitatespuckerZitatFreeNextOnly(ZitatToFree);

	return;
}

void ZitatespuckerZitatFreeNextOnly(ZitatespuckerZitat *ZitatToFree)
{
	ZitatespuckerZitat *next;

	while (ZitatToFree != NULL) {
		if (ZitatToFree->author != NULL)
			free((void *) ZitatToFree->author);

		if (ZitatToFree->zitat != NULL)
			free((void *) ZitatToFree->zitat);

		if (ZitatToFree->comment != NULL)
			free((void *) ZitatToFree->comment);
		
		if (ZitatToFree->nextZitat != NULL) {
			next = ZitatToFree->nextZitat;
			free((void *) ZitatToFree);
			ZitatToFree = next;
		} else
			break;
	}

	return;
}

void ZitatespuckerZitatFreePrevOnly(ZitatespuckerZitat *ZitatToFree)
{
	ZitatespuckerZitat *prev;

	while (ZitatToFree != NULL) {
		if (ZitatToFree->author != NULL)
			free((void *) ZitatToFree->author);

		if (ZitatToFree->zitat != NULL)
			free((void *) ZitatToFree->zitat);

		if (ZitatToFree->comment != NULL)
			free((void *) ZitatToFree->comment);
		
		if (ZitatToFree->prevZitat != NULL) {
			prev = ZitatToFree->prevZitat;
			free((void *) ZitatToFree);
			ZitatToFree = prev;
		} else
			break;
	}

	return;
}

size_t ZitatespuckerZitatListLen(ZitatespuckerZitat *ZitatList)
{
	if (ZitatList == NULL)
		return 0;
	
	size_t ret = 1;

	ZitatespuckerZitat *ZitatListPrev = ZitatList;
	while ((ZitatListPrev = ZitatListPrev->prevZitat) != NULL)
		ret++;

	while ((ZitatList = ZitatList->nextZitat) != NULL)
		ret++;
	
	return ret;
}
