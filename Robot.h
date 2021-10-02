#pragma once

#include "Entity.h"

class Robot : public Entity {
public:
	Robot();

	virtual void update(sf::Time elapsed) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Stats
	float moveSpeed = 80;
	float acceleration = 15;

	sf::Sprite sprite;
};

