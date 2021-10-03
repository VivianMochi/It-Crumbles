#include "Robot.h"

#include "Map.h"
#include "Bullet.h"
#include "Bomb.h"
#include "Helpers.h"
#include "ResourceManager.h"
#include "Stats.h"

Robot::Robot() {
	size = sf::Vector2f(6, 10);

	sprite.setTexture(rm::loadTexture("Resource/Image/Robot.png"));
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(), ROBOT_FRAME_SIZE));
	sprite.setOrigin(ROBOT_FRAME_SIZE.x / 2, ROBOT_FRAME_SIZE.y);
}

void Robot::update(sf::Time elapsed) {
	// Tick cooldowns
	fireCooldown -= elapsed.asSeconds();
	bombCooldown -= elapsed.asSeconds();

	// Control
	if (haveControl()) {
		if (!isRocketing()) {
			sf::Vector2f desiredVelocity = moveDirection * stats.at("Movespeed");
			velocity += (desiredVelocity - velocity) * elapsed.asSeconds() * (onGround() ? stats.at("GroundAcceleration") : stats.at("AirAcceleration"));

			if (jumpControl && onGround()) {
				Block *standingOn = map->getBlockAt(getPosition());
				if (standingOn && standingOn->launcher) {
					verticalVelocity = -200;
					velocity = sf::Vector2f();
					rocketTime = 10;
					standingOn->verticalVelocity = -100;
				}
				else {
					verticalVelocity = -20;
				}
			}
		}

		if (fireControl && fireCooldown <= 0) {
			fireCooldown = stats.at("BulletCooldown");
			float aimDeviation = stats.at("BulletDeviation");
			sf::Vector2f bulletPosition = getPosition() + vm::normalize(aimDirection) * 6.0f;
			sf::Vector2f bulletVelocity = vm::rotate(vm::normalize(aimDirection) * stats.at("BulletSpeed"), std::rand() % 101 / 100.0f * aimDeviation - aimDeviation / 2);
			std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>(bulletPosition, bulletVelocity, stats.at("BulletDamage"), false, verticalPosition);
			newBullet->flightTime = stats.at("BulletFlightTime");
			map->addEntity(newBullet);
			map->sounds.playSound("Shoot", 100, -1);
		}

		if (abilityControl && bombCooldown <= 0) {
			bombCooldown = stats.at("BombCooldown");
			sf::Vector2f bombPosition = getPosition() + vm::normalize(aimDirection);
			if (vm::magnitude(aimDirection) > stats.at("BombRange")) {
				aimDirection = vm::normalize(aimDirection) * stats.at("BombRange");
			}
			sf::Vector2f bombVelocity = aimDirection;
			std::shared_ptr<Bomb> bomb = std::make_shared<Bomb>(bombPosition, bombVelocity, stats.at("BombDamage"), stats.at("BombRadius"), true, verticalPosition);
			bomb->fuseTime = stats.at("BombFuse");
			map->addEntity(bomb);
			map->sounds.playSound("Throw", 100, -1);
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
				map->createDust(getPosition(), -vm::normalize(velocity));
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
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(adjustedFrame * ROBOT_FRAME_SIZE.x, bombCooldown > 0 ? ROBOT_FRAME_SIZE.y : 0), ROBOT_FRAME_SIZE));

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
	}
	sprite.setScale(facingRight ? 1 : -1, 1);
	sprite.setPosition(std::round(getPosition().x), std::round(getPosition().y + verticalPosition));
	sprite.setColor(getFallingColor());
}

void Robot::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	Entity::draw(target, states);

	target.draw(sprite, states);
}

void Robot::dealDamage(float amount, sf::Vector2f direction, std::string damageType) {
	// The robot doesn't take damage, it creates an explosion instead
	if (damageType != "Explosion") {
		map->createExplosion(getPosition() - vm::normalize(direction), 15, 20);
	}
}
