#pragma once
#include <vector>
#include <memory>
#include "Ship.h"

class World
{
public:
	//Constructor
	World();

	//Getter & Setter
	void setWorld(std::vector<std::vector<std::shared_ptr<Ship>>> world);
	std::shared_ptr<std::vector<std::vector<std::shared_ptr<Ship>>>> getWorld();

	//Functions
	void initWorld();

	bool positionPossible(Ship movedShip);
	bool isTargetAtPosition(std::shared_ptr<Ship> ship, int x, int y);
	void addShipToMap(std::shared_ptr<Ship> newShip);
	void removeShipFromMap(std::shared_ptr<Ship> removedShip);
	void removeAllDeadShipsFromMap();

	void moveShip(std::shared_ptr<Ship> movingShip);
	void rotateShip(std::shared_ptr<Ship> rotatingShip);
	bool aimCannons(std::shared_ptr<Ship> currentShip);
	void shootCannons(std::shared_ptr<Ship> currentShip);

	void render(std::shared_ptr<Ship> currentShip);

	bool positionIsObserved(std::vector<std::vector<int>> observablePositions, int x, int y);

private:
	//Variables
	std::shared_ptr<std::vector<std::vector<std::shared_ptr<Ship>>>> _world;
};