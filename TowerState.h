#pragma once

#include "State.h"
#include "Map.h"
#include "BitmapText.h"

class TowerState : public State {
public:
	TowerState();
	virtual ~TowerState();

	void init() override;
	void gotEvent(sf::Event event) override;
	void update(sf::Time elapsed) override;
	void render(sf::RenderWindow &window) override;

	void changeFloor(int floor);

	sf::Vector2f cameraPosition;

	int currentFloor = -1;
	std::vector<std::shared_ptr<Map>> floors;

	std::shared_ptr<Robot> robot;

	sf::Sprite hudBackdrop;
	BitmapText floorText;

	sf::Music music;
};

