/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with sqlite to read from SQL-files

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


/* Standard headers */
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


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

/*
	Get the string content of column iCol from prepared statement ZitatStmt.

	NULL may be a valid return.
	This function duplicates the string and check for proper NULL-termination.
*/
static inline char *ZitatespuckerSQLGetStringAllocated(sqlite3_stmt *ZitatStmt, int iCol);


/* Externally callable */

size_t ZitatespuckerSQLGetAmountFromFile(const char *filename)
{
	// perhaps one could replace this using a SQL COUNT statement
	// but as of now, this is on par with the JSON backend

	if (filename == NULL) {
		(void) fprintf(stderr, "%s:%d:%s: recieved NULL filename!\n", __FILE__, __LINE__, __func__);
		return 0;
	}
	
	sqlite3 *db;
	size_t ret = 0;

	if (sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_open_v2() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		return ret;
	}

	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2(db, "SELECT * FROM ZitatespuckerZitat", -1, &statement, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_prepare_v2() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
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
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
	(void) sqlite3_close(db);

	return ret;
}

ZitatespuckerZitat *ZitatespuckerSQLGetZitatAllFromFile(const char *filename)
{
	if (filename == NULL) {
		(void) fprintf(stderr, "%s:%d:%s: recieved NULL filename!\n", __FILE__, __LINE__, __func__);
		return NULL;
	}
	
	sqlite3 *db;

	if (sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_open_v2() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		return NULL;
	}

	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2(db, "SELECT * FROM ZitatespuckerZitat", -1, &statement, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_prepare_v2() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
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
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
	(void) sqlite3_close(db);

	return ret;
}

ZitatespuckerZitat *ZitatespuckerSQLGetZitatAllFromFileByAuthor(const char *filename, const char *authorname)
{
	if (filename == NULL) {
		(void) fprintf(stderr, "%s:%d:%s: recieved NULL filename!\n", __FILE__, __LINE__, __func__);
		return NULL;
	} else if (authorname == NULL) {
		(void) fprintf(stderr, "%s:%d:%s: recieved NULL authorname!\n", __FILE__, __LINE__, __func__);
		return NULL;
	}

	sqlite3 *db;

	if (sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_open_v2() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		return NULL;
	}

	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2(db, "SELECT * FROM ZitatespuckerZitat WHERE author=?1", -1, &statement, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_prepare_v2() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	// insert the author into the query
	if (sqlite3_bind_text(statement, 1, authorname, -1, SQLITE_STATIC) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_bind_text() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		if (sqlite3_finalize(statement) != SQLITE_OK)
			(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
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
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
	(void) sqlite3_close(db);

	return ret;
}

ZitatespuckerZitat *ZitatespuckerSQLGetZitatAllFromFileByDate(const char *filename, bool annodomini, uint16_t year, uint8_t month, uint8_t day)
{
	if (filename == NULL) {
		(void) fprintf(stderr, "%s:%d:%s: recieved NULL filename!\n", __FILE__, __LINE__, __func__);
		return NULL;
	} else if (year == 0 && annodomini == false) {
		(void) fprintf(stderr, "%s:%d:%s: annodomini cannot be false when year is 0.\n", __FILE__, __LINE__, __func__);
		return NULL;
	} else if (day != 0 && month == 0) {
		(void) fprintf(stderr, "%s:%d:%s: month cannot be 0 when day is not 0.\n", __FILE__, __LINE__, __func__);
		return NULL;
	}
	
	sqlite3 *db;

	if (sqlite3_open_v2(filename, &db, SQLITE_OPEN_READONLY, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_open_v2() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		return NULL;
	}

	char *sSQL;
	// determine what SQL query to use
	if (day != 0)
		sSQL = "SELECT * FROM ZitatespuckerZitat WHERE annodomini = ?1 AND year = ?2 AND month = ?3 AND day = ?4";
	else if (month != 0)
		sSQL = "SELECT * FROM ZitatespuckerZitat WHERE annodomini = ?1 AND year = ?2 AND month = ?3";
	else
		sSQL = "SELECT * FROM ZitatespuckerZitat WHERE annodomini = ?1 AND year = ?2";
	
	sqlite3_stmt *statement;
	if (sqlite3_prepare_v2(db, sSQL, -1, &statement, NULL) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_prepare_v2() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	// insert everything
	// annodomini
	// I am not actually sure if SQLITE_TRANSIENT is correct in this place
	if (sqlite3_bind_text(statement, 1, (annodomini ? "true" : "false"), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_bind_int() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		if (sqlite3_finalize(statement) != SQLITE_OK)
			(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	// year
	if (sqlite3_bind_int(statement, 2, year) != SQLITE_OK) {
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_bind_int() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		if (sqlite3_finalize(statement) != SQLITE_OK)
			(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	// month
	if (month != 0) {
		if (sqlite3_bind_int(statement, 3, month) != SQLITE_OK) {
			(void) fprintf(stderr, "%s:%d:%s: sqlite3_bind_int() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
			if (sqlite3_finalize(statement) != SQLITE_OK)
				(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
			sqlite3_close(db);
			return NULL;
		}
	}

	// day
	if (day != 0) {
		if (sqlite3_bind_int(statement, 4, day) != SQLITE_OK) {
			(void) fprintf(stderr, "%s:%d:%s: sqlite3_bind_int() failed:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
			if (sqlite3_finalize(statement) != SQLITE_OK)
				(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
			sqlite3_close(db);
			return NULL;
		}
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
		(void) fprintf(stderr, "%s:%d:%s: sqlite3_finalize() reported an error:\n%s\n", __FILE__, __LINE__, __func__, sqlite3_errmsg(db));
	(void) sqlite3_close(db);

	return ret;
}


/* Static function definitions */

static ZitatespuckerZitat *ZitatespuckerSQLGetPopulatedStruct(sqlite3_stmt *ZitatStmt)
{
	// mucho importante: SQLite type coercion table
	ZitatespuckerZitat *Zitat;
	if ((Zitat = (ZitatespuckerZitat *) malloc(sizeof(ZitatespuckerZitat))) == NULL) {
		(void) fprintf(stderr, "%s:%d:%s: malloc() returned NULL.\n", __FILE__, __LINE__, __func__);
		return NULL;
	}
	// init
	ZitatespuckerZitatInit(Zitat);

	// author
	Zitat->author = ZitatespuckerSQLGetStringAllocated(ZitatStmt, 0);

	// zitat
	Zitat->zitat = ZitatespuckerSQLGetStringAllocated(ZitatStmt, 1);

	// comment
	Zitat->comment = ZitatespuckerSQLGetStringAllocated(ZitatStmt, 2);

	// day
	int32_t tmpInt = sqlite3_column_int(ZitatStmt, 3);
	if (tmpInt < 0)
		tmpInt = 0;
	else if (tmpInt > UINT8_MAX)
		tmpInt = UINT8_MAX;
	Zitat->day = (uint8_t) tmpInt;

	// month
	tmpInt = sqlite3_column_int(ZitatStmt, 4);
	if (tmpInt < 0)
		tmpInt = 0;
	else if (tmpInt > UINT8_MAX)
		tmpInt = UINT8_MAX;
	Zitat->month = (uint8_t) tmpInt;

	// year
	tmpInt = sqlite3_column_int(ZitatStmt, 5);
	if (tmpInt < 0)
		tmpInt = 0;
	else if (tmpInt > UINT16_MAX)
		tmpInt = UINT16_MAX;
	Zitat->year = (uint16_t) tmpInt;

	// annodomini
	size_t bytelen = sqlite3_column_bytes(ZitatStmt, 6);
	if (bytelen != 5)
		Zitat->annodomini = false;
	else {
		const char *tmpS = (char *) sqlite3_column_text(ZitatStmt, 6);
		if (tmpS == NULL) // only on OOM
			Zitat->annodomini = false;
		else if (strncmp(tmpS, "true", 5) == 0)
			Zitat->annodomini = true;
		else
			Zitat->annodomini = false;
	}

	return Zitat;
}

static inline char *ZitatespuckerSQLGetStringAllocated(sqlite3_stmt *ZitatStmt, int iCol)
{
	size_t bytelen = sqlite3_column_bytes(ZitatStmt, iCol);
	if (bytelen == 0)
		return NULL;
	
	const char *tmpS = (char *) sqlite3_column_text(ZitatStmt, iCol);
	if (tmpS == NULL) // happens only on OOM, theoretically
		return NULL;
	
	if (tmpS[bytelen - 1] == '\0') {
		// yay
		char *ret = (char *) malloc(bytelen * sizeof(char));
		if (ret != NULL)
			(void) strcpy(ret, tmpS);
		return ret;
	} else {
		// oof, can happen if you try to get BLOB as TEXT
		char *ret = (char *) malloc((bytelen + 1) * sizeof(char));
		if (ret != NULL) {
			(void) strncpy(ret, tmpS, bytelen);
			ret[bytelen] = '\0';
		}
		return ret;
	}
}
