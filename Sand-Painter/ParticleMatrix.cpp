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

	// Update particle in matrix if not already the same
    if (m_matrix[pos_x][pos_y].m_is_empty != particle.m_is_empty)
    {
        m_matrix[pos_x][pos_y] = particle;

        // Update vertex colors
        setCellVertexColours(pos_x, pos_y, particle.m_colour);
	}
}

// Gravity and collisions particle physics
void ParticleMatrix::processPhysics()
{
    // Loop matrix - bottom to top
    for (int y = m_rows - 1; y >= 0; --y)
    {
        for (unsigned int x = 0; x < m_columns; ++x)
        {
            // Get particle
            Particle& particle = m_matrix[x][y];

            // Apply gravity if not empty particle
            if (!m_matrix[x][y].m_is_empty) {
                particle.m_velocity.y += 0.33f;           // gravity
                if (particle.m_velocity.y > 3.0f)
                    particle.m_velocity.y = 3.0f;         // terminal velocity
            }

            // Calculate fall distance
            int fall_distance = static_cast<int>(std::round(particle.m_velocity.y));
            int new_y = y;

            // Fall until hitting a non empty particle or matrix bottom row
            for (int i = 1; i <= fall_distance; ++i) {
                int target_y = y + i;
                // If at bottom of matrix
                if (target_y >= m_rows)
                {
                    particle.m_velocity.y = 0.0f; // reset velocity
                    break; // Stop incrementing newY
                }
                // If colliding with a non empty particle
                if (!m_matrix[x][target_y].m_is_empty) 
                {
					// Try to move diagonally left or right
                      
					// Check left diagonal
                    if (x > 0 && m_matrix[x - 1][target_y].m_is_empty) {
                        // Swap particle to left diagonal position
                        setCellParticle(x - 1, target_y, particle);
                        setCellParticle(x, y, Particle());
                    }
                    // Check right diagonal
                    else if (x < m_columns - 1 && m_matrix[x + 1][target_y].m_is_empty) {
                        // Swap particle to right diagonal position
                        setCellParticle(x + 1, target_y, particle);
                        setCellParticle(x, y, Particle());
                    }
                    else {
                        particle.m_velocity.y = 0.0f; // reset velocity
                        break; // Stop incrementing newY
                    }
                }
                else {
                    new_y = target_y;
                }
            }

            // Swap particle to new position if y has changed
            if (new_y != y) {
                // Update particles
                setCellParticle(x, new_y, particle); // set new position to hold referenced particle
                setCellParticle(x, y, Particle()); // set old position to empty particle
            }
        }
    }
}