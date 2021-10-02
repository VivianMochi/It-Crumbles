#pragma once

#include <SFML/Graphics.hpp>

const int TILE_SIZE = 10;
const int DEPTH_SIZE = 5;

class Block : public sf::Drawable {
public:
	Block(sf::Vector2i position = sf::Vector2i(), bool big = false, bool wall = false, int health = 10);

	void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	sf::Vector2i position;
	bool big;
	int size;
	bool wall;
	float maxHealth;
	float health;

	sf::Sprite sprite;
};

