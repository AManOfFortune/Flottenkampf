#include <iostream>
#include "World.h"
#include "Config.h"
#include <Windows.h>

World::World()
{
	initWorld();
}

void World::setWorld(std::vector<std::vector<std::shared_ptr<Ship>>> world)
{
	_world = std::make_shared<std::vector<std::vector<std::shared_ptr<Ship>>>>(world);
}

std::shared_ptr<std::vector<std::vector<std::shared_ptr<Ship>>>> World::getWorld()
{
	return _world;
}

void World::initWorld()
{
	//Create an empty world
	std::vector<std::vector<std::shared_ptr<Ship>>> tempWorld (WORLD_SIZE, std::vector <std::shared_ptr<Ship>>(WORLD_SIZE));

	//Fill each spot with a nullptr
	for (int x = 0; x < tempWorld.size(); x++) {
		for (int y = 0; y < tempWorld.at(x).size(); y++) {
			tempWorld.at(x).at(y) = nullptr;
		}
	}

	setWorld(tempWorld);
}

bool World::positionPossible(Ship movedShip)
{
	for (int i = 0; i < movedShip.getSize(); i++) {

		if (getWorld()->at(movedShip.getPosY()).at(movedShip.getPosX()) != nullptr) {
			return false;
		}

		movedShip.moveReversed(movedShip.getOrientation());
	}

	return true;
}

bool World::isTargetAtPosition(std::shared_ptr<Ship> ship, int x, int y)
{
	for (int i = 0; i < ship->getTargetPos().size(); i += 2) {
		if (ship->getTargetPos().at(0 + i) == y && ship->getTargetPos().at(1 + i) == x) {
			return true;
		}
	}

	return false;
}

void World::addShipToMap(std::shared_ptr<Ship> newShip)
{
	//Necessary to move the ship without moving the actual ship
	Ship shipCopy(*newShip);

	for (int i = 0; i < newShip->getSize(); i++) {

		getWorld()->at(shipCopy.getPosY()).at(shipCopy.getPosX()) = newShip;

		shipCopy.moveReversed(newShip->getOrientation());
	}
}

void World::removeShipFromMap(std::shared_ptr<Ship> removedShip)
{
	//Necessary to move the ship without moving the actual ship
	Ship shipCopy(*removedShip);

	for (int i = 0; i < removedShip->getSize(); i++) {

		getWorld()->at(shipCopy.getPosY()).at(shipCopy.getPosX()) = nullptr;

		shipCopy.moveReversed(shipCopy.getOrientation());
	}
}

void World::removeAllDeadShipsFromMap()
{
	for (int y = 0; y < getWorld()->size(); y++) {

		for (int x = 0; x < getWorld()->at(y).size(); x++) {

			if (getWorld()->at(y).at(x) != nullptr && !getWorld()->at(y).at(x)->isAlive())
				removeShipFromMap(getWorld()->at(y).at(x));
		}
	}

}

void World::moveShip(std::shared_ptr<Ship> movingShip)
{
	movingShip->setCurrentAction(MOVE);

	//Removes the moving ship from the map (gets added again at end of function)
	removeShipFromMap(movingShip);

	//Creates a copy of the moving ship so we can test movement without moving the original ship
	Ship shipCopy(*movingShip);

	shipCopy.move(shipCopy.getOrientation());

	//Try to move into the direction the ship is facing
	if (positionPossible(shipCopy)) {
		movingShip->setPosX(shipCopy.getPosX());
		movingShip->setPosY(shipCopy.getPosY());
	}

	//Add ship back to map with updated position
	addShipToMap(movingShip);
}

void World::rotateShip(std::shared_ptr<Ship> rotatingShip)
{
	rotatingShip->setCurrentAction(MOVE);

	removeShipFromMap(rotatingShip);

	Ship shipCopy(*rotatingShip);

	//Try to rotate, if no direction possible don't rotate
	std::vector<char> triedDirections;
	do {
		char orientation = UP;

		int random = rand() % 4;

		switch (random)
		{
		case 0: orientation = UP; break;
		case 1: orientation = DOWN; break;
		case 2: orientation = LEFT; break;
		case 3: orientation = RIGHT; break;
		}

		shipCopy.setOrientation(orientation);

		//Safety stop an endless loop if no direction is possible
		bool duplicate = false;
		for (int i = 0; i < triedDirections.size(); i++) {
			if (triedDirections.at(i) == orientation) {
				duplicate = true;
				break;
			}
		}
		if (!duplicate)
			triedDirections.push_back(orientation);

		if (positionPossible(shipCopy)) {
			rotatingShip->setOrientation(shipCopy.getOrientation());
			break;
		}

	} while (triedDirections.size() < 4);

	//Adds the ship back to the map with the updated rotation
	addShipToMap(rotatingShip);
}

bool World::aimCannons(std::shared_ptr<Ship> currentShip)
{
	currentShip->setTargetPos(std::vector<int>()); //Empty the target position, then aim

	std::vector<std::vector<int>> observablePositions = currentShip->calcObservablePositions();

	currentShip->setCurrentAction(AIM);

	for (int pos = 0; pos < observablePositions.size(); pos++) {
		std::shared_ptr<Ship> shipAtPosition = getWorld()->at(observablePositions.at(pos).at(1)).at(observablePositions.at(pos).at(0));

		if (shipAtPosition != nullptr && shipAtPosition != currentShip && shipAtPosition->getTeamNr() != currentShip->getTeamNr()) {

			std::vector<int> targetPos;
			targetPos.push_back(observablePositions.at(pos).at(0));
			targetPos.push_back(observablePositions.at(pos).at(1));

			currentShip->setTargetPos(targetPos);
			break;
		}
	}

	//Sets a random position as the target if ship does not see enemy ships
	//("Fires a hail mary")
	//This position only gets fired on if the ship cannot move anymore
	if (!currentShip->getTargetPos().size()) {
		int randomX = rand() % WORLD_SIZE;
		int randomY = rand() % WORLD_SIZE;

		std::vector<int> targetPos;
		targetPos.push_back(randomX);
		targetPos.push_back(randomY);

		currentShip->setTargetPos(targetPos);

		return false;
	}

	return true;
}

void World::shootCannons(std::shared_ptr<Ship> currentShip)
{
	for (int i = 0; i < currentShip->getTargetPos().size(); i += 2) {
		std::shared_ptr<Ship> shipAtPosition = getWorld()->at(currentShip->getTargetPos().at(1 + i)).at(currentShip->getTargetPos().at(0 + i)); //Determine what is at the ships target position

		//Shoots and saves if the shot hit or not
		if (currentShip->shoot(shipAtPosition))
			currentShip->setCurrentAction(HIT);
		else
			currentShip->setCurrentAction(MISS);
	}
}

void World::render(std::shared_ptr<Ship> currentShip)
{
	auto observablePositionsOfCurrentShip = currentShip->calcObservablePositions();

	system("cls");

	//Top Border
	std::cout << "+";
	for (int x = 0; x < getWorld()->size(); x++) {
		std::cout << "--";
	}
	std::cout << "+" << std::endl;

	//Main game area
	for (int x = 0; x < getWorld()->size(); x++) {

		std::cout << "|";

		for (int y = 0; y < getWorld()->at(x).size(); y++) {

			//Targetposition of next shot
			if (currentShip->getTargetPos().size() > 0 && currentShip->getCurrentAction() == AIM && isTargetAtPosition(currentShip, x, y))
				std::cout << "\033[103;30m" << "XX" << "\033[;m";
			//Hitting target at position
			else if (currentShip->getTargetPos().size() > 0 && currentShip->getCurrentAction() == HIT && isTargetAtPosition(currentShip, x, y))
				std::cout << "\033[101;30m" << "##" << "\033[;m";
			//Missing target at position
			else if (currentShip->getTargetPos().size() > 0 && currentShip->getCurrentAction() == MISS && isTargetAtPosition(currentShip, x, y))
				std::cout << "\033[100;30m" << "xx" << "\033[;m";
			//Observed, empty position
			else if (positionIsObserved(observablePositionsOfCurrentShip, y, x) && getWorld()->at(x).at(y) == nullptr)
				std::cout << "\033[107;30m" << "  " << "\033[;m";
			//Empty position
			else if (getWorld()->at(x).at(y) == nullptr)
				std::cout << "  ";
			//Dead boat
			else if(!getWorld()->at(x).at(y)->isAlive())
				std::cout << "\033[101;30m" << "##" << "\033[;m";
			//Head position of boat
			else if (y == getWorld()->at(x).at(y)->getPosX() && x == getWorld()->at(x).at(y)->getPosY()) {
				std::cout << "\033[4" << getWorld()->at(x).at(y)->getTeamNr() + 3 << ";30m";
				std::cout << getWorld()->at(x).at(y)->getOrientation() << getWorld()->at(x).at(y)->getOrientation();
				std::cout << "\033[;m";
			}
			//Tail position of boat
			else {
				std::cout << "\033[4" << getWorld()->at(x).at(y)->getTeamNr() + 3 << ";30m";
				std::cout << "  ";
				std::cout << "\033[;m";
			}

		}

		std::cout << "|" << std::endl;
	}

	//Bottom Border
	std::cout << "+";
	for (int x = 0; x < getWorld()->size(); x++) {
		std::cout << "--";
	}
	std::cout << "+" << std::endl;

	Sleep(SPEED);
}

bool World::positionIsObserved(std::vector<std::vector<int>> observablePositions, int x, int y)
{
	for (int i = 0; i < observablePositions.size(); i++) {
		if (observablePositions.at(i).at(0) == x && observablePositions.at(i).at(1) == y)
			return true;
	}
	return false;
}


//
//void World::setColor(int field)
//{
//	switch (field)
//	{
//	case -3:
//	case -2:
//	case -1:
//		std::cout << "\033[101;30m"; //Light Red
//		break;
//	case 0:
//		std::cout << "\033[40;30m"; //Black
//		break;
//	case 1:
//		std::cout << "\033[107;30m"; //White
//		break;
//	case 2:
//		std::cout << "\033[100;30m"; //Grey
//		break;
//	case 3:
//		std::cout << "\033[103;30m"; //Light yellow
//		break;
//	case 4:
//		std::cout << "\033[43;30m"; //Yellow
//		break;
//	case 5:
//		std::cout << "\033[42;30m"; //Green
//		break;
//	case 6:
//		std::cout << "\033[106;30m"; //Light turquoise
//		break;
//	case 7:
//		std::cout << "\033[46;30m"; //Turquoise
//		break;
//	case 8:
//		std::cout << "\033[104;30m"; //Light blue
//		break;
//	case 9:
//		std::cout << "\033[44;30m"; //Blue
//		break;
//	}
//}