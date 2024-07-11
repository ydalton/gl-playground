#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"

#include "render/vertex-array.h"

struct _VertexArray {
	unsigned int render_ID;
	unsigned int elements;
	size_t offset;
};

VertexArray *vertex_array_alloc(void)
{
	VertexArray *arr;

	arr = calloc(1, sizeof(*arr));
	/* might be useless */
	arr->elements = 0;
	arr->offset = 0;
	glGenVertexArrays(1, &arr->render_ID);

	return arr;
}

void vertex_array_bind(VertexArray *array)
{
	glBindVertexArray(array->render_ID);
}

void vertex_array_unbind(void)
{
	glBindVertexArray(0);
}

void vertex_array_add_attribute(VertexArray *array, int count, int stride)
{
	glVertexAttribPointer(array->elements,
			      count,
			      GL_FLOAT,
			      GL_FALSE,
			      stride,
			      (void*) array->offset);
	glEnableVertexAttribArray(array->elements);
	array->elements++;
	array->offset += count * sizeof(float);
}

void vertex_array_free(VertexArray *array)
{
	glDeleteVertexArrays(1, &array->render_ID);
	free(array);
}
