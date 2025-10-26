#include "Includes.h"

class GameWindow {
public:
    // Constructor / Destructor
    GameWindow(unsigned int windowSize_X, unsigned int windowSize_Y, unsigned int scaleFactor, ParticleMatrix &particleMatrix);
    ~GameWindow();

	// Main loop
    void run();

private:
    // Attributes
    sf::RenderWindow window;
    sf::VertexArray gridLines;
	ParticleMatrix &particleMatrix;
    // Functions
    void processEvents();
    void render();
	void processPhysics();
};
