#ifndef __RENDER_INDEX_BUFFER_H_
#define __RENDER_INDEX_BUFFER_H_

#include <stddef.h>

struct IndexBufferPrivate;

typedef struct {
	unsigned int count;
	struct IndexBufferPrivate *priv;
} IndexBuffer;

IndexBuffer *index_buffer_alloc(void);
void index_buffer_init(IndexBuffer *buffer, 
		       unsigned int *data, 
		       size_t count);
void index_buffer_bind(IndexBuffer *buffer);
void index_buffer_unbind(void);
void index_buffer_free(IndexBuffer *buffer);

#endif
