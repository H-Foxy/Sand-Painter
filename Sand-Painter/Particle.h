#pragma once
#include <SFML/Graphics.hpp>

class Particle 
{
public:
	sf::Color m_colour;
	sf::Vector2f m_velocity;
	bool m_is_empty;

	Particle();
	Particle(sf::Color colour);
};