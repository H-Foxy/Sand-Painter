#pragma once
#include "Brush.h"
#include "Particle.h"
#include "ParticleMatrix.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class GameWindow 
{
public:
	// Attributes
    sf::RenderWindow m_window;

    // Constructor / Destructor
    GameWindow(unsigned int window_size_x, unsigned int window_size_y, unsigned int scale_factor, ParticleMatrix &particle_matrix, Brush &brush);

	// Main loop
    void run();

private:
    // Attributes
    sf::VertexArray m_gridLines;
	bool m_isDrawGridLines;
	ParticleMatrix &m_particle_matrix;
    Brush &m_brush;
	bool m_is_left_mouse_button_down;
	bool m_is_right_mouse_button_down;
    sf::Vector2i m_last_left_click_pos;
	sf::Vector2i m_last_right_click_pos;
    sf::Vector2i m_mouse_pos;

    // Functions
    void render();
    void processEvents();
	void processPhysics();
	void drawMouseMovement(sf::Vector2i start, sf::Vector2i end, Particle particle);
};
