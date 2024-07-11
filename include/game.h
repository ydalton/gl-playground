#ifndef __GAME_H_
#define __GAME_H_

#define TRUE	1
#define FALSE	0

struct Game {
	const char *name;
	unsigned int width;
	unsigned int height;
	void *priv;
};

typedef struct Game Game;

Game *game_alloc(void);
int game_run(Game *game);
void game_free(Game *game);

#endif /* __GAME_H_ */
