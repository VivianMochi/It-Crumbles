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
	// Tick timers
	damageTimer -= elapsed.asSeconds();

	bool wasInAir = !onGround();

	// Fall to ground
	verticalVelocity += GRAVITY * elapsed.asSeconds();
	verticalPosition += verticalVelocity * elapsed.asSeconds();
	if (verticalPosition >= 0 && !falling) {
		// Check to make sure there is ground
		bool onFloor = map->checkBoxCollision(getFloorHitbox(), true);
		if (onFloor) {
			verticalPosition = 0;
			verticalVelocity = 0;

			// If touching down inside a wall, bounce
			bool insideWall = map->checkBoxCollision(getFloorHitbox());
			if (insideWall) {
				rocketTime += 1;
				verticalVelocity = -20;
				velocity += sf::Vector2f(std::rand() % 41 - 20, std::rand() % 41 - 20);
			}
			else {
				// Otherwise stop rocketing
				rocketTime = 0;
			}

			// Create a splash effect
			// Todo: using canRocket might not be right
			if (wasInAir && canRocket) {
				map->createSplash(getPosition(), 10, sf::Color::Transparent, 0.2);
			}
		}
		else {
			// Below the map and falling
			falling = true;
			if (verticalPosition == 0) {
				verticalPosition = 0.01;
			}
			rocketTime = 5;
		}
	}

	if (falling && verticalPosition >= ENTITY_FALLEN_DEPTH) {
		dead = true;
	}

	// Do drag if on ground or falling
	if (onGround() || falling) {
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
		rocketSmokeCooldown = 0.05 + std::rand() % 5 / 100.0f;
		sf::Vector2f smokeOffset = -vm::normalize(velocity);
		sf::Vector2f smokePosition = getPosition() + smokeOffset;
		sf::Vector2f smokeVelocity = sf::Vector2f(std::rand() % 11 - 5, std::rand() % 11 - 5);
		float smokeLifespan = 0.2f + (std::rand() % 20 / 100.0f);
		map->addEntity(std::make_shared<Smoke>(smokePosition, smokeVelocity, smokeRadius, smokeLifespan, verticalPosition, sf::Color(30, 30, 30), false));
	}
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	// Draw shadow
	if (!onGround() && !falling) {
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
	bool insideWall = map->checkBoxCollision(getFloorHitbox());

	// If high enough or inside a wall, ignore collisions
	if (verticalPosition < -WALL_HEIGHT * 2 || insideWall) {
		move(delta);
	}
	else if (isRocketing() && !falling) {
		// Also ignore collisions if rocketing
		move(delta);
	}
	else {
		sf::Vector2f hitboxPosition = sf::Vector2f(getFloorHitbox().left, getFloorHitbox().top);
		sf::Vector2f hitboxSize = sf::Vector2f(getFloorHitbox().width, getFloorHitbox().height);

		// Try x movement, and include floors in collision if falling
		if (!map->checkBoxCollision(hitboxPosition + sf::Vector2f(delta.x, 0), hitboxSize, falling)) {
			move(delta.x, 0);
		}
		else {
			velocity.x = 0;
		}

		// Try y movement, and include floors in collision if falling
		if (!map->checkBoxCollision(hitboxPosition + sf::Vector2f(0, delta.y), hitboxSize, falling)) {
			move(0, delta.y);
		}
		else {
			velocity.y = 0;
		}
	}
}

void Entity::dealDamage(float amount, sf::Vector2f direction, std::string damageType) {
	health -= amount;
	damageTimer = 0.1;
	if (health <= 0) {
		dead = true;
	}
	// Todo: this may be too strong
	velocity += direction * amount * 50.f;
}

void Entity::onDeath() {

}

void Entity::setMaxHealth(int maxHealth) {
	this->maxHealth = maxHealth;
	health = maxHealth;
}

void Entity::installBrain(std::shared_ptr<Brain> brain) {
	this->brain = brain;
	this->brain->entity = this;
}

void Entity::plummet() {
	velocity = sf::Vector2f(0, 1);
	verticalPosition = CEILING_HEIGHT;
	verticalVelocity = 80;
	rocketTime = 10;
}

sf::FloatRect Entity::getFloorHitbox() {
	sf::Vector2f hitboxSize = sf::Vector2f(size.x, size.x / 2);
	if (hitboxSize.x < 2) {
		hitboxSize.x = 2;
	}
	if (hitboxSize.y < 2) {
		hitboxSize.y = 2;
	}
	sf::Vector2f hitboxPosition = sf::Vector2f(getPosition().x - hitboxSize.x / 2, getPosition().y - hitboxSize.y / 2);
	return sf::FloatRect(hitboxPosition, hitboxSize);
}

sf::Color Entity::getFallingColor(sf::Color color) {
	float fadeStart = 5;
	if (verticalPosition >= ENTITY_FALLEN_DEPTH) {
		color.a = 0;
	}
	else if (verticalPosition >= fadeStart) {
		color.a = (ENTITY_FALLEN_DEPTH - verticalPosition) / (ENTITY_FALLEN_DEPTH - fadeStart) * 255;
	}
	else {
		color.a = 255;
	}
	return color;
}

bool Entity::haveControl() const {
	return !falling && !dead;
}

bool Entity::onGround() const {
	return verticalPosition == 0;
}

bool Entity::isRocketing() const {
	return canRocket && rocketTime > 0 && !dead;
}
