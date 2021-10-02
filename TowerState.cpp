#include "TowerState.h"

TowerState::TowerState() {

}

TowerState::~TowerState() {

}

void TowerState::init() {
	floors.resize(1);
	floors[0].generateMap();
}

void TowerState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			floors[currentFloor].createExplosion(getGame()->getCursorPosition() + cameraPosition, 5, 40);
		}
	}
}

void TowerState::update(sf::Time elapsed) {
	// Update floors
	// Todo: maybe only update current floor?
	for (Map &map : floors) {
		map.update(elapsed);
	}

	// Update camera
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		cameraPosition.x -= elapsed.asSeconds() * 60;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		cameraPosition.x += elapsed.asSeconds() * 60;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		cameraPosition.y -= elapsed.asSeconds() * 60;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		cameraPosition.y += elapsed.asSeconds() * 60;
	}
}

void TowerState::render(sf::RenderWindow &window) {
	sf::RenderStates cameraTransform;
	cameraTransform.transform.translate(std::round(-cameraPosition.x), std::round(-cameraPosition.y));

	if (currentFloor >= 0 && currentFloor < floors.size()) {
		window.draw(floors[currentFloor], cameraTransform);
	}
}
