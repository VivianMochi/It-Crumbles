#include "TowerState.h"

#include "Helpers.h"

TowerState::TowerState() {

}

TowerState::~TowerState() {

}

void TowerState::init() {
	floors.resize(1);
	floors[0].generateMap();

	robot = std::make_shared<Robot>();
	robot->installBrain(std::make_shared<PlayerBrain>());
	floors[0].entities.push_back(robot);
}

void TowerState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			floors[currentFloor].createExplosion(getGame()->getCursorPosition() + cameraPosition, 5, 40);
			//floors[currentFloor].getBlockAt(getGame()->getCursorPosition() + cameraPosition)->damage(1);
		}
	}
	else if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Num1) {
			getGame()->changeState(new TowerState());
		}
	}
}

void TowerState::update(sf::Time elapsed) {
	// Give the robot an aim direction
	robot->aimDirection = getGame()->getCursorPosition() + cameraPosition - robot->getPosition();

	// Update floors
	// Todo: maybe only update current floor?
	for (Map &map : floors) {
		map.update(elapsed);
	}

	// Update camera
	sf::Vector2f desiredCameraPosition = (robot->getPosition() + robot->getPosition() + (getGame()->getCursorPosition() + cameraPosition)) / 3.0f - sf::Vector2f(getGame()->gameSize / 2);
	cameraPosition += (desiredCameraPosition - cameraPosition) * elapsed.asSeconds() * 10.0f;
}

void TowerState::render(sf::RenderWindow &window) {
	sf::RenderStates cameraTransform;
	cameraTransform.transform.translate(std::round(-cameraPosition.x), std::round(-cameraPosition.y));

	if (currentFloor >= 0 && currentFloor < floors.size()) {
		window.draw(floors[currentFloor], cameraTransform);
	}
}
