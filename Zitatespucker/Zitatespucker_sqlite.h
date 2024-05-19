/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with sqlite to read from SQL-files (header)

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
#ifndef ZITATESPUCKER_SQLITE_H
#define ZITATESPUCKER_SQLITE_H


/* Standard headers */
#include <stddef.h>


/* Internal headers */
#include "Zitatespucker/Zitatespucker_common.h"


/*
    Returns the number of rows within the ZitatespuckerZitat table
    0 if none or an error occured.
*/
size_t ZitatespuckerSQLGetAmountFromFile(const char *filename);

/*
    Returns a pointer to the first element in a linked list.
    NULL on error. (Only finding one element in the file is not considered an error.)
    
    This function allocates, and the given object/objects must be freed with ZitatespuckerZitatFree().
*/
ZitatespuckerZitat *ZitatespuckerSQLGetZitatAllFromFile(const char *filename);

/*
    Returns a pointer to the first element in a linked list, filtered by the author given in authorname.
    NULL on error. (Only finding one element in the file is not considered an error.)
	authorname is not optional, and it being NULL results in a NULL return.
    
    This function allocates, and the given object/objects must be freed with ZitatespuckerZitatFree().
*/
ZitatespuckerZitat *ZitatespuckerSQLGetZitatAllByAuthor(const char *filename, const char *authorname);

/*
	Returns a pointer to the first element in a linked list, filtered by the given date information.
    NULL on error. (Only finding one element in the file is not considered an error.)
	The date information is parsed in the order year->month->day,
	becoming more restrictive as it goes.
	month and day are optional, year is not.
	0 is considered an invalid value, and results in a NULL return.
    
    This function allocates, and the given object/objects must be freed with ZitatespuckerZitatFree().
*/
ZitatespuckerZitat *ZitatespuckerSQLGetZitatAllByDate(const char *filename, uint8_t year, uint8_t month, uint8_t day);


#endif
