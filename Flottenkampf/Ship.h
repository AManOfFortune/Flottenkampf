#pragma once
#include <vector>
#include <memory>

class Ship
{
public:
	Ship(int hull, int size, int damage, int posX, int posY, char orientation, int teamNr);

	//Getters & Setters
	int getTeamNr();
	int getHull();
	int getSize();
	int getDamage();
	int getSightRange();
	int getLevel();
	int getMaxShotsPerRound();
	char getOrientation();
	int getPosX();
	int getPosY();
	bool isMyTurn();
	std::vector<int> getTargetPos();
	char getCurrentAction();

	void setTeamNr(int teamNr);
	void setHull(int hull);
	void setSize(int size);
	void setDamage(int damage);
	void setSightRange(int range);
	void setLevel(int level);
	void setMaxShotsPerRound(int shots);
	void setOrientation(char orientation);
	bool setPosX(int posX);
	bool setPosY(int posY);
	void setMyTurn(bool turn);
	virtual void setTargetPos(std::vector<int> target);
	void setCurrentAction(char currentAction);

	//Functions
	bool move(char direction);
	void moveReversed(char direction);
	virtual bool shoot(std::shared_ptr<Ship> targetAtPos);
	std::vector<std::vector<int>> calcObservablePositions();
	bool isAlive();
	virtual void levelUp();

	std::vector<int> _targetPosition;

private:
	int _teamNr;
	int _hull;
	int _size;
	int _damage;
	int _sightRange;

	int _level;

	int _maxShotsPerRound;

	char _orientation;
	int _posX;
	int _posY;

	bool _myTurn;
	char _currentAction;
};