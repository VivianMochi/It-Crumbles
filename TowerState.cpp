#include "TowerState.h"

#include "Helpers.h"
#include "ResourceManager.h"
#include "Constants.h"
#include "Stats.h"
#include "MessageState.h"

TowerState::TowerState() {

}

TowerState::~TowerState() {

}

void TowerState::init() {
	initializeStats();

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

	bombIcon.setTexture(rm::loadTexture("Resource/Image/BombHud.png"));
	bombIcon.setTextureRect(sf::IntRect(0, 20, 20, 20));
	bombIcon.setPosition(2, 99);
	bombBar.setTexture(rm::loadTexture("Resource/Image/BombHud.png"));
	bombBar.setTextureRect(sf::IntRect(20, 0, 27, 12));
	bombBar.setPosition(bombIcon.getPosition() + sf::Vector2f(20, 8));

	floorText.setTexture(rm::loadTexture("Resource/Image/Font.png"));
	floorText.setPosition(2, 2);
	floorText.setColor(sf::Color::White);

	tokenText.setTexture(rm::loadTexture("Resource/Image/Font.png"));
	tokenText.setColor(sf::Color::White);

	cameraPosition = robot->getPosition() - sf::Vector2f(getGame()->gameSize / 2);

	for (int i = 0; i < 6; i++) {
		music.push_back(std::make_shared<sf::Music>());
		music.back()->openFromFile("Resource/Music/TowerOfTheAncients" + std::to_string(i) + ".ogg");
		music.back()->setLoop(true);
		music.back()->setVolume(0);
		music.back()->play();
	}
}

void TowerState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			//floors[currentFloor].createExplosion(getGame()->getCursorPosition() + cameraPosition, 5, 40);
			//floors[currentFloor].getBlockAt(getGame()->getCursorPosition() + cameraPosition)->damage(1);
		}
	}
	else if (event.type == sf::Event::KeyPressed) {
		bool debugEnabled = false;
		if (debugEnabled) {
			if (event.key.code == sf::Keyboard::Num1) {
				getGame()->changeState(new TowerState());
			}
			if (event.key.code == sf::Keyboard::Num2) {
				changeFloor(currentFloor + 1);
			}
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
	tokenText.setText(std::to_string(tokens) + " Tokens");
	tokenText.setPosition(240 - tokenText.getWidth() - 2, 135 - 10);
	if (robot->bombCooldown > 0) {
		bombBar.setTextureRect(sf::IntRect(20, 12 * std::floor((1 - robot->bombCooldown / stats.at("BombCooldown")) * 4), 27, 12));
	}
	else {
		bombBar.setTextureRect(sf::IntRect(20, 48, 27, 12));
	}

	// Update camera
	sf::Vector2f desiredCameraPosition = (robot->getPosition() + robot->getPosition() + (getGame()->getCursorPosition() + cameraPosition)) / 3.0f - sf::Vector2f(getGame()->gameSize / 2);
	cameraPosition += (desiredCameraPosition - cameraPosition) * elapsed.asSeconds() * 10.0f;

	// Update music
	for (int i = 0; i < 6; i++) {
		float desiredVolume = 0;
		if (currentFloor >= i) {
			desiredVolume = 25;
		}
		music[i]->setVolume(music[i]->getVolume() + (desiredVolume - music[i]->getVolume()) * elapsed.asSeconds() * 2);
	}
}

void TowerState::render(sf::RenderWindow &window) {
	sf::RenderStates cameraTransform;
	cameraTransform.transform.translate(std::round(-cameraPosition.x), std::round(-cameraPosition.y));

	window.draw(*floors[currentFloor], cameraTransform);

	window.draw(hudBackdrop);
	window.draw(floorText);
	window.draw(tokenText);
	window.draw(bombIcon);
	window.draw(bombBar);
}

void TowerState::changeFloor(int floor) {
	bool upward = floor > currentFloor;
	if (floor < 0) {
		floor = 0;
		getGame()->changeState(new MessageState("Death"));
	}
	else if (floor >= floors.size()) {
		floor = floors.size() - 1;
		getGame()->changeState(new MessageState("Victory with " + std::to_string(tokens) + " Tokens"));
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
		robot->plummet();
	}

	int buffer = 25;
	if (robot->getPosition().x < buffer) {
		robot->setPosition(buffer, robot->getPosition().y);
	}
	else if (robot->getPosition().x > 400 - buffer) {
		robot->setPosition(400 - buffer, robot->getPosition().y);
	}
	if (robot->getPosition().y < buffer) {
		robot->setPosition(robot->getPosition().x, buffer);
	}
	else if (robot->getPosition().y > 400 - buffer) {
		robot->setPosition(robot->getPosition().x, 400 - buffer);
	}
}

void TowerState::createNotification(std::string text, sf::Color color) {
	Notification notification;
	notification.text = text;
	notification.color = color;
	notifications.push_back(notification);
}
