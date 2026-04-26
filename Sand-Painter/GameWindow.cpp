#include "GameWindow.h"

GameWindow::GameWindow(unsigned int window_size_x, unsigned int window_size_y, unsigned int cell_size)
	: m_window_size({ window_size_x, window_size_y }),
    m_window(sf::VideoMode({ window_size_x, window_size_y }), std::string("Sand Painter")),
    m_world_view(sf::FloatRect({ 0.f, 0.f }, { static_cast<float>(window_size_x), static_cast<float>(window_size_y) })),
	m_gridLines(sf::PrimitiveType::Lines),
	m_isDrawGridLines(true),
	m_particle_matrix(window_size_x, window_size_y, cell_size),
    m_brush(cell_size),
	m_is_left_mouse_button_down(false),
    m_is_right_mouse_button_down(false),
	m_mouse_pos({ -1, -1 }),
    m_last_left_click_pos({ -1, -1 }),
    m_last_right_click_pos({ -1, -1 })
{
	m_window.setFramerateLimit(60);
    m_window.setView(m_world_view);

    const sf::Color grid_line_color(0, 0, 0, 64);

    // Set up grid lines
    for (int x = 0; x <= window_size_x; x += cell_size)
    {
        m_gridLines.append(sf::Vertex(sf::Vector2f(x, 0), grid_line_color));
        m_gridLines.append(sf::Vertex(sf::Vector2f(x, window_size_y), grid_line_color));
    }
    for (int y = 0; y <= window_size_y; y += cell_size)
    {
        m_gridLines.append(sf::Vertex(sf::Vector2f(0, y), grid_line_color));
        m_gridLines.append(sf::Vertex(sf::Vector2f(window_size_x, y), grid_line_color));
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
            updateMousePosition(move->position);
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

        if (event->is<sf::Event::KeyPressed>())
        {
            const auto* keyEvent = event->getIf<sf::Event::KeyPressed>();

			// Toggle grid lines with G key
            if (keyEvent->code == sf::Keyboard::Key::G)
            {
                m_isDrawGridLines = !m_isDrawGridLines;
            }
        }

        if (event->is<sf::Event::Resized>())
        {
            const auto* resized = event->getIf<sf::Event::Resized>();
            updateView(resized->size);
        }

        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }
    }
}

void GameWindow::updateMousePosition(sf::Vector2i pixel_position)
{
    const sf::Vector2f world_position = m_window.mapPixelToCoords(pixel_position);

    m_mouse_pos = 
    {
        static_cast<int>(std::lround(world_position.x)),
        static_cast<int>(std::lround(world_position.y))
    };

    m_brush.moveBrush(m_mouse_pos);
}

void GameWindow::updateView(sf::Vector2u window_size)
{
    const float window_aspect_ratio = static_cast<float>(window_size.x) / static_cast<float>(window_size.y);
    const sf::Vector2f view_size = m_world_view.getSize();
    const float view_aspect_ratio = view_size.x / view_size.y;

    float viewport_scale_x = 1.f;
    float viewport_scale_y = 1.f;
    float viewport_offset_x = 0.f;
    float viewport_offset_y = 0.f;

    if (window_aspect_ratio > view_aspect_ratio)
    {
        viewport_scale_x = view_aspect_ratio / window_aspect_ratio;
        viewport_offset_x = (1.f - viewport_scale_x) * 0.5f;
    }
    else
    {
        viewport_scale_y = window_aspect_ratio / view_aspect_ratio;
        viewport_offset_y = (1.f - viewport_scale_y) * 0.5f;
    }

    m_world_view.setViewport(sf::FloatRect
    (
        { viewport_offset_x  , viewport_offset_y  },
        { viewport_scale_x, viewport_scale_y }
    ));

    m_window.setView(m_world_view);
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
		m_brush.moveBrush(sf::Vector2i(x0, y0));
        m_brush.brushParticles(m_particle_matrix, particle);

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