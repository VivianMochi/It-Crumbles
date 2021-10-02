#pragma once

#include <SFML/Graphics.hpp>

class Entity;

class Brain {
public:
	virtual void update(sf::Time elapsed);

	Entity *entity;
};

