#include "ParticleSystem.h"
#include <iostream>
#include <cmath>
#include <vector>

#define PI 3.14159265358979323846f
#define GRAVITY 9.8f * 100.f
#define RESTITUTION 0.4f

using namespace std;
using namespace sf;

/*
	Creates particles only and only when there's a collision, which is constantly checked on game loop.
	Since we don't have to load all these particles to avoid heavy memory usage, we can normalize them by using Weber-Fechner Law :DDDD
*/ 

void ParticleSystem::createBurst(Vector2f position, Color color, float amount, float size)
{
	float maxParticles = 50.f;
	float minKE = 100.f;
	float maxKE = 50000.f;

	amount = max(amount, minKE);
	float normalized = log(amount / minKE) / log(maxKE / minKE); // weber-fechner
	normalized = min(max(normalized, 0.f), 1.f);

	float particleCount = normalized * maxParticles;

	for (float i = 0; i < particleCount; i++)
	{
		float angleDeg = rand() % 360;
		float angleRad = angleDeg * (PI / 180.f);
		float speed = 50 + (rand() % 150);

		particles.push_back({
			position,
			Vector2f(cos(angleRad) * speed, sin(angleRad) * speed), // creates random burst direction for particles
			color,
			true,
			10.f,
			size + (rand() % 20)
		});
	}
}

void ParticleSystem::updateParticles(float deltaTime)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].position += particles[i].velocity * deltaTime;

		particles[i].velocity.y += GRAVITY * deltaTime;

		particles[i].lifetime -= deltaTime;

		particles[i].size *= 0.99f; // shrinks over time

		if (particles[i].lifetime <= 0.001f || particles[i].size < 0.5f) {
			particles.erase(particles.begin() + i);
		}
	}
}

void ParticleSystem::drawParticles(RenderWindow& window)
{
	for (auto& p : particles)
	{
		CircleShape particle(p.size);
		particle.setPosition(p.position);
		particle.setOrigin(Vector2f(p.size, p.size));

		particle.setFillColor(Color(p.color.r, p.color.g, p.color.b));

		window.draw(particle);
	}
}

void ParticleSystem::drawParticleShadow(RenderWindow& window)
{
	for (auto& p : particles)
	{
		float light_origin_x = 0;
		float light_origin_y = 0;

		Vector2f lightToObject = p.position - Vector2f(light_origin_x, light_origin_y);
		float distanceFromLight = lightToObject.length();

		if (distanceFromLight < 0.1f) return;

		Vector2f direction = lightToObject / distanceFromLight;

		float baseShadowLength = 15.0f;

		float distanceFactor = 1.0f + (distanceFromLight / 500.f);
		float shadowLength = baseShadowLength * distanceFactor;

		Vector2f shadowOffset = direction * shadowLength;

		CircleShape shadow(p.size * 1.1f);
		shadow.setPosition(p.position + shadowOffset);
		shadow.setFillColor(Color(0, 0, 0, 100));
		shadow.setOrigin(Vector2f(p.size * 1.1f, p.size * 1.1f));

		window.draw(shadow);
	}
}

bool ParticleSystem::handleBoundsCollision(float windowWidth, float windowHeight)
{
	bool anyCollision = false;

	for (auto& p : particles) {
		bool particleCollided = false;

		// Left wall
		if (p.position.x - p.size <= 0) {
			p.position.x = p.size;
			p.velocity.x = -p.velocity.x * RESTITUTION;
			particleCollided = true;
		}

		// Right wall
		else if (p.position.x + p.size >= windowWidth) {
			p.position.x = windowWidth - p.size;
			p.velocity.x = -p.velocity.x * RESTITUTION;
			particleCollided = true;
		}

		// Ceiling
		if (p.position.y - p.size <= 0) {
			p.position.y = p.size;
			p.velocity.y = -p.velocity.y * RESTITUTION;
			particleCollided = true;
		}

		// Floor
		else if (p.position.y + p.size >= windowHeight) {
			p.position.y = windowHeight - p.size;
			p.velocity.y = -p.velocity.y * RESTITUTION;
			particleCollided = true;
		}

		if (particleCollided) {
			p.hasCollided = true;
			anyCollision = true;
		}
		else {
			p.hasCollided = false;
		}
	}

	return anyCollision;
}