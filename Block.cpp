#include "Block.h"

#include "ResourceManager.h"

Block::Block(sf::Vector2i position, bool big, bool wall, int health) {
	this->position = position;
	this->big = big;
	this->size = big ? 2 * TILE_SIZE : TILE_SIZE;
	this->wall = wall;
	this->maxHealth = big ? health * 4 : health;
	this->health = this->maxHealth;

	if (big) {
		sprite.setTexture(rm::loadTexture("Resource/Image/BigBlock.png"));
	}
	else {
		sprite.setTexture(rm::loadTexture("Resource/Image/Block.png"));
	}
	sprite.setTextureRect(sf::IntRect(0, 0, size, size + DEPTH_SIZE));
}

void Block::update(sf::Time elapsed) {

}

void Block::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform.translate(sf::Vector2f(position * TILE_SIZE));
	target.draw(sprite, states);
}
