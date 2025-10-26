#include "Includes.h"

class ParticleMatrix {
public:
	// Attributes
	unsigned int rows;
	unsigned int columns;
	unsigned int scaleFactor;
	sf::VertexArray particlesVertices;
	std::vector<std::vector<Particle>> matrix;

	// Constructor / Destructor
	ParticleMatrix(unsigned int sizeX, unsigned int sizeY, unsigned int scaleFactor);
	~ParticleMatrix();

	// Functions
	void setCell(int x, int y, sf::Color colour);
};