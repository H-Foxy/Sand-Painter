#include "Includes.h"

class Brush {

public:
	// Attributes
	int m_radius;
	int m_radius_scaled;
	int m_scale_factor;
	sf::Vector2i m_centre;
	sf::Vector2i m_centre_scaled;
	sf::CircleShape m_circle;
	std::vector<sf::Vector2i> m_occupied_coordinates;

	// Constructor
	Brush(int radius, int scale_factor, sf::Vector2i centre);

	// Member functions
	void moveBrush(sf::Vector2i new_centre);
	void brushParticles(ParticleMatrix& particle_matrix, Particle particle);
	void calculateOccupiedCoordinates();

};