#include "Includes.h"

std::string vec2ToString(const sf::Vector2i vec2)
{
    return '(' + std::to_string(vec2.x) + ", " + std::to_string(vec2.y) + ')';
}

GameWindow::GameWindow(unsigned int window_size_x, unsigned int window_size_y, unsigned int scale_factor, ParticleMatrix &particle_matrix)
    : m_window(sf::VideoMode({ window_size_x, window_size_y }), std::string("Sand Painter")), m_particle_matrix(particle_matrix), m_gridLines(sf::PrimitiveType::Lines)
{
	// Set frame rate limit
	m_window.setFramerateLimit(60);

    // Set up grid lines
	// Y axis lines
    for (int x = 0; x <= window_size_x; x += scale_factor)
    {
        m_gridLines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(100, 100, 100)));
        m_gridLines.append(sf::Vertex(sf::Vector2f(x, window_size_y), sf::Color(100, 100, 100)));
    }
	// X axis lines
    for (int y = 0; y <= window_size_y; y += scale_factor)
    {
        m_gridLines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(100, 100, 100)));
        m_gridLines.append(sf::Vertex(sf::Vector2f(window_size_x, y), sf::Color(100, 100, 100)));
	}
}

// Destructor
GameWindow::~GameWindow() 
{
    m_window.close();
}

// Main loop
void GameWindow::run() 
{
    while (m_window.isOpen()) 
    {
        processEvents();
		render();
        processPhysics();
    }
}

void GameWindow::processEvents() 
{
	// Left mouse button click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
		// Get mouse position, scale to cell coordinates
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(m_window);
        int cell_x = mouse_pos.x / m_particle_matrix.m_scale_factor;
        int cell_y = mouse_pos.y / m_particle_matrix.m_scale_factor;

		// Check boundaries, place particle
        if (cell_x >= 0 && cell_x < m_particle_matrix.m_columns &&
            cell_y >= 0 && cell_y < m_particle_matrix.m_rows)
        {
            m_particle_matrix.setCell(cell_x, cell_y, sf::Color::Red);
        }

        std::cout << "Left mouse button clicked at: " << vec2ToString(mouse_pos) << std::endl;
    }

    // Right mouse button click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        // Get mouse position, scale to cell coordinates
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(m_window);
        int cell_x = mouse_pos.x / m_particle_matrix.m_scale_factor;
        int cell_y = mouse_pos.y / m_particle_matrix.m_scale_factor;

        // Check boundaries, place particle
        if (cell_x >= 0 && cell_x < m_particle_matrix.m_columns &&
            cell_y >= 0 && cell_y < m_particle_matrix.m_rows)
        {
            m_particle_matrix.setCell(cell_x, cell_y, sf::Color::Yellow);
        }

        std::cout << "Right mouse button clicked at: " << vec2ToString(mouse_pos) << std::endl;
    }

	// Event polling
    while (const std::optional event = m_window.pollEvent()) 
    {
		// Close window
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}

// Render to window
void GameWindow::render() 
{
    m_window.clear();
	m_window.draw(m_particle_matrix.m_particles_vertices);
    m_window.draw(m_gridLines);
    m_window.display();
}

// Gravity and collisions particle physics
void GameWindow::processPhysics() 
{
    // Loop matrix
    for (int y = m_particle_matrix.m_rows -1; y >= 0; --y)
    {
        for (unsigned int x = 0; x < m_particle_matrix.m_columns; ++x)
        {
            // Get particle
            Particle &particle = m_particle_matrix.m_matrix[x][y];

            // Apply gravity
            if (m_particle_matrix.m_matrix[x][y].m_colour != sf::Color::Yellow) {
                particle.m_velocity.y += 0.33f;           // gravity
                if (particle.m_velocity.y > 3.0f)
                    particle.m_velocity.y = 3.0f;         // terminal velocity
            }

			// Calculate fall distance
            int fall_distance = static_cast<int>(std::round(particle.m_velocity.y));
            int new_y = y;

            // Fall until hitting a non-yellow particle or matrix bottom row
            for (int i = 1; i <= fall_distance; ++i) {
                int target_y = y + i;
                if (target_y >= m_particle_matrix.m_rows || m_particle_matrix.m_matrix[x][target_y].m_colour != sf::Color::Yellow) {
                    break; // Stop incrementing newY
                }
                else {
                    new_y = target_y;
                }
            }

			// Swap particle to new position if y has changed
            if (new_y != y) {
                std::swap(m_particle_matrix.m_matrix[x][y], m_particle_matrix.m_matrix[x][new_y]);
                m_particle_matrix.setCell(x, y, sf::Color::Yellow); // Set old position to air particle
                m_particle_matrix.setCell(x, new_y, sf::Color::Red); // Set new position to red particle
            }
        }
    }
}
