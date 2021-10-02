#include "Robot.h"

#include "Map.h"
#include "Bullet.h"
#include "Helpers.h"

Robot::Robot() {
	size = sf::Vector2f(8, 8);
}

void Robot::update(sf::Time elapsed) {
	sf::Vector2f desiredVelocity = moveDirection * moveSpeed;
	velocity += (desiredVelocity - velocity) * elapsed.asSeconds() * acceleration;

	if (dodgeControl && verticalPosition == 0) {
		verticalVelocity = -25;
	}

	if (fireControl && std::rand() % 2 == 0) {
		sf::Vector2f bulletVelocity = vm::rotate(vm::normalize(aimDirection) * 100.0f, std::rand() % 101 / 100.0f * 0.5 - 0.25) + velocity;
		map->addEntity(std::make_shared<Bullet>(getPosition(), bulletVelocity, 3, false));
	}

	Entity::update(elapsed);
}

void Robot::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Entity::draw(target, states);

	sf::RectangleShape sprite;
	sprite.setSize(size);
	sprite.setOrigin(size.x / 2, size.y);
	sprite.setPosition(getPosition() + sf::Vector2f(0, verticalPosition));
	sprite.setOutlineThickness(-1);
	sprite.setFillColor(sf::Color::Transparent);
	sprite.setOutlineColor(sf::Color::Red);
	target.draw(sprite, states);
}
