#pragma once

#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class State {
public:
	State();
	virtual ~State();

	virtual void init() = 0;
	virtual void gotEvent(sf::Event event) = 0;
	virtual void update(sf::Time elapsed) = 0;
	virtual void render(sf::RenderWindow &window) = 0;

	void setGame(Game *game);
	Game *getGame();

	// This stuff is used by level states
	virtual float getBeatLength();
	virtual void createSmoke(sf::Vector2f position, sf::Vector2f velocity, float lifespan, sf::Color color = sf::Color::White);
	virtual void makeSound(sf::Vector2f position, float volume, bool playerCreated = false);
	virtual bool checkBoxCollision(sf::Vector2f position, int width, int height, bool forceful = false);
	virtual sf::Vector2f checkRayCollision(sf::Vector2f start, sf::Vector2f direction);
	virtual bool isMetal(sf::Vector2f position);

protected:
	Game *game = nullptr;
};

