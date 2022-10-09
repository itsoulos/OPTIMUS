#include "nncneuralprogram.h"

NNCNeuralProgram::NNCNeuralProgram(Dataset *d1,Dataset *d2)
{
    setDimension(d1->getdimension());
    train = d1;
    test = d2;
}

double NNCNeuralProgram::getTrainError()
{
    double sum = 0.0;
    for(int i=0;i<train->getpatterns();i++)
    {
        Data xpoint = train->getx(i);
        double v = neuralparser->eval(xpoint);
        sum+=(v-train->gety(i))*(v-train->gety(i));
    }
    return sum;
}

double NNCNeuralProgram::getTestError()
{
    double sum = 0.0;
    for(int i=0;i<test->getpatterns();i++)
    {
        Data xpoint = test->getx(i);
        double v = neuralparser->eval(xpoint);
        sum+=(v-test->gety(i))*(v-test->gety(i));
    }
    return sum;
}


NNCNeuralProgram::~NNCNeuralProgram()
{

}
