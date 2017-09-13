#include "Player.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "float.h"
#include <string>

namespace ducks {

    Player::Player() {
    }

    Action Player::shoot(const GameState &pState, const Deadline &pDue) {
        /*
         * Here you should write your clever algorithms to get the best action.
         * This skeleton never shoots.
         */

        if (reset)
        {
            model.initializeRound(pState.getNumBirds());
            reset = false;
        }



        double bestPrediction = 0.60;
        int bestIndex = -1, index = -1;
        EMovement bestMovement ;

        if (pState.getBird(0).getSeqLength() != 50)
            return cDontShoot;

        for (std::pair <double, EMovement> thePrediction: model.predictMovements(pState))
        {

            if (thePrediction.first > bestPrediction)
            {

                bestIndex = index;
                bestMovement = thePrediction.second;
            }
            index++;
        }

        if (bestIndex == -1)
        {
            return cDontShoot;
        }
        else
        {
            return Action(bestIndex, bestMovement);
        }

        //This line would predict that bird 0 will move right and shoot at it
        //return Action(0, MOVE_RIGHT);
    }

    std::vector <ESpecies> Player::guess(const GameState &pState, const Deadline &pDue) {
        /*
         * theZs
         * Here you should write your clever algorithms to guess the species of each bird.
         * This skeleton makes no guesses, better safe than sorry!
         */

        std::cerr << "GUESS" << std::endl;

        // called each round
        reset = true;
        model.reset();

        std::vector <ESpecies> lGuesses(pState.getNumBirds(), SPECIES_UNKNOWN);
        return lGuesses;
    }

    void Player::hit(const GameState &pState, int pBird, const Deadline &pDue) {
        /*
         * If you hit the bird you are trying to shoot, you will be notified through this function.
         */
        std::cerr << "HIT BIRD!!!" << std::endl;
    }

    void Player::reveal(const GameState &pState, const std::vector <ESpecies> &pSpecies, const Deadline &pDue) {
        /*
         * If you made any guesses, you will find out the true species of those birds in this function.
         */
    }


} /*namespace ducks*/
