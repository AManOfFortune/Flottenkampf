#pragma once
#include "Ship.h"

//SPECIAL:
//  - Level 3: Shoots all previous positions with each shot

class Cruiser :
    public Ship
{
public:
    Cruiser(int posX, int posY, char orientation, int teamNr) : Ship(250, 8, 50, posX, posY, orientation, teamNr) { _multiTargetEnabled = false; };

    void setTargetPos(std::vector<int> target) override;

    bool shoot(std::shared_ptr<Ship> targetAtPos) override;
    void levelUp() override;

private:
    bool _multiTargetEnabled;
};