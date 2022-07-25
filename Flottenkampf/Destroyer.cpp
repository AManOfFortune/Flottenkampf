#include "Destroyer.h"

bool Destroyer::shoot(std::shared_ptr<Ship> targetAtPos)
{
    bool hits = false;

    if (targetAtPos != nullptr) {
        int hitDice = (rand() % 10) + 1;

        //Targeting easier by 2
        if (targetAtPos->getSize() <= hitDice + 2) {
            hits = true;

            targetAtPos->setHull(targetAtPos->getHull() - getDamage());
            levelUp();
        }
    }

    return hits;
}

void Destroyer::levelUp()
{
    Ship::levelUp();

    setDamage(getDamage() + 10);

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
