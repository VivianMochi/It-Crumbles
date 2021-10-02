#include "Map.h"

bool blockSortFunction(Block &first, Block &second) {
	return first.position.y < second.position.y;
}

void Map::update(sf::Time elapsed) {

}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (const Block &block : blocks) {
		target.draw(block, states);
	}
}

void Map::generateMap() {
	sf::Vector2i mapSize = { 40, 40 };

	float bigBlockRate = 0.8;

	// Generate big blocks
	for (int tries = mapSize.x * mapSize.y / 4 * bigBlockRate; tries > 0; tries--) {
		sf::Vector2i position = sf::Vector2i(std::rand() % (mapSize.x - 1), std::rand() % (mapSize.y - 1));
		if (isAreaEmpty(position)) {
			blocks.emplace_back(position, true);
		}
	}

	// Fill with small blocks
	for (int y = 0; y < mapSize.y; y++) {
		for (int x = 0; x < mapSize.x; x++) {
			if (!getBlockAt(sf::Vector2i(x, y))) {
				blocks.emplace_back(sf::Vector2i(x, y));
			}
		}
	}

	// Order the blocks by their y positions
	std::sort(blocks.begin(), blocks.end(), blockSortFunction);
}

Block *Map::getBlockAt(sf::Vector2i position) {
	for (Block &block : blocks) {
		if (block.position == position) {
			return &block;
		}
		else if (block.big) {
			if (block.position + sf::Vector2i(1, 0) == position) {
				return &block;
			}
			else if (block.position + sf::Vector2i(0, 1) == position) {
				return &block;
			}
			else if (block.position + sf::Vector2i(1, 1) == position) {
				return &block;
			}
		}
	}
	return nullptr;
}

bool Map::isAreaEmpty(sf::Vector2i position, sf::Vector2i size) {
	for (int y = position.y; y < position.y + size.y; y++) {
		for (int x = position.x; x < position.x + size.x; x++) {
			if (getBlockAt(sf::Vector2i(x, y))) {
				return false;
			}
		}
	}
	return true;
}
