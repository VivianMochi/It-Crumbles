#include "Entity.h"

#include "Map.h"
#include "Constants.h"
#include "Smoke.h"
#include "Helpers.h"

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

		// If touching down inside a wall, bounce
		sf::Vector2f hitboxPosition = sf::Vector2f(getPosition().x - size.x / 2, getPosition().y - size.x / 4);
		sf::Vector2f hitboxSize = sf::Vector2f(size.x, size.x / 2);
		bool insideWall = map->checkBoxCollision(hitboxPosition, hitboxSize);
		if (insideWall) {
			rocketTime += 1;
			verticalVelocity = -20;
			velocity += sf::Vector2f(std::rand() % 41 - 20, std::rand() % 41 - 20);
		}
		else {
			// Otherwise stop rocketing
			rocketTime = 0;
		}
	}

	// Do drag if on ground
	if (verticalPosition == 0) {
		velocity = velocity * float(std::pow(0.1, elapsed.asSeconds()));
	}

	// Do movement
	moveWithCollision(velocity * elapsed.asSeconds());

	// Update rocket time
	rocketTime -= elapsed.asSeconds();
	if (rocketTime < 0) {
		rocketTime = 0;
	}
	rocketSmokeCooldown -= elapsed.asSeconds();

	// Create smoke if rocketing
	float smokeRadius = size.y / 2;
	if (isRocketing() && smokeRadius > 0 && rocketSmokeCooldown <= 0) {
		rocketSmokeCooldown = 0.05 + std::rand() % 10 / 100.0f;
		sf::Vector2f smokeOffset = -vm::normalize(velocity);
		sf::Vector2f smokePosition = getPosition() + smokeOffset;
		sf::Vector2f smokeVelocity = sf::Vector2f(std::rand() % 11 - 5, std::rand() % 11 - 5);
		float smokeLifespan = 0.4f + (std::rand() % 20 / 100.0f);
		map->addEntity(std::make_shared<Smoke>(smokePosition, smokeVelocity, smokeRadius, smokeLifespan, verticalPosition, sf::Color(30, 30, 30), false));
	}
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

	// Draw hitbox
	bool drawHitbox = false;
	if (drawHitbox) {
		sf::RectangleShape sprite;
		sprite.setSize(size);
		sprite.setOrigin(size.x / 2, size.y);
		sprite.setPosition(getPosition() + sf::Vector2f(0, verticalPosition));
		sprite.setOutlineThickness(-1);
		sprite.setFillColor(sf::Color::Transparent);
		sprite.setOutlineColor(sf::Color::Red);
		target.draw(sprite, states);
	}
}

void Entity::moveWithCollision(sf::Vector2f delta) {
	sf::Vector2f hitboxPosition = sf::Vector2f(getPosition().x - size.x / 2, getPosition().y - size.x / 4);
	sf::Vector2f hitboxSize = sf::Vector2f(size.x, size.x / 2);

	bool insideWall = map->checkBoxCollision(hitboxPosition, hitboxSize);

	// If high enough or inside a wall, ignore collisions
	if (verticalPosition < -WALL_HEIGHT * 2 || insideWall) {
		move(delta);
	}
	else if (isRocketing()) {
		// Also ignore collisions if rocketing
		move(delta);
	}
	else {
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
}

void Entity::installBrain(std::shared_ptr<Brain> brain) {
	this->brain = brain;
	this->brain->entity = this;
}

bool Entity::isRocketing() const {
	return rocketTime > 0 && !dead;
}
