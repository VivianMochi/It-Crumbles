#include "Smoke.h"

Smoke::Smoke(sf::Vector2f position, sf::Vector2f velocity, float radius, float lifespan, float verticalPosition, sf::Color color) {
	setPosition(position);
	this->velocity = velocity;
	this->radius = radius;
	this->lifespan = lifespan;
	this->maxLifespan = lifespan;
	this->verticalPosition = verticalPosition;
	this->color = color;

	sprite.setRadius(radius / 4.0f);
	sprite.setOrigin(sprite.getRadius(), sprite.getRadius() * 2);
	sprite.setFillColor(color);
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
}

void Smoke::update(sf::Time elapsed) {
	lifespan -= elapsed.asSeconds();

	Entity::update(elapsed);

	// Update sprite
	float desiredRadius = radius * lifespan / maxLifespan;
	sprite.setRadius(sprite.getRadius() + (desiredRadius - sprite.getRadius()) * elapsed.asSeconds() * 10);
	if (sprite.getRadius() <= 0) {
		dead = true;
	}
	sprite.setOrigin(sprite.getRadius(), sprite.getRadius() * 2);
	sprite.setFillColor(color);
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
}

void Smoke::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Entity::draw(target, states);

	target.draw(sprite, states);
}
