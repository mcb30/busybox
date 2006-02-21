/* vi: set sw=4 ts=4: */
/*
 * Linux32/linux64 allows for changing uname emulation.
 *
 * Copyright 2002 Andi Kleen, SuSE Labs.
 * This file is subject to the GNU General Public License v2 
 *
 * Licensed under GPL v2 or later, see file License for details.
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/personality.h>

#include "busybox.h"

int setarch_main(int argc, char **argv)
{
	int pers = -1;

	/* Figure out what personality we are supposed to switch to ...
	 * we can be invoked as either:
	 * argv[0],argv[1] -> "setarch","personality"
	 * argv[0]         -> "personality"
	 */
retry:
	if (!strcmp(argv[0], "linux64"))
		pers = PER_LINUX;
	else if (!strcmp(argv[0], "linux32"))
		pers = PER_LINUX32;
	else if (pers == -1 && argv[1] != NULL) {
		pers = PER_LINUX32;
		++argv;
		goto retry;
	}

	/* make user actually gave us something to do */
	++argv;
	if (argv[0] == NULL)
		bb_show_usage();

	/* Try to set personality */
	if (personality(pers) < 0)
		goto failure;

	/* Try to execute the program */
	execvp(argv[0], argv);

failure:
	bb_perror_msg_and_die(argv[0]);
}