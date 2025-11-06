#include "Includes.h"

class Particle {
public:
	// Attributes
	sf::Color m_colour;
	sf::Vector2f m_velocity;

	// Constructors / Destructor
	Particle(sf::Color colour, sf::Vector2f velocity);
	Particle(sf::Color colour);
	~Particle();
};