#include "Includes.h"

int main()
{
	// Particle matrix parameters
	const unsigned int matrix_X = 1920;
	const unsigned int matrix_Y = 1080;
	const unsigned int scaleFactor = 10;
	// Game window parameters
	const unsigned int windowSize_X = 1920;
	const unsigned int windowSize_Y = 1080;

	// Load Game Matrix
	ParticleMatrix particleMatrix(matrix_X, matrix_Y, scaleFactor);
	// Load Game Window
	GameWindow game(windowSize_X, windowSize_Y, scaleFactor, particleMatrix);
	// Start Game Loop
	game.run();
}