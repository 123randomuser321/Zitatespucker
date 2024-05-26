/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with sqlite to read from SQL-files (Tests)

	Copyright (C) 2024 by Sembo Sadur <labmailssadur@gmail.com>

	Permission to use, copy, modify, and/or distribute this software
	for any purpose with or without fee is hereby granted.

	THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
	WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
	OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
	NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
