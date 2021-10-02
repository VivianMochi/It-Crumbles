#pragma once

#include <SFML/Graphics.hpp>

namespace vm {
	// Compute the magnitude of the input vector
	inline float magnitude(sf::Vector2f input) {
		return sqrt(input.x * input.x + input.y * input.y);
	}

	// Normalize the input vector
	inline sf::Vector2f normalize(sf::Vector2f input) {
		if (vm::magnitude(input) > .1)
			return input / magnitude(input);
		else
			return sf::Vector2f();
	}

	// Compute the dot product of the given input vectors
	inline float dot(sf::Vector2f left, sf::Vector2f right) {
		return left.x * right.x + left.y * right.y;
	}

	// Project a vector onto another
	inline sf::Vector2f projection(sf::Vector2f left, sf::Vector2f right) {
		// This was the old implementation
		//return right * dot(left, right) / magnitude(right) / magnitude(right);
		return normalize(right) * dot(left, normalize(right));
	}

	// Compute the cross product of the given input vectors
	inline float cross(sf::Vector2f left, sf::Vector2f right) {
		return left.x * right.y - left.y * right.x;
	}

	// Rotate a 2D vector by the given angle in radians
	inline sf::Vector2f rotate(sf::Vector2f input, float angle) {
		sf::Vector2f output;
		output.x = input.x * std::cos(angle) - input.y * std::sin(angle);
		output.y = input.x * std::sin(angle) + input.y * std::cos(angle);
		return output;
	}
}
