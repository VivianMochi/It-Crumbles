#pragma once

#include "Brain.h"
#include "SFML/Graphics.hpp"
#include <memory>

class Map;

class Entity : public sf::Transformable, public sf::Drawable {
public:
	void updateBrain(sf::Time elapsed);
	virtual void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	void updatePosition(sf::Time elapsed, Map *map);

	void installBrain(std::shared_ptr<Brain> brain);

	// Data
	sf::Vector2f size;
	sf::Vector2f velocity;
	float verticalPosition = 0;
	float verticalVelocity = 0;

	// Brain socket
	std::shared_ptr<Brain> brain;

	// Controllable via brain
	sf::Vector2f moveDirection;
	sf::Vector2f aimDirection;
	bool fireControl = false;
	bool dodgeControl = false;
	bool abilityControl = false;
	bool bombControl = false;
};

