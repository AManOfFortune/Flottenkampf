#pragma once
#include "Ship.h"

//SPECIAL:
//  - Gets every standard leveling up effect twice

class Destroyer :
    public Ship
{
public:
    Destroyer(int posX, int posY, char orientation, int teamNr) : Ship::Ship(150, 6, 60, posX, posY, orientation, teamNr) {};

    bool shoot(std::shared_ptr<Ship> targetAtPos) override;
    void levelUp() override;
};

