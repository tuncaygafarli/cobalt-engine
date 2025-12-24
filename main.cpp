#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

#include "Physics/CircleObject.h"
#include "Graphics/ParticleSystem.h"
#include "Managers/SoundManager.h"

#define WIDTH           800
#define HEIGHT          800

int main()
{   
    sf::RenderWindow window(VideoMode({ WIDTH, HEIGHT }), "Cobalt 2D");

    sf::Color colors[7] = {
    Color::Red,
    Color::Green,
    Color::Blue,
    Color::Yellow,
    Color::Magenta,
    Color::Cyan,
    Color::White,
    };

    SoundManager soundManager;

    int currentIndex = 0;
    float colorChangeTimer = 0;

    std::vector<CircleObject> circles;
    std::vector<CircleShape> shapes;
    ParticleSystem particles;

    float randomPosX = 100 + rand() % (WIDTH - 200);
    float randomPosY = 100 + rand() % (300);

    CircleObject firstCircle(30.f,
        Vector2f(randomPosX, randomPosY),
        Vector2f((rand() % 200) - 100, 0));

    circles.push_back(firstCircle);

    sf::CircleShape firstShape(firstCircle.radius);
    firstShape.setFillColor(colors[rand() % 7]);
    firstShape.setOrigin(Vector2f(firstCircle.radius, firstCircle.radius));
    firstShape.setPosition(firstCircle.position);
    shapes.push_back(firstShape);

    bool isDragging = true;

    Clock clock;

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        colorChangeTimer += deltaTime;

        particles.updateParticles(deltaTime);
        particles.handleBoundsCollision(WIDTH, HEIGHT);

        for (int i = 0; i < circles.size(); i++) {
            circles[i].update(deltaTime);
        }

        for (int i = 0; i < circles.size(); i++) {
                // bound collision handling
                if (circles[i].handleBoundsCollision(WIDTH, HEIGHT))
                {
                    float KE = circles[i].calculateKEnergy();
                    particles.createBurst(circles[i].position, colors[currentIndex], KE, circles[i].radius / 8.f);
                    soundManager.playHitSound("Assets/Sounds/kenney/tone1.ogg");
                }

                // circle collision handling
                for (int j = i + 1; j < circles.size(); j++) 
                {
                    if (circles[i].checkCollision(circles[j]))
                    {
                        float KE = circles[i].calculateKEnergy();
                        circles[i].handleBallsCollision(circles[j]);
                        particles.createBurst(circles[i].position, colors[currentIndex], KE, circles[i].radius / 8.f);
                    }
                }

            // updating shapes here
            shapes[i].setPosition(circles[i].position);
            shapes[i].setFillColor(colors[currentIndex]);
        }

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) 
            {
                window.close();
            }

            
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {

                // make ball jump to the left
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    for (int i = 0; i < circles.size(); i++) {
                        circles[i].velocity.y = -300.f;
                        circles[i].velocity.x = -300.f;
                        currentIndex = (currentIndex + 1) % 7;
                        firstShape.setFillColor(colors[currentIndex]);
                    }
                }

                // make ball jump to the right
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    for (int i = 0; i < circles.size(); i++) {
                        circles[i].velocity.y = -300.f;
                        circles[i].velocity.x = 300.f;
                        currentIndex = (currentIndex + 1) % 7;
                        firstShape.setFillColor(colors[currentIndex]);
                    }
                }
            }

            // new ball creation
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N))
            {
                CircleObject newCircle(30.f,
                    Vector2f(randomPosX, randomPosY),
                    Vector2f((rand() % 200) - 100, 0));
                circles.push_back(newCircle);

                sf::CircleShape newShape(newCircle.radius);
                newShape.setFillColor(colors[rand() % 7]);
                newShape.setOrigin(Vector2f(newCircle.radius, newCircle.radius));
                newShape.setPosition(newCircle.position);
                shapes.push_back(newShape);
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                for (int i = 0; i < circles.size(); i++) {
                    circles[i].velocity.y = -500.f;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                for (int i = 0; i < circles.size(); i++) {
                    circles[i].velocity.x = -400.f;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                for (int i = 0; i < circles.size(); i++) {
                    circles[i].velocity.x = 400.f;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                for (int i = 0; i < circles.size(); i++) {
                    circles[i].velocity.y = 400.f;
                }
            }


        }

        // Draw
        window.clear(Color(30, 30, 40));
        particles.drawParticles(window);
        particles.drawParticleShadow(window);

        for (int i = 0; i < circles.size(); i++) {
            circles[i].drawShadow(window, deltaTime);
            window.draw(shapes[i]);
        }
        window.display();
    }

    return 0;
}