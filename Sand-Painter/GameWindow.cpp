#include "GameWindow.h"

GameWindow::GameWindow(unsigned int window_size_x, unsigned int window_size_y, unsigned int cell_size, ParticleMatrix &particle_matrix, Brush &brush)
	: m_window(sf::VideoMode({ window_size_x, window_size_y }), std::string("Sand Painter")),
    m_particle_matrix(particle_matrix), 
    m_brush(brush),
	m_gridLines(sf::PrimitiveType::Lines),
	m_isDrawGridLines(true),
	m_is_left_mouse_button_down(false),
    m_is_right_mouse_button_down(false),
	m_mouse_pos({ -1, -1 }),
    m_last_left_click_pos({ -1, -1 }),
    m_last_right_click_pos({ -1, -1 })
{
	m_window.setFramerateLimit(60);

    // Set up grid lines
    for (int x = 0; x <= window_size_x; x += cell_size)
    {
        m_gridLines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color::Black));
        m_gridLines.append(sf::Vertex(sf::Vector2f(x, window_size_y), sf::Color::Black));
    }
    for (int y = 0; y <= window_size_y; y += cell_size)
    {
        m_gridLines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color::Black));
        m_gridLines.append(sf::Vertex(sf::Vector2f(window_size_x, y), sf::Color::Black));
	}
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

void GameWindow::render()
{
    m_window.clear();
    m_window.draw(m_particle_matrix.m_particles_vertices);

    if (m_isDrawGridLines) 
        m_window.draw(m_gridLines);

    m_window.draw(m_brush.m_circle);
    m_window.display();
}

void GameWindow::processEvents() 
{
    if (m_is_left_mouse_button_down)
    {
        drawMouseMovement(m_last_left_click_pos, m_mouse_pos, Particle(sf::Color::Red));
        m_last_left_click_pos = m_mouse_pos;
    }
    if (m_is_right_mouse_button_down)
    {
        drawMouseMovement(m_last_right_click_pos, m_mouse_pos, Particle());
        m_last_right_click_pos = m_mouse_pos;
    }

    while (const std::optional<sf::Event> event = m_window.pollEvent())
    {
        if (event->is<sf::Event::MouseMoved>())
        {
            const auto* move = event->getIf<sf::Event::MouseMoved>();
            m_mouse_pos = { move->position.x, move->position.y };

            m_brush.moveBrush(m_mouse_pos);
        }

        if (event->is<sf::Event::MouseButtonPressed>())
        {
            const auto* mb = event->getIf<sf::Event::MouseButtonPressed>();

            if (mb->button == sf::Mouse::Button::Left)
            {
                m_is_left_mouse_button_down = true;
                m_last_left_click_pos = m_mouse_pos;

				m_brush.brushParticles(m_particle_matrix, Particle(sf::Color::Red)); // Coloured Particle
            }

            if (mb->button == sf::Mouse::Button::Right)
            {
                m_is_right_mouse_button_down = true;
                m_last_right_click_pos = m_mouse_pos;

                m_brush.brushParticles(m_particle_matrix, Particle()); // Empty Particle
            }
        }

        if (event->is<sf::Event::MouseButtonReleased>())
        {
            const auto* mb = event->getIf<sf::Event::MouseButtonReleased>();

            if (mb->button == sf::Mouse::Button::Left)
                m_is_left_mouse_button_down = false;

            if (mb->button == sf::Mouse::Button::Right)
                m_is_right_mouse_button_down = false;
        }

		// Keyboard events
        if (event->is<sf::Event::KeyPressed>())
        {
            const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();

			// Toggle grid lines with G key
            if (keyEvent->code == sf::Keyboard::Key::G)
            {
                m_isDrawGridLines = !m_isDrawGridLines;
            }
        }

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