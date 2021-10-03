#include "EnemyBrain.h"

#include "Entity.h"
#include "Helpers.h"
#include "Map.h"

EnemyBrain::EnemyBrain() {
	desiredRange = 20 + std::rand() % 55;
	orbitSpeed = std::rand() % 100 / 100.0f * 2 - 1;
	orbitAngle = std::rand() % 100 / 100.0f * 2 * 3.14159;
}

void EnemyBrain::update(sf::Time elapsed) {
	std::shared_ptr<Entity> target = entity->map->getNearestEnemy(entity->getPosition(), 70);
	if (target && !target->isRocketing() && !entity->isRocketing()) {
		// Keep range and shoot enemy
		orbitAngle += orbitSpeed * elapsed.asSeconds();
		sf::Vector2f desiredPosition = target->getPosition() + vm::rotate(sf::Vector2f(desiredRange, 0), orbitAngle);
		entity->moveDirection = vm::normalize(desiredPosition - entity->getPosition());
		entity->aimDirection = target->getPosition() - entity->getPosition();

		targetCounter += elapsed.asSeconds();
		entity->fireControl = targetCounter >= 0.5;
	}
	else {
		targetCounter = 0;
		orbitAngle = std::rand() % 100 / 100.0f * 2 * 3.14159;
		// Wander
		phaseCooldown -= elapsed.asSeconds();
		if (phaseCooldown <= 0) {
			if (phase == idle) {
				phase = wandering;
				phaseCooldown = 0.5 + std::rand() % 3 / 2.0f;
				entity->moveDirection = vm::normalize(sf::Vector2f(std::rand() % 11 - 5, std::rand() % 11 - 5));
			}
			else if (phase == wandering) {
				phase = idle;
				phaseCooldown = 0.5 + std::rand() % 6 / 2.0f;
				entity->moveDirection = sf::Vector2f();
			}
		}
		entity->aimDirection = sf::Vector2f();
		entity->fireControl = false;
	}
}
