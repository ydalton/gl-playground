#include "glad/glad.h"

#include "render/renderer.h"
#include "core/log.h"

typedef struct {
	int initialized;
} Renderer;

static Renderer renderer = {0};

int renderer_create(void)
{
	if(renderer.initialized) {
		WARN("Attempt to call create on an already initialized renderer!\n");
		return 1;
	}

	return 0;
}

void renderer_clear(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_draw(RenderObject *obj, Shader *shader, Texture *texture)
{
	if(texture)
		texture_bind(texture);
	shader_bind(shader);
	shader_set_mat4(shader, "model", (float *) obj->model);

	glBindVertexArray(obj->vao);
	glDrawArrays(GL_TRIANGLES, 0, obj->vertex_count);
}

void renderer_destroy(void)
{
}
