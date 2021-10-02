#pragma once

#include "Block.h"
#include "Robot.h"
#include "PlayerBrain.h"
#include <SFML/Graphics.hpp>
#include <vector>

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
	bool checkBoxCollision(sf::Vector2f position, sf::Vector2f size);

	// Interaction
	void addEntity(std::shared_ptr<Entity> entity);
	void createExplosion(sf::Vector2f position, float damage, float radius);

	std::vector<Block> blocks;
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Entity>> entitiesToAdd;

private:

};
