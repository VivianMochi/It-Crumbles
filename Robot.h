#pragma once

#include "Entity.h"

class Robot : public Entity {
public:
	Robot();

	virtual void update(sf::Time elapsed) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Stats
	float moveSpeed = 80;
	float groundAcceleration = 15;
	float airAcceleration = 1;

	float bombMaxCooldown = 2;
	float bombCooldown = 0;

	sf::Sprite sprite;
};

