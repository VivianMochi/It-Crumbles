#pragma once

#include "State.h"
#include "Map.h"
#include "BitmapText.h"

struct Notification {
	float y = 135;
	std::string text;
	sf::Color color;
	float lifespan = 2;
	float alpha = 255;
};

class TowerState : public State {
public:
	TowerState();
	virtual ~TowerState();

	void init() override;
	void gotEvent(sf::Event event) override;
	void update(sf::Time elapsed) override;
	void render(sf::RenderWindow &window) override;

	void changeFloor(int floor);

	void createNotification(std::string text, sf::Color color);

	sf::Vector2f cameraPosition;

	int currentFloor = -1;
	std::vector<std::shared_ptr<Map>> floors;

	std::shared_ptr<Robot> robot;

	sf::Sprite hudBackdrop;
	sf::Sprite bombIcon;
	sf::Sprite bombBar;
	BitmapText floorText;
	BitmapText tokenText;
	std::vector<Notification> notifications;

	sf::Music music;
};

