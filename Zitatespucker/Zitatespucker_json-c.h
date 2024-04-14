/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with json-c to read from .json files (header)
*/
#ifndef ZITATESPUCKER_JSON_C_H
#define ZITATESPUCKER_JSON_C_H


/* Standard headers */
#include <stddef.h>


/* Internal headers */
#include "Zitatespucker_common.h"


/* Externally callable */

/*
	Returns the number of ZitatespuckerZitat elements within a given filename (aka array length).
	0 if none or an error occured.
*/
size_t ZitatespuckerJSONGetAmountFromFile(const char *filename);

/*
	Returns a pointer to single populated ZitatespuckerZitat.
	idx refers to the array index within filename.
	NULL on error.

	This function allocates, and the given object must be freed with ZitatespuckerZitatFree().
*/
ZitatespuckerZitat *ZitatespuckerJSONGetZitatSingleFromFile(const char *filename, const size_t idx);

/*
	Returns a pointer to the first element in a linked list.
	NULL on error. (Only finding one element in the file is not considered an error.)
	lenstore is used to store the number of elements within the linked list, but is optional (can be NULL).

	This function allocates, and the given object/objects must be freed with ZitatespuckerZitatFree().
*/
ZitatespuckerZitat *ZitatespuckerJSONGetZitatAllFromFile(const char *filename, size_t *lenstore);

// TODO:
// Filter functions:
// ZitatespuckerZitat *ZitatespuckerJSONGetZitatAllBy* where * is Author, Year, etc
// function for retrieving from a series of .json files? (link lists together)


#endif
