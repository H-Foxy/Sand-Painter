#include "GameWindow.h"
#include "ParticleMatrix.h"
#include "Brush.h"

int main()
{
	// Game Settings
	const unsigned int window_size_x = 1920;
	const unsigned int window_size_y = 1080;
	const unsigned int cell_size = 5;

	ParticleMatrix particle_matrix(window_size_x, window_size_y, cell_size);
	Brush brush(50, cell_size, sf::Vector2i{100,100});
	GameWindow game(window_size_x, window_size_y, cell_size, particle_matrix, brush);

	// Game Loop
	game.run();
}