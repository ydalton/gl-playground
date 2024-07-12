#ifndef __GAME_FILE_H_
#define __GAME_FILE_H_

#include <stddef.h>

struct File {
	unsigned char *data;
	size_t size;
};

struct File *read_file(const char *name);
void file_free(struct File *file);
	
#endif
