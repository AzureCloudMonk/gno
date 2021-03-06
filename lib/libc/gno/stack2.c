/*
 * Implementation by Devin Reade, November 1997.  See the stack(3) manual
 * page for details.
 *
 * This file is formatted for tab stops every eight columns.
 *
 * $Id: stack2.c,v 1.2 1998/03/28 16:46:56 gdr-ftp Exp $
 */

#ifdef __ORCAC__
#pragma memorymodel 1
#pragma debug 0
#pragma optimize 78
segment "libc_gno__";
#endif

#include <stdlib.h>
#include <err.h>
#include <gno/gno.h>

static void
_printStack (void) {
	warnx("stack usage: %d bytes", _endStackCheck());
}

void
_reportStack (void) {
	_beginStackCheck();
	atexit(_printStack);
}
	
void
_assertStack (unsigned int bytes, int line, const char *file) {
	static const char *fname = "_assertStack";
	static const char *nostack = "insufficient stack";
	static unsigned int minstack = 0;

	unsigned int currentstack;
	int diff;

	/* the the stack bottom if we don't already have it */
	if (minstack == 0) {
		minstack = _getStackBottom();
	}

	/* get the current stack pointer */
#ifdef __ORCAC__
	asm {
		tsc
		sta	currentstack
	}
#else
#	error "This file requires ORCA/C to compile"
#endif

	/* do the check */
	diff = currentstack - minstack;
	if (diff < bytes) {
		if (file == NULL) {
		    errx(1,"%s: %s (expected %u, had %d bytes left)",
			 fname, nostack, bytes, diff);
		} else {
		    errx(1,"%s at %s:%d: %s (expected %u, had %d bytes left)",
			 fname, file, line, nostack, bytes, diff);
		}
	}
}
