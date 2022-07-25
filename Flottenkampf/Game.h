#pragma once
#include <memory>

#include "Hunter.h"
#include "Destroyer.h"
#include "Cruiser.h"

#include "World.h"

class Game
{
public:

	void startGame();

private:
	std::shared_ptr<World> _gameWorld;
	std::shared_ptr<std::vector<std::vector<std::shared_ptr<Ship>>>> _teams;

	int _currentTeamIndex;
	std::shared_ptr<std::vector<int>> _currentShipIndex;


	//Getters & Setters
	std::shared_ptr<World> getWorld();
	std::shared_ptr<std::vector<std::vector<std::shared_ptr<Ship>>>> getTeams();
	int getCurrentTeamIndex();
	std::shared_ptr<std::vector<int>> getCurrentShipIndex();

	void setWorld(World newWorld);
	void setTeams(std::vector<std::vector<std::shared_ptr<Ship>>> teams);
	void setCurrentTeamIndex(int currentTeamIndex);
	void setCurrentShipIndex(std::vector<int> currentShipIndex);

	//Functions
	std::shared_ptr<Ship> getCurrentShip();
	void nextShip();
	void initTeams();
	void gameLoop();
	int winningTeamIndex();
	void removeDeadShipsFromVector();
};