#include "Bullet.h"

#include "Map.h"
#include "Helpers.h"
#include "Constants.h"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f velocity, float damage, bool evil) {
	setPosition(position);
	this->velocity = velocity;
	this->damage = damage;
	this->evil = evil;
	size = sf::Vector2f(2, 2);
	verticalPosition = -4;

	sprite.setRadius(2);
	sprite.setOrigin(sprite.getRadius(), sprite.getRadius());
	sprite.setFillColor(sf::Color::Red);
	sprite.setOutlineColor(sf::Color::White);
	sprite.setOutlineThickness(1);
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
}

void Bullet::update(sf::Time elapsed) {
	flightTime -= elapsed.asSeconds();
	if (flightTime > 0) {
		verticalPosition = -4;
		verticalVelocity = 0;
	}

	// Simplification of Entity::update()
	verticalVelocity += GRAVITY * elapsed.asSeconds();
	verticalPosition += verticalVelocity * elapsed.asSeconds();
	if (verticalPosition >= 0) {
		verticalPosition = 0;
		verticalVelocity = 0;
	}
	move(velocity * elapsed.asSeconds());

	// Collide with wall
	Block *wallCollision = map->getBlockAt(getPosition());
	if (wallCollision && wallCollision->isBlocking()) {
		dead = true;
		wallCollision->damage(damage);
	}

	// Collide with floor
	if (verticalPosition == 0) {
		dead = true;
		Block *groundCollision = map->getBlockAt(getPosition());
		if (groundCollision) {
			groundCollision->damage(damage);
		}
	}

	// Update sprite
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
}

void Bullet::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Entity::draw(target, states);

	target.draw(sprite, states);
}
