#include "SoundManager.h"

#include "ResourceManager.h"

void SoundManager::playSound(std::string sound, float volume, float pitch) {
	if (sounds.size() >= MAX_SOUNDS) {
		sounds.pop_front();
	}
	sounds.push_back(std::make_shared<sf::Sound>(rm::loadSoundBuffer("Resource/Sound/" + sound + ".wav")));
	sounds.back()->setVolume(volume);
	if (pitch == -1) {
		pitch = 0.9 + std::rand() % 20 / 100.0f;
	}
	sounds.back()->setPitch(pitch);
	sounds.back()->play();
}
