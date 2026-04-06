#include "GameWindow.h"
#include "ParticleMatrix.h"
#include "Brush.h"

int main()
{
	// Particle matrix parameters
	const unsigned int matrix_x = 1920;
	const unsigned int matrix_y = 1080;
	const unsigned int scale_factor = 5;
	// Game window parameters
	const unsigned int window_size_x = 1920;
	const unsigned int window_size_y = 1080;

	// Load Game Matrix
	ParticleMatrix particle_matrix(matrix_x, matrix_y, scale_factor);
	// Load Brush
	Brush brush(50, scale_factor, sf::Vector2i{100,100});
	// Load Game Window
	GameWindow game(window_size_x, window_size_y, scale_factor, particle_matrix, brush);
	// Start Game Loop
	game.run();
}