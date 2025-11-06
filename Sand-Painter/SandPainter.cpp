#include "Includes.h"

int main()
{
	// Particle matrix parameters
	const unsigned int matrix_x = 1920;
	const unsigned int matrix_y = 1080;
	const unsigned int scale_factor = 10;
	// Game window parameters
	const unsigned int window_size_x = 1920;
	const unsigned int window_size_y = 1080;

	// Load Game Matrix
	ParticleMatrix particleMatrix(matrix_x, matrix_y, scale_factor);
	// Load Game Window
	GameWindow game(window_size_x, window_size_y, scale_factor, particleMatrix);
	// Start Game Loop
	game.run();
}