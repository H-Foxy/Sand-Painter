#pragma once
#include "Brush.h"
#include "Particle.h"
#include "ParticleMatrix.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class GameWindow 
{
public:
    sf::RenderWindow m_window;

    GameWindow(unsigned int window_size_x, unsigned int window_size_y, unsigned int cell_size, ParticleMatrix &particle_matrix, Brush &brush);

    void run();

private:
    sf::VertexArray m_gridLines;
	bool m_isDrawGridLines;
	ParticleMatrix &m_particle_matrix;
    Brush &m_brush;
	bool m_is_left_mouse_button_down;
	bool m_is_right_mouse_button_down;
    sf::Vector2i m_last_left_click_pos;
	sf::Vector2i m_last_right_click_pos;
    sf::Vector2i m_mouse_pos;

    void render();
    void processEvents();
	void processPhysics();
	void drawMouseMovement(sf::Vector2i start, sf::Vector2i end, Particle particle);
};
