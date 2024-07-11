#ifndef __RENDER_VERTEX_BUFFER_H_
#define __RENDER_VERTEX_BUFFER_H_

#include <stddef.h>

struct _VertexBuffer;

typedef struct _VertexBuffer VertexBuffer;

VertexBuffer *vertex_buffer_alloc(void);
void vertex_buffer_set_data(VertexBuffer *buffer, float *data, size_t size);
void vertex_buffer_free(VertexBuffer *buffer);
void vertex_buffer_bind(VertexBuffer *buffer);
void vertex_buffer_unbind(void);

#endif
