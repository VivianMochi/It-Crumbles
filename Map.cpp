#include "Map.h"

#include "Helpers.h"

bool blockSortFunction(Block &first, Block &second) {
	return first.position.y < second.position.y;
}

bool entitySortFunction(std::shared_ptr<Entity> &first, std::shared_ptr<Entity> &second) {
	return first->getPosition().y < second->getPosition().y;
}

void Map::update(sf::Time elapsed) {
	// Update blocks
	for (Block &block : blocks) {
		block.update(elapsed);
	}

	// Update entities
	auto entity = entities.begin();
	while (entity != entities.end()) {
		(*entity)->updateBrain(elapsed);
		(*entity)->update(elapsed);

		if ((*entity)->dead) {
			entity = entities.erase(entity);
		}
		else {
			entity++;
		}
	}

	// Add any new entities
	for (std::shared_ptr<Entity> &entity : entitiesToAdd) {
		entities.push_back(entity);
	}
	entitiesToAdd.clear();

	// Order the entities by their y positions
	std::sort(entities.begin(), entities.end(), entitySortFunction);
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	auto currentBlock = blocks.begin();
	auto currentEntity = entities.begin();

	// Render everything in y order
	while (currentBlock != blocks.end() && currentEntity != entities.end()) {
		if (currentBlock->getBack().y <= (*currentEntity)->getPosition().y) {
			target.draw(*currentBlock, states);
			currentBlock++;
		}
		else {
			target.draw(**currentEntity, states);
			currentEntity++;
		}
	}

	// Render any leftovers
	while (currentBlock != blocks.end()) {
		target.draw(*currentBlock, states);
		currentBlock++;
	}
	while (currentEntity != entities.end()) {
		target.draw(**currentEntity, states);
		currentEntity++;
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
		if (vm::magnitude(block.getCenter() - position) <= radius) {
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
		if (block.isBlocking()) {
			if (testBox.intersects(sf::FloatRect(sf::Vector2f(block.position * TILE_SIZE), sf::Vector2f(block.size, block.size)))) {
				return true;
			}
		}
	}
	return false;
}

void Map::addEntity(std::shared_ptr<Entity> entity) {
	entitiesToAdd.push_back(entity);
	entity->map = this;
}

void Map::createExplosion(sf::Vector2f position, float damage, float radius) {
	float rippleSpeed = 150;

	for (Block *block : getBlocksWithinRadius(position, radius)) {
		float distance = vm::magnitude(block->getCenter() - position);
		block->damage(damage, distance / rippleSpeed);
	}

	for (auto &entity : entities) {
		if (vm::magnitude(entity->getPosition() - position) <= radius) {
			sf::Vector2f direction = vm::normalize(entity->getPosition() - position);
			entity->velocity = direction * 100.0f;
			entity->verticalVelocity = -40 - std::rand() % 5;
		}
	}
}
