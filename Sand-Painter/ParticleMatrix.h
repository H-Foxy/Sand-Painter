#include "Includes.h"

class ParticleMatrix {
public:
	// Attributes
	unsigned int m_rows;
	unsigned int m_columns;
	unsigned int m_scale_factor;
	sf::VertexArray m_particles_vertices;
	std::vector<std::vector<Particle>> m_matrix;

	// Constructor / Destructor
	ParticleMatrix(unsigned int size_x, unsigned int size_y, unsigned int scale_factor);
	~ParticleMatrix();

	void setCellParticle(int x, int y, Particle particle);

private:
	// Functions
	void setCellVertexColours(int x, int y, sf::Color colour);
};