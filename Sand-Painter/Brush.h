#pragma once
#include "Particle.h"
#include "ParticleMatrix.h"
#include <SFML/Graphics.hpp>

class Brush 
{
public:
	int m_radius;
	int m_radius_scaled;
	int m_cell_size;
	sf::Vector2i m_centre;
	sf::Vector2i m_centre_scaled;
	sf::CircleShape m_circle;
	std::vector<sf::Vector2i> m_occupied_coordinates;

	Brush(int radius, int cell_size, sf::Vector2i centre);

	void moveBrush(sf::Vector2i new_centre);
	void brushParticles(ParticleMatrix& particle_matrix, Particle particle);
	void calculateOccupiedCoordinates();
};