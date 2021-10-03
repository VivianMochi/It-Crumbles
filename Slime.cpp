#include "Slime.h"

#include "Map.h"
#include "ResourceManager.h"
#include "Bullet.h"
#include "Helpers.h"

Slime::Slime(sf::Vector2f position) {
	setPosition(position);
	size = sf::Vector2f(6, 6);
	setMaxHealth(5);

	sprite.setTexture(rm::loadTexture("Resource/Image/Slime.png"));
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(), SLIME_FRAME_SIZE));
	sprite.setOrigin(SLIME_FRAME_SIZE.x / 2, SLIME_FRAME_SIZE.y);
}

void Slime::update(sf::Time elapsed) {
	// Tick cooldowns
	fireCooldown -= elapsed.asSeconds();

	// Control
	if (haveControl()) {
		if (!isRocketing()) {
			sf::Vector2f desiredVelocity = moveDirection * moveSpeed;
			velocity += (desiredVelocity - velocity) * elapsed.asSeconds() * (onGround() ? groundAcceleration : airAcceleration);

			if (dodgeControl && onGround()) {
				verticalVelocity = -20;
			}
		}

		if (fireControl && fireCooldown <= 0) {
			fireCooldown = fireMaxCooldown;
			float bulletSpeed = 30;
			float aimDeviation = 0.5;
			float damage = 1;
			sf::Vector2f bulletPosition = getPosition() + vm::normalize(aimDirection) * 6.0f;
			sf::Vector2f bulletVelocity = vm::rotate(vm::normalize(aimDirection) * bulletSpeed, std::rand() % 101 / 100.0f * aimDeviation - aimDeviation / 2);
			std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>(bulletPosition, bulletVelocity, damage, true, verticalPosition);
			newBullet->flightTime = 0.8;
			map->addEntity(newBullet);
		}
	}

	Entity::update(elapsed);

	// Update frame
	if (isRocketing()) {
		frameCounter += elapsed.asSeconds();
		if (frameCounter >= 0.05) {
			frameCounter -= 0.05;
			frame++;
			if (frame >= 4) {
				frame -= 4;
			}
		}
	}
	else if (!onGround()) {
		frame = 1;
	}
	else if (vm::magnitude(moveDirection) > 0) {
		frameCounter += elapsed.asSeconds();
		if (frameCounter >= 0.05) {
			frameCounter -= 0.05;
			frame++;
			if (frame >= 4) {
				frame -= 4;
			}
		}
	}
	else {
		frame = 0;
	}

	int adjustedFrame = frame;
	if (isRocketing()) {
		adjustedFrame += 4;
	}
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(adjustedFrame * SLIME_FRAME_SIZE.x, damageTimer > 0 ? SLIME_FRAME_SIZE.y : 0), SLIME_FRAME_SIZE));

	// Update sprite
	if (isRocketing()) {
		facingRight = velocity.x > 0;
	}
	else {
		if (aimDirection.x > 0) {
			facingRight = true;
		}
		else if (aimDirection.x < 0) {
			facingRight = false;
		}
		else {
			if (moveDirection.x > 0) {
				facingRight = true;
			}
			else if (moveDirection.x < 0) {
				facingRight = false;
			}
		}
	}
	sprite.setScale(facingRight ? 1 : -1, 1);
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
	sprite.setColor(getFallingColor());
}

void Slime::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	Entity::draw(target, states);

	target.draw(sprite, states);
}

void Slime::onDeath() {
	if (!falling) {
		map->createSplash(getPosition(), 10, sf::Color(0x891F1FFF), 0.2);
		map->createSplash(getPosition(), 6, sf::Color(0xFF6666FF), 0.2);
	}
}
