#pragma once

#include "State.h"
#include "BitmapText.h"

class MessageState : public State {
public:
	MessageState(std::string message = "");
	virtual ~MessageState();

	void init() override;
	void gotEvent(sf::Event event) override;
	void update(sf::Time elapsed) override;
	void render(sf::RenderWindow &window) override;

	std::string message = "";
	BitmapText messageText;
};

