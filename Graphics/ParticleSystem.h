#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

class ParticleSystem
{
public:
	struct Particle
	{
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Color color;
		bool hasCollided;
		float lifetime;
		float size;
		float mass;
	};

	void createBurst(sf::Vector2f position, sf::Color color, float amount, float size);
	void updateParticles(float deltaTime);
	void drawParticles(sf::RenderWindow& window);
	void drawParticleShadow(sf::RenderWindow& window);
	bool handleBoundsCollision(float windowWidth, float windowHeight);
	void handleBallsCollision(Particle& other);

private:
	std::vector<Particle> particles;
};