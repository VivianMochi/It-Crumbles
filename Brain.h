#pragma once

#include <SFML/Graphics.hpp>

class Entity;

class Brain {
public:
	virtual void update(sf::Time elapsed);

	// References
	Entity *entity;

	// Data
	bool enemy = true;
};

