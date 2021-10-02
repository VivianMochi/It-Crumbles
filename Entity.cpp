#include "Entity.h"

#include "Map.h"

void Entity::updateBrain(sf::Time elapsed) {
	if (brain) {
		brain->update(elapsed);
	}
}

void Entity::update(sf::Time elapsed) {
	velocity = velocity * float(std::pow(0.1, elapsed.asSeconds()));
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {

}

void Entity::updatePosition(sf::Time elapsed, Map *map) {
	sf::Vector2f delta = velocity * elapsed.asSeconds();

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
