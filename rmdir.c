/*
 * This file is part of mehutils.
 *
 * Copyright (C) 2026 Mehmet Doğan
 *
 * mehutils is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mehutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mehutils. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

int main(int argc, char* argv[]) {
    if (argc<2) {
	fprintf(stderr,"rmdir: missing operand\n");
	return 1;
    }

    char is_parent = 0;
    char something_failed_noexit = 0;
    int count = 0; 

    for(int i = 1; i < argc; i++) {
	if (argv[i][0] == '-') {
	    if (argv[i][1]=='p') {
		is_parent = 1;
	    } else {
		fprintf(stderr,"rmdir: invalid option -- '%s'\n",argv[i]);
		return 1;
	    }
	    continue;
	}

	count++;
    }

    if(count==0) {
	fprintf(stderr,"rmdir: missing operand\n");
	return 1;
    }

    char* path;
    
    for(int i = 1; i < argc; i++) {
	if (argv[i][0] == '-')  continue;
	path = argv[i];

	for(;;) {
	    if(rmdir(path) == -1) {
		fprintf(stderr,"rmdir: failed to remove '%s': %s\n",argv[i],strerror(errno));
		something_failed_noexit = 1;
	    } else if (is_parent) { // use else because the child dir needs to be deleted first
		path = dirname(path);

		if(strcmp(path,".")) continue;	
	    }

	    break;
	}
	
    }

    if(something_failed_noexit) {
	return 1;
    }
    return 0;
}
