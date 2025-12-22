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

	// TODO : Fix unused heap memory usage by implementing "SoundManager::update()" 
	// Instead of implementing seperate function, maybe we can just remove the unused sound objects before emplacing new one?

	// Nah we will still implement seperate function

	clearStoppedSounds();

	activeSounds.emplace_back(buffer);
	activeSounds.back().setVolume(50.f);
	activeSounds.back().play();
}

void SoundManager::clearStoppedSounds()
{
	for (auto it = activeSounds.begin(); it != activeSounds.end();) {
		if (it->getStatus() == sf::Sound::Status::Stopped) {
			it = activeSounds.erase(it);
			continue;
		}
		it++;
	}
}