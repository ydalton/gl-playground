#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "core/file.h"
#include "core/log.h"

static inline size_t get_size(FILE *f)
{
	size_t size;

	fseek (f, 0, SEEK_END);
	size = ftell (f);
	fseek (f, 0, SEEK_SET);

	return size;
}

struct File *read_file(const char *name)
{
	struct File *file;
	size_t ret;
	FILE *fp;

	fp = fopen(name, "rb");
	if(!fp) {
		WARN("Could not open file %s\n", name);
		return NULL;
	}


	file = calloc(1, sizeof(*file));
	file->size = get_size(fp);
	file->data = malloc(file->size);

	ret = fread(file->data, 1, file->size, fp);
	/* otherwise we couldn't read the entire file */
	assert(ret == file->size);

	fclose(fp);

	return file;
}

void file_free(struct File *file)
{
	free(file->data);
	free(file);
}
