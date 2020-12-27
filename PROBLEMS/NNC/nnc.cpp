#include "nnc.h"
# include "interval.h"
# include "omp.h"
# include "tolmin.h"
# include "converter.h"
# include "gensolver.h"
extern "C"
{
    //parameters
    const int maxthreads=64;
    double leftMargin=0.0;
    double rightMargin=255.0;
    QString trainfile="";
    QString testfile="";
    QString urlpath="http://itsoulos.teiep.gr/genclass/";
    //QString urlpath="https://app-1525680166.000webhostapp.com/";
    int chromosomeSize=200;
    //end of parameters
    //names: leftmargin, rightmargin, trainfile, testfile, chromosomesize

    int populationCount=0;
    int dimension=1;
    vector<Data> trainx;
    Data trainy;
    vector<Data> testx;
    Data testy;
    Data dclass;
    int dcount=0;
    vector<NNCNeuralProgram> program;

    QByteArray readNetworkFile(QString file)
    {
        QEventLoop loop;
        QUrl serviceUrl = QUrl(file);
        QNetworkAccessManager *networkManager = new QNetworkAccessManager();
        QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
        QNetworkRequest networkRequest(serviceUrl);
        networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"text/plain");
        QNetworkReply *rep=networkManager->get(networkRequest);

        new QReplyTimeout(rep, 30000);

        loop.exec();//hangs
        QByteArray bts = rep->readAll();
        return bts;
    }

    QByteArray readLocalFile(QString file)
    {
       //if file starts with file://
            setlocale(LC_ALL,"C");
        file=file.mid(QString("file://").size());
        QFile fp(file);
       fp.open(QIODevice::ReadOnly|QIODevice::Text);
       QByteArray st=fp.readAll();
       fp.close();
       return st;
    }
    void loadTrain()
    {
       QFile fp(trainfile);
       fp.open(QIODevice::ReadOnly|QIODevice::Text);
       QTextStream st(&fp);
       st>>dimension;
       int count;
       st>>count;
       trainx.resize(count);
       trainy.resize(count);
       for(int i=0;i<count;i++)
       {
           trainx[i].resize(dimension);
           for(int j=0;j<dimension;j++)
           {
               st>>trainx[i][j];
           }
           st>>trainy[i];
           bool found=false;
           for(int j=0;j<dclass.size();j++)
           {
               if(fabs(dclass[j]-trainy[i])<1e-7)
               {
                   found=true;
                   break;
               }

           }
           if(!found)
               dclass.push_back(trainy[i]);
       }
       fp.close();
    }



    void loadTest()
    {
        QFile fp(testfile);
        fp.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream st(&fp);
        st>>dimension;
        int count;
        st>>count;
        testx.resize(count);
        testy.resize(count);
        for(int i=0;i<count;i++)
        {
            testx[i].resize(dimension);
            for(int j=0;j<dimension;j++)
            {
                st>>testx[i][j];
            }
            st>>testy[i];
        }
        fp.close();
    }

    void    init(QJsonObject obj)
    {
      setlocale(LC_ALL,"C");
        if(obj.contains("urlpath")) urlpath=obj["urlpath"].toString();
        if(!obj.contains("trainName") || !obj.contains("testName"))
        {
            trainfile="wine.train";
            testfile="wine.test";
        }
            else
        {
            trainfile=obj["trainName"].toString();
            testfile=obj["testName"].toString();
        }
        if(obj.contains("leftmargin"))
            leftMargin=obj["leftmargin"].toString().toDouble();
        if(obj.contains("rightmargin"))
            rightMargin=obj["rightmargin"].toString().toDouble();
        if(obj.contains("chromosomesize"))
            chromosomeSize=obj["chromosomesize"].toString().toInt();

        loadTrain();
        loadTest();
        program.resize(maxthreads);
        for(int i=0;i<maxthreads;i++)
        {
            program[i].setAll(dimension,trainx,trainy,testx,testy);
       // program[i].setData(MODEL_RBF,features,scale_factor,trainx,trainy);
       // program[i].getModel()->setNumOfWeights(weights);
        }
        populationCount=chromosomeSize;
    }

    int	getdimension()
    {
       return populationCount;
    }

    void 	getmargins(vector<Interval> &x)
    {
        for(int i=0;i<getdimension();i++)
        {
                x[i]=Interval(leftMargin,rightMargin);
        }
    }
    QString replacePlus(QString s)
    {
        return s.replace("+","#");
    }


    int thread()
    {
        return omp_get_thread_num();
    }

    double	funmin(vector<double> &x)
    {
      setlocale(LC_ALL,"C");
      vector<int>  genome;
      genome.resize(getdimension());
      for(int i=0;i<getdimension();i++) genome[i]=(int)fabs(x[i]);
	program[thread()].neuralparser->sigcount=0;
	program[thread()].neuralparser->violcount=0;
      double f=program[thread()].fitness(genome);
	double percent=program[thread()].neuralparser->violcount*1.0/program[thread()].neuralparser->sigcount;
//	return -f * (1.0+percent);
      return -f;
    }
    double dmax(double a,double b){return a>b?a:b;}

    void    granal(vector<double> &x,vector<double> &g)
    {
        for(int i=0;i<x.size();i++)
             {
                 double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
                 x[i]+=eps;
                 double v1=funmin(x);
                 x[i]-=2.0 *eps;
                 double v2=funmin(x);
                 g[i]=(v1-v2)/(2.0 * eps);
                 x[i]+=eps;
             }

    }

    QJsonObject    done(Data &x)
    {
        double sum=0.0;
        vector<int> genome;
	genome.resize(getdimension());
        for(int i=0;i<getdimension();i++)
            genome[i]=(int)fabs(x[i]);
     	double ff=program[thread()].fitness(genome);
 	int tries=0;
   	MinInfo Info1;
        Info1.iters=2001;
        Info1.problem=&program[thread()];
	double value=0.0;
	double old_f=0.0;
	Data w;
        program[thread()].neuralparser->getWeights(w);
	GenSolve(&program[thread()],w,value,0,0);

        do
        {
           value=program[thread()].getTrainError();
           printf("Neural[%3d]=%20.10lf\n",tries,value);
           value=tolmin(w,Info1);
           program[thread()].neuralparser->getWeights(w);
           if(fabs(old_f-value)<1e-5) break;
           old_f=value;
           fflush(stdout);
           tries++;
           if(tries>=20) break;
         }while(1);
	  program[thread()].neuralparser->getWeights(w);
          value=program[thread()].getTrainError();
	  Converter con(w,w.size()/(dimension+2),dimension);
          con.convert(genome);
     	ff=program[thread()].fitness(genome);



        QJsonObject result;
        result["nodes"]=10;
        result["testError"]=program[thread()].getTestError();
        result["classError"]=program[thread()].getClassTestError(genome);
	QString bestProgram=program[thread()].printProgram(genome).c_str();
        result["string"]=bestProgram;
        return result;
    }

}
