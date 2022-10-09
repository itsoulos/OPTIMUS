#ifndef NNCNEURALPROGRAM_H
#define NNCNEURALPROGRAM_H

#include "neuralprogram.h"
# include "dataset.h"

class NNCNeuralProgram : public NeuralProgram
{
private:
    Dataset *train;
    Dataset *test;
public:
    NNCNeuralProgram(Dataset *d1,Dataset *d2);
    double getTrainError();
    double getTestError();
    ~NNCNeuralProgram();
};

#endif // NNCNEURALPROGRAM_H
