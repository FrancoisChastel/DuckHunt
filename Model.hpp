/*************************************************************************
                        Model  -  description
 -------------------
 Begin                : 13/09/2017
 Copyright            : (C) 2017 by Francois
 *************************************************************************/

//---------- Interface of <Model>'s class (file Model.h) -------------
#if !defined ( MODEL_HPP )
#define MODEL_HPP

//-------------------------------------------------------- Used interfaces
#include <vector>
#include "Constants.hpp"
#include "GameState.hpp"
#include "HMM.hpp"


//------------------------------------------------------------- Constantes
namespace ducks {


//------------------------------------------------------------------------
// Class's job <Model>
//
//
//------------------------------------------------------------------------
    class Model {
//----------------------------------------------------------------- PUBLIC

    public:
//------------------------------------------------------- Publics methods
        void addObservation(EMovement anObservation, int aDuckIndex);
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //

        std::vector<std::pair<double, EMovement>> predictMovements(const GameState &pState);
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //

        std::pair<double, ducks::EMovement> predictMovement(int aBirdIndex);
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //

        void trainSpeciesPredictor(std::map<ESpecies, std::vector<std::vector<EMovement >> > classifiedObservations,
                                   std::vector<std::pair<std::vector<double>, ESpecies> > pastModel = {});
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        ESpecies guessSpeciesPredictor(std::vector<EMovement > anObservation);
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        void initializeRound(size_t numberOfBirds);
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        void resetSpeciesPredictor();
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        void PrintMatrix(std::vector<std::vector<int>> aMatrix);


        void reset();
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

//---------------------------------------------- Constructors - destructor
        Model();
        // Manual :
        //
        // Contract :
        //


        virtual ~Model();
        // Manual :
        //
        // Contract :
        //

//---------------------------------------------------------------- PRIVATE
    protected:
//------------------------------------------------------ Protected methods

    private:
//-------------------------------------------------------- Private methods
        ducks::EMovement intToMovement(int aMovementIndex);
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        int movementToInt(EMovement aMovementType);
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        std::vector<ducks::EMovement> intsToMovements(std::vector<int> aMovementIndex);
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        std::vector<int> movementsToInts(std::vector<EMovement> aMovementType);
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        void train();
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        void resetRound();
        // Parameters :
        //      - a bird index that define the bird which needed to be predicted
        // Manual :
        //
        // Contract :
        //

        std::vector<std::pair<std::vector<double>, ESpecies> > clusters;

    protected:
//--------------------------------------------------- Protected attributes

    private:
//----------------------------------------------------- Private attributes
        std::vector<ModelHolder> modelsHolder;
        std::vector<std::vector<int> > observations;
        HMM hmm;

//-------------------------------------------------------- Friends classes

//-------------------------------------------------------- Private classes

//---------------------------------------------------------- Private types


    };

//---------------------------------------- Dependant's types of <Model>

    // MODEL_HPP
}
#endif