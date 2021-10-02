#pragma once

#include "Brain.h"

class PlayerBrain : public Brain {
	virtual void update(sf::Time elapsed) override;
};

