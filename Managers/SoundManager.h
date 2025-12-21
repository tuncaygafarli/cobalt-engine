#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>

class SoundManager
{

public:
	void playHitSound(const std::string& path);

private:
	std::vector<std::pair<sf::SoundBuffer, sf::Sound>> soundInstances;
};