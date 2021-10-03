#pragma once

#include "Brain.h"

class EnemyBrain : public Brain {
public:
	EnemyBrain();

	virtual void update(sf::Time elapsed) override;

	enum {
		wandering,
		idle,
	} phase = idle;
	float phaseCooldown = 0.2;

	float desiredRange = 30;
	float orbitAngle = 0;
	float orbitSpeed = 0.5;
};

