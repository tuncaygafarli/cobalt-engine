#include "circleObject.h"

#define PI              3.14159265359f
#define GRAVITY         9.8f * 100.f

CircleObject::CircleObject(float r, Vector2f pos, Vector2f vel)
    : radius(r), position(pos), velocity(vel) {
}

float CircleObject::calculateArea() const {
    return PI * radius * radius;
}

float CircleObject::calculateKEnergy() {
    float speedSquared = velocity.x * velocity.x + velocity.y * velocity.y;
    return 0.5f * mass * speedSquared;
}

void CircleObject::updateSpeed(float deltaTime) {
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
}

void CircleObject::updatePosition(float deltaTime) {
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime + GRAVITY * deltaTime * deltaTime * 0.5f;
}

void CircleObject::update(float deltaTime) {
    updateSpeed(deltaTime);
    updatePosition(deltaTime);
}

bool CircleObject::handleBoundsCollision(float windowWidth, float windowHeight) {
    bool collided = false;

    // Left wall
    if (position.x - radius <= 0) {
        position.x = radius;
        velocity.x = -velocity.x * RESTITUTION;

        collided = true;
    }

    // Right wall
    if (position.x + radius >= windowWidth) {
        position.x = windowWidth - radius;
        velocity.x = -velocity.x * RESTITUTION;

        collided = true;
    }

    // Ceiling
    if (position.y - radius <= 0) {
        position.y = radius;
        velocity.y = -velocity.y * RESTITUTION;

        collided = true;
    }

    // Floor
    if (position.y + radius >= windowHeight) {
        position.y = windowHeight - radius;
        velocity.y = -velocity.y * RESTITUTION;

        collided = true;
    }

    hasCollided = collided;
    return collided;
}

bool CircleObject::checkCollision(const CircleObject& other) const {
    float dx = position.x - other.position.x;
    float dy = position.y - other.position.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = radius + other.radius;

    return distanceSquared < radiusSum * radiusSum;
}

/*
   Applied derivation of two dimensional solution for impulse here

   Source : https://en.wikipedia.org/wiki/Elastic_collision
*/ 

void CircleObject::handleBallsCollision(CircleObject& other) {
    Vector2f deltaPos = position - other.position;
    float distanceSquared = deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y;
    float distance = std::sqrt(distanceSquared);
    float minDistance = radius + other.radius;

    const float SLOP = 0.001f;  // applying tiny tolerance to avoid sticking because of slight precise in float numbers
    if (distanceSquared >= (minDistance + SLOP) * (minDistance + SLOP)) {
        return;
    }

    Vector2f normal = deltaPos / distance;

    Vector2f relativeVel = velocity - other.velocity;
    float approachSpeed = normal.x * relativeVel.x + normal.y * relativeVel.y;

    if (approachSpeed > -SLOP) return; // same slop for float-precise number approach

    float totalMass = mass + other.mass;
    float reducedMass = (mass * other.mass) / totalMass;
    float impulseMagnitude = -2.0f * reducedMass * approachSpeed;

    Vector2f impulse = normal * impulseMagnitude;

    velocity += impulse * (1.0f / mass);
    other.velocity -= impulse * (1.0f / other.mass);

    float overlap = minDistance - distance;
    if (overlap > SLOP) {
        Vector2f separation = normal * ((overlap - SLOP) * 0.5f);
        position += separation;
        other.position -= separation;
    }
}

void CircleObject::drawShadow(RenderWindow& window, float deltaTime) {
    float light_origin_x = 0;
    float light_origin_y = 0;

    Vector2f lightToObject = position - Vector2f(light_origin_x, light_origin_y);
    float distanceFromLight = lightToObject.length();

    if (distanceFromLight < 0.1f) return;

    Vector2f direction = lightToObject / distanceFromLight;

    float baseShadowLength = 10.0f;

    float distanceFactor = 1.0f + (distanceFromLight / 1000.0f);
    float shadowLength = baseShadowLength * distanceFactor;

    Vector2f shadowOffset = direction * shadowLength;

    CircleShape shadow(radius * 1.1f);
    shadow.setPosition(position + shadowOffset);
    shadow.setFillColor(Color(0, 0, 0, 100));
    shadow.setOrigin(Vector2f(radius * 1.1f, radius * 1.1f));

    window.draw(shadow);
}