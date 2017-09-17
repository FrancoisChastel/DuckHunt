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

        int MINIMUM_STEP = 99-(pState.getNumBirds()*2);
        if (pState.getNumBirds() == 0)
            return cDontShoot;

        if (pState.getBird(0).getSeqLength() < MINIMUM_STEP)
        {
            return cDontShoot;
        }
        else {
            if (pState.getBird(0).getSeqLength() == MINIMUM_STEP) {
                modelsHolder = std::vector<ModelHolder*>(pState.getNumBirds(), NULL);

                for (int i = 0; i < pState.getNumBirds(); i++) {
                    Bird theBird = pState.getBird(i);
                    if (theBird.isAlive()) {
                        std::vector<EMovement> observations = buildVectorOfMovement(theBird);
                        ModelHolder *modelHolder = new ModelHolder();
                        model.trainMovementsPredictor(modelHolder, observations);
                        modelsHolder[i] = modelHolder;
                    }
                }

            }

            for (int i = 0; i < pState.getNumBirds(); i++) {

                Bird theBird = pState.getBird(i);
                if (theBird.isAlive()) {
                    if(lastShot==i)
                    {
                        std::vector<EMovement> observations = buildVectorOfMovement(theBird);
                        ModelHolder *modelHolder = new ModelHolder();
                        model.trainMovementsPredictor(modelHolder, observations);
                        modelsHolder[i] = modelHolder;
                    }
                    std::cerr << "bird: "<< i << "\n";
                    EMovement prediction = model.guessMovement(modelsHolder[i], buildVectorOfMovement(theBird));
                    if (prediction != EMovement::MOVE_DEAD && prediction != EMovement::MOVE_LEFT && prediction != EMovement::MOVE_RIGHT) {
                        lastShot=i;
                        return Action(i, prediction);
                    }
                }
            }
        }
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
                std::vector<EMovement> movements = buildVectorOfMovement(pState.getBird(i));

                lGuesses[i] = model.guessSpeciesPredictor(movements);
                pastGuess = lGuesses;
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
            if (pSpecies[i] != SPECIES_UNKNOWN)
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
