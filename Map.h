#pragma once

#include "Block.h"
#include <SFML/Graphics.hpp>
#include <vector>

bool blockSortFunction(Block &first, Block &second);

class Map : public sf::Drawable {
public:
	void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Map generation
	void generateMap();

	// Map helpers
	Block *getBlockAt(sf::Vector2i position);
	std::vector<Block*> getBlocksWithinBox(sf::Vector2i position, sf::Vector2i size);
	std::vector<Block*> getBlocksWithinRadius(sf::Vector2f position, float radius);
	bool isAreaEmpty(sf::Vector2i position, sf::Vector2i size = sf::Vector2i(2, 2));

	// Interaction
	void createExplosion(sf::Vector2f position, float damage, float radius);

	std::vector<Block> blocks;

private:

};
