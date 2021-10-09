#include "PlayerBrain.h"

#include "Entity.h"
#include "Helpers.h"

PlayerBrain::PlayerBrain() {
	enemy = false;
}

void PlayerBrain::update(sf::Time elapsed) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		entity->moveDirection.x = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		entity->moveDirection.x = 1;
	}
	else {
		entity->moveDirection.x = 0;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		entity->moveDirection.y = -1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		entity->moveDirection.y = 1;
	}
	else {
		entity->moveDirection.y = 0;
	}
	entity->moveDirection = vm::normalize(entity->moveDirection);

	// Aim direction is computed in the state that owns the robot

	entity->fireControl = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	entity->jumpControl = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
	entity->abilityControl = sf::Mouse::isButtonPressed(sf::Mouse::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::E);
}
