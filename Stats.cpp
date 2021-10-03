#include "Stats.h"
#include "Constants.h"

int tokens = 0;
std::map<std::string, float> stats;

void initializeStats() {
	tokens = 0;

	stats["Movespeed"] = 80;
	stats["GroundAcceleration"] = 15;
	stats["AirAcceleration"] = 1;
	stats["RobotGravity"] = GRAVITY; // NOT USED

	stats["BulletCooldown"] = 0.5;
	stats["BulletDamage"] = 3;
	stats["BulletFlightTime"] = 3; // NOT USED

	stats["BombCooldown"] = 4;
	stats["BombFuse"] = 3;
	stats["BombDamage"] = 8;
	stats["BombRadius"] = 30;
	stats["BombRange"] = 50;
}
