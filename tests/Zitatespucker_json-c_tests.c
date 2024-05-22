/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Interface with json-c to read from .json files (Tests)

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
#define ZITATESPUCKER_FEATURE_JSON_C
#include "Zitatespucker/Zitatespucker.h"


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