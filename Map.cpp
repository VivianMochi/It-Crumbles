#include "Map.h"

bool blockSortFunction(Block &first, Block &second) {
	return first.position.y < second.position.y;
}

bool entitySortFunction(std::shared_ptr<Entity> &first, std::shared_ptr<Entity> &second) {
	return first->getPosition().y < second->getPosition().y;
}

void Map::update(sf::Time elapsed) {
	for (Block &block : blocks) {
		block.update(elapsed);
	}

	for (std::shared_ptr<Entity> &entity: entities) {
		entity->updateBrain(elapsed);
		entity->update(elapsed);
		entity->updatePosition(elapsed, this);
	}

	// Order the entities by their y positions
	std::sort(entities.begin(), entities.end(), entitySortFunction);
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (const Block &block : blocks) {
		target.draw(block, states);
	}

	for (const std::shared_ptr<Entity> &entity : entities) {
		target.draw(*entity, states);
	}
}

void Map::generateMap() {
	sf::Vector2i mapSize = { 40, 40 };

	float bigBlockRate = 0.8;
	float wallRate = 0.5;

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

	// Make walls
	for (int walls = (mapSize.x + mapSize.y) / 2 * wallRate; walls > 0; walls--) {
		sf::Vector2i size = sf::Vector2i(std::rand() % ((mapSize.x + mapSize.y) / 4) + 5, std::rand() % 3 + 1);
		if (std::rand() % 2) {
			std::swap(size.x, size.y);
		}
		sf::Vector2i position = sf::Vector2i(std::rand() % (mapSize.x - size.x + 1), std::rand() % (mapSize.y - size.y + 1));

		for (Block *block : getBlocksWithinBox(position, size)) {
			block->wall = true;
			block->verticalPosition = -WALL_HEIGHT;
			block->color = sf::Color(150, 150, 150);
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

Block *Map::getBlockAt(sf::Vector2f position) {
	return getBlockAt(sf::Vector2i(position.x / TILE_SIZE, position.y / TILE_SIZE));
}

std::vector<Block*> Map::getBlocksWithinBox(sf::Vector2i position, sf::Vector2i size) {
	std::vector<Block*> output;
	for (int y = position.y; y < position.y + size.y; y++) {
		for (int x = position.x; x < position.x + size.x; x++) {
			Block *block = getBlockAt(sf::Vector2i(x, y));
			if (block && std::find(output.begin(), output.end(), block) == output.end()) {
				output.push_back(block);
			}
		}
	}
	return output;
}

std::vector<Block*> Map::getBlocksWithinRadius(sf::Vector2f position, float radius) {
	std::vector<Block*> output;
	for (Block &block : blocks) {
		float xOffset = block.getCenter().x - position.x;
		float yOffset = block.getCenter().y - position.y;
		if (std::sqrt(xOffset * xOffset + yOffset * yOffset) <= radius) {
			output.push_back(&block);
		}
	}
	return output;
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

bool Map::checkBoxCollision(sf::Vector2f position, sf::Vector2f size) {
	sf::FloatRect testBox(position, size);
	for (Block &block : blocks) {
		if (block.wall && block.verticalPosition <= WALL_HEIGHT / 2) {
			if (testBox.intersects(sf::FloatRect(sf::Vector2f(block.position * TILE_SIZE), sf::Vector2f(block.size, block.size)))) {
				return true;
			}
		}
	}
	return false;
}

void Map::createExplosion(sf::Vector2f position, float damage, float radius) {
	float rippleSpeed = 150;

	for (Block *block : getBlocksWithinRadius(position, radius)) {
		float xOffset = block->getCenter().x - position.x;
		float yOffset = block->getCenter().y - position.y;
		float distance = std::sqrt(xOffset * xOffset + yOffset * yOffset);
		block->damage(damage, distance / rippleSpeed);
	}
}
