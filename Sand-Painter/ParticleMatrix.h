#pragma once
#include <iostream>
#include "Particle.h"
#include <SFML/Graphics.hpp>
#include <vector>

class ParticleMatrix 
{
public:
	unsigned int m_rows;
	unsigned int m_columns;
	unsigned int m_cell_size;
	sf::VertexArray m_particles_vertices;
	std::vector<std::vector<Particle>> m_matrix;

	ParticleMatrix(unsigned int size_x, unsigned int size_y, unsigned int cell_size);

	void setCellParticle(int x, int y, Particle particle);
	void processPhysics();

private:
	void setCellVertexColours(int x, int y, sf::Color colour);
};