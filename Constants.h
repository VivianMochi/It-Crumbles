#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

const float GRAVITY = 60;
const int BLOCK_FALLEN_DEPTH = 10;
const int ENTITY_FALLEN_DEPTH = 30;

const std::vector<sf::Color> floorColors = { sf::Color::White,
	sf::Color(0xB8D8D3FF),
	sf::Color(0xCDD6B8FF),
	sf::Color(0xD6C3B8FF),
	sf::Color(0xD6B8CDFF),
	sf::Color(0xABB2C6FF) };