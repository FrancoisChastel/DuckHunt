/*************************************************************************
                        Model  -  description
 -------------------
 Begin                : 13/09/2017
 copyright            : (C) 2017 par Francois
 *************************************************************************/

//---------- Realization of <Model>'s class (file Model.cpp) --

//---------------------------------------------------------------- INCLUDE

//------------------------------------------------------- System's include
#include <ostream>
#include <iostream>
#include <math.h>

//---------------------------------------------------- Personnal's include
#include "Model.hpp"

//-------------------------------------------------------------- Constants

//-------------------------------------------------------- Class variables

//---------------------------------------------------------- Private types

namespace ducks {
//----------------------------------------------------------------- PUBLIC
//----------------------------------------------------- Friendly's methods


//--------------------------------------------------------- Public methods
    std::pair<double, EMovement> Model::predictMovement(int aBirdIndex)
    // Algorithm :
    //
    {
        hmm.optimumSequence(modelsHolder[0], {1});

        unsigned long NUMBER_OF_STATES = modelsHolder[0].A.size();
        unsigned long NUMBER_OF_EMISSIONS = modelsHolder[0].B[0].size();

        std::vector<double> PStates(NUMBER_OF_STATES);
        std::vector<double> PMove(NUMBER_OF_EMISSIONS);

        double maxProb=0;
        short int nextMove;


        for (int i = 0; i < NUMBER_OF_STATES; i++) {
            PStates[i] = 0;
            for (int j = 0; j < NUMBER_OF_STATES; j++) {
                modelsHolder[aBirdIndex].A[j][i];
                PStates[i] += modelsHolder[aBirdIndex].gamma[j][observations[aBirdIndex].size() - 1] * modelsHolder[aBirdIndex].A[j][i];
            }
        }


        for (short int i = 0; i < NUMBER_OF_EMISSIONS; i++) {
            PMove[i] = 0;

            for (int j = 0; j < NUMBER_OF_STATES; j++) {
                PMove[i] += PStates[j] * modelsHolder[aBirdIndex].B[j][i];
            }

            if (PMove[i] > maxProb) {
                maxProb = PMove[i];
                nextMove = i;
            }
        }


        return std::pair<double,EMovement> (maxProb, intToMovement(nextMove));
    }//----- End of method

    void Model::PrintMatrix(std::vector<std::vector<int>> aMatrix)
    {
        for (int theCursorX = 0; theCursorX < aMatrix.size(); theCursorX++)
        {
            for (int theCursorY = 0; theCursorY < aMatrix.size(); theCursorY++)
            {
                std::cerr << " ";
                std::cerr << aMatrix[theCursorX][theCursorY];
            }
            std::cerr << std::endl;
        }
        std::cerr << " ================================ ";
        std::cerr << std::endl;

    }

    ESpecies Model::predictSpecies(int aBirdIndex)
    // Algorithm :
    //
    {
        //TODO
        return ESpecies::SPECIES_UNKNOWN ;
    }//----- End of method

    void Model::addObservation(EMovement anObservation, int aBirdIndex)
    {
        observations[aBirdIndex].push_back(movementToInt(anObservation));
    }//----- End of method


    //----------------------------------------- Constructors - destructor
    Model::Model()
    // Algorithm :
    //
    {
    #ifdef MAP
        std::cout << "Call of constructor <Model>" << endl;
    #endif

    } //----- End of Model


    Model::~Model()
    // Algorithm :
    //
    {
    #ifdef MAP
    std::cout << "Call of destructor <Model>" << endl;
    #endif
    } //----- End of Model

    std::vector<std::pair<double, EMovement>> Model::predictMovements(const GameState &pState)
    {
        std::vector<std::pair<double, EMovement>> predictions(pState.getNumBirds());
        for (size_t birdIndex = 0; birdIndex < pState.getNumBirds(); birdIndex++)
        {
            addObservation(pState.getBird(birdIndex).getLastObservation(), birdIndex);
        }

        train();

        for (size_t birdIndex = 0; birdIndex < pState.getNumBirds(); birdIndex++)
        {
            //HERE
            predictions[birdIndex] = predictMovement(birdIndex);
        }

        return predictions;
    }

//---------------------------------------------------------------- PRIVATE

//------------------------------------------------------ Protected methods

//-------------------------------------------------------- Private methods
    EMovement Model::intToMovement(int aMovementIndex)
    {
        EMovement theMovement;
        switch (aMovementIndex) {
            case 4 : theMovement = EMovement::MOVE_UP_LEFT;
                break;
            case 0 : theMovement = EMovement::MOVE_UP;
                break;
            case 5 : theMovement = EMovement::MOVE_UP_RIGHT;
                break;
            case 2 : theMovement = EMovement::MOVE_LEFT;
                break;
            case 8 : theMovement = EMovement::MOVE_STOPPED;
                break;
            case 3 : theMovement = EMovement::MOVE_RIGHT;
                break;
            case 6 : theMovement = EMovement::MOVE_DOWN_LEFT;
                break;
            case 1 : theMovement = EMovement::MOVE_DOWN;
                break;
            case 7 : theMovement = EMovement::MOVE_DOWN_RIGHT;
                break;
            default: theMovement = EMovement::COUNT_MOVE;
                break;
        }
        return theMovement;
    }//----- End of method

    int Model::movementToInt(EMovement aMovementType)
    {
        short int theIndexMovement;
        switch (aMovementType)
        {
            case MOVE_UP_LEFT :     theIndexMovement = 4;
                break;
            case MOVE_UP :          theIndexMovement = 0;
                break;
            case MOVE_UP_RIGHT :    theIndexMovement = 5;
                break;
            case MOVE_LEFT :        theIndexMovement = 2;
                break;
            case MOVE_STOPPED :     theIndexMovement = 8;
                break;
            case MOVE_RIGHT :       theIndexMovement = 3;
                break;
            case MOVE_DOWN_LEFT :   theIndexMovement = 6;
                break;
            case MOVE_DOWN :        theIndexMovement = 1;
                break;
            case MOVE_DOWN_RIGHT :  theIndexMovement = 7;
                break;
            case MOVE_DEAD :        theIndexMovement = -1;
                break;
            default:                theIndexMovement = -1;
                break;
        }

        return theIndexMovement;
    }//----- End of method

    void Model::reset()
    {

    }//----- End of method

    void Model::train()
    {
        for (size_t cursor = 0; cursor < observations.size(); cursor++)
        {
            modelsHolder[cursor] = hmm.correctModel(modelsHolder[cursor], observations[cursor]);
        }
    }//----- End of method

    void Model::resetRound()
    {

    }//----- End of method

    void Model::initializeRound(size_t numberOfBirds)
    {
        std::vector<std::vector<double>> DEFAULT_A =
                {{0.6, 0.1, 0.1, 0.1, 0.1},
                 {0.1, 0.6, 0.1, 0.1, 0.1},
                 {0.1, 0.1, 0.6, 0.1, 0.1},
                 {0.1, 0.1, 0.1, 0.6, 0.1},
                 {0.1, 0.1, 0.1, 0.1, 0.6}};

        std::vector<std::vector<double>> DEFAULT_B =
                {{0.125, 0.125, 0.125, 0.125, 0.11, 0.11, 0.11, 0.11, 0.06},
                 {0.02,  0.02,  0.02,  0.02,  0.43, 0.43, 0.02, 0.02, 0.02},
                 {0.02,  0.02,  0.02,  0.02,  0.43, 0.43, 0.02, 0.02, 0.02},
                 {0.2,   0.2,   0.2,   0.2,   0.04, 0.04, 0.04, 0.04, 0.04},
                 {0.125, 0.12,  0.13,  0.12,  0.13, 0.12, 0.13, 0.12, 0.05}};

        std::vector<double> DEFAULT_PI = {0.4, 0.1, 0.3, 0.1, 0.1};

        observations = std::vector<std::vector<int> > (numberOfBirds, std::vector<int>(0));
        std::cerr << "initilize round";
        modelsHolder = std::vector<ModelHolder>(numberOfBirds);

        for (size_t cursor = 0; cursor < observations.size(); cursor++)
        {
            modelsHolder[cursor].A = DEFAULT_A;
            modelsHolder[cursor].B = DEFAULT_B;
            modelsHolder[cursor].Pi = DEFAULT_PI;

        }
    }//----- End of method
}
