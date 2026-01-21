#include "Includes.h"

std::string vec2ToString(const sf::Vector2i vec2)
{
    return '(' + std::to_string(vec2.x) + ", " + std::to_string(vec2.y) + ')';
}

GameWindow::GameWindow(unsigned int window_size_x, unsigned int window_size_y, unsigned int scale_factor, ParticleMatrix &particle_matrix, Brush &brush)
	: m_window(sf::VideoMode({ window_size_x, window_size_y }), std::string("Sand Painter")), m_particle_matrix(particle_matrix), m_brush(brush),
	m_gridLines(sf::PrimitiveType::Lines), m_last_left_click_pos({ -1, -1 }), m_inactivity_tick_count(2), m_last_right_click_pos({ -1, -1 })
{
	// Set frame rate limit
	m_window.setFramerateLimit(60);

    // Set up grid lines
	// Y axis lines
    for (int x = 0; x <= window_size_x; x += scale_factor)
    {
        m_gridLines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color::Black));
        m_gridLines.append(sf::Vertex(sf::Vector2f(x, window_size_y), sf::Color::Black));
    }
	// X axis lines
    for (int y = 0; y <= window_size_y; y += scale_factor)
    {
        m_gridLines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color::Black));
        m_gridLines.append(sf::Vertex(sf::Vector2f(window_size_x, y), sf::Color::Black));
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
        render();
        processEvents();
        m_particle_matrix.processPhysics();
    }
}

// Render to window
void GameWindow::render()
{
    m_window.clear();
    m_window.draw(m_particle_matrix.m_particles_vertices);
    //m_window.draw(m_gridLines);
    m_window.draw(m_brush.m_circle);
    m_window.display();
}

void GameWindow::processEvents() 
{
	// Get mouse position
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(m_window);
	m_brush.moveBrush(mouse_pos);

	// Left mouse button click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        // Check if mouse is dragged whilst clicked
        if (m_last_left_click_pos.x != -1 && m_last_left_click_pos.y != -1)
        {
            // Track mouse movement between last and current position
            drawMouseMovement(m_last_left_click_pos, mouse_pos, sf::Color::Red);
        }
        else
        {
            // Set red particle
            m_brush.brushParticles(m_particle_matrix, Particle(sf::Color::Red)); 
        }
        // Update inactivity tick count and last left click position
        m_inactivity_tick_count = 2;
        m_last_left_click_pos = mouse_pos;

		// Debug output
        std::cout << "Left mouse button clicked at: " << vec2ToString(mouse_pos) << std::endl;
    }

    // Right mouse button click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        // Check if mouse is dragged whilst clicked
        if (m_last_right_click_pos.x != -1 && m_last_right_click_pos.y != -1)
        {
            // Track mouse movement between last and current position
            drawMouseMovement(m_last_right_click_pos, mouse_pos, Particle());
        }
        else
        {
            // Set empty particle
            m_brush.brushParticles(m_particle_matrix, Particle());
        }
        // Update inactivity tick count and last right click position
        m_inactivity_tick_count = 2;
        m_last_right_click_pos = mouse_pos;
        
		// Debug output
        std::cout << "Right mouse button clicked at: " << vec2ToString(mouse_pos) << std::endl;
    }

	// Decrease tick count
    m_inactivity_tick_count--;
    if (m_inactivity_tick_count <= 0)
    {
        m_inactivity_tick_count = 0;
        m_last_left_click_pos = { -1, -1 };
		m_last_right_click_pos = { -1, -1 };
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

// Bresenham's line algorithm for drawing straight lines between two mouse points
void GameWindow:: drawMouseMovement(sf::Vector2i start, sf::Vector2i end, Particle particle)
{
    int x0 = start.x;
    int y0 = start.y;
    int x1 = end.x;
    int y1 = end.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
		// Move Brush and fill in particles
		m_brush.moveBrush(sf::Vector2i(x0, y0));
        m_brush.brushParticles(m_particle_matrix, particle);

        // Stop when end point reached
        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}