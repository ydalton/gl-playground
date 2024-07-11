#include <stdlib.h>

#include "glad/glad.h"
#include "render/vertex-buffer.h"

struct _VertexBuffer {
	unsigned int render_ID;
};

VertexBuffer *vertex_buffer_alloc(void)
{
	VertexBuffer *buffer;

	buffer = calloc(1, sizeof(*buffer));
	glGenBuffers(1, &buffer->render_ID);

	return buffer;
}

void vertex_buffer_set_data(VertexBuffer *buffer, float *data, size_t size)
{
	vertex_buffer_bind(buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void vertex_buffer_bind(VertexBuffer *buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer->render_ID);
}

void vertex_buffer_unbind(void)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vertex_buffer_free(VertexBuffer *buffer)
{
	glDeleteBuffers(1, &buffer->render_ID);
	free(buffer);
}
