/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with json-c to read from .json files
*/


/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>


/* json-c headers */
#include <json-c/json.h>


/* Internal headers */
#include "Zitatespucker/Zitatespucker_json-c.h"


/* Static function declarations */

/*
	Get a json_type_array json_object from filename, using the predefined key.
	Returns NULL on error.

	The returned (non-NULL) object must be freed using json_object_put().
*/
static json_object *ZitatespuckerJSONGetZitatArrayFromFile(const char *filename);

/*
	Populate a ZitatespuckerZitat struct with the information within ZitatObj and return it.
	NULL on error.
	
	This function allocates, and the given object must be freed with ZitatespuckerZitatFree().
*/
static ZitatespuckerZitat *ZitatespuckerJSONPopulateStruct(json_object *ZitatObj);

/*
	Get child object of name keyName from Parent, returning its string content.

	NULL on error.
*/
static inline char *ZitatespuckerJSONGetStringAllocated(json_object *Parent, const char *keyName, json_object *child);

/*
	Get child object of name keyName from Parent, returning its integer content.
*/
static inline int32_t ZitatespuckerJSONGetInt(json_object *Parent, const char *keyName, json_object *child);


/* Externally callable */

size_t ZitatespuckerJSONGetAmountFromFile(const char *filename) {

	json_object *ZitatArray;
	if ((ZitatArray = ZitatespuckerJSONGetZitatArrayFromFile(filename)) == NULL)
		return 0;
	else {
		size_t ret = json_object_array_length(ZitatArray);
		json_object_put(ZitatArray);
		return ret;
	}

}

ZitatespuckerZitat *ZitatespuckerJSONGetZitatSingle(const char *filename, const size_t idx) {

	json_object *ZitatArray;
	if ((ZitatArray = ZitatespuckerJSONGetZitatArrayFromFile(filename)) == NULL)
		return NULL;
	else {
		json_object *ZitatObj = json_object_array_get_idx(ZitatArray, idx);
		ZitatespuckerZitat *ret = ZitatespuckerJSONPopulateStruct(ZitatObj);
		json_object_put(ZitatArray);
		return ret;
	}

}


/* Static function definitions */

static json_object *ZitatespuckerJSONGetZitatArrayFromFile(const char *filename) {

	json_object *globalscope;
	if ((globalscope = json_object_from_file(filename)) == NULL) {
		fprintf(stderr, "%s:%d:%s: json_object_from_file() failed:\n%s\n", __FILE__, __LINE__, __func__, json_util_get_last_err());
		return NULL;
	}

	json_object *ZitatArray;
	if (!json_object_object_get_ex(globalscope, ZITATESPUCKERZITATKEYNAME, &ZitatArray)) {
		fprintf(stderr, "%s:%d:%s: json_object_object_get_ex() failed:\nKey does not exist.\n", __FILE__, __LINE__, __func__);
		json_object_put(globalscope);
		return NULL;
	} else {
		json_object_get(ZitatArray);
		json_object_put(globalscope);
		return ZitatArray;
	}

}

static ZitatespuckerZitat *ZitatespuckerJSONPopulateStruct(json_object *ZitatObj) {

	ZitatespuckerZitat *Zitat;
	if ((Zitat = (ZitatespuckerZitat *) malloc(sizeof(ZitatespuckerZitat))) == NULL) {
		fprintf(stderr, "%s:%d:%s: malloc() returned NULL.\n", __FILE__, __LINE__, __func__);
		return NULL;
	}
	// init
	ZitatespuckerZitatInit(Zitat);

	json_object *tmpObj = NULL;

	// author
	Zitat->author = ZitatespuckerJSONGetStringAllocated(ZitatObj, AUTHOR, tmpObj);
	tmpObj = NULL;

	// zitat
	Zitat->zitat = ZitatespuckerJSONGetStringAllocated(ZitatObj, ZITAT, tmpObj);
	tmpObj = NULL;

	// day
	int32_t tmpInt = ZitatespuckerJSONGetInt(ZitatObj, DAY, tmpObj);
	if (tmpInt < 0)
		tmpInt = 0;
	else if (tmpInt > UINT8_MAX)
		tmpInt = UINT8_MAX;
	Zitat->day = (uint8_t) tmpInt;

	// month
	tmpInt = ZitatespuckerJSONGetInt(ZitatObj, MONTH, tmpObj);
	if (tmpInt < 0)
		tmpInt = 0;
	else if (tmpInt > UINT8_MAX)
		tmpInt = UINT8_MAX;
	Zitat->month = (uint8_t) tmpInt;

	// year
	tmpInt = ZitatespuckerJSONGetInt(ZitatObj, YEAR, tmpObj);
	if (tmpInt < 0)
		tmpInt = 0;
	else if (tmpInt > UINT16_MAX)
		tmpInt = UINT16_MAX;
	Zitat->year = (uint16_t) tmpInt;

	// annodomini
	if (json_object_object_get_ex(ZitatObj, ANNODOMINI, &tmpObj)) {
		Zitat->annodomini = json_object_get_boolean(tmpObj);
		tmpObj = NULL;
	}


	return Zitat;

}

static inline char *ZitatespuckerJSONGetStringAllocated(json_object *Parent, const char *keyName, json_object *child) {

	if (json_object_object_get_ex(Parent, keyName, &child)) {
		size_t len;
		if ((len = json_object_get_string_len(child)) != 0) {
			char *tmpS = malloc(len + 1);
			strcpy(tmpS, json_object_get_string(child));
			return tmpS;
		}
	} else // TODO: print an error
		return NULL;

}

static inline int32_t ZitatespuckerJSONGetInt(json_object *Parent, const char *keyName, json_object *child) {

	if (json_object_object_get_ex(Parent, keyName, &child)) {
		int32_t tmpInt = json_object_get_int(child);
		if (errno == EINVAL) // errno is not guaranteed to be unset, so this might trigger a false positive
			fprintf(stderr, "%s:%d:%s: No valid conversion for key \"%s\".\n", __FILE__, __LINE__, __func__, keyName);
		return tmpInt;
	} else {
		fprintf(stderr, "%s:%d:%s: Key \"%s\" not found.\n", __FILE__, __LINE__, __func__, keyName);
		return 0;
	}

}
