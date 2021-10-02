#include "Robot.h"

Robot::Robot() {
	size = sf::Vector2f(8, 8);
}

void Robot::update(sf::Time elapsed) {
	sf::Vector2f desiredVelocity = moveDirection * moveSpeed;
	velocity += (desiredVelocity - velocity) * elapsed.asSeconds() * acceleration;

	Entity::update(elapsed);
}

void Robot::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	sf::RectangleShape sprite;
	sprite.setSize(size);
	sprite.setOrigin(size.x / 2, size.y);
	sprite.setPosition(getPosition());
	sprite.setOutlineThickness(-1);
	sprite.setFillColor(sf::Color::Transparent);
	sprite.setOutlineColor(sf::Color::Red);
	target.draw(sprite, states);
}
