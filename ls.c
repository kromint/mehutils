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
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

// this idea is stolen from busybox by the way
enum { 
    OPT_a = (1 << 0),
    OPT_A = (1 << 1)
};

/* FUNCTION PROTOTYPES */
static char** buffer_entries(DIR* dir, uint64_t* size, size_t* count);
// strcmp wrapper because stupid qsort wont accept strcmp
static int compare_names(const void* a, const void* b);

int main(int argc,char* argv[]) {
    int cliopt; // used to find what arguments are used (only the getopt part)
    int opt=0;
    while((cliopt = getopt(argc, argv, "aA"))!=-1) {
	switch(cliopt) {
	    case('a'):
		opt |= OPT_a;
		break;
	    case('A'):
		opt |= OPT_A;
		break;
	}
    }

    DIR* opened_dir;

    char* path = NULL;

    for(int i = 1; i < argc; i++) {
	if(argv[i][0] == '-') continue;
	path=argv[i];
	if(path!=NULL) break;
    }

    opened_dir = path ? opendir(path) : opendir(".");

    if(opened_dir == NULL) {
	fprintf(stderr, "ls: cannot access '%s': %s\n", path, strerror(errno));
	return 2;
    }

    uint64_t buffer_size;
    size_t buffer_count;
    char** dir_buffer;

    dir_buffer = buffer_entries(opened_dir,&buffer_size,&buffer_count);
    if(dir_buffer == NULL) {
	closedir(opened_dir);
	return 1;
    }

    closedir(opened_dir);

    qsort(dir_buffer,buffer_count,sizeof(char*),compare_names);

    for(size_t i = 0; i<buffer_count; i++) {
	printf("%s\n",dir_buffer[i]);
    }

    free(dir_buffer); 
    return 0;
}

static char** buffer_entries(DIR* dir, uint64_t* size, size_t* count) {
    *size = 256;
    char** entry = malloc(*size*sizeof(char*));
    if(entry == NULL) {
	perror("ls: allocation error");
	return NULL;
    }

    struct dirent* dirent_tmp;

    for(*count = 0; (dirent_tmp = readdir(dir)) != NULL; (*count)++) {
	if(*count>=*size-1) {
	    *size *= 2;
	    char** tmp = realloc(entry,*size*sizeof(char*));
	    if(tmp == NULL) {
		perror("ls: allocation error");
		free(entry);
		return NULL;
	    }
	    entry=tmp;
	}
	entry[*count] = strdup(dirent_tmp->d_name);
    }

    return entry;
}

static int compare_names(const void *a, const void *b) {
    const char * const *s1 = a;
    const char * const *s2 = b;

    return strcmp(*s1, *s2);
}


