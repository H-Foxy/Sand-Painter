#include "Includes.h"

// Constructor
Particle::Particle(sf::Color colour, sf::Vector2f velocity)
    : colour(colour), velocity(velocity)
{
	std::cout << "Particle Created - Colour: " << colour.toInteger() << ", Velocity: (" << velocity.x << ", " << velocity.y << ")" << std::endl;
}
// Constructor
Particle::Particle(sf::Color colour)
	: colour(colour), velocity({0,0}) 
{}
// Destructor
Particle::~Particle() 
{}