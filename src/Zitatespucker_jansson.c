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


/* Externally callable */

size_t ZitatespuckerJSONGetAmountFromFile(const char *filename)
{
	json_error_t err;
	json_t *globalscope = json_load_file(filename, 0, &err); // remember: reference count
	if (globalscope == NULL) {
		#ifndef ZITATESPUCKER_NOPRINT
		(void) fprintf(stderr, "%s:%d:%s: json_load_file() failed:\n%s", __FILE__, __LINE__, __func__, err.text);
		#endif
		return 0;
	}

	json_t *zitatscope = json_object_get(globalscope, ZITATESPUCKERZITATKEYNAME);
	if (zitatscope == NULL) {
		json_decref(globalscope);
		#ifndef ZITATESPUCKER_NOPRINT
		(void) fprintf(stderr, "%s:%d:%s: json_object_get() failed:\nMaybe the key %s does not exist?", __FILE__, __LINE__, __func__, ZITATESPUCKERZITATKEYNAME);
		#endif
		return 0;
	}

	json_incref(zitatscope);
	json_decref(globalscope);

	// json_array_size() also checks this, but the return value does not tell us if it was the case on error
	if (!json_is_array(zitatscope)) {
		json_decref(zitatscope);
		#ifndef ZITATESPUCKER_NOPRINT
		(void) fprintf(stderr, "%s:%d:%s: Key %s is not an array!\n", __FILE__, __LINE__, __func__, ZITATESPUCKERZITATKEYNAME);
		#endif
		return 0;
	}

	size_t ret = json_array_size(zitatscope);
	json_decref(zitatscope);

	return ret;
}
