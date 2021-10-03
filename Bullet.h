#pragma once

#include "Entity.h"

class Bullet : public Entity {
public:
	Bullet(sf::Vector2f position, sf::Vector2f velocity, float damage = 1, bool evil = true, float verticalOffset = 0);

	virtual void update(sf::Time elapsed) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Data
	float damage = 1;
	bool evil = true;
	float flightTime = 0.2;

	sf::CircleShape sprite;
};

