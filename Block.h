#pragma once

#include <SFML/Graphics.hpp>

const int TILE_SIZE = 10;
const int DEPTH_SIZE = 5;
const int WALL_HEIGHT = 4;

const float CRUMBLE_TIME = 1;

struct damageEvent {
	float amount;
	float delay;
};

class Block : public sf::Drawable {
public:
	Block(sf::Vector2i position = sf::Vector2i(), bool big = false, bool wall = false, int health = 10);

	void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Helpers
	sf::Vector2f getCenter() const;
	sf::Vector2f getBack() const;

	// Checks
	bool isBlocking() const;

	// Interaction
	void dealDamage(float amount, float delay = 0);

	sf::Vector2i position;
	bool big;
	int size;
	bool wall;
	float maxHealth;
	float health;
	sf::Color color = sf::Color::White;
	bool immune = false;
	bool launcher = false;
	bool victory = false;

	float fallCounter = 0;
	bool fallen = false;
	float verticalPosition = 0;
	float verticalVelocity = 0;
	bool easingToPosition = false;

	std::vector<damageEvent> damageEvents;

	sf::Sprite sprite;
};

