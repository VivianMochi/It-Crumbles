#pragma once

#include "Block.h"
#include "Robot.h"
#include "PlayerBrain.h"
#include "SoundManager.h"
#include <SFML/Graphics.hpp>
#include <vector>

const sf::Vector2i MAP_SIZE = { 40, 40 };

bool blockSortFunction(Block &first, Block &second);
bool entitySortFunction(std::shared_ptr<Entity> &first, std::shared_ptr<Entity> &second);

class Map : public sf::Drawable {
public:
	void update(sf::Time elapsed);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	// Map generation
	void generateMap();

	// Map helpers
	Block *getBlockAt(sf::Vector2i position);
	Block *getBlockAt(sf::Vector2f position);
	std::vector<Block*> getBlocksWithinBox(sf::Vector2i position, sf::Vector2i size);
	std::vector<Block*> getBlocksWithinRadius(sf::Vector2f position, float radius);
	bool isAreaEmpty(sf::Vector2i position, sf::Vector2i size = sf::Vector2i(2, 2));
	bool checkBoxCollision(sf::Vector2f position, sf::Vector2f size, bool floors = false);
	bool checkBoxCollision(sf::FloatRect hitbox, bool floors = false);
	std::shared_ptr<Entity> getNearestEnemy(sf::Vector2f position, float range = -1, bool evil = false);
	sf::Vector2f getEmptySpot();

	// Interaction
	void addEntity(std::shared_ptr<Entity> entity);
	void createExplosion(sf::Vector2f position, float damage, float radius);
	void createDust(sf::Vector2f position, sf::Vector2f direction);
	void createSplash(sf::Vector2f position, float radius, sf::Color color = sf::Color::Transparent, float fullness = 1);

	std::vector<Block> blocks;
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Entity>> entitiesToAdd;

	SoundManager sounds;

private:

};
