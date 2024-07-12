#include "core/game.h"


int main(void)
{
	GameInfo game_info;
	int ret;

	game_info.width = 1000;
	game_info.height = 800;
	game_info.name = "Foo";

	ret = game_run(&game_info);

	return ret;
}
