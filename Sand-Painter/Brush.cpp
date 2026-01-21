#include "Includes.h"

Brush::Brush(int radius, int scale_factor , sf::Vector2i centre)
	: m_radius(radius), m_radius_scaled(radius / scale_factor), m_centre(centre), m_centre_scaled(centre / scale_factor), m_scale_factor(scale_factor), m_circle(radius) {
	m_circle.setFillColor(sf::Color::Transparent);
	m_circle.setOutlineThickness(1.f);     // outline width
	m_circle.setOutlineColor(sf::Color::Black);
	calculateOccupiedCoordinates();
};

void Brush::moveBrush(sf::Vector2i new_centre)
{
	m_centre = new_centre;
	m_centre_scaled = new_centre / m_scale_factor;
	m_circle.setPosition({ static_cast<float>(new_centre.x - m_radius), static_cast<float>(new_centre.y - m_radius) });
	calculateOccupiedCoordinates();
}

void Brush::brushParticles(ParticleMatrix& particle_matrix, Particle particle)
{
	for (const sf::Vector2i& coord : m_occupied_coordinates)
	{
		particle_matrix.setCellParticle(coord.x, coord.y, particle);
	}
}

void Brush::calculateOccupiedCoordinates()
{
	std::vector<sf::Vector2i> pixels;
	int r2 = m_radius_scaled * m_radius_scaled;
	for (int y = -m_radius_scaled; y <= m_radius_scaled; ++y)
	{
		int yy = y * y;
		// max x for this y
		int xMax = static_cast<int>(std::sqrt(r2 - yy));
		for (int x = -xMax; x <= xMax; ++x)
		{
			pixels.push_back(sf::Vector2i(m_centre_scaled.x + x, m_centre_scaled.y + y));
		}
	}
	m_occupied_coordinates = pixels;
}
