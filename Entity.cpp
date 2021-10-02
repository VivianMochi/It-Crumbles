#include "Entity.h"

#include "Map.h"
#include "Constants.h"

void Entity::updateBrain(sf::Time elapsed) {
	if (brain) {
		brain->update(elapsed);
	}
}

void Entity::update(sf::Time elapsed) {
	// Fall to ground
	verticalVelocity += GRAVITY * elapsed.asSeconds();
	verticalPosition += verticalVelocity * elapsed.asSeconds();
	if (verticalPosition >= 0) {
		verticalPosition = 0;
		verticalVelocity = 0;
	}

	// Do drag if on ground
	if (verticalPosition == 0) {
		velocity = velocity * float(std::pow(0.1, elapsed.asSeconds()));
	}

	// Do movement
	moveWithCollision(velocity * elapsed.asSeconds());
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	// Draw shadow
	if (verticalPosition < 0) {
		sf::RectangleShape shadow;
		shadow.setSize(sf::Vector2f(size.x, 2));
		shadow.setOrigin(sf::Vector2f(size.x / 2, 2));
		float shadowAlpha = -verticalPosition * 25;
		if (shadowAlpha > 150) {
			shadowAlpha = 150;
		}
		shadow.setFillColor(sf::Color(0, 0, 0, shadowAlpha));
		shadow.setPosition(getPosition());
		target.draw(shadow, states);
	}
}

void Entity::moveWithCollision(sf::Vector2f delta) {
	sf::Vector2f hitboxPosition = sf::Vector2f(getPosition().x - size.x / 2, getPosition().y - size.x / 4);
	sf::Vector2f hitboxSize = sf::Vector2f(size.x, size.x / 2);

	// Do x
	if (!map->checkBoxCollision(hitboxPosition + sf::Vector2f(delta.x, 0), hitboxSize)) {
		move(delta.x, 0);
	}
	else {
		velocity.x = 0;
	}

	// Do y
	if (!map->checkBoxCollision(hitboxPosition + sf::Vector2f(0, delta.y), hitboxSize)) {
		move(0, delta.y);
	}
	else {
		velocity.y = 0;
	}
}

void Entity::installBrain(std::shared_ptr<Brain> brain) {
	this->brain = brain;
	this->brain->entity = this;
}
