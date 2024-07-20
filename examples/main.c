#include "sephine/sephine.h"

#include "bowser.png.h"
#include "checkertile.png.h"

#include "vert.glsl.h"
#include "frag.glsl.h"

RenderObject *cube;
RenderObject *floor_obj;
Shader *shader;
mat4 projection = GLM_MAT4_IDENTITY_INIT,
     view = GLM_MAT4_IDENTITY_INIT;

Texture *bowser, *checker;

void game_init(void)
{
	float verts[] = {
		/* vertex		tex coords  normals */
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,	0.0, 0.0, -1.0,
		1.0f, -1.0f, -1.0f,	1.0f, 0.0f,	0.0, 0.0, -1.0,
		1.0f,  1.0f, -1.0f,	1.0f, 1.0f,	0.0, 0.0, -1.0,
		1.0f,  1.0f, -1.0f,	1.0f, 1.0f,	0.0, 0.0, -1.0,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,	0.0, 0.0, -1.0,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,	0.0, 0.0, -1.0,

		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,	0.0, 0.0, 1.0,
		1.0f, -1.0f,  1.0f,	1.0f, 0.0f,	0.0, 0.0, 1.0,
		1.0f,  1.0f,  1.0f,	1.0f, 1.0f,	0.0, 0.0, 1.0,
		1.0f,  1.0f,  1.0f,	1.0f, 1.0f,	0.0, 0.0, 1.0,
		-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,	0.0, 0.0, 1.0,
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,	0.0, 0.0, 1.0,

		-1.0f,  1.0f,  1.0f,	1.0f, 0.0f,	-1.0, 0.0, 0.0,
		-1.0f,  1.0f, -1.0f,	1.0f, 1.0f,	-1.0, 0.0, 0.0,
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,	-1.0, 0.0, 0.0,
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,	-1.0, 0.0, 0.0,
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,	-1.0, 0.0, 0.0,
		-1.0f,  1.0f,  1.0f,	1.0f, 0.0f,	-1.0, 0.0, 0.0,

		1.0f,  1.0f,  1.0f,	1.0f, 0.0f,	1.0, 0.0, 0.0,
		1.0f,  1.0f, -1.0f,	1.0f, 1.0f,	1.0, 0.0, 0.0,
		1.0f, -1.0f, -1.0f,	0.0f, 1.0f,	1.0, 0.0, 0.0,
		1.0f, -1.0f, -1.0f,	0.0f, 1.0f,	1.0, 0.0, 0.0,
		1.0f, -1.0f,  1.0f,	0.0f, 0.0f,	1.0, 0.0, 0.0,
		1.0f,  1.0f,  1.0f,	1.0f, 0.0f,	1.0, 0.0, 0.0,

		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,	0.0, -1.0, 0.0,
		1.0f, -1.0f, -1.0f,	1.0f, 1.0f,	0.0, -1.0, 0.0,
		1.0f, -1.0f,  1.0f,	1.0f, 0.0f,	0.0, -1.0, 0.0,
		1.0f, -1.0f,  1.0f,	1.0f, 0.0f,	0.0, -1.0, 0.0,
		-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,	0.0, -1.0, 0.0,
		-1.0f, -1.0f, -1.0f,	0.0f, 1.0f,	0.0, -1.0, 0.0,

		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,	0.0, 1.0, 0.0,
		1.0f,  1.0f, -1.0f,	1.0f, 1.0f,	0.0, 1.0, 0.0,
		1.0f,  1.0f,  1.0f,	1.0f, 0.0f,	0.0, 1.0, 0.0,
		1.0f,  1.0f,  1.0f,	1.0f, 0.0f,	0.0, 1.0, 0.0,
		-1.0f,  1.0f,  1.0f,	0.0f, 0.0f,	0.0, 1.0, 0.0,
		-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,	0.0, 1.0, 0.0,
	};

	float floor_verts[] = {
		-1.0, -1.0, -1.0,	0.0, 16.0,	0.0, 1.0, 0.0,
		1.0, -1.0, -1.0,	16.0, 16.0,	0.0, 1.0, 0.0,
		1.0, -1.0,  1.0,	16.0, 0.0,	0.0, 1.0, 0.0,
		1.0, -1.0,  1.0,	16.0, 0.0,	0.0, 1.0, 0.0,
		-1.0, -1.0,  1.0,	0.0, 0.0,	0.0, 1.0, 0.0,
		-1.0, -1.0, -1.0,	0.0, 16.0,	0.0, 1.0, 0.0,
	};

	renderer_create();

	cube = render_object_alloc();
	render_object_set_vertex_data(cube, verts, sizeof(verts), 8*sizeof(float));

	floor_obj = render_object_alloc();
	render_object_set_vertex_data(floor_obj,
				      floor_verts,
				      sizeof(floor_verts),
				      8 * sizeof(float));

	shader = shader_alloc();
	shader_set_data(shader, vert_glsl, frag_glsl);

	glm_perspective(glm_rad(45.0), 1000.0/800.0, 0.1, 100, projection);

	glm_vec3_copy((vec3){0.0, 0.5, 6.0}, player.camera_pos);
	glm_vec3_copy((vec3){0.0, 0.0, -1.0}, player.camera_front);
	glm_vec3_copy((vec3){0.0, 1.0, 0.0}, player.camera_up);

	shader_bind(shader);
	shader_set_mat4(shader, "projection", (float *) projection);

	bowser = texture_alloc();
	texture_set_data(bowser, bowser_png, sizeof(bowser_png));

	checker = texture_alloc();
	texture_set_data(checker, checkertile_png, sizeof(checkertile_png));
	glm_scale(floor_obj->model, (vec3){20,1,20});
	glm_translate(cube->model, (vec3){0.0, 0.5, 0.0});
}

void game_frame_func(void)
{
	vec3 tmp;

	glm_vec3_add(player.camera_pos, player.camera_front, tmp);
	glm_lookat(player.camera_pos, tmp, player.camera_up, view);
	shader_set_mat4(shader, "view", (float *) view);
	glm_rotate(cube->model, glm_rad(0.5), (vec3){0.0, 1.0, 0.0});

	renderer_draw(cube, shader, bowser);
	renderer_draw(floor_obj, shader, checker);
}

void game_cleanup_func(void)
{
	render_object_free(cube);
	render_object_free(floor_obj);
}

int main(void)
{
	GameInfo game_info = {0};
	int ret;

	game_info.width = 1000;
	game_info.height = 800;
	game_info.name = "Foo";

	game_info.clear_color.r = 0.1;
	game_info.clear_color.g = 0.1;
	game_info.clear_color.b = 0.1;
	game_info.clear_color.a = 1.0;

	game_info.init_func = game_init;
	game_info.run_func = game_frame_func;
	game_info.cleanup_func = game_cleanup_func;

	ret = game_run(&game_info);

	return ret;
}
