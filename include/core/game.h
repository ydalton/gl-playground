#ifndef __GAME_H_
#define __GAME_H_

#define TRUE	1
#define FALSE	0

typedef struct {
	const char *name;
	unsigned int width;
	unsigned int height;

	void (*init_func)(void);
} GameInfo;

int game_run(GameInfo *game_info);

#endif /* __GAME_H_ */
