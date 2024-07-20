#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "glad/glad.h"

#include "sephine/core/game.h"
#include "sephine/render/renderer.h"

static struct {
	const char *name;
	int width, height;
	SDL_Window *window;
	SDL_GLContext context;
	int running;
} game_instance;

Player player = {0};

static void game_init(GameInfo *game_info)
{
	game_instance.width = game_info->width;
	game_instance.height = game_info->height;
	game_instance.name = SDL_strdup(game_info->name);

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
		player.pitch = 0;
		player.yaw = -90;
		first_mouse = false;
	}

	player.yaw += xoffset;
	player.pitch -= yoffset;

	if(player.pitch > 89.0f)
		player.pitch = 89.0f;
	if(player.pitch < -89.0f)
		player.pitch = -89.0f;

	vec3 direction;
	direction[0] = cos(glm_rad(player.yaw)) * cos(glm_rad(player.pitch));
	direction[1] = sin(glm_rad(player.pitch));
	direction[2] = sin(glm_rad(player.yaw)) * cos(glm_rad(player.pitch));
	vec3 normalized;
	glm_normalize_to(direction, normalized);
	glm_vec3_copy(normalized, player.camera_front);
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
		glm_vec3_mul(player.camera_front, front_vector, mul);
		glm_vec3_scale(mul, player.camera_speed, mul);
		glm_vec3_add(player.camera_pos, mul, player.camera_pos);
	}
	if(keystates[SDL_SCANCODE_S]) {
		glm_vec3_mul(player.camera_front, front_vector, mul);
		glm_vec3_scale(mul, player.camera_speed, mul);
		glm_vec3_sub(player.camera_pos, mul, player.camera_pos);
	}
	if(keystates[SDL_SCANCODE_A]) {
		glm_cross(player.camera_front, player.camera_up, mul);
		glm_normalize(mul);
		glm_vec3_scale(mul, player.camera_speed, mul);
		glm_vec3_sub(player.camera_pos, mul, player.camera_pos);
	}
	if(keystates[SDL_SCANCODE_D]) {
		glm_cross(player.camera_front, player.camera_up, mul);
		glm_normalize(mul);
		glm_vec3_scale(mul, player.camera_speed, mul);
		glm_vec3_add(player.camera_pos, mul, player.camera_pos);
	}

	if(keystates[SDL_SCANCODE_LSHIFT]) {
		player.camera_speed = 0.3;
	} else {
		player.camera_speed = 0.1;
	}
}

int game_run(GameInfo *game_info)
{
	game_init(game_info);

	game_instance.window = SDL_CreateWindow(game_instance.name,
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					game_instance.width,
					game_instance.height,
					SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	assert(game_instance.window != NULL);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	game_instance.context = SDL_GL_CreateContext(game_instance.window);
	assert(gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress));

	renderer_set_clear_color(game_info->clear_color.r,
				 game_info->clear_color.g,
				 game_info->clear_color.b,
				 game_info->clear_color.a);

	if(game_info->init_func)
		game_info->init_func();

	while(game_instance.running) {
		handle_events();
		renderer_clear();

		if(game_info->run_func)
			game_info->run_func();

		SDL_GL_SwapWindow(game_instance.window);
	}

	if(game_info->cleanup_func)
		game_info->cleanup_func();
	renderer_destroy();
	SDL_GL_DeleteContext(game_instance.context);
	SDL_DestroyWindow(game_instance.window);
	SDL_Quit();

	return 0;
}
