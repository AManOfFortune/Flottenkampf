#pragma once
#include "Ship.h"

//SPECIAL:
//  - Gets an additional shot for each level up

class Hunter :
    public Ship
{
    public:
        Hunter(int posX, int posY, char orientation, int teamNr) : Ship(75, 4, 30, posX, posY, orientation, teamNr) { setMaxShotsPerRound(3); };

        bool shoot(std::shared_ptr<Ship> targetAtPos) override;
        void levelUp() override;
};

