#include "Map.h"

#include "Helpers.h"
#include "Smoke.h"
#include "EnemyBrain.h"
#include "Slime.h"

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
			(*entity)->onDeath();
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
	float bigBlockRate = 0.8;
	float wallRate = 0.5;
	float enemyRate = 0.5;
	float damageRate = 0;

	// Generate big blocks
	for (int tries = MAP_SIZE.x * MAP_SIZE.y / 4 * bigBlockRate; tries > 0; tries--) {
		sf::Vector2i position = sf::Vector2i(std::rand() % (MAP_SIZE.x - 1), std::rand() % (MAP_SIZE.y - 1));
		if (isAreaEmpty(position)) {
			blocks.emplace_back(position, true);
		}
	}

	// Fill with small blocks
	for (int y = 0; y < MAP_SIZE.y; y++) {
		for (int x = 0; x < MAP_SIZE.x; x++) {
			if (!getBlockAt(sf::Vector2i(x, y))) {
				blocks.emplace_back(sf::Vector2i(x, y));
			}
		}
	}

	// Make walls
	for (int walls = (MAP_SIZE.x + MAP_SIZE.y) / 2 * wallRate; walls > 0; walls--) {
		sf::Vector2i size = sf::Vector2i(std::rand() % ((MAP_SIZE.x + MAP_SIZE.y) / 4) + 5, std::rand() % 3 + 1);
		if (std::rand() % 2) {
			std::swap(size.x, size.y);
		}
		sf::Vector2i position = sf::Vector2i(std::rand() % (MAP_SIZE.x - size.x + 1), std::rand() % (MAP_SIZE.y - size.y + 1));

		for (Block *block : getBlocksWithinBox(position, size)) {
			block->wall = true;
			block->verticalPosition = -WALL_HEIGHT;
			block->color = sf::Color(150, 150, 150);
		}
	}

	// Order the blocks by their y positions
	std::sort(blocks.begin(), blocks.end(), blockSortFunction);

	// Do initial damage
	for (int i = 0; i < MAP_SIZE.x * MAP_SIZE.y / 20 * damageRate; i++) {
		createExplosion(getEmptySpot(), std::rand() % 10 + 2, std::rand() % 60 + 10);
	}

	// Place down a bunch of enemies
	for (int i = 0; i < MAP_SIZE.x * MAP_SIZE.y / 40 * enemyRate; i++) {
		std::shared_ptr<Slime> slime = std::make_shared<Slime>(getEmptySpot());
		slime->installBrain(std::make_shared<EnemyBrain>());
		addEntity(slime);
	}
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

bool Map::checkBoxCollision(sf::Vector2f position, sf::Vector2f size, bool floors) {
	sf::FloatRect testBox(position, size);
	for (Block &block : blocks) {
		if (block.isBlocking() || (floors && !block.fallen)) {
			if (testBox.intersects(sf::FloatRect(sf::Vector2f(block.position * TILE_SIZE), sf::Vector2f(block.size, block.size)))) {
				return true;
			}
		}
	}
	return false;
}

bool Map::checkBoxCollision(sf::FloatRect hitbox, bool floors) {
	return checkBoxCollision(sf::Vector2f(hitbox.left, hitbox.top), sf::Vector2f(hitbox.width, hitbox.height), floors);
}

std::shared_ptr<Entity> Map::getNearestEnemy(sf::Vector2f position, float range, bool evil) {
	float lastDistance = range;
	std::shared_ptr<Entity> output = nullptr;
	if (range == -1) {
		lastDistance = 1000000;
	}
	for (auto &entity : entities) {
		if (entity->brain && entity->brain->enemy == evil) {
			float thisDistance = vm::magnitude(entity->getPosition() - position);
			if (thisDistance <= lastDistance) {
				lastDistance = thisDistance;
				output = entity;
			}
		}
	}
	return output;
}

sf::Vector2f Map::getEmptySpot() {
	sf::Vector2f output = sf::Vector2f(-100, -100);
	while (!getBlockAt(output) || getBlockAt(output)->wall || getBlockAt(output)->fallen) {
		output = sf::Vector2f((std::rand() % MAP_SIZE.x) * TILE_SIZE + TILE_SIZE / 2, (std::rand() % MAP_SIZE.y) * TILE_SIZE + TILE_SIZE / 2);
	}
	return output;
}

void Map::addEntity(std::shared_ptr<Entity> entity) {
	entitiesToAdd.push_back(entity);
	entity->map = this;
}

void Map::createExplosion(sf::Vector2f position, float damage, float radius) {
	float rippleSpeed = 150;

	// Damage blocks
	for (Block *block : getBlocksWithinRadius(position, radius)) {
		float distance = vm::magnitude(block->getCenter() - position);
		block->dealDamage(damage, distance / rippleSpeed);
	}

	// Damage entities
	for (auto &entity : entities) {
		if (vm::magnitude(entity->getPosition() - position) <= radius) {
			sf::Vector2f direction = vm::normalize(entity->getPosition() - position);
			entity->dealDamage(damage, direction, "Explosion");
			entity->velocity = direction * 100.0f;
			entity->verticalVelocity = -40 - std::rand() % 5;
			entity->rocketTime = 1;
		}
	}

	// Create smoke
	createSplash(position, radius, sf::Color(30, 30, 30));

	// Play sound
	sounds.playSound("Explosion", 100, -1);
}

void Map::createDust(sf::Vector2f position, sf::Vector2f direction) {
	if (getBlockAt(position)) {
		float smokeLifespan = 0.2f + (std::rand() % 20 / 100.0f);
		addEntity(std::make_shared<Smoke>(position, direction * 20.0f, 3, smokeLifespan, 0, getBlockAt(position)->color));
		sounds.playSound("Impact", 50, -1);
	}
}

void Map::createSplash(sf::Vector2f position, float radius, sf::Color color, float fullness) {
	if (color == sf::Color::Transparent && getBlockAt(position)) {
		color = getBlockAt(position)->color;
	}

	if (color != sf::Color::Transparent) {
		for (float angle = 0; angle < 2 * 3.14159; angle += 0.2 / (fullness + 0.01)) {
			sf::Vector2f smokeOffset = vm::rotate(sf::Vector2f(1, 0), angle);
			sf::Vector2f smokeVelocity = smokeOffset * radius * (3.6f + (std::rand() % 40 / 100.0f));
			float smokeLifespan = 0.2f + (std::rand() % 20 / 100.0f);
			addEntity(std::make_shared<Smoke>(position, smokeVelocity, radius / 4.0f, smokeLifespan, 0, color));
		}
		sounds.playSound("Impact", 100, -1);
	}
}
