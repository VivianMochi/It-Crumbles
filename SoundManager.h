#pragma once

#include <SFML/Audio.hpp>
#include <deque>

const int MAX_SOUNDS = 200;

class SoundManager {
public:
	// A pitch of -1 will give a slightly random pitch
	void playSound(std::string sound, float volume = 100, float pitch = 1);

	std::deque<std::shared_ptr<sf::Sound>> sounds;
};
