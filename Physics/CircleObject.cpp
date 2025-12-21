#include "circleObject.h"

CircleObject::CircleObject(float r, Vector2f pos, Vector2f vel)
    : radius(r), position(pos), velocity(vel) {
}

float CircleObject::calculateArea() const {
    return PI * radius * radius;
}

float CircleObject::calculateKEnergy() {
    float speedSquared = velocity.x * velocity.x + velocity.y * velocity.y;
    return 0.5f * MASS * speedSquared;
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
        cout << "Hit left wall with energy: " << calculateKEnergy() << "J" << endl;
        collided = true;
    }

    // Right wall
    if (position.x + radius >= windowWidth) {
        position.x = windowWidth - radius;
        velocity.x = -velocity.x * RESTITUTION;
        cout << "Hit right wall with energy: " << calculateKEnergy() << "J" << endl;
        collided = true;
    }

    // Ceiling
    if (position.y - radius <= 0) {
        position.y = radius;
        velocity.y = -velocity.y * RESTITUTION;
        cout << "Hit ceiling with energy: " << calculateKEnergy() << "J" << endl;
        collided = true;
    }

    // Floor
    if (position.y + radius >= windowHeight) {
        position.y = windowHeight - radius;
        velocity.y = -velocity.y * RESTITUTION;
        cout << "Hit floor with energy: " << calculateKEnergy() << "J" << endl;
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

void CircleObject::handleBallsCollision(CircleObject& other) {
    std::swap(velocity, other.velocity);

    Vector2f diff = other.position - position;
    float distance = sqrt(diff.x * diff.x + diff.y * diff.y);

    if (distance > 0) {
        float overlap = (radius + other.radius - distance) * 0.5f;
        diff /= distance;

        position -= diff * overlap;
        other.position += diff * overlap;
    }
}

void CircleObject::drawShadow(RenderWindow& window, int windowWidth, int windowHeight, float deltaTime) {
    float light_origin_x = windowWidth / 2;
    float light_origin_y = windowHeight / 2;

    int base_offset = 5;
    Vector2f offsetMultiplier(1, 1);

    if (position.x < light_origin_x && position.y > light_origin_y) {
        offsetMultiplier = Vector2f(-1, 1);
    }
    else if (position.x > light_origin_x && position.y > light_origin_y) {
        offsetMultiplier = Vector2f(1, 1);
    }
    else if (position.x < light_origin_x && position.y < light_origin_y) {
        offsetMultiplier = Vector2f(-1, -1);
    }
    else if (position.x > light_origin_x && position.y < light_origin_y) {
        offsetMultiplier = Vector2f(1, -1);
    }

    Vector2f shadowOffset(
        base_offset * offsetMultiplier.x,
        base_offset * offsetMultiplier.y
    );

    CircleShape shadow(radius * 1.1f);
    shadow.setPosition(position + shadowOffset);
    shadow.setFillColor(Color(0, 0, 0, 100));
    shadow.setOrigin(Vector2f(radius * 1.1f, radius * 1.1f));

    window.draw(shadow);
}