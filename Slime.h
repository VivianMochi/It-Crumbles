#pragma once

#include "Entity.h"

const sf::Vector2i SLIME_FRAME_SIZE = { 10, 10 };

class Slime : public Entity {
public:
	Slime(sf::Vector2f position, int type = 0, float powerMultiplier = 1);

	virtual void update(sf::Time elapsed) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	virtual void dealDamage(float amount, sf::Vector2f direction = sf::Vector2f(), std::string damageType = "") override;
	virtual void onDeath() override;

	int type = 0;

	// Stats
	float moveSpeed = 30;
	float groundAcceleration = 15;
	float airAcceleration = 1;

	float fireMaxCooldown = 2;
	float fireCooldown = 0;

	float bulletSpeed = 30;
	float aimDeviation = 0.5;
	float bulletDamage = 2;
	float bulletFlightTime = 0.8;

	bool facingRight = true;
	int frame = 0;
	float frameCounter = 0;

	sf::Sprite sprite;
};

