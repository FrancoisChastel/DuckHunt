#ifndef _DUCKS_BIRD_HPP_
#define _DUCKS_BIRD_HPP_

#include "Constants.hpp"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>

namespace ducks
{

/**
 * Represents a bird with history of movements
 */
class Bird
{
public:
    ///length of the sequence of past actions of the duck
    int getSeqLength() const
    {
        return mSeq.size();
    }

    ///returns the last action of the bird
    EMovement getLastObservation() const
    {
        return getObservation(mSeq.size() - 1);
    }

    ///returns one action in the sequence of actions of the bird
    EMovement getObservation(int i) const
    {
        return mSeq[i];
    }

    ///returns true if the bird is dead
    bool isDead() const
    {
        return getLastObservation() == MOVE_DEAD;
    }

    ///returns true if the duck was dead at time step i
    bool wasDead(int i) const
    {
        return getObservation(i) == MOVE_DEAD;
    }

    ///returns true if the bird is alive
    bool isAlive() const
    {
        return !isDead();
    }

    ///returns true if the duck was alive at time step i
    bool wasAlive(int i) const
    {
        return !wasDead(i);
    }


    std::string printObservations() const
    {
        std::string result = "";
        for (EMovement movement : mSeq)
        {
            switch (movement)
            {
                case MOVE_UP_LEFT :
                    result += "MOVE_UP_LEFT";
                    break;

                case MOVE_UP :
                    result += "MOVE_UP";
                    break;

                case MOVE_UP_RIGHT :
                    result += "MOVE_UP_RIGHT";
                    break;

                case MOVE_LEFT :
                    result += "MOVE_LEFT";
                    break;

                case MOVE_STOPPED :
                    result += "MOVE_STOPPED";
                    break;

                case MOVE_RIGHT :
                    result += "MOVE_RIGHT";
                    break;

                case MOVE_DOWN_LEFT :
                    result += "MOVE_DOWN_LEFT";
                    break;

                case MOVE_DOWN :
                    result += "MOVE_DOWN";
                    break;

                case MOVE_DOWN_RIGHT :
                    result += "MOVE_DOWN_RIGHT";
                    break;

                case MOVE_DEAD :
                    result += "MOVE_DEAD";
                    break;

                default:
                    break;
            }

            result += " / ";
        }
        result += "\r\n";
        return result;
    }

    /**
     * The following methods are used by the Client class
     * You should not use them yourself!
     */
    void addObservation(EMovement pMovement)
    {
        if (!mSeq.empty() && mSeq.back() == MOVE_DEAD)
            mSeq.emplace_back(MOVE_DEAD);
        else
            mSeq.emplace_back(pMovement);
    }

    void kill()
    {
        mSeq.back() = MOVE_DEAD;
    }

private:
    std::vector<EMovement> mSeq;

    friend class Client;
    friend class GameServer;
};

} /*namespace ducks*/

#endif
