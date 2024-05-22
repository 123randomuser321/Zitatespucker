/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with sqlite to read from SQL-files (Tests)

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
#include <assert.h>


/* Zitatespucker */
#define ZITATESPUCKER_FEATURE_SQLITE
#include <Zitatespucker/Zitatespucker.h>


int main(int argc, char **argv)
{
	printf("ZitatespuckerSQLGetAmountFromFile:\n");
	printf("Checking whether an incorrect filename results in zero size...\n");
	assert(ZitatespuckerSQLGetAmountFromFile("wrongfilename.json") == 0);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerSQLGetZitatAllFromFile:\n");
	printf("Checking whether a NULL filename results in a NULL pointer...\n");
	assert(ZitatespuckerSQLGetZitatAllFromFile(NULL) == NULL);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerSQLGetZitatAllFromFileByAuthor:\n");
	printf("Checking whether a NULL filename results in a NULL pointer...\n");
	assert(ZitatespuckerSQLGetZitatAllFromFileByAuthor(NULL, NULL) == NULL);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerSQLGetZitatAllFromFileByAuthor:\n");
	printf("Checking whether a NULL authorname results in a NULL pointer...\n");
	assert(ZitatespuckerSQLGetZitatAllFromFileByAuthor("testfile.sqlite", NULL) == NULL);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerSQLGetZitatAllFromFileByDate:\n");
	printf("Checking whether a NULL filename results in a NULL pointer...\n");
	assert(ZitatespuckerSQLGetZitatAllFromFileByDate(NULL, true, 0, 0, 0) == NULL);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerSQLGetZitatAllFromFileByDate:\n");
	printf("Checking whether year == 0 && annodomini == false results in a NULL pointer...\n");
	assert(ZitatespuckerSQLGetZitatAllFromFileByDate("testfile.sqlite", false, 0, 0, 0) == NULL);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerSQLGetZitatAllFromFileByDate:\n");
	printf("Checking whether day != 0 && month == 0 results in a NULL pointer...\n");
	assert(ZitatespuckerSQLGetZitatAllFromFileByDate("testfile.sqlite", true, 0, 0, 1) == NULL);
	printf("OKAY!\n\n\n");

	printf("ALL CHECKS PASSED!\n\n\n\n\n");
}
