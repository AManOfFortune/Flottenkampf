#include "Game.h"
#include "Config.h"
#include <stdexcept>
#include <iostream>
#include <Windows.h>

void Game::startGame()
{
	//Very very terrible, but whatever at least the main game looks cool I guess
	std::cout << "How many teams do you want? (2 - 5)" << std::endl;

	do {
		std::cout << ": ";
		std::cin >> NUM_OF_TEAMS;
	} while (NUM_OF_TEAMS < 2 || NUM_OF_TEAMS > 5);

	std::cout << "How big should the world be? (10 - 50)" << std::endl;

	do {
		std::cout << ": ";
		std::cin >> WORLD_SIZE;
	} while (WORLD_SIZE < 10 || WORLD_SIZE > 50);

	std::cout << "How many ships per Team? (1 - 20)" << std::endl;

	do {
		std::cout << ": ";
		std::cin >> MAX_SHIPS_PER_TEAM;
	} while (MAX_SHIPS_PER_TEAM < 1 || MAX_SHIPS_PER_TEAM > 20);

	setWorld(World());

	initTeams();
	gameLoop();
}

std::shared_ptr<World> Game::getWorld()
{
	return _gameWorld;
}

std::shared_ptr<std::vector<std::vector<std::shared_ptr<Ship>>>> Game::getTeams()
{
	return _teams;
}

int Game::getCurrentTeamIndex()
{
	return _currentTeamIndex;
}

std::shared_ptr<std::vector<int>> Game::getCurrentShipIndex()
{
	return _currentShipIndex;
}

void Game::setWorld(World newWorld)
{
	_gameWorld = std::make_shared<World>(newWorld);
}

void Game::setTeams(std::vector<std::vector<std::shared_ptr<Ship>>> teams)
{
	_teams = std::make_shared<std::vector<std::vector<std::shared_ptr<Ship>>>>(teams);
}

void Game::setCurrentTeamIndex(int currentTeamIndex)
{
	if (currentTeamIndex >= getTeams()->size())
		_currentTeamIndex = 0;
	else if (currentTeamIndex < 0)
		_currentTeamIndex = 0;
	else
		_currentTeamIndex = currentTeamIndex;
}

void Game::setCurrentShipIndex(std::vector<int> currentShipIndex)
{
	_currentShipIndex = std::make_shared<std::vector<int>>(currentShipIndex);
}

std::shared_ptr<Ship> Game::getCurrentShip()
{
	return getTeams()->at(getCurrentTeamIndex()).at(getCurrentShipIndex()->at(getCurrentTeamIndex()));
}

void Game::nextShip()
{
	getCurrentShip()->setMyTurn(false);

	if (getCurrentShipIndex()->at(getCurrentTeamIndex()) + 1 >= getTeams()->at(getCurrentTeamIndex()).size())
		getCurrentShipIndex()->at(getCurrentTeamIndex()) = 0;
	else
		getCurrentShipIndex()->at(getCurrentTeamIndex()) += 1;

	setCurrentTeamIndex(getCurrentTeamIndex() + 1);

	getCurrentShip()->setMyTurn(true);
}

void Game::initTeams()
{
	std::vector<std::vector<std::shared_ptr<Ship>>> teams (NUM_OF_TEAMS, std::vector<std::shared_ptr<Ship>>());

	for (int teamNr = 0; teamNr < teams.size(); teamNr++) {

		for (int shipNr = 0; shipNr < MAX_SHIPS_PER_TEAM; shipNr++) {

			for (int safety = 0; safety < 100; safety++) {
				char orientation = UP;

				int random = rand() % 4;

				switch (random)
				{
				case 0: orientation = UP; break;
				case 1: orientation = DOWN; break;
				case 2: orientation = LEFT; break;
				case 3: orientation = RIGHT; break;
				}

				int posX = rand() % WORLD_SIZE;
				int posY = rand() % WORLD_SIZE;

				int shipType = rand() % 3;
				
				switch (shipType) {
				case 0:
					teams.at(teamNr).push_back(std::shared_ptr<Ship>(new Cruiser(posX, posY, orientation, teamNr)));
					break;
				case 1:
					teams.at(teamNr).push_back(std::shared_ptr<Ship>(new Destroyer(posX, posY, orientation, teamNr)));
					break;
				case 2:
					teams.at(teamNr).push_back(std::shared_ptr<Ship>(new Hunter(posX, posY, orientation, teamNr)));
					break;
				}

				Ship newShip(10, 8, 10, posX, posY, orientation, teamNr);

				if (getWorld()->positionPossible(newShip)) {
					getWorld()->addShipToMap(teams.at(teamNr).at(shipNr));
					break;
				}
				else {
					teams.at(teamNr).pop_back();
				}
			}
		}
	}

	setTeams(teams);
}

void Game::gameLoop()
{
	setCurrentTeamIndex(getTeams()->size() - 1); //The first ship of the first team will start
	setCurrentShipIndex(std::vector<int>(getTeams()->size()));
	getCurrentShip()->setMyTurn(true);

	while (winningTeamIndex() < 0) {

		nextShip(); //Next ship

		getWorld()->render(getCurrentShip()); //Render to show the new current ship

		int maxShotsPerRound = getCurrentShip()->getMaxShotsPerRound();

		//The smaller a ship is, the more moves it can do
		for (int moves = getCurrentShip()->getSize(); moves < 10; moves++) {

			getWorld()->moveShip(getCurrentShip()); //Movement

			getWorld()->render(getCurrentShip()); //Render to show movement

			//Rotate(only rotates with 10% chance)
			if (rand() % 100 < 10) {
				getWorld()->rotateShip(getCurrentShip());

				getWorld()->render(getCurrentShip()); //Render to show rotation
			}

			//If ship has shots left, a guaranteed target is seen (aim cannons returns true), or its the last move, shoot cannons
			if ((maxShotsPerRound && getWorld()->aimCannons(getCurrentShip())) || (moves == 9 && maxShotsPerRound)) {

				getWorld()->render(getCurrentShip()); //Render to show position ship is about to shoot at

				getWorld()->shootCannons(getCurrentShip()); //Shoot

				getWorld()->render(getCurrentShip()); //Render to show if shot hit or not, and if hit ship died

				getCurrentShip()->setTargetPos(std::vector<int>()); //Reset target vector once shot was rendered

				getWorld()->removeAllDeadShipsFromMap(); //Remove all ships that have died from the map
				removeDeadShipsFromVector(); //Removes all dead ships from the teams-vector

				maxShotsPerRound--; //Decrements the shots counter (once it reaches zero the ship will stop shooting)
			}
		}
	}

	system("cls");
	std::cout << "Team " << winningTeamIndex() + 1 << " won!" << std::endl;
	system("pause");
}

int Game::winningTeamIndex()
{
	int teamsAlive = getTeams()->size();
	int winningTeamIndex = 0;

	for (int teamNr = 0; teamNr < getTeams()->size(); teamNr++) {

		if (getTeams()->at(teamNr).size() <= 0)
			teamsAlive--;
		else
			winningTeamIndex = teamNr;
	}

	if (teamsAlive > 1)
		return -1;
	else
		return winningTeamIndex;
}

void Game::removeDeadShipsFromVector()
{
	for (int teamNr = 0; teamNr < getTeams()->size(); teamNr++) {
		for (int shipNr = 0; shipNr < getTeams()->at(teamNr).size(); shipNr++) {
			if (!getTeams()->at(teamNr).at(shipNr)->isAlive()) {
				getTeams()->at(teamNr).erase(getTeams()->at(teamNr).begin() + shipNr);
				getCurrentShipIndex()->at(teamNr)--;
			}
		}
	}
}
