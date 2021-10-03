#pragma once

#include "Entity.h"

class Smoke : public Entity {
public:
	Smoke(sf::Vector2f position, sf::Vector2f velocity, float radius = 4, float lifespan = 1, float verticalPosition = 0, sf::Color color = sf::Color(30, 30, 30), bool grow = true);

	virtual void update(sf::Time elapsed) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Data
	float radius;
	float maxLifespan;
	float lifespan;
	sf::Color color;

	sf::CircleShape sprite;
};

