#include "game.h"

int main(void)
{
	Game *game;
	int ret;

	game = game_alloc();

	game->width = 1000;
	game->height = 800;
	game->name = "Foo";

	ret = game_run(game);
	game_free(game);

	return ret;
}
