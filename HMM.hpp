/*************************************************************************
                        HMM  -  description
 -------------------
 Begin                : 13/09/2017
 Copyright            : (C) 2017 by Francois
 *************************************************************************/

//---------- Interface of <HMM>'s class (file Model.h) -------------
#if !defined ( HMM_HPP )
#define HMM_HPP

//-------------------------------------------------------- Used interfaces
#include <vector>
#include <math.h>
#include <iostream>
#include <cmath>
#include <map>

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------------
// Class's job <Model>
//
//
//------------------------------------------------------------------------
namespace ducks {

    struct ModelHolder {
        std::vector<std::vector<double>> A;
        std::vector<std::vector<double>> B;
        std::vector<double> Pi;
        std::vector<std::vector<double>> gamma;
    };

    class HMM {
//----------------------------------------------------------------- PUBLIC

    public:
//------------------------------------------------------- Publics methods
        /**
         * Duck Prediction part
         */
        std::vector<double> buildVectorMovement(int aNumberOfEmissions, std::vector<int> observation)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            std::vector<double> result(aNumberOfEmissions, 0);

            for (int cursor=0; cursor < observation.size(); cursor++)
            {
                if (observation[cursor] >= 0)
                {
                    result[observation[cursor]] += 1;
                }
            }
            return result;
        }

        double maxValue(std::vector<double> aVector)
        {
            double bestValue = INT16_MIN;
            for (int value : aVector)
                if (value > bestValue)
                    bestValue = value;
            return bestValue;
        }

        std::vector<double> normalizeVector(std::vector<double> aVector)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {

            std::vector<double> theNormalizedVector(aVector.size(), 0);

            double aMaxValue = maxValue(aVector);

            for (int cursor=0; cursor < aVector.size(); cursor++)
            {
                theNormalizedVector[cursor] = aVector[cursor] / aMaxValue;
            }

            return theNormalizedVector;
        }



        std::vector<double> meanVector(std::vector<std::vector<double>> observations)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            std::vector<double> result(observations[0].size(), 0);

            for (int j=0; j < observations[0].size(); j++)
            {
                for (int i=0; i < observations.size(); i++)
                {
                    result[j] += observations[i][j];
                }
                result[j] = (result[j]/observations.size());
            }

            return result;
        }

        double euclidianDistance(std::vector<double> aVector, std::vector<double> anotherVector)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            double result = 0;

            for (int cursor=0; cursor < aVector.size(); cursor++)
            {
                result += std::pow(aVector[cursor] - anotherVector[cursor], 2);
            }

            if (result == 0)
            {
                printVector(aVector);
                printVector(anotherVector);
                std::cerr << "Euclidean distance : " << result << std::endl;
            }


            return std::sqrt(result);
        }

        void printVector(std::vector<double> aVector)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            std::cerr << "[";
            for (double element : aVector)
            {
                std::cerr << " " << element;
            }
            std::cerr << " ] " << std::endl;
        }
        /**
         * Duck movement part
         */
        // HMMO
        std::pair<double, int> predictNextMove(ModelHolder anHolder, int numberOfObservations)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            std::vector<double> PMove (anHolder.A.size());
            std::vector<double> PStates(anHolder.A.size());

            for (int i = 0; i < anHolder.A.size(); i++)
            {
                PStates[i] = 0;

                for (int j = 0; j < anHolder.A.size(); j++)
                {
                    PStates[i] += anHolder.gamma[j][numberOfObservations-1] * anHolder.A[j][i];
                }
            }

            double maxProb = 0;
            int nextMove = 0;

            for (int i = 0; i < anHolder.B[0].size(); i++)
            {
                PMove[i] = 0;

                for (int j = 0; j < anHolder.A.size(); j++)
                {
                    PMove[i] += PStates[j] * anHolder.B[j][i];
                }

                if (PMove[i] > maxProb)
                {
                    maxProb = PMove[i];
                    nextMove = i;
                }
            }

            return std::pair<double, int> (maxProb, nextMove);
        }

        // HMM2
        std::vector<int> optimumSequence(ModelHolder anHolder, std::vector<int> observations)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            int aNumberOfStates = anHolder.A.size();
            int aNumberOfEmissions = anHolder.B[0].size();
            int aNumberOfObservations = observations.size();

            std::vector <std::vector<double> > delta(aNumberOfStates, std::vector<double> (aNumberOfObservations));
            std::vector <std::vector<int> >state(aNumberOfStates, std::vector<int>(aNumberOfObservations));
            std::vector <double> d1(aNumberOfStates);
            std::vector <int> solution(aNumberOfObservations);

            double s0;

            for(int t=0; t < aNumberOfObservations; t++)
            {
                if(t==0)
                {
                    for(int i=0; i < aNumberOfStates; i++)
                    {
                        delta[i][t]=log(anHolder.Pi[i]*anHolder.B[i][observations[t]]);
                        if(i==0)
                        {
                            s0=delta[i][t];
                            state[i][t]=0;
                        }
                        else
                        {
                            if(delta[i][t]>s0)
                            {
                                s0=delta[i][t];
                                state [i][t]=i;
                            }
                        }

                    }
                }
                else
                {
                    for(int i=0; i<aNumberOfStates; i++)
                    {
                        for(int j=0; j<aNumberOfStates; j++)
                        {
                            d1[j]=(delta[j][t-1]+log(anHolder.A[j][i])+log(anHolder.B[i][observations[t]]));
                            if(j==0)
                            {
                                delta[i][t]=d1[j];
                                state[i][t]=j;
                            }
                            else
                            {
                                if (d1[j]>delta[i][t])
                                {
                                    delta[i][t]=d1[j];
                                    state[i][t]=j;
                                }
                            }
                        }
                    }
                }
            }

            solution[aNumberOfObservations-1]=0;
            double mx=delta[0][aNumberOfObservations-1];
            for(int i=1; i<aNumberOfStates; i++)
            {
                if(delta[i][aNumberOfObservations-1]>mx)
                {
                    mx=delta[i][aNumberOfObservations-1];
                    solution[aNumberOfObservations-1]=i;
                }
            }
            for(int t=aNumberOfObservations-2; t>-1; t--)
            {
                solution[t]=state[solution[t+1]][t+1];
            }

            return solution;
        }

        // HMM4
        ModelHolder correctModel(ModelHolder anHolder, std::vector<int> observations)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            int aNumberOfStates = anHolder.A.size();
            int aNumberOfEmissions = anHolder.B[0].size();
            int aNumberOfObservations = observations.size();


            int maxIters = INT8_MAX;
            int iters = 0;
            double oldLogProb = INT16_MIN;
            double numer;
            double logProb;

            double denom = 0;

            std::vector<double> c(aNumberOfObservations, 0);

            std::vector <std::vector <double> > alpha(aNumberOfStates, std::vector<double> (aNumberOfObservations, 0));
            std::vector <std::vector <double> > beta(aNumberOfStates, std::vector<double> (aNumberOfObservations, 0));
            std::vector <std::vector <double> > gamma(aNumberOfStates, std::vector<double> (aNumberOfObservations));
            std::vector <std::vector <std::vector <double> > > digamma(aNumberOfStates, std::vector <std::vector <double> > (aNumberOfStates, std::vector <double> (aNumberOfObservations)));

            while(1)
            {

                // Alpha-pass
                c[0]=0;
                for(int i=0; i<aNumberOfStates; i++)
                {
                    alpha[i][0] = anHolder.Pi[i] * anHolder.B[i][observations[0]];
                    c[0]+=alpha[i][0];
                }
                c[0]=1/c[0];
                for(int i=0; i<aNumberOfStates; i++)
                {
                    alpha[i][0] = c[0] * alpha[i][0];
                }
                for(int t=1; t<aNumberOfObservations; t++)
                {
                    c[t]=0;
                    for(int i=0; i<aNumberOfStates; i++)
                    {
                        alpha[i][t]=0;
                        for(int j=0; j<aNumberOfStates; j++)
                        {
                            alpha[i][t] += alpha[j][t-1]* anHolder.A[j][i];
                        }
                        alpha[i][t] *= anHolder.B[i][observations[t]];
                        c[t]+=alpha[i][t];
                    }
                    c[t]=1/c[t];
                    for(int i=0;i<aNumberOfStates;i++)
                    {
                        alpha[i][t]=c[t]*alpha[i][t];
                    }
                }

                // Beta-Pass;
                for(int i=0;i<aNumberOfStates;i++)
                {
                    beta[i][aNumberOfObservations-1]=c[aNumberOfObservations-1];
                }

                for(int t= aNumberOfObservations-2; t > -1; t--)
                {
                    for(int i=0;i<aNumberOfStates;i++)
                    {
                        beta[i][t]=0;
                        for(int j=0; j<aNumberOfStates; j++)
                        {
                            beta[i][t]+=(anHolder.A[i][j]*anHolder.B[j][observations[t+1]]*beta[j][t+1]);
                        }
                        beta[i][t]=c[t]*beta[i][t];
                    }
                }

                // Gamma di-gamma
                for(int t=0; t<aNumberOfObservations-1; t++)
                {
                    denom=0;
                    for(int i=0; i<aNumberOfStates;i++)
                    {
                        for(int j=0; j<aNumberOfStates; j++)
                        {
                            denom=denom+alpha[i][t]* anHolder.A[i][j] * anHolder.B[j][observations[t+1]]*beta[j][t+1];
                        }
                    }
                    for(int i=0; i<aNumberOfStates; i++)
                    {
                        gamma[i][t]=0;
                        for(int j=0; j<aNumberOfStates; j++)
                        {
                            digamma[i][j][t]=(alpha[i][t]*anHolder.A[i][j]*anHolder.B[j][observations[t+1]]*beta[j][t+1])/denom;
                            gamma[i][t]+=digamma[i][j][t];
                        }
                    }
                }

                denom=0;
                for(int i=0; i<aNumberOfStates; i++)
                {
                    denom+=alpha[i][aNumberOfObservations-1];
                }
                for(int i=0; i<aNumberOfStates; i++)
                {
                    gamma[i][aNumberOfObservations-1]=alpha[i][aNumberOfObservations-1]/denom;
                }

                // New PI
                for(int i=0; i<aNumberOfStates; i++)
                {
                    anHolder.Pi[i] = gamma[i][0];
                }

                // New A
                for(int i=0; i<aNumberOfStates; i++)
                {
                    for(int j=0; j<aNumberOfStates; j++)
                    {
                        numer=0;
                        denom=0;
                        for(int t=0; t<aNumberOfObservations-1; t++)
                        {
                            numer=numer+digamma[i][j][t];
                            denom=denom+gamma[i][t];
                        }
                        anHolder.A[i][j]=numer/denom;
                    }
                }

                // New B
                for(int i=0; i<aNumberOfStates; i++)
                {
                    for(int j=0; j<aNumberOfEmissions; j++)
                    {
                        numer=0;
                        denom=0;
                        for(int t=0; t<aNumberOfObservations; t++)
                        {
                            if(observations[t]==j)
                            {
                                numer=numer+gamma[i][t];
                            }
                            denom=denom+gamma[i][t];
                        }
                        anHolder.B[i][j]=numer/denom;
                    }
                }

                logProb=0;
                for(int i=0; i<aNumberOfObservations; i++)
                {
                    logProb+=log(c[i]);
                }
                logProb=(-logProb);
                iters++;

                if(iters<maxIters && logProb>oldLogProb)
                {
                    oldLogProb=logProb;
                }
                else
                {
                    break;
                }

            }

            anHolder.gamma = gamma;

            return anHolder;
        }


        void PrintMatrix(std::vector<std::vector<double>> aMatrix)
        {
            for (int theCursorX = 0; theCursorX < aMatrix.size(); theCursorX++)
            {
                for (int theCursorY = 0; theCursorY < aMatrix[theCursorX].size(); theCursorY++)
                {
                    std::cerr << " ";
                    std::cerr << aMatrix[theCursorX][theCursorY];
                }
                std::cerr << std::endl;
            }
            std::cerr << " ================================ ";
            std::cerr << std::endl;

        }


//---------------------------------------------------------------- PRIVATE
    protected:
//------------------------------------------------------ Protected methods

    private:
//-------------------------------------------------------- Private methods
        double alpha(ModelHolder aModelHolder,
                     std::vector<double> anObs)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            // Init:
            std::vector<double> theAlpha = multiplyVector(aModelHolder.Pi,
                                                          aModelHolder.B[anObs[0]]);

            // Rec :
            for (int cursor = 1; cursor < anObs.size(); cursor++) {

                std::vector<double> tmpVector(theAlpha.size());

                for (int j = 0; j < aModelHolder.A.size(); j++) {
                    for (int k = 0; k < aModelHolder.A[j].size(); k++) {
                        tmpVector[j] += theAlpha[k] * aModelHolder.A[j][k];
                    }
                }

                theAlpha = multiplyVector(tmpVector, aModelHolder.B[anObs[cursor]]);
            }

            double result = 0;
            for (double theValue : theAlpha) {
                result += theValue;
            }
            return result;
        }


        std::vector<double> multiplyVector(std::vector<double> aVector,
                                           std::vector<double> anotherVector)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            std::vector<double> theVector(aVector.size());

            for (unsigned long theCursor = 0;
                 theCursor < aVector.size();
                 theCursor++) {
                theVector[theCursor] = aVector[theCursor] * anotherVector[theCursor];
            }

            return theVector;
        }
    };

//---------------------------------------- Dependant's types of <Model>
}
#endif // HMM_HPP
