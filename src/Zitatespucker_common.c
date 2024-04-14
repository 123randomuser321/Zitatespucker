/*
	Zitatespucker: Library to spit out quotes (and relating information)
	Core
*/


/* Standard headers */
#include <stdlib.h>


/* Internal headers */
#include "Zitatespucker/Zitatespucker_common.h"


/* Common functions */

void ZitatespuckerZitatInit(ZitatespuckerZitat *ZitatToInit) {

	ZitatToInit->author = NULL;
	ZitatToInit->zitat = NULL;
	ZitatToInit->day = 0;
	ZitatToInit->month = 0;
	ZitatToInit->year = 0;
	ZitatToInit->annodomini = true;
	ZitatToInit->nextZitat = NULL;

	return;

}

void ZitatespuckerZitatFree(ZitatespuckerZitat *ZitatToFree) {

	if (ZitatToFree->author != NULL)
		free((void *) ZitatToFree->author);
	
	if (ZitatToFree->zitat != NULL)
		free((void *) ZitatToFree->zitat);
	
	if (ZitatToFree->nextZitat != NULL)
		ZitatespuckerZitatFree(ZitatToFree->nextZitat);
	
	free((void *) ZitatToFree);

	return;

}
