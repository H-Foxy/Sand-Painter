#include "ParticleMatrix.h"

ParticleMatrix::ParticleMatrix(unsigned int size_x, unsigned int size_y, unsigned int cell_size)
	: m_columns(size_x / cell_size),
    m_rows(size_y / cell_size), 
    m_cell_size(cell_size),
    m_particles_vertices(sf::PrimitiveType::Triangles)
{
	// Initialise matrix with empty Particles (White)
	m_matrix.resize(m_columns, std::vector<Particle>(m_rows, Particle()));

    // Convert coordinates to triangle Vertices for rendering
    for (unsigned int x = 0; x < m_columns; ++x)
    {
        for (unsigned int y = 0; y < m_rows; ++y)
        {
            sf::Color color = m_matrix[x][y].m_colour;

			// Calculate scaled pixel positions
            float px = x * m_cell_size;
            float py = y * m_cell_size;

            // Calculate positions of the 4 cell corners
            sf::Vector2f top_left(px, py);
            sf::Vector2f top_right(px + m_cell_size, py);
            sf::Vector2f bottom_right(px + m_cell_size, py + m_cell_size);
            sf::Vector2f bottom_left(px, py + m_cell_size);

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

void ParticleMatrix::setCellVertexColours(int pos_x, int pos_y, sf::Color colour)
{
	// Calculate the starting index for the cell's vertices in the vertex array
    int i = (pos_y + (pos_x * m_rows)) * 6;

    for (int v = 0; v < 6; ++v) 
    {
        m_particles_vertices[v + i].color = colour;
    }
}

void ParticleMatrix::setCellParticle(int pos_x, int pos_y, Particle particle)
{
    // Bounds checking
    if (pos_x < 0 || pos_x >= m_columns || pos_y < 0 || pos_y >= m_rows)
        return;

	// Check particle is different from current matrix cell 
    if (m_matrix[pos_x][pos_y].m_is_empty == particle.m_is_empty)
        return;

    m_matrix[pos_x][pos_y] = particle;
    setCellVertexColours(pos_x, pos_y, particle.m_colour);
}

void ParticleMatrix::processPhysics()
{
    // Loop matrix - bottom to top
    for (int y = m_rows - 1; y >= 0; --y)
    {
        // Left to right
        for (unsigned int x = 0; x < m_columns; ++x)
        {
            Particle& particle = m_matrix[x][y];

            if (!m_matrix[x][y].m_is_empty) 
            {
                particle.m_velocity.y += 0.33f;           // gravity
                if (particle.m_velocity.y > 3.0f)
                    particle.m_velocity.y = 3.0f;         // terminal velocity
            }

            int fall_distance = static_cast<int>(std::round(particle.m_velocity.y));
            int new_y = y;

            // Fall until hitting a non empty particle or matrix bottom row
            for (int i = 1; i <= fall_distance; ++i) 
            {
                int target_y = y + i;

                if (target_y >= m_rows)
                {
                    particle.m_velocity.y = 0.0f; // reset velocity
                    break;
                }

                // Collision with non-empty particle
                if (!m_matrix[x][target_y].m_is_empty)
                {
                    // Check left diagonal move is free
                    bool can_move_left = (x > 0) &&
                        m_matrix[x - 1][target_y].m_is_empty &&
                        m_matrix[x - 1][y].m_is_empty;

                    // Check right diagonal move is free
                    bool can_move_right = (x < m_columns - 1) &&
                        m_matrix[x + 1][target_y].m_is_empty &&
                        m_matrix[x + 1][y].m_is_empty;

                    int dx = 0;

					// Randomly choose left or right if both free
                    if (can_move_left && can_move_right) 
                    {
                        dx = (rand() % 2 == 0) ? -1 : 1;
                    }
                    else if (can_move_left) 
                    {
                        dx = -1;
                    }
                    else if (can_move_right) 
                    {
                        dx = 1;
                    }

                    // Move particle
                    if (dx != 0) 
                    {
                        setCellParticle(x + dx, target_y, particle);
                        setCellParticle(x, y, Particle());
                    }
                    else 
                    {
                        particle.m_velocity.y = 0.0f;
                    }
                }
                else 
                {
                    new_y = target_y;
                }
            }

            // Update particle position
            if (new_y != y) 
            {
                setCellParticle(x, new_y, particle);
                setCellParticle(x, y, Particle());
            }
        }
    }
}