#include "Bullet.h"

#include "Map.h"
#include "Helpers.h"
#include "Constants.h"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f velocity, float damage, bool evil, float verticalOffset) {
	setPosition(position);
	this->velocity = velocity;
	this->damage = damage;
	this->evil = evil;
	size = sf::Vector2f(2, 2);
	verticalPosition = -4 + verticalOffset;

	sprite.setRadius(1);
	sprite.setOrigin(sprite.getRadius(), sprite.getRadius());
	if (evil) {
		sprite.setFillColor(sf::Color(0x891F1FFF));
		sprite.setOutlineColor(sf::Color::White);
	}
	else {
		sprite.setFillColor(sf::Color(0x005777FF));
		sprite.setOutlineColor(sf::Color::White);
	}
	sprite.setOutlineThickness(1);
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
}

void Bullet::update(sf::Time elapsed) {
	flightTime -= elapsed.asSeconds();
	if (flightTime > 0) {
		if (verticalPosition > -4) {
			verticalPosition = -4;
		}
		if (verticalVelocity > 0) {
			verticalVelocity = 0;
		}
	}

	// Simplification of Entity::update()
	verticalVelocity += GRAVITY * elapsed.asSeconds();
	verticalPosition += verticalVelocity * elapsed.asSeconds();
	move(velocity * elapsed.asSeconds());

	// Collide with enemies
	float collideDistance = 6;
	std::shared_ptr<Entity> entityCollision = map->getNearestEnemy(getPosition(), collideDistance, !evil);
	if (entityCollision && std::abs(entityCollision->verticalPosition - verticalPosition) <= collideDistance) {
		dead = true;
		entityCollision->dealDamage(damage, vm::normalize(velocity));
	}

	// Collide with wall
	if (verticalPosition >= -WALL_HEIGHT * 2) {
		Block *wallCollision = map->getBlockAt(getPosition());
		if (wallCollision && wallCollision->isBlocking()) {
			dead = true;
			wallCollision->dealDamage(damage);
		}
	}

	// Collide with floor or fall out
	if (verticalPosition >= 0) {
		bool onFloor = map->checkBoxCollision(getFloorHitbox(), true);
		if (onFloor) {
			dead = true;
			Block *groundCollision = map->getBlockAt(getPosition());
			if (groundCollision) {
				groundCollision->dealDamage(damage);
			}
		}
		else if (verticalPosition >= ENTITY_FALLEN_DEPTH) {
			dead = true;
		}
	}

	// Update sprite
	sprite.setRadius(sprite.getRadius() + (2 - sprite.getRadius()) * elapsed.asSeconds() * 10);
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
	sprite.setFillColor(getFallingColor(sprite.getFillColor()));
	sprite.setOutlineColor(getFallingColor(sprite.getOutlineColor()));
}

void Bullet::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Entity::draw(target, states);

	target.draw(sprite, states);
}
