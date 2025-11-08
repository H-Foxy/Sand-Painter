#include "Includes.h"

ParticleMatrix::ParticleMatrix(unsigned int size_x, unsigned int size_y, unsigned int scale_factor)
	: m_columns(size_x / scale_factor), m_rows(size_y / scale_factor), m_scale_factor(scale_factor), m_particles_vertices(sf::PrimitiveType::Triangles)
{
	// Initialise matrix with empty Particles (White)
	m_matrix.resize(m_columns, std::vector<Particle>(m_rows, Particle()));
	std::cout << "Game Matrix Initialized: " << m_columns << " x " << m_rows << " cells of size " << scale_factor << " pixels." << std::endl;

    // Convert coordinates to triangle Vertices for rendering
	// Loop through each cell in the matrix
    for (unsigned int x = 0; x < m_columns; ++x)
    {
        for (unsigned int y = 0; y < m_rows; ++y)
        {
			// Get color of the current cell
            sf::Color color = m_matrix[x][y].m_colour;

			// Calculate pixel positions
            float px = x * m_scale_factor;
            float py = y * m_scale_factor;

            // Calculate positions of the 4 cell corners
            sf::Vector2f top_left(px, py);
            sf::Vector2f top_right(px + m_scale_factor, py);
            sf::Vector2f bottom_right(px + m_scale_factor, py + m_scale_factor);
            sf::Vector2f bottom_left(px, py + m_scale_factor);

            // First triangle (top-left, top-right, bottom-right)
            m_particles_vertices.append({ top_left, color });
            m_particles_vertices.append({ top_right, color });
            m_particles_vertices.append({ bottom_right, color });

            // Second triangle (bottom-right, bottom-left, top-left)
            m_particles_vertices.append({ bottom_right, color });
            m_particles_vertices.append({ bottom_left, color });
            m_particles_vertices.append({ top_left, color });
        }
    }
}

// Destructor
ParticleMatrix::~ParticleMatrix() 
{}

void ParticleMatrix::setCellVertexColours(int pos_x, int pos_y, sf::Color colour)
{
    // Calculate starting index for the cell's vertices
    int i = (pos_y + (pos_x * m_rows)) * 6;

    // Update color of all 6 vertices
    for (int v = 0; v < 6; ++v) {
        m_particles_vertices[v + i].color = colour;
    }
}

void ParticleMatrix::setCellParticle(int pos_x, int pos_y, Particle particle)
{
    // Bounds checking
    if (pos_x < 0 || pos_x >= m_columns || pos_y < 0 || pos_y >= m_rows) {
        std::cerr << "Error: setCell(" << pos_x << ", " << pos_y << ") is out of bounds." << std::endl;
        return;
    }

    // Update particle in matrix
    m_matrix[pos_x][pos_y] = particle;

    // Update vertex colors
    setCellVertexColours(pos_x, pos_y, particle.m_colour);
}