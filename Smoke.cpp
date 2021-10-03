#include "Smoke.h"

Smoke::Smoke(sf::Vector2f position, sf::Vector2f velocity, float radius, float lifespan, float verticalPosition, sf::Color color, bool grow) {
	setPosition(position);
	this->velocity = velocity;
	this->radius = radius;
	this->lifespan = lifespan;
	this->maxLifespan = lifespan;
	this->verticalPosition = verticalPosition;
	this->color = color;
	canRocket = false;

	if (grow) {
		sprite.setRadius(radius / 4.0f);
	}
	else {
		sprite.setRadius(radius);
	}
	sprite.setOrigin(sprite.getRadius(), sprite.getRadius() * 2);
	sprite.setFillColor(getFallingColor(color));
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
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
	sprite.setFillColor(getFallingColor(color));
}

void Smoke::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(sprite, states);
}
