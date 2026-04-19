#include "Particle.h"

Particle::Particle()
	: m_colour(sf::Color::White), m_velocity({ 0,0 }), m_is_empty(true)
{}

Particle::Particle(sf::Color colour)
	: m_colour(colour), m_velocity({0,0}), m_is_empty(false)
{}