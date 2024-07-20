#ifndef __GAME_H_
#define __GAME_H_

#define TRUE	1
#define FALSE	0

#include "cglm/cglm.h"

typedef struct {
	const char *name;
	unsigned int width;
	unsigned int height;

	struct {
		float r;
		float g;
		float b;
		float a;
	} clear_color;

	void (*init_func)(void);
	void (*run_func)(void);
	void (*cleanup_func)(void);
} GameInfo;

typedef struct {
	vec3 camera_pos, camera_front, camera_up;
	float pitch, yaw, camera_speed;
} Player;

extern Player player;

int game_run(GameInfo *game_info);

#endif /* __GAME_H_ */
