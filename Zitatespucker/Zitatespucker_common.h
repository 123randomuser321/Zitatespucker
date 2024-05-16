/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Internal definitions (header)

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
#ifndef ZITATESPUCKER_COMMON_H
#define ZITATESPUCKER_COMMON_H


/* Standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


/* Key names for the ZitatespuckerZitat arrays within a .json */
#define ZITATESPUCKERZITATKEYNAME 		"ZitatespuckerZitat"
#define AUTHOR							"author"
#define ZITAT							"zitat"
#define COMMENT							"comment"
#define DAY								"day"
#define MONTH							"month"
#define YEAR							"year"
#define ANNODOMINI						"annodomini"


/* The Zitat */
typedef struct ZitatespuckerZitat {
	char *author; /* Author of the quote */
	char *zitat; /* The quote itself */
	char *comment; /* A comment on the quote */
	uint8_t day; /* Day of origin */
	uint8_t month; /* Month of origin */
	uint16_t year; /* Year of origin; if this is 0 and annodomini is false, this value is considered invalid */
	bool annodomini; /* true --> AD; false --> BC */
	struct ZitatespuckerZitat *nextZitat; /* Points to the next ZitatespuckerZitat (linked list behavior) */
	struct ZitatespuckerZitat *prevZitat; /* Points to the previous ZitatespuckerZitat (doubly linked list behavior) */
} ZitatespuckerZitat;


/* Common functions */

/*
	Get the current version of the library.

	Or, alternatively, the version you statically linked.

	Version is returned by storing numbers in the passed pointers, each of which may be NULL (to not store something).
*/
void ZitatespuckerGetVersion(uint8_t *major, uint8_t *minor, uint8_t *patch);


/*
	Initialize a ZitatespuckerZitat.

	You generally do not want to call this yourself, as the backends do the allocation.
*/
void ZitatespuckerZitatInit(ZitatespuckerZitat *ZitatToInit);

/*
	free a ZitatespuckerZitat linked list
	This function doesn't care for the length of the list and will work even on just one element.
*/
void ZitatespuckerZitatFree(ZitatespuckerZitat *ZitatToFree);

/*
	free a ZitatespuckerZitat linked list, only following the nextZitat pointer
	This function doesn't care for the length of the list and will work even on just one element.
*/
void ZitatespuckerZitatFreeNextOnly(ZitatespuckerZitat *ZitatToFree);

/*
	free a ZitatespuckerZitat linked list, only following the prevZitat pointer
	This function doesn't care for the length of the list and will work even on just one element.
*/
void ZitatespuckerZitatFreePrevOnly(ZitatespuckerZitat *ZitatToFree);

/*
	Returns the number of entries in the linked list ZitatList.
	This check both ways! (prevZitat and nextZitat)
	
	0 if passed a NULL pointer.
*/
size_t ZitatespuckerZitatListLen(ZitatespuckerZitat *ZitatList);


#endif
