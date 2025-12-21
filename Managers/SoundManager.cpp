#include "SoundManager.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>

void SoundManager::playHitSound(const std::string& path)
{
	static sf::SoundBuffer buffer;

	if (!buffer.loadFromFile(path)) 
	{
		std::cout << "Failed to load sound file." << std::endl;
	}

	activeSounds.emplace_back(buffer);
	activeSounds.back().setVolume(100.f);
	activeSounds.back().play();
}