#include "GameWindow.h"

int main()
{
	// Game Settings
	const unsigned int window_size_x = 1920;
	const unsigned int window_size_y = 1080;
	const unsigned int cell_size = 5;

	GameWindow game(window_size_x, window_size_y, cell_size);

	// Game Loop
	game.run();
}
