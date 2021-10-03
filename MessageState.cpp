#include "MessageState.h"

#include "ResourceManager.h"
#include "TowerState.h"

MessageState::MessageState(std::string message) {
	this->message = message;
}

MessageState::~MessageState() {

}

void MessageState::init() {
	messageText.setTexture(rm::loadTexture("Resource/Image/Font.png"));
	messageText.setColor(sf::Color::White);
	messageText.setText(message);
}

void MessageState::gotEvent(sf::Event event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Space) {
			getGame()->changeState(new TowerState());
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		getGame()->changeState(new TowerState());
	}
}

void MessageState::update(sf::Time elapsed) {
	messageText.setPosition(240 / 2 - messageText.getWidth() / 2, 135 / 2 - 4);
}

void MessageState::render(sf::RenderWindow &window) {
	window.draw(messageText);
}
