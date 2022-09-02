# include <QCoreApplication>
# include <Optimus/dllproblem.h>
# include <Optimus/optimizer.h>
# include <QDebug>
# include <QLibrary>
# include <QMap>
# include <QProcessEnvironment>
# include <QDir>
# include <omp.h>
# include "getoptions.h"

QStringList optimizerName;
QJsonObject availableParams;
QProcessEnvironment env;
QMap<QString,Optimizer*> optimizerList;
QVector<QLibrary*> optimizerDll;
QString OptimusPath="";
DllProblem *dllProblem=NULL;
QJsonObject problemParams;
Problem  *mainProblem=NULL;

double avgTrainError=0.0;
double avgTestError=0.0;
double avgClassError=0.0;
double minTrainError=0.0;
double minTestError=0.0;
double minClassError=0.0;
QVector<double> statisticsMinimum;
QVector<int>    statisticsFunctionCalls;
bool  statisticsIsNeural=false;

void printParams()
{
    QStringList keys=availableParams.keys();
    for(int i=0;i<keys.size();i++)
    {
        QString s=keys[i];
        if(s.endsWith("_desc"))
            qDebug()<<QString("--")
                                   +  s.mid(0,s.size()-QString("_desc").size())
                                  <<"\t"<<availableParams[s].toString();
    }
}


void loadProblem()
{
    if(filename.size()!=0 &&  mainProblem==NULL)
    {

        QFile fp(filename);
        if(fp.exists())
        {
          qDebug()<<"PROBLEM: "<<filename<<" loaded.";
          dllProblem=new DllProblem(filename,problemParams);
         dllProblem->setRandomSeed(randomSeed);

          QStringList keys=optimizerList.keys();
          mainProblem=new Problem(dllProblem);

          for(int i=0;i<optimizerList.size();i++)
          {
              optimizerList[keys[i]]->setProblem(mainProblem);
                //collect params values from the list
              QJsonObject optimizeParams=optimizerList[keys[i]]->getParameters();
              QStringList optKeys=optimizeParams.keys();
              for(int j=0;j<optKeys.size();j++)
              {
                  optimizeParams[optKeys[j]]=availableParams[optKeys[j]];
              }
              optimizerList[keys[i]]->setSettings(optimizeParams);
              optimizerList[keys[i]]->setThreads(threads);
          }
        }
        else filename="";
    }
}

void parseCmdLine(QStringList args)
{
    availableParams["threads"]=QString::number(threads);
    availableParams["threads_desc"]="Number of threads for OpenMP";
    availableParams["filename"]=filename;
    availableParams["filename_desc"]="The name of the Problem file.";
    availableParams["random_seed"]=QString::number(randomSeed);
    availableParams["random_seed_desc"]="The random seed generator";
    availableParams["opt_method"]=optMethod;
    availableParams["opt_method_desc"]="The main optimization method.";
    availableParams["iterations"]=QString::number(iterations);
    availableParams["iterations_desc"]="Number of iterations with different seed (30).";
    QString lastParam="";
    QStringList keys=availableParams.keys();

    for(int i=1;i<args.size();i++)
    {
        if(args[i]=="--help")
        {
            printParams();
            exit(EXIT_SUCCESS);
        }
        QStringList sp=args[i].split("=");
        QString param=sp[0];
        QString value=sp[1];
        if(param=="--filename" ||  param=="-f")
        {
              filename=value;
        }
        else
        if(param=="--opt_method" ||param=="-o")
        {
            if(optimizerName.contains(value))
            optMethod=value;
            else qDebug()<<"Not found "<<value<<endl;
        }
        else
        if(param=="--threads" || param=="-t")
        {
            if(value.toInt()<=omp_get_max_threads())
            threads=value.toInt();
            else threads=omp_get_max_threads();
        }
        else
        if(param=="--random_seed" || param=="-r")
        {
            randomSeed=value.toInt();
        }
        else
        if(param=="--iterations" || param=="-i")
        {
            iterations=value.toInt();
        }
        else
        {
            if(param.startsWith("--"))
            {
                param=param.mid(2);
            }
            else
            if(param.startsWith("-"))
                param=param.mid(1);

            if(keys.contains(param))
            {
                availableParams[param]=value;
            }
            else
               {
                problemParams[param]=value;
            }
        }
    }

    qDebug()<<"Problem Params: "<<problemParams;
}

void  loadOptimizers()
{

    QStringList optimizerFile;

  //  optimizerFile<<"libGenetic.so"<<"libMultistart.so"<<"libPso.so"<<"libMinfinder.so"<<"libiPso.so"
//                  <<"libGenmin.so"<<"libPrice.so"<<"libBfgs.so"<<"libMinCenter.so"<<"libTmlsl.so"<<"libde.so"<<"libgende.so";
    optimizerFile<<"libGenetic.so"<<"libMultistart.so"<<"libiPso.so"<<"libMinfinder.so"<<"libPrice.so"<<"libgende.so"<<"libPso.so"<<"libBfgs.so"
	    <<"libPSearch.so"<<"libDAdam.so"<<"libSearch.so";
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

    if(mainProblem!=NULL) delete mainProblem;
    if(dllProblem!=NULL)
    {
        delete dllProblem;
    }
}

void printStatistics()
{
    if(statisticsIsNeural)
    {
        int tries=iterations;
    printf("AVERAGE TRAIN ERROR: %20.10lg\n",avgTrainError/tries);
    printf("AVERAGE TEST  ERROR: %20.10lg\n",avgTestError/tries);
    printf("AVERAGE CLASS ERROR: %20.10lg%%\n",avgClassError/tries);
    printf("MINIMUM TRAIN ERROR: %20.10lg\n",minTrainError);
    printf("MINIMUM TEST  ERROR: %20.10lg\n",minTestError);
    printf("MINIMUM CLASS ERROR: %20.10lg%%\n",minClassError);
    }
    else
    {
        double minimumValue=statisticsMinimum[0];
        double averageFunctionCalls=0.0;
        int minimumTimesFound=0;
        for(int i=0;i<statisticsMinimum.size();i++)
        {
            if(statisticsMinimum[i]<minimumValue)
                minimumValue=statisticsMinimum[i];
            averageFunctionCalls+=statisticsFunctionCalls[i];
            printf("Iteration:%4d Minimum:%20.10lf Function calls: %7d\n",
                   i+1,statisticsMinimum[i],statisticsFunctionCalls[i]);
        }
        for(int i=0;i<statisticsMinimum.size();i++)
        {
            if(fabs(statisticsMinimum[i]-minimumValue)<1e-7)
                minimumTimesFound++;
        }
        averageFunctionCalls/=statisticsMinimum.size();
	//compute std
	double std = 0.0;
	for(int i=0;i<statisticsMinimum.size();i++)
	{
		std+=pow((statisticsFunctionCalls[i]-averageFunctionCalls),2.0);
	}
	std = 1.0/statisticsMinimum.size() * std;
	std = sqrt(std);
        printf("Global minimum: %20.10lf\n",minimumValue);
        printf("Average Calls : %20.2lf\n",averageFunctionCalls);
	printf("Std     Calls : %20.4lf\n",std);
        printf("Global found  : %20.2lf%%\n",minimumTimesFound*100.0/statisticsMinimum.size());
    }
}

int main(int argc, char *argv[])
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

  qDebug()<<"OPTIMUS PATH: "<<OptimusPath;
  loadOptimizers();
  parseCmdLine(app.arguments());
  loadProblem();
  if(filename=="")
  {
      printParams();
      exit(EXIT_FAILURE);
  }

  //run the selected optimizer
  if(!optMethod.isEmpty())
  {


//#pragma omp parallel for num_threads(threads)
    for(int i=1;i<=iterations;i++)
    {

      mainProblem->resetFunctionCalls(problemParams);
      optimizerList[optMethod]->Solve();

      Data bestx=mainProblem->getBestx();
      double besty=mainProblem->getBesty();
      QJsonObject result=mainProblem->done(bestx);
    #pragma omp critical
    {
      avgTrainError+=besty;
      statisticsMinimum<<besty;
      statisticsFunctionCalls<<mainProblem->getFunctionCalls();

      if(!result.isEmpty())
       {
          statisticsIsNeural=true;
           avgTestError+=result["testError"].toDouble();
           avgClassError+=result["classError"].toDouble();

          printf("GENETIC STEP[%4d]=%20.10lg %20.10lg%% ",i,besty,result["classError"].toDouble());
          cout<<"Expr: "<<result["string"].toString().toStdString()<<endl;
       if(i==1 || besty<minTrainError)
       {
           minTrainError=besty;
           minTestError=result["testError"].toDouble();
           minClassError=result["classError"].toDouble();
       }
      }
}
    }
  }
  printStatistics();
  unloadOptimizers();
  return 0;
}
