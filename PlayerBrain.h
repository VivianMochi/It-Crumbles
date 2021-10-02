#pragma once

#include "Brain.h"

class PlayerBrain : public Brain {
public:
	PlayerBrain();

	virtual void update(sf::Time elapsed) override;
};

