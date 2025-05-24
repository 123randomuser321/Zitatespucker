/*
	SPDX-License-Identifier: 0BSD
	
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with json-c to read from .json files (Tests)

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
#define ZITATESPUCKER_JSON
#include "../Zitatespucker/Zitatespucker.h"


int main(int argc, char **argv)
{
	printf("ZitatespuckerJSONGetAmountFromFile:\n");
	printf("Checking whether an incorrect filename results in zero array size...\n");
	assert(ZitatespuckerJSONGetAmountFromFile("wrongfilename.json") == 0);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerJSONGetZitatSingleFromFile:\n");
	printf("Checking whether an incorrect filename results in a NULL pointer...\n");
	assert(ZitatespuckerJSONGetZitatSingleFromFile("wrongfilename.json", 0) == NULL);
	printf("OKAY!\n\n");
	printf("Checking whether an incorrect index results in a NULL pointer...\n");
	assert(ZitatespuckerJSONGetZitatSingleFromFile("../testfile.json", 800) == NULL);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerJSONGetZitatAllFromFile:\n");
	printf("Checking whether an incorrect filename results in a NULL pointer...\n");
	assert(ZitatespuckerJSONGetZitatAllFromFile("wrongfilename.json") == NULL);
	printf("OKAY!\n\n\n");

	printf("ZitatespuckerJSONGetZitatArrayFromFile (static):\n");
	printf("Check whether incorrect KeyName triggers json_object_object_get_ex to return false...\n");
	assert(ZitatespuckerJSONGetZitatAllFromFile("../testfile_noarray.json") == NULL);
	printf("OKAY!\n\n\n");

	printf("ALL CHECKS PASSED!\n\n\n\n\n");
}
