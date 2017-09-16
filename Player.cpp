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
        /**
        if (!reset)
        {
            model.initializeRound(pState.getNumBirds());
            reset = true;
        }



        double bestPrediction = 0.50;
        int bestIndex = -1, index = -1;
        EMovement bestMovement ;

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
        }**/

        //This line would predict that bird 0 will move right and shoot at it
        return cDontShoot;
    }

    std::vector <ESpecies> Player::guess(const GameState &pState, const Deadline &pDue) {
        /*
         * theZs
         * Here you should write your clever algorithms to guess the species of each bird.
         * This skeleton makes no guesses, better safe than sorry!
         */

        ESpecies DEFAULT_BIRD = SPECIES_PIGEON;

        std::vector <ESpecies> lGuesses(pState.getNumBirds(), DEFAULT_BIRD);

        if(pState.getRound() != 0)
        {
            for (int i=0; i < pState.getNumBirds(); i++)
            {
                //if (pState.getBird(i).isAlive())
                //{
                    std::vector<EMovement> movements = buildVectorOfMovement(pState.getBird(i));

                    lGuesses[i] = model.guessSpeciesPredictor(movements);
                    pastGuess = lGuesses;
                //}
            }
        }
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
        std::map<ESpecies, std::vector<std::vector<EMovement>> > classifiedObservations = pastObservations;

        for (int i = 0; i < pState.getNumBirds(); i++) {
            if (pSpecies[i] != SPECIES_UNKNOWN /**&& pState.getBird(i).isAlive()**/)
            {
                std::vector<EMovement> movements = buildVectorOfMovement(pState.getBird(i));

                if (classifiedObservations.find(pSpecies[i]) == classifiedObservations.end()) {
                    classifiedObservations[pSpecies[i]] = {movements};
                } else {
                    classifiedObservations.find(pSpecies[i])->second.push_back(movements);
                }
            }
        }

        pastObservations = classifiedObservations;
        model.trainSpeciesPredictor(classifiedObservations);

        printGuess(this->pastGuess, pSpecies);

    }

    std::vector<EMovement> Player::buildVectorOfMovement(Bird aBird)
    {
        std::vector<EMovement> result(aBird.getSeqLength());

        for (int i = 0; i < aBird.getSeqLength(); i++) {
            result[i] = aBird.getObservation(i);
        }

        return result;
    }


    void Player::printGuess(std::vector<ESpecies> guess, std::vector<ESpecies> revealed)
    {
        int numberOfDifference = 0;
        int numberOfPrediction = 0;
        for (int i=0; i < guess.size(); i++)
        {
            std::cerr << " Obtain : " << revealed[i] << " but guessed : " << guess[i] << std::endl;

            if (guess[i] != ESpecies::SPECIES_UNKNOWN)
            {
                numberOfPrediction++;
                if (revealed[i] != guess[i] && guess[i] != ESpecies::SPECIES_UNKNOWN)
                {
                    numberOfDifference++;
                }
            }

        }
        std::cerr << numberOfDifference << " bad(s) predictions for " << numberOfPrediction << std::endl;
    }


} /*namespace ducks*/
