#include <stdlib.h>

#include "glad/glad.h"
#include "render/index-buffer.h"

struct IndexBufferPrivate {
	unsigned int render_ID;
};

static struct IndexBufferPrivate *get_private(IndexBuffer *buffer)
{
	return buffer->priv;
}

IndexBuffer *index_buffer_alloc(void)
{
	IndexBuffer *buf; 
	struct IndexBufferPrivate *priv;

	buf = calloc(1, sizeof(*buf));
	buf->priv = calloc(1, sizeof(struct IndexBufferPrivate));

	priv = buf->priv;

	glGenBuffers(1, &priv->render_ID);

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
	struct IndexBufferPrivate *priv = get_private(buffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, priv->render_ID);
}
void index_buffer_unbind(void)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer_free(IndexBuffer *buffer)
{
	struct IndexBufferPrivate *priv = get_private(buffer);

	glDeleteBuffers(1, &priv->render_ID);

	free(buffer->priv);
	free(buffer);
}

