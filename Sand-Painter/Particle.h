#include "Includes.h"

class Particle {
public:
	// Attributes
	sf::Color m_colour;
	sf::Vector2f m_velocity;
	bool m_is_empty;

	// Constructors / Destructor
	Particle();
	Particle(sf::Color colour);
	~Particle();
};