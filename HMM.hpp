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
        std::vector<std::vector<std::vector<double>>> gamma;
    };

    class HMM {
//----------------------------------------------------------------- PUBLIC

    public:
//------------------------------------------------------- Publics methods
        std::vector<double> optimumSequence(double anObservation, int aDuckIndex)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {

            return {1, 3, 4, 5};
        }

        ModelHolder correctModel(ModelHolder anHolder, std::vector<double> observations)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            unsigned long NUMBER_OF_OBSERVATIONS = observations.size();
            unsigned long NUMBER_OF_STATES = anHolder.A.size();
            unsigned long NUMBER_OF_EMISSIONS = anHolder.B[0].size();

            std::vector<double> c(NUMBER_OF_OBSERVATIONS, 0);

            std::vector<std::vector<double> > alpha(NUMBER_OF_STATES, std::vector<double>(NUMBER_OF_OBSERVATIONS, 0));
            std::vector<std::vector<double> > beta(NUMBER_OF_STATES, std::vector<double>(NUMBER_OF_OBSERVATIONS, 0));
            std::vector<std::vector<double> > gamma(NUMBER_OF_STATES, std::vector<double>(NUMBER_OF_OBSERVATIONS));

            std::vector<std::vector<std::vector<double> > > digamma(NUMBER_OF_STATES,
                                                                    std::vector<std::vector<double> >(NUMBER_OF_STATES,
                                                                                                      std::vector<double>(
                                                                                                              NUMBER_OF_OBSERVATIONS)));
            int i, j;
            unsigned long t;
            double numer, denom, logProb;
            double oldLogProb = -INT32_MAX;
            double maxProb = 0;
            unsigned short int nextMove;
            int iters = 0;
            int maxIters = INT8_MAX;

            while (1) {
                // alpha-pass
                c[0] = 0;

                for (i = 0; i < NUMBER_OF_STATES; i++) {
                    alpha[i][0] = anHolder.Pi[i] * anHolder.B[i][observations[0]];
                    c[0] += alpha[i][0];
                }
                c[0] = 1 / c[0];
                for (i = 0; i < NUMBER_OF_STATES; i++) {
                    alpha[i][0] = c[0] * alpha[i][0];
                }
                for (t = 1; t < NUMBER_OF_OBSERVATIONS; t++) {
                    c[t] = 0;
                    for (i = 0; i < NUMBER_OF_STATES; i++) {
                        alpha[i][t] = 0;
                        for (j = 0; j < NUMBER_OF_STATES; j++) {
                            alpha[i][t] += alpha[j][t - 1] * anHolder.A[j][i];
                        }
                        alpha[i][t] *= anHolder.B[i][observations[t]];
                        c[t] += alpha[i][t];
                    }
                    c[t] = 1 / c[t];
                    for (i = 0; i < NUMBER_OF_STATES; i++) {
                        alpha[i][t] = c[t] * alpha[i][t];
                    }
                }

                //beta-pass
                for (i = 0; i < NUMBER_OF_STATES; i++) {
                    beta[i][NUMBER_OF_OBSERVATIONS - 1] = c[NUMBER_OF_OBSERVATIONS - 1];
                }
                for (t = NUMBER_OF_OBSERVATIONS - 2; t > -1; t--) {
                    for (i = 0; i < NUMBER_OF_STATES; i++) {
                        beta[i][t] = 0;
                        for (j = 0; j < NUMBER_OF_STATES; j++) {
                            beta[i][t] += (anHolder.A[i][j] * anHolder.B[j][observations[t + 1]] * beta[j][t + 1]);
                        }
                        beta[i][t] = c[t] * beta[i][t];
                    }
                }

                // gamma and digamma
                for (t = 0; t < NUMBER_OF_OBSERVATIONS - 1; t++) {
                    denom = 0;
                    for (i = 0; i < NUMBER_OF_STATES; i++) {
                        for (j = 0; j < NUMBER_OF_STATES; j++) {
                            denom = denom + alpha[i][t] * anHolder.A[i][j] * anHolder.B[j][observations[t + 1]] *
                                            beta[j][t + 1];
                        }
                    }
                    for (i = 0; i < NUMBER_OF_STATES; i++) {
                        gamma[i][t] = 0;
                        for (j = 0; j < NUMBER_OF_STATES; j++) {
                            digamma[i][j][t] = (alpha[i][t] * anHolder.A[i][j] * anHolder.B[j][observations[t + 1]] *
                                                beta[j][t + 1]) / denom;
                            gamma[i][t] += digamma[i][j][t];
                        }
                    }
                }

                denom = 0;
                for (i = 0; i < NUMBER_OF_STATES; i++) {
                    denom += alpha[i][NUMBER_OF_OBSERVATIONS - 1];
                }
                for (i = 0; i < NUMBER_OF_STATES; i++) {
                    gamma[i][NUMBER_OF_OBSERVATIONS - 1] = alpha[i][NUMBER_OF_OBSERVATIONS - 1] / denom;
                }


                // re-estimate pi
                for (i = 0; i < NUMBER_OF_STATES; i++) {
                    anHolder.Pi[i] = gamma[i][0];
                }

                // re-estimate A
                for (i = 0; i < NUMBER_OF_STATES; i++) {
                    for (j = 0; j < NUMBER_OF_STATES; j++) {
                        numer = 0;
                        denom = 0;
                        for (t = 0; t < NUMBER_OF_OBSERVATIONS - 1; t++) {
                            numer = numer + digamma[i][j][t];
                            denom = denom + gamma[i][t];
                        }
                        anHolder.A[i][j] = numer / denom;
                    }
                }

                // re-estimate B
                for (i = 0; i < NUMBER_OF_STATES; i++) {
                    for (j = 0; j < NUMBER_OF_EMISSIONS; j++) {
                        numer = 0;
                        denom = 0;
                        for (t = 0; t < NUMBER_OF_OBSERVATIONS; t++) {
                            if (observations[t] == j) {
                                numer = numer + gamma[i][t];
                            }
                            denom = denom + gamma[i][t];
                        }
                        anHolder.B[i][j] = numer / denom;
                    }
                }

                // Compute log[P (O | λ)]
                logProb = 0;
                for (i = 0; i < NUMBER_OF_OBSERVATIONS; i++) {
                    logProb += log(c[i]);
                }
                logProb = (-logProb);

                iters++;

                if (iters < maxIters && logProb > oldLogProb) {
                    oldLogProb = logProb;
                }
                else {
                    break;
                }
            }

            return anHolder;
        }


        ModelHolder correctModelAverage(ModelHolder anHolder, std::vector<std::vector<double>> observations)
        // Parameters :
        //
        // Manual :
        //
        // Contract :
        //
        {
            unsigned long aNumberOfObsVectors = observations.size();
            unsigned long aNumberOfStates = anHolder.A.size();
            unsigned long aNumberOfEmissions = anHolder.B[0].size();
            unsigned long aNumberOfObservations = observations[0].size();

            std::vector<std::vector<std::vector<double> > > At(aNumberOfObsVectors,
                                                               std::vector<std::vector<double> >(aNumberOfStates,
                                                                                                 std::vector<double>(
                                                                                                         aNumberOfStates)));
            std::vector<std::vector<std::vector<double> > > Bt(aNumberOfObsVectors,
                                                               std::vector<std::vector<double> >(aNumberOfStates,
                                                                                                 std::vector<double>(
                                                                                                         aNumberOfEmissions)));
            std::vector<std::vector<double> > pit(aNumberOfObsVectors, std::vector<double>(aNumberOfStates));

            int k;
            PrintMatrix(anHolder.A);
            PrintMatrix(anHolder.B);



            for (k = 0; k < aNumberOfObsVectors; k++) {
                pit[k] = anHolder.Pi;
            }
            for (k = 0; k < aNumberOfObsVectors; k++) {
                At[k] = anHolder.A;
            }


            for (k = 0; k < aNumberOfObsVectors; k++) {
                Bt[k] = anHolder.B;
            }


            std::vector<double> c(aNumberOfObservations, 0);
            std::vector<std::vector<double> > alpha(aNumberOfStates, std::vector<double>(aNumberOfObservations, 0));
            std::vector<std::vector<double> > beta(aNumberOfStates, std::vector<double>(aNumberOfObservations, 0));
            std::vector<std::vector<std::vector<double> > > gamma(aNumberOfObsVectors,
                                                                  std::vector<std::vector<double> >(aNumberOfStates,
                                                                                                    std::vector<double>(
                                                                                                            aNumberOfObservations)));
            std::vector<std::vector<std::vector<double> > > digamma(aNumberOfStates,
                                                                    std::vector<std::vector<double> >(aNumberOfStates,
                                                                                                      std::vector<double>(
                                                                                                              aNumberOfObservations)));
            std::vector<double> P(aNumberOfObsVectors, 0);

            int i, j;
            unsigned long t;
            double numer, denom, logProb;
            double maxProb = 0;
            double oldLogProb;
            unsigned short int nextMove;
            int iters = 0;
            int maxIters = INT8_MAX;
            double W = 0;

            for (k = 0; k < aNumberOfObsVectors; k++) {
                oldLogProb = -INT32_MAX;
                iters = 0;
                while (1) {
                    c[0] = 0;
                    for (i = 0; i < aNumberOfStates; i++) {
                        alpha[i][0] = pit[k][i] * Bt[k][i][observations[k][0]];
                        c[0] += alpha[i][0];
                    }
                    c[0] = 1 / c[0];
                    for (i = 0; i < aNumberOfStates; i++) {
                        alpha[i][0] = c[0] * alpha[i][0];
                    }
                    for (t = 1; t < aNumberOfObservations; t++) {
                        c[t] = 0;
                        for (i = 0; i < aNumberOfStates; i++) {
                            alpha[i][t] = 0;
                            for (j = 0; j < aNumberOfStates; j++) {
                                alpha[i][t] += alpha[j][t - 1] * At[k][j][i];
                            }
                            alpha[i][t] *= Bt[k][i][observations[k][t]];
                            c[t] += alpha[i][t];
                        }
                        c[t] = 1 / c[t];
                        for (i = 0; i < aNumberOfStates; i++) {
                            alpha[i][t] = c[t] * alpha[i][t];
                        }
                    }
                    P[k] = 0;
                    for (i = 0; i < aNumberOfStates; i++) {
                        P[k] += alpha[i][aNumberOfObservations - 1];
                    }


                    for (i = 0; i < aNumberOfStates; i++) {
                        beta[i][aNumberOfObservations - 1] = c[aNumberOfObservations - 1];
                    }
                    for (t = aNumberOfObservations - 2; t > -1; t--) {
                        for (i = 0; i < aNumberOfStates; i++) {
                            beta[i][t] = 0;
                            for (j = 0; j < aNumberOfStates; j++) {
                                beta[i][t] += (At[k][i][j] * Bt[k][j][observations[k][t + 1]] * beta[j][t + 1]);
                            }
                            beta[i][t] = c[t] * beta[i][t];
                        }
                    }

                    for (t = 0; t < aNumberOfObservations - 1; t++) {
                        denom = 0;
                        for (i = 0; i < aNumberOfStates; i++) {
                            for (j = 0; j < aNumberOfStates; j++) {
                                denom = denom +
                                        alpha[i][t] * At[k][i][j] * Bt[k][j][observations[k][t + 1]] * beta[j][t + 1];
                            }
                        }
                        for (i = 0; i < aNumberOfStates; i++) {
                            gamma[k][i][t] = 0;
                            for (j = 0; j < aNumberOfStates; j++) {
                                digamma[i][j][t] = (alpha[i][t] * At[k][i][j] * Bt[k][j][observations[k][t + 1]] *
                                                    beta[j][t + 1]) / denom;
                                gamma[k][i][t] += digamma[i][j][t];
                            }
                        }
                    }

                    denom = 0;
                    for (i = 0; i < aNumberOfStates; i++) {
                        denom += alpha[i][aNumberOfObservations - 1];
                    }
                    for (i = 0; i < aNumberOfStates; i++) {
                        gamma[k][i][aNumberOfObservations - 1] = alpha[i][aNumberOfObservations - 1] / denom;
                    }


                    for (i = 0; i < aNumberOfStates; i++) {
                        pit[k][i] = gamma[k][i][0];
                    }

                    for (i = 0; i < aNumberOfStates; i++) {
                        for (j = 0; j < aNumberOfStates; j++) {
                            numer = 0;
                            denom = 0;
                            for (t = 0; t < aNumberOfObservations - 1; t++) {
                                numer = numer + digamma[i][j][t];
                                denom = denom + gamma[k][i][t];
                            }
                            At[k][i][j] = numer / denom;
                        }
                    }

                    for (i = 0; i < aNumberOfStates; i++) {
                        for (j = 0; j < aNumberOfEmissions; j++) {
                            numer = 0;
                            denom = 0;
                            for (t = 0; t < aNumberOfObservations; t++) {
                                if (observations[k][t] == j) {
                                    numer = numer + gamma[k][i][t];
                                }
                                denom = denom + gamma[k][i][t];
                            }
                            Bt[k][i][j] = numer / denom;
                        }
                    }

                    logProb = 0;
                    for (i = 0; i < aNumberOfObservations; i++) {
                        logProb += log(c[i]);
                    }
                    logProb = (-logProb);
                    iters++;


                    if (iters < maxIters && logProb > oldLogProb) {
                        oldLogProb = logProb;
                    }
                    else {

                        break;
                    }
                }
            }



            for (i = 0; i < aNumberOfStates; i++) {
                for (j = 0; j < aNumberOfStates; j++) {
                    anHolder.A[i][j] = 0;
                    W = 0;
                    for (k = 0; k < aNumberOfObsVectors; k++) {
                        anHolder.A[i][j] += ((1 / P[k]) * At[k][i][j]);
                        W += (1 / P[k]);
                    }
                    anHolder.A[i][j] = anHolder.A[i][j] / W;
                }
            }


            for (i = 0; i < aNumberOfStates; i++) {

                for (j = 0; j < aNumberOfEmissions; j++) {

                    anHolder.B[i][j] = 0;
                    W = 0;
                    for (k = 0; k < aNumberOfObsVectors; k++) {
                        //HERE
                        anHolder.B[i][j] += ((1 / P[k]) * Bt[k][i][j]);
                        W += (1 / P[k]);
                    }
                    anHolder.B[i][j] = anHolder.B[i][j] / W;
                }
            }

            for (i = 0; i < aNumberOfStates; i++) {
                anHolder.Pi[i] = 0;
                W = 0;
                for (k = 0; k < aNumberOfObsVectors; k++) {
                    //cout << pit[k][i] << " ";
                    anHolder.Pi[i] += (1 / P[k]) * pit[k][i];
                    W += (1 / P[k]);
                }
                //cout << "\n";
                anHolder.Pi[i] = anHolder.Pi[i] / W;
            }

            anHolder.gamma = gamma;

            return anHolder;
        }

        void PrintMatrix(std::vector<std::vector<double>> aMatrix)
        {
            for (int theCursorY = 0; theCursorY < aMatrix[0].size(); theCursorY++)
            {
                for (int theCursorX = 0; theCursorX < aMatrix.size(); theCursorX++)
                {
                    std::cerr << " ";
                    std::cerr << aMatrix[theCursorY][theCursorX];
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
