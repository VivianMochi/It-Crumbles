#include "Bomb.h"

#include "Map.h"

Bomb::Bomb(sf::Vector2f position, sf::Vector2f velocity, float damage, float radius, bool raised, float verticalOffset) {
	setPosition(position);
	this->velocity = velocity;
	this->damage = damage;
	this->radius = radius;
	size = sf::Vector2f(6, 6);
	if (raised) {
		verticalVelocity = -10;
		verticalPosition = -5 + verticalOffset;
	}
	else {
		verticalVelocity = -18;
		verticalPosition = verticalOffset;
	}

	sprite.setRadius(3);
	sprite.setOrigin(sprite.getRadius(), sprite.getRadius() * 2);
	sprite.setFillColor(sf::Color(30, 30, 30));
	sprite.setOutlineColor(sf::Color::White);
	sprite.setOutlineThickness(1);
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
}

void Bomb::update(sf::Time elapsed) {
	fuseTime -= elapsed.asSeconds();
	if (fuseTime <= 0) {
		dead = true;
		map->createExplosion(getPosition(), damage, radius);
	}

	Entity::update(elapsed);

	// Update sprite
	flashCounter += elapsed.asSeconds();
	float flashInterval = fuseTime / 4 + 0.1;
	if (flashCounter >= flashInterval && !dead) {
		flashCounter -= flashInterval;
		map->sounds.playSound("Fuse", 100, 1.5 - fuseTime / 10);
	}
	if (flashCounter < 0.1) {
		sprite.setFillColor(sf::Color::White);
		sprite.setOutlineColor(sf::Color(30, 30, 30));
	}
	else {
		sprite.setFillColor(sf::Color(30, 30, 30));
		sprite.setOutlineColor(sf::Color::White);
	}
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
	sprite.setFillColor(getFallingColor(sprite.getFillColor()));
	sprite.setOutlineColor(getFallingColor(sprite.getOutlineColor()));
}

void Bomb::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Entity::draw(target, states);

	target.draw(sprite, states);
}
