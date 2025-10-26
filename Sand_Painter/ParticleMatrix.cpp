#include "Includes.h"

ParticleMatrix::ParticleMatrix(unsigned int sizeX, unsigned int sizeY, unsigned int scaleFactor)
	: columns(sizeX / scaleFactor), rows(sizeY / scaleFactor), scaleFactor(scaleFactor), particlesVertices(sf::PrimitiveType::Triangles)
{
	// Initialise matrix with default Particles (Yellow)
	Particle defaultParticle(sf::Color::Yellow);
	matrix.resize(columns, std::vector<Particle>(rows, defaultParticle));
	std::cout << "Game Matrix Initialized: " << columns << " x " << rows << " cells of size " << scaleFactor << " pixels." << std::endl;

    // Convert coordinates to triangle Vertices for rendering
	// Loop through each cell in the matrix
    for (unsigned int x = 0; x < columns; ++x)
    {
        for (unsigned int y = 0; y < rows; ++y)
        {
			// Get color of the current cell
            sf::Color color = matrix[x][y].colour;

			// Calculate pixel positions
            float px = x * scaleFactor;
            float py = y * scaleFactor;

            // Calculate positions of the 4 cell corners
            sf::Vector2f topLeft(px, py);
            sf::Vector2f topRight(px + scaleFactor, py);
            sf::Vector2f bottomRight(px + scaleFactor, py + scaleFactor);
            sf::Vector2f bottomLeft(px, py + scaleFactor);

            // First triangle (top-left, top-right, bottom-right)
            particlesVertices.append({ topLeft, color });
            particlesVertices.append({ topRight, color });
            particlesVertices.append({ bottomRight, color });

            // Second triangle (bottom-right, bottom-left, top-left)
            particlesVertices.append({ bottomRight, color });
            particlesVertices.append({ bottomLeft, color });
            particlesVertices.append({ topLeft, color });
        }
    }
}

// Destructor
ParticleMatrix::~ParticleMatrix() 
{}

void ParticleMatrix::setCell(int x, int y, sf::Color colour) 
{
	// Bounds checking
	if (x < 0 || x >= columns || y < 0 || y >= rows) {
		std::cerr << "Error: setCell(" << x << ", " << y << ") is out of bounds." << std::endl;
		return;
	}

	// Calculate starting index for the cell's vertices
    int i = (y + (x * rows)) * 6;

    // Update color of all 6 vertices
    for (int v = 0; v < 6; ++v) {
        particlesVertices[v + i].color = colour;
    }
	// Update particle colour in matrix
	matrix[x][y].colour = colour;
}