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
        /**
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
**/

        return std::pair<double,EMovement> (0, intToMovement(0));
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

    void Model::resetSpeciesPredictor()
    // Algorithm :
    //
    {

    }


    void Model::trainSpeciesPredictor(std::map<ESpecies,
            std::vector<std::vector<EMovement>> > classifiedObservations,
                                      std::vector<std::pair<std::vector<double>, ESpecies> > pastModel)
    // Algorithm :
    //
    {
        std::vector<std::pair<std::vector<double>, ESpecies> > theFinalCluster = pastModel;

        for(auto const &theClassifiedObservation : classifiedObservations) {
            std::vector<std::vector<double>> tmpVector;

            for(std::vector<EMovement> theObservation : theClassifiedObservation.second)
            {
                std::vector<int> observation = movementsToInts(theObservation);

                tmpVector.push_back(hmm.buildVectorMovement(9, observation));
            }

            theFinalCluster.push_back(std::make_pair(hmm.normalizeVector(hmm.meanVector(tmpVector)),
                                                     theClassifiedObservation.first));

        }
        //std::cerr << "Number of species after learning : " << theFinalCluster.size() << std::endl;


        this->clusters = theFinalCluster;

    }//----- End of method



    ESpecies Model::guessSpeciesPredictor(std::vector<EMovement> anObservation)
    // Algorithm :
    //
    {
        std::vector<int> observation = movementsToInts(anObservation);

        double bestPrediction = 0.66;
        ESpecies bestSpecies = ESpecies::SPECIES_UNKNOWN;
        std::vector<double> normalizedVector =  hmm.normalizeVector(hmm.buildVectorMovement(9, observation));

        for(std::pair<std::vector<double>, ESpecies> cluster : clusters) {
            double predictionConfidence = hmm.euclidianDistance(cluster.first, normalizedVector);
            if (predictionConfidence <= bestPrediction)
            {
                bestSpecies = cluster.second;
                bestPrediction = predictionConfidence;
            }
        }
        //std::cerr << "My guess is " << bestSpecies << " with confidence " << bestPrediction << std::endl;

        return bestSpecies;
    }//----- End of method


    ModelHolder Model::trainMovementsPredictor(std::vector<EMovement> observations)
    {
        ModelHolder modelHolder;

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


        modelHolder.A = DEFAULT_A;
        modelHolder.B = DEFAULT_B;
        modelHolder.Pi = DEFAULT_PI;

        return hmm.correctModel(modelHolder, movementsToInts(observations));
    }//----- End of method


   EMovement Model::guessMovement(ModelHolder aModel, std::vector<EMovement> anObservation)
   {
       std::vector<int> observation = movementsToInts(anObservation);
       int tmp_prediction = -1;
       std::vector<double> gamma = hmm.findGamma(aModel.A.size(), hmm.AlphaPass1(aModel, observation));
       tmp_prediction = hmm.predictNextMove(aModel, gamma, observation.size());

       if (tmp_prediction != -1)
       {
           return intToMovement(tmp_prediction);
       }
       return EMovement::MOVE_DEAD;
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
            default: theMovement = EMovement::MOVE_DEAD;
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

    std::vector<ducks::EMovement> Model::intsToMovements(std::vector<int> aMovementIndex)
    {
        std::vector<ducks::EMovement> result(aMovementIndex.size());

        for (int i=0; i < aMovementIndex.size(); i++)
        {
            result[i] = intToMovement(aMovementIndex[i]);
        }

        return result;
    }//----- End of method

    std::vector<int> Model::movementsToInts(std::vector<EMovement> aMovementType)
    {
        std::vector<int> result(aMovementType.size());

        for (int i=0; i < aMovementType.size(); i++)
        {
            result[i] = movementToInt(aMovementType[i]);
        }

        return result;
    }//----- End of method
}
