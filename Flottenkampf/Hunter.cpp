#include "Hunter.h"

bool Hunter::shoot(std::shared_ptr<Ship> targetAtPos)
{
    bool hits = false;

    if (targetAtPos != nullptr) {
        int hitDice = (rand() % 10) + 1;

        if (targetAtPos->getSize() <= hitDice) {
            hits = true;

            //Crits if hit roll was 9 or 10
            if(hitDice >= 9)
                targetAtPos->setHull(targetAtPos->getHull() - (getDamage() * 2));
            else 
                targetAtPos->setHull(targetAtPos->getHull() - getDamage());

            levelUp();
        }
    }

    return hits;
}

void Hunter::levelUp()
{
    Ship::levelUp();

    setMaxShotsPerRound(getMaxShotsPerRound() + 1);
}
