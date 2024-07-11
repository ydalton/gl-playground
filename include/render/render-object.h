#ifndef __RENDER_RENDER_OBJECT_H_
#define __RENDER_RENDER_OBJECT_H_

#include <stddef.h>

struct RenderObject_;

typedef struct RenderObject_ RenderObject;

RenderObject *render_object_alloc(void);
void render_object_set_vertex_data(RenderObject *object, float *data, size_t size, size_t stride);
void render_object_free(RenderObject *object);
void render_object_draw(RenderObject *object);

#endif
