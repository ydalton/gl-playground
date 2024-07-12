#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "glad/glad.h"

#include "core/game.h"
#include "render/renderer.h"
#include "render/render-object.h"
#include "render/texture.h"
#include "render/shader.h"

#include "vert.glsl.h"
#include "frag.glsl.h"

#include "bowser.png.h"
#include "checkertile.png.h"

static struct {
	const char *name;
	int width, height;
	SDL_Window *window;
	SDL_GLContext context;
	int running;
} game_instance;

struct Player {
	vec3 camera_pos, camera_front, camera_up;
	float pitch, yaw, camera_speed;
} p = {0};

static void game_init(GameInfo *game_info)
{
	game_instance.width = game_info->width;
	game_instance.height = game_info->height;
	game_instance.name = game_info->name;

	game_instance.running = TRUE;

	SDL_Init(0);

	SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "0");
}

static void update_camera_rotation(float xoffset, float yoffset)
{
	float sensitivity = 0.01;
	static bool first_mouse = true;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	if(first_mouse) {
		p.pitch = 0;
		p.yaw = -90;
		first_mouse = false;
	}

	p.yaw += xoffset;
	p.pitch -= yoffset;

	if(p.pitch > 89.0f)
		p.pitch = 89.0f;
	if(p.pitch < -89.0f)
		p.pitch = -89.0f;

	vec3 direction;
	direction[0] = cos(glm_rad(p.yaw)) * cos(glm_rad(p.pitch));
	direction[1] = sin(glm_rad(p.pitch));
	direction[2] = sin(glm_rad(p.yaw)) * cos(glm_rad(p.pitch));
	vec3 normalized;
	glm_normalize_to(direction, normalized);
	glm_vec3_copy(normalized, p.camera_front);
}

static void handle_events(void)
{
	SDL_Event ev;
	while(SDL_PollEvent(&ev)) {
		switch(ev.type) {
		case SDL_QUIT:
			game_instance.running = FALSE;
			break;
		case SDL_KEYDOWN:
			if(ev.key.keysym.sym == 'q') {
				game_instance.running = FALSE;
			}
			break;
		case SDL_MOUSEMOTION:
			update_camera_rotation(ev.motion.xrel, ev.motion.yrel);
			break;
		}
	}
	const unsigned char *keystates = SDL_GetKeyboardState(NULL);
	vec3 front_vector = {1.0, 0.0, 1.0};
	vec3 mul;

	if(keystates[SDL_SCANCODE_W]) {
		glm_vec3_mul(p.camera_front, front_vector, mul);
		glm_vec3_scale(mul, p.camera_speed, mul);
		glm_vec3_add(p.camera_pos, mul, p.camera_pos);
	}
	if(keystates[SDL_SCANCODE_S]) {
		glm_vec3_mul(p.camera_front, front_vector, mul);
		glm_vec3_scale(mul, p.camera_speed, mul);
		glm_vec3_sub(p.camera_pos, mul, p.camera_pos);
	}
	if(keystates[SDL_SCANCODE_A]) {
		glm_cross(p.camera_front, p.camera_up, mul);
		glm_normalize(mul);
		glm_vec3_scale(mul, p.camera_speed, mul);
		glm_vec3_sub(p.camera_pos, mul, p.camera_pos);
	}
	if(keystates[SDL_SCANCODE_D]) {
		glm_cross(p.camera_front, p.camera_up, mul);
		glm_normalize(mul);
		glm_vec3_scale(mul, p.camera_speed, mul);
		glm_vec3_add(p.camera_pos, mul, p.camera_pos);
	}

	if(keystates[SDL_SCANCODE_LSHIFT]) {
		p.camera_speed = 0.3;
	} else {
		p.camera_speed = 0.1;
	}
}

RenderObject *cube;
RenderObject *floor_obj;
Shader *shader;
mat4 projection = GLM_MAT4_IDENTITY_INIT,
     view = GLM_MAT4_IDENTITY_INIT;

struct {
	GLuint frag, vert, prog;
} ogl;

static void init_gl()
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

	game_instance.context = SDL_GL_CreateContext(game_instance.window);

	assert(gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress));

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

	glm_vec3_copy((vec3){0.0, 0.5, 6.0}, p.camera_pos);
	glm_vec3_copy((vec3){0.0, 0.0, -1.0}, p.camera_front);
	glm_vec3_copy((vec3){0.0, 1.0, 0.0}, p.camera_up);

	shader_bind(shader);
	shader_set_mat4(shader, "projection", (float *) projection);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1, 0.1, 0.1, 1.0);
}

int game_run(GameInfo *game_info)
{
	Texture *bowser, *checker;
	vec3 tmp;

	game_init(game_info);

	game_instance.window = SDL_CreateWindow("Game",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					game_instance.width,
					game_instance.height,
					SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	assert(game_instance.window != NULL);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	init_gl();


	bowser = texture_alloc();
	texture_set_data(bowser, bowser_png, sizeof(bowser_png));

	checker = texture_alloc();
	texture_set_data(checker, checkertile_png, sizeof(checkertile_png));
	glm_scale(floor_obj->model, (vec3){20,1,20});
	glm_translate(cube->model, (vec3){0.0, 0.5, 0.0});

	while(game_instance.running) {
		handle_events();

		renderer_clear();

		glm_vec3_add(p.camera_pos, p.camera_front, tmp);
		glm_lookat(p.camera_pos, tmp, p.camera_up, view);
		shader_set_mat4(shader, "view", (float *) view);
		glm_rotate(cube->model, glm_rad(0.5), (vec3){0.0, 1.0, 0.0});

		renderer_draw(cube, shader, bowser);
		renderer_draw(floor_obj, shader, checker);

		SDL_GL_SwapWindow(game_instance.window);
	}

	render_object_free(cube);
	renderer_destroy();
	SDL_GL_DeleteContext(game_instance.context);
	SDL_DestroyWindow(game_instance.window);
	SDL_Quit();

	return 0;
}
