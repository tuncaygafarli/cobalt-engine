#include "SoundManager.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>

void SoundManager::playHitSound(const std::string& path)
{
	sf::Music sound(path);
	sound.play();
}