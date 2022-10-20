# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <QCoreApplication>
# include <Optimus/dllproblem.h>
# include <Optimus/optimizer.h>
# include <QDebug>
# include <QLibrary>
# include <QMap>
# include <QProcessEnvironment>
# include <QDir>
# include <Optimus/dllproblem.h>
# include <Optimus/optimizer.h>
# include <Optimus/neuralsampler.h>
# include <Optimus/rbfsampler.h>
# include <Optimus/nncsampler.h>
# include "fitoptions.h"
# include "fitproblem.h"
# include <QFile>

QString OptimusPath="";
QProcessEnvironment env;
QMap<QString,Optimizer*> optimizerList;
QVector<QLibrary*> optimizerDll;
QStringList optimizerName;
QJsonObject availableParams;
FitProblem *fitProblem = NULL;
QJsonObject problemParams;
Problem *mainProblem = NULL;
void printOption(QString fullName,
                 QString description,QString value)
{
    QString s=QString("%1 %2 %3 %4").arg(fullName,-27).
            arg(description,-35).arg("Default value:",-15).arg(value,-10);
    qDebug().noquote()<<s;
}

void printOption(QString fullName,
                 QString description,int value)
{
    QString s=QString("%1 %2 %3 %4").arg(fullName,-27).
            arg(description,-35).arg("Default value:",-15).arg(value,-10);
    qDebug().noquote()<<s;
}
bool checkParameter(
                    QString name,QString value,
                    QString fullName,QString shortName,
                    QString &parameter)
{
    if(name == fullName || name == shortName)
    {
        parameter = value;
        return true;
    }
    return false;
}

bool checkParameter(
                    QString name,QString value,
                    QString fullName,QString shortName,
                    int &parameter)
{
    if(name == fullName || name == shortName)
    {
        parameter = value.toInt();
        return true;
    }
    return false;
}

bool checkParameter(
                    QString name,QString value,
                    QString fullName,QString shortName,
                    double &parameter)
{
    if(name == fullName || name == shortName)
    {
        parameter = value.toDouble();
        return true;
    }
    return false;

}
void error(QString message)
{
    qDebug().noquote()<<QString("Fatal Error: %1").arg(message,40)<<endl;
    qApp->exit(0);
    exit(0);
}

void printParams()
{
    qDebug().noquote()<<" PARAMETER LIST"<<"\n"<<"===============";
    printOption("--trainFile","Define the training file to be used",trainName);
    printOption("--testFile","Define the test file to be used",testName);
    printOption("--randomSeed","Random seed",randomSeed);
    printOption("--datapath","Default Data directory",datapath);
    printOption("--model","Default used model",model);
    printOption("--optMethod","Default optimization method",optMethod);
    printOption("--weights","Weights for rbf and mlp",weights);
    printOption("--nncsize","Size of NNC model",nncsize);
    qApp->exit(1);
    exit(1);
}


void  loadOptimizers()
{

    QStringList optimizerFile;

    optimizerFile
                <<"libGenetic.so"<<"libMultistart.so"
                <<"libiPso.so"<<"libPrice.so"<<"libgende.so"
                <<"libPso.so"<<"libBfgs.so"
                <<"libPSearch.so"<<"libTmlsl.so"
                <<"libSearch.so"<<"libgcrs.so"
                <<"libIntegerGenetic.so"<<"libDoubleGenetic.so"
                <<"libNeuralMinimizer.so";

    for(int i=0;i<optimizerFile.size();i++)
    {
        QString name=optimizerFile[i];
        QLibrary *library=new QLibrary(OptimusPath+QDir::separator()+"lib"+QDir::separator()+name);
        if(library->load())
       {
           typedef Optimizer* (*create_TestDLL_lib_fun)(Problem *p);
           create_TestDLL_lib_fun create_TestDLL_lib = (create_TestDLL_lib_fun)library->resolve("createOptimizer");

                  if (create_TestDLL_lib)
                  {
                      Optimizer *myClassInstance = create_TestDLL_lib(NULL);
                      if (myClassInstance)
                      {
                         QString optimizerNameIs=name.mid(3);
                         optimizerNameIs=optimizerNameIs.mid(0,optimizerNameIs.size()-3);
                         qDebug()<<"OPTIMIZER: "<<optimizerNameIs<<" loaded.";
                         optimizerName<<optimizerNameIs;
                         optimizerList[optimizerNameIs]=myClassInstance;
                         QJsonObject optimizerParams=myClassInstance->getParameters();
                         QStringList keys=optimizerParams.keys();
                         for(int j=0;j<keys.size();j++)
                         {

                                availableParams[keys[j]]=optimizerParams[keys[j]].toString();


                         }
                      }
                  }
                  else qDebug()<<"Could not load "<<name;
                   optimizerDll<<library;

       }
    else
        qDebug()<<library->errorString();
    }

}
void    unloadOptimizers()
{
    for(int i=0;i<optimizerDll.size();i++)
    {
        optimizerDll[i]->unload();
        delete optimizerDll[i];
    }

    if(fitProblem!=NULL)
    {
        delete mainProblem;
        delete fitProblem;
    }
}

QStringList methodParameterName;
QStringList methodParameterValue;
void parseCmdLine(QStringList args)
{
    QString lastParam="";
    for(int i=1;i<args.size();i++)
    {

        if(args[i]=="--help") printParams();
        QStringList sp=args[i].split("=");
        QString name = sp[0];
        QString value = sp[1];
        if(value=="")
        {
            error(QString("Param %1 is empty.").arg(value));
        }
        bool b1=checkParameter(name,value,"--datapath","-d",datapath);
        bool b2=checkParameter(name,value,"--trainFile","-f",trainName);
        bool b3=checkParameter(name,value,"--testFile","-t",testName);
        bool b4=checkParameter(name,value,"--randomSeed","-r",randomSeed);
        bool b5=checkParameter(name,value,"--model","-m",model);
        bool b6=checkParameter(name,value,"--optMethod","-o",optMethod);
        bool b7=checkParameter(name,value,"--weights","-w",weights);
        bool b8=checkParameter(name,value,"--nncsize","-n",nncsize);
        bool b9=checkParameter(name,value,"--iterations","-i",iterations);

        if(!b1 && !b2 && !b3 && !b4 && !b5 && !b6 && !b7 && !b8 && !b9)
        {
            methodParameterName.append(name.mid(2));
            methodParameterValue.append(value);
        }

    }

    problemParams["datapath"]=datapath;
    problemParams["trainName"]=datapath+QDir::separator()+trainName;
    problemParams["testName"]=datapath+QDir::separator()+testName;
    problemParams["model"]=model;
    problemParams["weights"]=QString::number(weights);
    problemParams["nncsize"]=QString::number(nncsize);
}


void loadProblem()
{

    fitProblem = new FitProblem(problemParams);
    fitProblem->setRandomSeed(randomSeed);
    mainProblem = new Problem(fitProblem);
    QStringList keys=optimizerList.keys();
    for(int i=0;i<optimizerList.size();i++)
            {
                QJsonObject optimizeParams=optimizerList[keys[i]]->getParameters();
                QStringList optKeys=optimizeParams.keys();
                for(int j=0;j<optKeys.size();j++)
                {
                    if(methodParameterName.contains(optKeys[j]))
                    {
                        int pos = methodParameterName.indexOf(optKeys[j]);
                        optimizeParams[optKeys[j]]=methodParameterValue[pos];

                    }
                }
                optimizerList[keys[i]]->setSettings(optimizeParams);
                optimizerList[keys[i]]->setProblem(mainProblem);
            }
}

int main(int argc,char **argv)
{
    QCoreApplication app(argc,argv);
    env=QProcessEnvironment::systemEnvironment();
    setlocale(LC_ALL,"C");
    OptimusPath=env.value("OPTIMUSPATH");
    if(OptimusPath.isEmpty())
    {
        qDebug()<<"Variable OPTIMUSPATH is not set. Aborting...";
        exit(EXIT_FAILURE);
    }
    else
    {
        qDebug()<<"Start to fit "<<endl;
    }
    parseCmdLine(app.arguments());
    if(datapath.isEmpty() || trainName.isEmpty() || testName.isEmpty()
            || optMethod.isEmpty())
    {
        qDebug()<<"Empty fields. Aborting...";
        exit(EXIT_FAILURE);
    }
    else
    {
        QFile f1(datapath+QDir::separator()+trainName);
        if(!f1.exists())
        {

            qDebug()<<"Can not open train file. Aborting...";
            exit(EXIT_FAILURE);
        }
        QFile f2(datapath+QDir::separator()+testName);
        if(!f2.exists())
        {

            qDebug()<<"Can not open test file. Aborting...";
            exit(EXIT_FAILURE);
        }
        loadOptimizers();
        loadProblem();

        double averageTrainError=  0.0;
        double averageTestError =  0.0;
        double averageClassError = 0.0;
        for(int iter=1;iter<=iterations;iter++)
        {
            optimizerList[optMethod]->Solve();
            Data bestx=mainProblem->getBestx();
            double besty=mainProblem->getBesty();
            QJsonObject result=mainProblem->done(bestx);
            averageTrainError+= result["trainError"].toDouble();
            averageTestError+=  result["testError"].toDouble();
            averageClassError+= result["classError"].toDouble();
        }
        unloadOptimizers();
        printf("Statistics====\n");
        printf("Average Train Error = %20.10lg\n",averageTrainError/iterations);
        printf("Average Test  Error = %20.10lg\n",averageTestError/iterations);
        printf("Average Class Error = %20.10lg%%\n",averageClassError/iterations);
    }
    return 0;
}
