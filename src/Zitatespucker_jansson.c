/*
	SPDX-License-Identifier: LGPL-3.0-only
	
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with jansson to read from .json files (header)

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


/* jansson headers */
#include <jansson.h>


/* Internal headers */
#include "Zitatespucker/Zitatespucker_jansson.h"


/* Static function declarations */

/*
	Get a json_t of type array from filename, using the predefined key.
	Returns NULL on error.

	The returned (non-NULL) object must be freed using json_decref().
*/
static json_t *ZitatespuckerJSONGetZitatArrayFromFile(const char *filename);

/*
	Returns a pointer to single populated ZitatespuckerZitat.
	idx refers to the array index within ZitatArray.
	NULL on error.

	This function allocates, and the given object must be freed with ZitatespuckerZitatFree().
*/
static ZitatespuckerZitat *ZitatespuckerJSONGetZitatSingle(json_t *ZitatArray, const size_t idx);


/* Externally callable */

size_t ZitatespuckerJSONGetAmountFromFile(const char *filename)
{
	json_t *zitatscope = ZitatespuckerJSONGetZitatArrayFromFile(filename);

	size_t ret = 0;
	if (zitatscope != NULL) {
		ret = json_array_size(zitatscope);
		json_decref(zitatscope);
	}
	
	return ret;
}

ZitatespuckerZitat *ZitatespuckerJSONGetZitatSingleFromFile(const char *filename, const size_t idx)
{
	json_t *zitatscope = ZitatespuckerJSONGetZitatArrayFromFile(filename);
	if (zitatscope == NULL)
		return NULL;
	else {
		ZitatespuckerZitat *ret = ZitatespuckerJSONGetZitatSingle(zitatscope, idx);
		json_decref(zitatscope);
		return ret;
	}
}

ZitatespuckerZitat *ZitatespuckerJSONGetZitatAllFromFile(const char *filename)
{
	json_t *ZitatArray;
	if ((ZitatArray = ZitatespuckerJSONGetZitatArrayFromFile(filename)) == NULL)
		return NULL;
	else {
		size_t len = json_array_size(ZitatArray);
		
		ZitatespuckerZitat *ret = ZitatespuckerJSONGetZitatSingle(ZitatArray, 0);
		if (ret != NULL) {
			size_t i = 1;
			ZitatespuckerZitat *cur = ret;
			ZitatespuckerZitat *prev;
			for ( ; i < len; i++) {
				cur->nextZitat = ZitatespuckerJSONGetZitatSingle(ZitatArray, i);
				if (cur->nextZitat != NULL) {
					prev = cur;
					cur = cur->nextZitat;
					cur->prevZitat = prev;
				} else
					break;
			}
		}
		json_decref(ZitatArray);
		return ret;
	}
}


/* Static function definitions */

static json_t *ZitatespuckerJSONGetZitatArrayFromFile(const char *filename)
{
	json_error_t err;
	json_t *globalscope = json_load_file(filename, 0, &err); // remember: reference count
	if (globalscope == NULL) {
		#ifndef ZITATESPUCKER_NOPRINT
		(void) fprintf(stderr, "%s:%d:%s: json_load_file() failed:\n%s", __FILE__, __LINE__, __func__, err.text);
		#endif
		return NULL;
	}

	json_t *zitatscope = json_object_get(globalscope, ZITATESPUCKERZITATKEYNAME);
	if (zitatscope == NULL) {
		json_decref(globalscope);
		#ifndef ZITATESPUCKER_NOPRINT
		(void) fprintf(stderr, "%s:%d:%s: json_object_get() failed:\nMaybe the key %s does not exist?", __FILE__, __LINE__, __func__, ZITATESPUCKERZITATKEYNAME);
		#endif
		return NULL;
	}

	json_incref(zitatscope);
	json_decref(globalscope);

	// json_array_size() also checks this, but the return value does not tell us if it was the case on error
	if (!json_is_array(zitatscope)) {
		json_decref(zitatscope);
		#ifndef ZITATESPUCKER_NOPRINT
		(void) fprintf(stderr, "%s:%d:%s: Key %s is not an array!\n", __FILE__, __LINE__, __func__, ZITATESPUCKERZITATKEYNAME);
		#endif
		return NULL;
	}

	return zitatscope;
}

static ZitatespuckerZitat *ZitatespuckerJSONGetZitatSingle(json_t *ZitatArray, const size_t idx)
{
	json_t *ZitatObj = json_array_get(ZitatArray, idx);
	if (ZitatObj != NULL) {
		ZitatespuckerZitat *ret = ZitatespuckerJSONGetPopulatedStruct(ZitatObj);
		return ret;
	} else {
		#ifndef ZITATESPUCKER_NOPRINT
		(void) fprintf(stderr, "%s:%d:%s: json_array_get() returned NULL, wrong index?\n", __FILE__, __LINE__, __func__);
		#endif
		return NULL;
	}
}
