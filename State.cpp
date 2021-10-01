#include "State.h"

State::State() {

}

State::~State() {

}

void State::setGame(Game *game) {
	this->game = game;
}

Game *State::getGame() {
	return game;
}

float State::getBeatLength() {
	return 60.0f / 130.0f;
}

void State::createSmoke(sf::Vector2f position, sf::Vector2f velocity, float lifespan, sf::Color color) {

}

void State::makeSound(sf::Vector2f position, float volume, bool playerCreated) {

}

bool State::checkBoxCollision(sf::Vector2f position, int width, int height, bool forceful) {
	return false;
}

sf::Vector2f State::checkRayCollision(sf::Vector2f start, sf::Vector2f direction) {
	return sf::Vector2f();
}

bool State::isMetal(sf::Vector2f position) {
	return false;
}
