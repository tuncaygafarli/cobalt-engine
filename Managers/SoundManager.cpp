#include "SoundManager.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>

void SoundManager::playHitSound(const std::string& path)
{
	sf::SoundBuffer buffer;

	if (!buffer.loadFromFile(path)) 
	{
		std::cout << "Failed to load sound file." << std::endl;
	}

	sf::Sound sound(buffer);
	sound.setVolume(100.f);
	sound.play();
}