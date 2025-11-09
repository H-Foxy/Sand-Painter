#include "Includes.h"

class GameWindow {
public:

    sf::RenderWindow m_window;

    // Constructor / Destructor
    GameWindow(unsigned int window_size_x, unsigned int window_size_y, unsigned int scale_factor, ParticleMatrix &particle_matrix);
    ~GameWindow();

	// Main loop
    void run();

private:
    // Attributes
    sf::VertexArray m_gridLines;
	ParticleMatrix &m_particle_matrix;
    sf::Vector2i m_last_left_click_pos;
	sf::Vector2i m_last_right_click_pos;
	int m_inactivity_tick_count;
    // Functions
    void render();
    void processEvents();
	void processPhysics();
	void drawMouseMovement(sf::Vector2i start, sf::Vector2i end, Particle particle);
};
