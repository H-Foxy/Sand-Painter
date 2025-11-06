#include "Includes.h"

// Constructor
Particle::Particle(sf::Color colour, sf::Vector2f velocity)
    : m_colour(colour), m_velocity(velocity)
{
	std::cout << "Particle Created - Colour: " << colour.toInteger() << ", Velocity: (" << velocity.x << ", " << velocity.y << ")" << std::endl;
}
// Constructor
Particle::Particle(sf::Color colour)
	: m_colour(colour), m_velocity({0,0}) 
{}
// Destructor
Particle::~Particle() 
{}