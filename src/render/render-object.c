#include <stdlib.h>
#include <assert.h>

#include "glad/glad.h"

#include "sephine/render/render-object.h"

struct RenderObjectPrivate {
	unsigned int vbo;

	size_t offset;
	unsigned int elements;
};

RenderObject *render_object_alloc(void)
{
	RenderObject *obj;

	obj = calloc(1, sizeof(*obj));
	obj->priv = calloc(1, sizeof(struct RenderObjectPrivate));
	glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, obj->model);

	return obj;
}

static void add_attribute(RenderObject *obj, int count, int stride)
{
	glVertexAttribPointer(obj->priv->elements,
			      count,
			      GL_FLOAT,
			      GL_FALSE,
			      stride,
			      (void*) obj->priv->offset);
	glEnableVertexAttribArray(obj->priv->elements);
	obj->priv->elements++;
	obj->priv->offset += count * sizeof(float);
}

void render_object_set_vertex_data(RenderObject *object, float *data, size_t size, size_t stride)
{
	unsigned int vao, vbo;

	object->vertex_count = size/stride;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	add_attribute(object, 3, stride);
	add_attribute(object, 2, stride);
	add_attribute(object, 3, stride);

	object->vao = vao;
	object->priv->vbo = vbo;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void render_object_free(RenderObject *object)
{
	glDeleteVertexArrays(1, &object->vao);
	glDeleteBuffers(1, &object->priv->vbo);
}
