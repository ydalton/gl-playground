#include <stdlib.h>

#include "glad/glad.h"
#include "render/index-buffer.h"

struct IndexBufferPrivate {
	unsigned int render_ID;
};

IndexBuffer *index_buffer_alloc(void)
{
	IndexBuffer *buf; 

	buf = calloc(1, sizeof(*buf));
	buf->priv = calloc(1, sizeof(struct IndexBufferPrivate));

	glGenBuffers(1, &buf->priv->render_ID);

	return buf;

}

void index_buffer_init(IndexBuffer *buffer, 
		       unsigned int *data,
		       size_t count)
{
	index_buffer_bind(buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		     count * sizeof(unsigned int),
		     data,
		     GL_STATIC_DRAW);
	index_buffer_unbind();
}

void index_buffer_bind(IndexBuffer *buffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->priv->render_ID);
}
void index_buffer_unbind(void)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer_free(IndexBuffer *buffer)
{
	glDeleteBuffers(1, &buffer->priv->render_ID);

	free(buffer->priv);
	free(buffer);
}

