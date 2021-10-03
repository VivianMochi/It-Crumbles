#pragma once

#include "State.h"
#include "Map.h"

class TowerState : public State {
public:
	TowerState();
	virtual ~TowerState();

	void init() override;
	void gotEvent(sf::Event event) override;
	void update(sf::Time elapsed) override;
	void render(sf::RenderWindow &window) override;

	sf::Vector2f cameraPosition;

	int currentFloor = 0;
	std::vector<Map> floors;

	std::shared_ptr<Robot> robot;

	sf::Music music;
};

