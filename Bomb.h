#pragma once

#include "Entity.h"

class Bomb : public Entity {
public:
	Bomb(sf::Vector2f position, sf::Vector2f velocity, float damage = 8, float radius = 30, bool raised = false, float verticalOffset = 0);

	virtual void update(sf::Time elapsed) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Data
	float damage = 8;
	float radius = 30;
	float fuseTime = 3;

	float flashCounter = 0;

	sf::CircleShape sprite;
};

