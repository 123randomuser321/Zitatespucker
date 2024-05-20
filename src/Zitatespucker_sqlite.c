/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with sqlite to read from SQL-files

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


/* Standard headers */
#include <stdio.h>


/* SQLite headers */
#include <sqlite3.h>


/* Internal headers */
#include "Zitatespucker/Zitatespucker_sqlite.h"


/* Static function declarations */

/*
	Populate a ZitatespuckerZitat struct the prepared SQL statement ZitatStmt and return it.
	NULL on error.
	
	This function allocates, and the given object must be freed with ZitatespuckerZitatFree().
*/
static ZitatespuckerZitat *ZitatespuckerSQLGetPopulatedStruct(sqlite3_stmt *ZitatStmt);


/* Externally callable */

size_t ZitatespuckerSQLGetAmountFromFile(const char *filename)
{
	// perhaps one could replace this using a SQL COUNT statement
	// but as of now, this is on par with the JSON backend
	
	sqlite3 *db;
	size_t ret = 0;

	if (sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_open_v2() failed:\n%s", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		return ret;
	}

	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2(db, "SELECT * FROM ZitatespuckerZitat", -1, &statement, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_prepare_v2() failed:\n%s", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		sqlite3_close(db);
		return ret;
	}

	// finally, we can start counting
	// if I am understanding the docs correctly
	// also, this doesn't properly communicate an error :(
	while (sqlite3_step(statement) == SQLITE_ROW)
		ret++;
	
	// but this should
	if (sqlite3_finalize(statement) != SQLITE_OK)
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
	(void) sqlite3_close(db);

	return ret;
}

ZitatespuckerZitat *ZitatespuckerSQLGetZitatAllFromFile(const char *filename)
{
	sqlite3 *db;

	if (sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_open_v2() failed:\n%s", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		return NULL;
	}

	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2(db, "SELECT * FROM ZitatespuckerZitat", -1, &statement, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_prepare_v2() failed:\n%s", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	ZitatespuckerZitat *ret = NULL;
	ZitatespuckerZitat *cur;
	
	while (sqlite3_step(statement) == SQLITE_ROW) {
		if (ret != NULL) {
			cur->nextZitat = ZitatespuckerSQLGetPopulatedStruct(statement);
			if (cur->nextZitat != NULL) {
				cur->nextZitat->prevZitat = cur;
				cur = cur->nextZitat;
			}
		} else {
			ret = ZitatespuckerSQLGetPopulatedStruct(statement);
			cur = ret;
		}
	}

	if (sqlite3_finalize(statement) != SQLITE_OK)
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
	(void) sqlite3_close(db);

	return ret;
}


/* Static function definitions */
