#include "Cruiser.h"

void Cruiser::setTargetPos(std::vector<int> target)
{
    if (_multiTargetEnabled) {
        if (target.size()) {
            _targetPosition.push_back(target.at(0));
            _targetPosition.push_back(target.at(1));
        }
    }
    else {
        _targetPosition = target;
    }
}

bool Cruiser::shoot(std::shared_ptr<Ship> targetAtPos)
{
    bool hits = false;

    if (targetAtPos != nullptr) {
        
        bool multipleHits = false;

        do {
            multipleHits = false;

            int hitDice = (rand() % 10) + 1;

            if (targetAtPos->getSize() <= hitDice) {
                multipleHits = true;
                hits = true;

                targetAtPos->setHull(targetAtPos->getHull() - getDamage());
                levelUp();
            }
        } while (multipleHits);
    }

    return hits;
}

void Cruiser::levelUp()
{
    Ship::levelUp();

    switch (getLevel())
    {
    case 3:
        _multiTargetEnabled = true;
    default:
        break;
    }
}
