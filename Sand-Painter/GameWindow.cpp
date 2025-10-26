#include "Includes.h"

std::string vec2ToString(const sf::Vector2i vec2)
{
    return '(' + std::to_string(vec2.x) + ", " + std::to_string(vec2.y) + ')';
}

GameWindow::GameWindow(unsigned int windowSize_X, unsigned int windowSize_Y, unsigned int scaleFactor, ParticleMatrix &particleMatrix)
    : window(sf::VideoMode({ windowSize_X, windowSize_Y }), std::string("Sand Painter")), particleMatrix(particleMatrix), gridLines(sf::PrimitiveType::Lines)
{
	// Set frame rate limit
	window.setFramerateLimit(60);

    // Set up grid lines
	// Y axis lines
    for (int x = 0; x <= windowSize_X; x += scaleFactor) 
    {
        gridLines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(100, 100, 100)));
        gridLines.append(sf::Vertex(sf::Vector2f(x, windowSize_Y), sf::Color(100, 100, 100)));
    }
	// X axis lines
    for (int y = 0; y <= windowSize_Y; y += scaleFactor) 
    {
        gridLines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(100, 100, 100)));
        gridLines.append(sf::Vertex(sf::Vector2f(windowSize_X, y), sf::Color(100, 100, 100)));
	}
}

// Destructor
GameWindow::~GameWindow() 
{
    window.close();
}

// Main loop
void GameWindow::run() 
{
    while (window.isOpen()) 
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
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int cell_X = mousePos.x / particleMatrix.scaleFactor;
        int cell_Y = mousePos.y / particleMatrix.scaleFactor;

		// Check boundaries, place particle
        if (cell_X >= 0 && cell_X < particleMatrix.columns &&
            cell_Y >= 0 && cell_Y < particleMatrix.rows)
        {
            particleMatrix.setCell(cell_X, cell_Y, sf::Color::Red);
        }

        std::cout << "Left mouse button clicked at: " << vec2ToString(mousePos) << std::endl;
    }

    // Right mouse button click
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        // Get mouse position, scale to cell coordinates
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        int cell_X = mousePos.x / particleMatrix.scaleFactor;
        int cell_Y = mousePos.y / particleMatrix.scaleFactor;

        // Check boundaries, place particle
        if (cell_X >= 0 && cell_X < particleMatrix.columns &&
            cell_Y >= 0 && cell_Y < particleMatrix.rows)
        {
            particleMatrix.setCell(cell_X, cell_Y, sf::Color::Yellow);
        }

        std::cout << "Right mouse button clicked at: " << vec2ToString(mousePos) << std::endl;
    }

	// Event polling
    while (const std::optional event = window.pollEvent()) 
    {
		// Close window
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
    }
}

// Render to window
void GameWindow::render() 
{
    window.clear();
	window.draw(particleMatrix.particlesVertices);
    window.draw(gridLines);
    window.display();
}

// Gravity and collisions particle physics
void GameWindow::processPhysics() 
{
    // Loop matrix
    for (int y = particleMatrix.rows -1; y >= 0; --y)
    {
        for (unsigned int x = 0; x < particleMatrix.columns; ++x)
        {
            // Get particle
            Particle &particle = particleMatrix.matrix[x][y];

            // Apply gravity
            if (particleMatrix.matrix[x][y].colour != sf::Color::Yellow) {
                particle.velocity.y += 0.33f;           // gravity
                if (particle.velocity.y > 3.0f)
                    particle.velocity.y = 3.0f;         // terminal velocity
            }

			// Calculate fall distance
            int fallDistance = static_cast<int>(std::round(particle.velocity.y));
            int newY = y;

            // Fall until hitting a non-yellow particle or matrix bottom row
            for (int i = 1; i <= fallDistance; ++i) {
                int targetY = y + i;
                if (targetY >= particleMatrix.rows || particleMatrix.matrix[x][targetY].colour != sf::Color::Yellow) {
                    break; // Stop incrementing newY
                }
                else {
					newY = targetY;
                }
            }

			// Swap particle to new position if y has changed
            if (newY != y) {
                std::swap(particleMatrix.matrix[x][y], particleMatrix.matrix[x][newY]);
				particleMatrix.setCell(x, y, sf::Color::Yellow); // Set old position to air particle
				particleMatrix.setCell(x, newY, sf::Color::Red); // Set new position to red particle
            }
        }
    }
}
