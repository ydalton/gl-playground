#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "glad/glad.h"

#include "render/render-object.h"
#include "render/vertex-array.h"
#include "render/vertex-buffer.h"

struct RenderObject_ {
	VertexArray *array;
	VertexBuffer *buffer;

	unsigned int vertex_count;
};

RenderObject *render_object_alloc(void)
{
	RenderObject *obj;

	obj = calloc(1, sizeof(*obj));
	obj->array = NULL;

	return obj;
}

void render_object_set_vertex_data(RenderObject *object, float *data, size_t size, size_t stride)
{
	assert(!object->array);
	assert(!object->buffer);


	object->array = vertex_array_alloc();
	object->buffer = vertex_buffer_alloc();
	object->vertex_count = size/stride;

	vertex_array_bind(object->array);
	vertex_buffer_set_data(object->buffer, data, size);
	vertex_array_add_attribute(object->array, 3, stride);
	vertex_array_add_attribute(object->array, 2, stride);
	vertex_array_add_attribute(object->array, 3, stride);

	vertex_buffer_unbind();
	vertex_array_unbind();
}

void render_object_free(RenderObject *object)
{
	vertex_array_free(object->array);
	vertex_buffer_free(object->buffer);
	free(object);
}

void render_object_draw(RenderObject *object)
{
	vertex_array_bind(object->array);
	glDrawArrays(GL_TRIANGLES, 0, object->vertex_count);
}
