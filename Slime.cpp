#include "Slime.h"

#include "Map.h"
#include "ResourceManager.h"
#include "Bullet.h"
#include "Helpers.h"

Slime::Slime(sf::Vector2f position) {
	setPosition(position);
	size = sf::Vector2f(6, 6);

	sprite.setTexture(rm::loadTexture("Resource/Image/Slime.png"));
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(), SLIME_FRAME_SIZE));
	sprite.setOrigin(SLIME_FRAME_SIZE.x / 2, SLIME_FRAME_SIZE.y);
}

void Slime::update(sf::Time elapsed) {
	// Tick cooldowns
	fireCooldown -= elapsed.asSeconds();

	// Control
	if (!isRocketing()) {
		sf::Vector2f desiredVelocity = moveDirection * moveSpeed;
		velocity += (desiredVelocity - velocity) * elapsed.asSeconds() * (verticalPosition == 0 ? groundAcceleration : airAcceleration);

		if (dodgeControl && verticalPosition == 0) {
			verticalVelocity = -20;
		}
	}

	if (fireControl && fireCooldown <= 0) {
		fireCooldown = fireMaxCooldown;
		sf::Vector2f bulletPosition = getPosition() + vm::normalize(aimDirection) * 6.0f;
		sf::Vector2f bulletVelocity = vm::rotate(vm::normalize(aimDirection) * 100.0f, std::rand() % 101 / 100.0f * 0.1 - 0.05);
		map->addEntity(std::make_shared<Bullet>(bulletPosition, bulletVelocity, 1, true, verticalPosition));
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
	else if (verticalPosition < 0) {
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
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(adjustedFrame * SLIME_FRAME_SIZE.x, 0), SLIME_FRAME_SIZE));

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
}

void Slime::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	Entity::draw(target, states);

	target.draw(sprite, states);
}
