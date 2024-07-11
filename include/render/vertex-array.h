#ifndef __RENDER_VERTEX_ARRAY_H_
#define __RENDER_VERTEX_ARRAY_H_

struct _VertexArray;

typedef struct _VertexArray VertexArray;

VertexArray *vertex_array_alloc(void);
void vertex_array_bind(VertexArray *array);
void vertex_array_unbind(void);
void vertex_array_free(VertexArray *array);
void vertex_array_add_attribute(VertexArray *array, int count, int stride);

#endif
