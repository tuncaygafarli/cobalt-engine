#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

#define PI              3.14159265359f
#define GRAVITY         9.8f * 100.f

struct CircleObject {
    Vector2f position;
    Vector2f velocity;
    Vector2f acceleration = { 0.f, GRAVITY };
    float radius;

    const float MASS = 5.f;
    const float RESTITUTION = 0.8f;
    bool hasCollided = false;

    CircleObject(float r = 30.f, Vector2f pos = { 0,0 }, Vector2f vel = { 0,0 });

    float calculateArea() const;
    float calculateKEnergy();
    void updateSpeed(float deltaTime);
    void updatePosition(float deltaTime);
    void update(float deltaTime);
    bool handleBoundsCollision(float windowWidth, float windowHeight);
    bool checkCollision(const CircleObject& other) const;
    void handleBallsCollision(CircleObject& other);
    void drawShadow(RenderWindow& window, int windowWidth, int windowHeight, float deltaTime);
};