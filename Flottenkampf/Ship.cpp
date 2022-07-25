#include "Ship.h"
#include "Config.h"
#include <stdexcept>

Ship::Ship(int hull, int size, int damage, int posX, int posY, char orientation, int teamNr)
{
    setHull(hull);
    setSize(size);
    setDamage(damage);

    setPosX(posX);
    setPosY(posY);
    setOrientation(orientation);
    
    setTeamNr(teamNr);
    setMyTurn(false);
    setTargetPos(std::vector<int>());
    setCurrentAction(MOVE);
    setMaxShotsPerRound(2);
    setLevel(1);
    setSightRange(3);
}

int Ship::getTeamNr()
{
    return _teamNr;
}

int Ship::getHull()
{
    return _hull;
}

int Ship::getSize()
{
    return _size;
}

int Ship::getDamage()
{
    return _damage;
}

int Ship::getSightRange()
{
    return _sightRange;
}

int Ship::getLevel()
{
    return _level;
}

int Ship::getMaxShotsPerRound()
{
    return _maxShotsPerRound;
}

char Ship::getOrientation()
{
    return _orientation;
}

int Ship::getPosX()
{
    return _posX;
}

int Ship::getPosY()
{
    return _posY;
}

bool Ship::isMyTurn()
{
    return _myTurn;
}

std::vector<int> Ship::getTargetPos()
{
    return _targetPosition;
}

char Ship::getCurrentAction()
{
    return _currentAction;
}

void Ship::setTeamNr(int teamNr)
{
    _teamNr = teamNr;
}

void Ship::setHull(int hull)
{
    if (hull < 0)
        _hull = 0;
    else
        _hull = hull;
}

void Ship::setSize(int size)
{
    _size = size;
}

void Ship::setDamage(int damage)
{
    _damage = damage;
}

void Ship::setSightRange(int range)
{
    _sightRange = range;
}

void Ship::setLevel(int level)
{
    _level = level;
}

void Ship::setMaxShotsPerRound(int shots)
{
    _maxShotsPerRound = shots;
}

void Ship::setOrientation(char orientation)
{
    _orientation = orientation;
}

bool Ship::setPosX(int posX)
{
    if (posX < 0) {
        _posX = 0;
        return false;
    }
    else if (posX >= WORLD_SIZE) {
        _posX = WORLD_SIZE - 1;
        return false;
    }
    _posX = posX;
    return true;
}

bool Ship::setPosY(int posY)
{
    if (posY < 0) {
        _posY = 0;
        return false;
    }
    else if (posY >= WORLD_SIZE) {
        _posY = WORLD_SIZE - 1;
        return false;
    }
    _posY = posY;
    return true;
}

void Ship::setMyTurn(bool turn)
{
    _myTurn = turn;
}

void Ship::setTargetPos(std::vector<int> target)
{
    _targetPosition = target;
}

void Ship::setCurrentAction(char currentAction)
{
    _currentAction = currentAction;
}

bool Ship::move(char direction)
{
    switch (direction) 
    {
    case UP: return setPosY(getPosY() - 1);
    case DOWN: return setPosY(getPosY() + 1);
    case LEFT: return setPosX(getPosX() + 1);
    case RIGHT: return setPosX(getPosX() - 1);
    }
}

void Ship::moveReversed(char direction)
{
    switch (direction) {
    case UP:
        setPosY(getPosY() + 1);
        break;
    case DOWN:
        setPosY(getPosY() - 1);
        break;
    case LEFT:
        setPosX(getPosX() - 1);
        break;
    case RIGHT:
        setPosX(getPosX() + 1);
        break;
    }
}

bool Ship::shoot(std::shared_ptr<Ship> targetAtPos)
{
    bool hits = false;

    if (targetAtPos != nullptr) {
        int hitDice = (rand() % 10) + 1;

        if (targetAtPos->getSize() <= hitDice) {
            hits = true;
            targetAtPos->setHull(targetAtPos->getHull() - getDamage());
            levelUp();
        }
    }

    return hits;
}

std::vector<std::vector<int>> Ship::calcObservablePositions()
{
    std::vector<std::vector<int>> observablePositions;

    for (int sizeNr = 0; sizeNr < getSize(); sizeNr++) {

        //Create a copy of the ship and move it to the needed position
        Ship posCopy(*this);
        for (int pos = 0; pos < sizeNr; pos++) {
            posCopy.moveReversed(posCopy.getOrientation());
        }

        char leftOfShip = LEFT;
        char rightOfShip = RIGHT;

        switch (posCopy.getOrientation())
        {
        case UP: leftOfShip = LEFT; rightOfShip = RIGHT; break;
        case DOWN: leftOfShip = RIGHT; rightOfShip = LEFT; break;
        case LEFT: leftOfShip = DOWN; rightOfShip = UP; break;
        case RIGHT: leftOfShip = UP; rightOfShip = DOWN; break;
        }

        //Move all the way to the left (viewed from direction of ship)
        int moveLessToRight = 0; //Variable necessary to get correct observable positions on borders of map
        for (int i = 0; i < getSightRange(); i++) {
            if(!posCopy.move(leftOfShip))
                moveLessToRight++;
        }

        //Move right and save each position
        int loopAmount = moveLessToRight;

        for (int i = moveLessToRight; i <= getSightRange() * 2; i++) {

            //Saves positions infront and behind of ship
            if (sizeNr == 0 || sizeNr == getSize() - 1) {

                if (i <= getSightRange())
                    loopAmount++;
                else if (i > getSightRange())
                    loopAmount--;

                Ship infrontBehindCopy(posCopy);

                if (sizeNr == 0) {
                    for (int infrontView = 0; infrontView < loopAmount; infrontView++) {
                        infrontBehindCopy.move(infrontBehindCopy.getOrientation());

                        observablePositions.push_back(std::vector<int>(2));
                        observablePositions.back().at(0) = infrontBehindCopy.getPosX();
                        observablePositions.back().at(1) = infrontBehindCopy.getPosY();
                    }
                }
                else {
                    for (int infrontView = 0; infrontView < loopAmount; infrontView++) {
                        infrontBehindCopy.moveReversed(infrontBehindCopy.getOrientation());

                        observablePositions.push_back(std::vector<int>(2));
                        observablePositions.back().at(0) = infrontBehindCopy.getPosX();
                        observablePositions.back().at(1) = infrontBehindCopy.getPosY();
                    }
                }
            }

            //Saves positions left and right of ship
            observablePositions.push_back(std::vector<int>(2));
            observablePositions.back().at(0) = posCopy.getPosX();
            observablePositions.back().at(1) = posCopy.getPosY();
            posCopy.move(rightOfShip);
        }
    }

    return observablePositions;
}

bool Ship::isAlive()
{
    return getHull() > 0;
}

void Ship::levelUp()
{
    setLevel(getLevel() + 1);
    setDamage(getDamage() + 10); //Increases the damage by 10 for each gained level

    switch (getLevel()) {
    case 2:
        setSightRange(getSightRange() + 1);
        break;
    case 3:
        setMaxShotsPerRound(getMaxShotsPerRound() + 1);
        break;
    default:
        break;
    }
}