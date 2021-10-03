#include "Block.h"

#include "ResourceManager.h"
#include "Constants.h"

Block::Block(sf::Vector2i position, bool big, bool wall, int health) {
	this->position = position;
	this->big = big;
	this->size = big ? 2 * TILE_SIZE : TILE_SIZE;
	this->wall = wall;
	this->maxHealth = big ? health * 4 : health;
	this->health = this->maxHealth;

	if (big) {
		sprite.setTexture(rm::loadTexture("Resource/Image/BigBlock.png"));
	}
	else {
		sprite.setTexture(rm::loadTexture("Resource/Image/Block.png"));
	}
	sprite.setTextureRect(sf::IntRect(0, 0, size, size + DEPTH_SIZE));
}

void Block::update(sf::Time elapsed) {
	// Update damage events
	auto damageEvent = damageEvents.begin();
	while (damageEvent != damageEvents.end()) {
		damageEvent->delay -= elapsed.asSeconds();
		if (damageEvent->delay <= 0) {
			if (health > 0) {
				if (!immune) {
					health -= damageEvent->amount;
				}
				verticalVelocity = 20 + (std::rand() % 5);
				if (health < 0) {
					health = 0;
				}
				else if (health > maxHealth) {
					health = maxHealth;
				}
			}
			damageEvent = damageEvents.erase(damageEvent);
		}
		else {
			damageEvent++;
		}
	}

	// Update fall status
	if (health <= 0) {
		fallCounter += elapsed.asSeconds();
	}
	if (health <= 0 && fallCounter >= CRUMBLE_TIME && verticalPosition >= BLOCK_FALLEN_DEPTH) {
		fallen = true;
	}

	// Update vertical
	float desiredVerticalPosition = 0;
	if (wall) {
		desiredVerticalPosition = -WALL_HEIGHT;
	}
	if (health <= 0 && fallCounter >= CRUMBLE_TIME) {
		desiredVerticalPosition = BLOCK_FALLEN_DEPTH;
		verticalVelocity += GRAVITY * elapsed.asSeconds();
	}
	else {
		if (easingToPosition) {
			verticalVelocity += ((desiredVerticalPosition - verticalPosition) * 2 - verticalVelocity) * elapsed.asSeconds() * 10;
		}
		else {
			verticalVelocity += (desiredVerticalPosition - verticalPosition) * elapsed.asSeconds() * 300;
			verticalVelocity = verticalVelocity * std::pow(0.01, elapsed.asSeconds());
		}
	}
	verticalPosition += elapsed.asSeconds() * verticalVelocity;
	if (easingToPosition && std::abs(desiredVerticalPosition - verticalPosition) < 1) {
		verticalPosition = desiredVerticalPosition;
		easingToPosition = false;
	}

	// Update color
	if (verticalPosition >= BLOCK_FALLEN_DEPTH) {
		color.a = 0;
	}
	else if (verticalPosition >= 5) {
		color.a = (BLOCK_FALLEN_DEPTH - verticalPosition) / 5 * 255;
	}
	else {
		color.a = 255;
	}

	// Update sprite
	if (immune) {
		int frame = 0;
		if (victory) {
			frame = 2;
		}
		else if (launcher) {
			frame = 1;
		}
		sprite.setTextureRect(sf::IntRect(frame * size, size + DEPTH_SIZE, size, size + DEPTH_SIZE));
	}
	else {
		int damageIndex = 0;
		if (health <= 0) {
			damageIndex = 3;
		}
		else if (health <= 0.35 * maxHealth) {
			damageIndex = 2;
		}
		else if (health <= 0.75 * maxHealth) {
			damageIndex = 1;
		}
		sprite.setTextureRect(sf::IntRect(damageIndex * size, 0, size, size + DEPTH_SIZE));
	}
	sprite.setColor(color);
}

void Block::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform.translate(sf::Vector2f(position * TILE_SIZE) + sf::Vector2f(0, verticalPosition));
	if (health <= 0 && fallCounter < CRUMBLE_TIME) {
		states.transform.translate(0, std::rand() % 2 - 1);
	}
	target.draw(sprite, states);
}

sf::Vector2f Block::getCenter() const {
	return sf::Vector2f(position * TILE_SIZE) + sf::Vector2f(size / 2, size / 2);
}

sf::Vector2f Block::getBack() const {
	return sf::Vector2f(position * TILE_SIZE) + sf::Vector2f(size / 2, 0);
}

bool Block::isBlocking() const {
	return wall && verticalPosition <= WALL_HEIGHT / 2;
}

void Block::dealDamage(float amount, float delay) {
	damageEvent newEvent;
	newEvent.amount = amount;
	newEvent.delay = delay;
	damageEvents.push_back(newEvent);
}
