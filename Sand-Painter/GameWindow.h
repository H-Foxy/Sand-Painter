#include "Includes.h"

class GameWindow {
public:
    // Constructor / Destructor
    GameWindow(unsigned int window_size_x, unsigned int window_size_y, unsigned int scale_factor, ParticleMatrix &particle_matrix);
    ~GameWindow();

	// Main loop
    void run();

private:
    // Attributes
    sf::RenderWindow m_window;
    sf::VertexArray m_gridLines;
	ParticleMatrix &m_particle_matrix;
    // Functions
    void processEvents();
    void render();
	void processPhysics();
};
