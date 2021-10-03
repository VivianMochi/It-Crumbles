#include "TowerState.h"

#include "Helpers.h"
#include "ResourceManager.h"
#include "Constants.h"

TowerState::TowerState() {

}

TowerState::~TowerState() {

}

void TowerState::init() {
	robot = std::make_shared<Robot>();
	robot->installBrain(std::make_shared<PlayerBrain>());
	robot->setPosition(200, 300);

	int totalFloors = 6;
	sf::Vector2i lastLauncher = sf::Vector2i(19, 29);
	for (int i = 0; i < totalFloors; i++) {
		floors.push_back(std::make_shared<Map>());
		lastLauncher = floors.back()->generateMap(lastLauncher, i);
	}
	changeFloor(0);

	hudBackdrop.setTexture(rm::loadTexture("Resource/Image/HudBackdrop.png"));

	floorText.setTexture(rm::loadTexture("Resource/Image/Font.png"));
	floorText.setPosition(2, 2);
	floorText.setColor(sf::Color::White);

	cameraPosition = robot->getPosition() - sf::Vector2f(getGame()->gameSize / 2);

	music.openFromFile("Resource/Music/TowerOfTheAncients.ogg");
	music.setLoop(true);
	music.setVolume(25);
	//music.play();
}

void TowerState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			//floors[currentFloor].createExplosion(getGame()->getCursorPosition() + cameraPosition, 5, 40);
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

	// Update floor
	floors[currentFloor]->update(elapsed);

	// Change floors
	if (robot->verticalPosition < -200) {
		changeFloor(currentFloor + 1);
	}
	if (robot->dead) {
		changeFloor(currentFloor - 1);
	}

	// Update UI
	floorText.setText("Floor " + std::to_string(currentFloor));

	// Update camera
	sf::Vector2f desiredCameraPosition = (robot->getPosition() + robot->getPosition() + (getGame()->getCursorPosition() + cameraPosition)) / 3.0f - sf::Vector2f(getGame()->gameSize / 2);
	cameraPosition += (desiredCameraPosition - cameraPosition) * elapsed.asSeconds() * 10.0f;
}

void TowerState::render(sf::RenderWindow &window) {
	sf::RenderStates cameraTransform;
	cameraTransform.transform.translate(std::round(-cameraPosition.x), std::round(-cameraPosition.y));

	window.draw(*floors[currentFloor], cameraTransform);

	window.draw(hudBackdrop);
	window.draw(floorText);
}

void TowerState::changeFloor(int floor) {
	bool upward = floor > currentFloor;
	if (floor < 0) {
		floor = 0;
		getGame()->changeState(new TowerState());
	}
	else if (floor >= floors.size()) {
		floor = floors.size() - 1;
	}
	currentFloor = floor;

	floors[currentFloor]->addEntity(robot);
	robot->dead = false;
	robot->falling = false;
	robot->rocketTime = 10;
	robot->velocity = sf::Vector2f(0, 1);
	if (upward) {
		robot->verticalPosition = 0;
		robot->verticalVelocity = -40;
		Block *openBlock = floors[currentFloor]->getBlockAt(robot->getPosition());
		if (openBlock) {
			openBlock->verticalPosition = BLOCK_FALLEN_DEPTH + 1;
			openBlock->easingToPosition = true;
		}
	}
	else {
		robot->verticalPosition = -150;
		robot->verticalVelocity = 80;
	}
}
