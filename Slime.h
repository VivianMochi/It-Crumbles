#pragma once

#include "Entity.h"

const sf::Vector2i SLIME_FRAME_SIZE = { 10, 10 };

class Slime : public Entity {
public:
	Slime(sf::Vector2f position);

	virtual void update(sf::Time elapsed) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Stats
	float moveSpeed = 30;
	float groundAcceleration = 15;
	float airAcceleration = 1;

	float fireMaxCooldown = 0.5;
	float fireCooldown = 0;

	bool facingRight = true;
	int frame = 0;
	float frameCounter = 0;


	sf::Sprite sprite;
};

