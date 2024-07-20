#include "glad/glad.h"

#include "sephine/render/renderer.h"
#include "sephine/core/log.h"

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

	glEnable(GL_DEPTH_TEST);

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

void renderer_set_clear_color(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void renderer_destroy(void)
{
}
