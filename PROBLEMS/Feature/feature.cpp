#include "feature.h"
#include "nnprogram.h"
#include <interval.h>
#include <omp.h>
#include <adept.h>
#include <adept_arrays.h>

extern "C"
{
    //parameters
    const int maxthreads = 64;
    double leftMargin = 0.0;
    double rightMargin = 255.0;
    int random_seed = 1;
    QString trainfile = "";
    QString testfile = "";
    QString urlpath = "http://itsoulos.teiep.gr/feature/";
    //QString urlpath="file://home/sheridan/CLASSIFICATION/TENFOLD/";
    int chromosomeSize = 40;
    int features = 1;
    double scale_factor = 10.0;
    QString logfile = "feature.txt";
    QString outputdirectory = "CONSTRUCTED/"; //where the program will print the new features
    //end of parameters
    //names: leftmargin, rightmargin, logifle,trainfile, testfile,
    //       chromosomesize, features,weights,factor

    int populationCount = 0;
    int dimension = 1;
    int weights = 10;
    int dcount = 0;
    QVector<NNprogram> program;
    vector<int> genome;

    typedef vector<double> Data;
    vector<Data> trainx;
    Data trainy;
    vector<Data> testx;
    Data testy;
    Data dclass;

    //int nodes = 10;
    //int normalTrain = 1;
    //This method reads a file from http location file
    QByteArray readNetworkFile(QString file)
    {
        QEventLoop loop;
        QUrl serviceUrl = QUrl(file);
        QNetworkAccessManager *networkManager = new QNetworkAccessManager();
        QObject::connect(networkManager, SIGNAL(finished(QNetworkReply *)), &loop, SLOT(quit()));
        QNetworkRequest networkRequest(serviceUrl);
        networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
        QNetworkReply *rep = networkManager->get(networkRequest);
        loop.exec();
        QByteArray bts = rep->readAll();
        rep->close();
        delete rep;
        return bts;
    }

    QByteArray readLocalFile(QString file)
    {
        //if file starts with file://
        //file=file.mid(QString("file:/").size());
        QFile fp(file);
        fp.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray st = fp.readAll();
        fp.close();
        return st;
    }

    void loadTrain()
    {
        QFile fp(trainfile);
        fp.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream st(&fp);
        st >> dimension;
        int count;
        st >> count;
        trainx.resize(count);
        trainy.resize(count);
        for (int i = 0; i < count; i++)
        {
            trainx[i].resize(dimension);
            for (int j = 0; j < dimension; j++)
            {
                st >> trainx[i][j];
            }
            st >> trainy[i];
            bool found = false;
            for (int j = 0; j < dclass.size(); j++)
            {
                if (fabs(dclass[j] - trainy[i]) < 1e-7)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                dclass.push_back(trainy[i]);
        }
        fp.close();
    }

    void loadTest()
    {
        QFile fp(testfile);
        fp.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream st(&fp);
        st >> dimension;
        int count;
        st >> count;
        testx.resize(count);
        testy.resize(count);
        for (int i = 0; i < count; i++)
        {
            testx[i].resize(dimension);
            for (int j = 0; j < dimension; j++)
            {
                st >> testx[i][j];
            }
            st >> testy[i];
        }
        fp.close();
    }

    void init(QJsonObject obj)
    {
        setlocale(LC_ALL, "C");
        if (obj.contains("urlpath"))
            urlpath = obj["urlpath"].toString();
        if (!obj.contains("trainName") || !obj.contains("testName"))
        {
            trainfile = "wine.train";
            testfile = "wine.test";
        }
        else
        {
            trainfile = obj["trainName"].toString();
            testfile = obj["testName"].toString();
        }
        if (obj.contains("leftmargin"))
            leftMargin = obj["leftmargin"].toString().toDouble();
        if (obj.contains("rightmargin"))
            rightMargin = obj["rightmargin"].toString().toDouble();
        if (obj.contains("chromosomesize"))
            chromosomeSize = obj["chromosomesize"].toString().toInt();
        if (obj.contains("features"))
            features = obj["features"].toString().toInt();

        if (obj.contains("weights"))
            weights = obj["weights"].toString().toInt();

        if (obj.contains("factor"))
            scale_factor = obj["factor"].toString().toDouble();

        if (obj.contains("seed"))
            random_seed = obj["seed"].toString().toInt();

        if (obj.contains("logfile"))
            logfile = obj["logfile"].toString();

        loadTrain();
        loadTest();
        program.resize(maxthreads);
        for (int i = 0; i < maxthreads; i++)
        {
            program[i].setData(MODEL_RBF, features, scale_factor, trainx, trainy);
            program[i].getModel()->setNumOfWeights(weights);
        }
        populationCount = chromosomeSize * features;
        genome.resize(populationCount);
    }

    void setParameter(QString name, QVariant value)
    {
        if (name == "normalTrain")
        {
            if (value.toInt() == 1)
                for (int i = 0; i < maxthreads; i++)
                    program[i].setSeed(random_seed + i);
        }
    }

    int getdimension()
    {
        return populationCount;
    }

    void getmargins(vector<Interval> &x)
    {
        for (int i = 0; i < getdimension(); i++)
        {
            x[i] = Interval(leftMargin, rightMargin);
        }
    }

    QString replacePlus(QString s)
    {
        return s.replace("+", "#");
    }

    int thread()
    {
        return omp_get_thread_num();
    }
    QJsonObject done(Data &x)
    {
        double sum = 0.0;

        for (int i = 0; i < getdimension(); i++)
            genome[i] = (int)fabs(x[i]);
        double ff = program[omp_get_thread_num()].fitness(genome);

        double avg_test_error = 0.0;
        double avg_class_error = 0.0;

        int ntimes = 30;

        QString bestProgram = QString::fromStdString(program[thread()].printF(genome));

        //Rbf *neural = new Rbf(program[thread()].getMapper());
        const int threads = 12;
//#pragma omp parallel for num_threads(threads)
        for (int i = 1; i <= ntimes; i++)
        {
            //Rbf *neural = new Rbf(program[0].getMapper());
            Neural *neural = new Neural(program[0].getMapper());
            neural->readPatterns(trainx, trainy);
            neural->setPatternDimension(features);
            neural->setNumOfWeights(10);
            double d = neural->train2();
            double testError = neural->testError(testx, testy);
            //if(testError>1e+4) continue;
//#pragma omp critical
            {
                double classTestError = neural->classTestError(testx, testy);
                avg_test_error += testError;
                avg_class_error += classTestError;
                printf("ITER=%4d VALUES:(%10.5lf %10.5lf %10.2lf%%)\n",
                       i,d,testError,classTestError);
            }
            delete neural;
        }
        avg_test_error /= ntimes;
        avg_class_error /= ntimes;
        printf("Average test is %lf \n", avg_class_error);
        /*
     QEventLoop loop;
     QUrl serviceUrl = QUrl(urlpath+QString("upload_result.php"));
     QString name=QString(trainfile).split(".").at(0);



     char s1[100];
     sprintf(s1,"%.6lf",-ff);
     char s2[100];
     sprintf(s2,"%.6lf",avg_class_error);

     //Network send. It needs corrections. It hangs on loop exec
     if(urlpath.startsWith("file://"))
     {

         QString filename=urlpath.mid(QString("file:/").size());
         filename=filename+"/"+logfile;
         QFile fp(filename);
         fp.open(QIODevice::Append|QIODevice::Text);
         QTextStream st(&fp);
         st<<name<<"\t"<<features<<"\t"<<s1<<"\t"<<s2<<"\t"<<bestProgram.replace("\n",";")<<endl;
         fp.close();
     }
     else
     {
     QByteArray postData;
     postData.append(QString("name=")+name+QString("&"));
     postData.append(QString("testerror=")+QString("")+s1+QString("&"));
     postData.append(QString("classtesterror=")+QString("")+s2+QString("&"));
     postData.append(QString("features=")+QString::number(features)+"&");
     postData.append(QString("bestprogram=")+replacePlus(bestProgram));
     QNetworkAccessManager *networkManager = new QNetworkAccessManager();
     QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
     QNetworkRequest networkRequest(serviceUrl);
     networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
     QNetworkReply *rep=networkManager->post(networkRequest,postData);
     loop.exec();
     QByteArray bts = rep->readAll();
     QString str(bts);
     rep->close();
     delete rep;
       qDebug()<<"Response from server was "<<str<<endl;
     }


	*/
        QJsonObject result;
        result["nodes"] = 10;
        result["testError"] = avg_test_error;
        result["classError"] = avg_class_error;
        result["string"] = bestProgram;
        return result;
    }

    double dmax(double a, double b) { return a > b ? a : b; }

    double funmin(vector<double> &x)
    {
        setlocale(LC_ALL, "C");
        vector<int> genome;
        genome.resize(getdimension());
        for (int i = 0; i < getdimension(); i++)
            genome[i] = (int)fabs(x[i]);
        double f = program[thread()].fitness(genome);
        return -f;
    }
    adept::adouble afunmin(adept::aVector &x)
    {
        setlocale(LC_ALL, "C");
        vector<int> genome;
        genome.resize(getdimension());
        for (int i = 0; i < getdimension(); i++)
            genome[i] = (int)fabs(x[i].value());
        adept::adouble f = program[thread()].fitness(genome);
        return -f;;

}
    void granal1(Data &x, Data &g)
    {

        adept::Stack stack;
        g.resize(x.size());
        for (int i = 0; i < g.size(); i++)
            g[i] = 0.0;
        Data gtemp;
        gtemp.resize(g.size());
        int fcount = 0;
        QJsonObject done(Data & x);
        stack.continue_recording();
        adept::aVector w(x.size());
        for (unsigned i = 0; i < x.size(); i++)
            //w << x[i];
            w[i]= x[i];
        stack.new_recording();
        adept::adouble y;
        //vector<double> ww;//βοηθητικός
        //for (unsigned i = 0; i < w.size(); i++)
        //    ww.push_back(w[i].value()); //ενημέρωση
        y = afunmin(w); //υπολόγισμός minimum
        //for (unsigned i = 0; i < ww.size(); i++)
        //    w << ww[i];//επαναφορά των τιμών στον aVector
        y.set_gradient(1.0); // αρχικοποίηση κλίσης ώστε να μην έχουμε εξαίρεση
        stack.compute_adjoint(); //υπολογισμός παραγώγου
        for (unsigned i = 0; i < x.size(); i++)
	{
            g[i] = w[i].get_gradient();//εξαγωγή παραγώγου
	}
        stack.pause_recording();

	
	double diff =0.0;
        for(int i=0;i<x.size();i++)
             {
                 double eps=pow(1e-18,1.0/3.0)*dmax(1.0,fabs(x[i]));
                 x[i]+=eps;
                 double v1=funmin(x);
                 x[i]-=2.0 *eps;
                 double v2=funmin(x);
                 double oldG=(v1-v2)/(2.0 * eps);
		 //diff+=(g[i]-oldG)*(g[i]-oldG);
		 diff+=g[i]*g[i];
                 x[i]+=eps;
             }
	printf("DIFF: %20.10lg \n",diff);
        //for (int i = 0; i < trainx.size(); i++)
        //{
        //    getDeriv(x, trainx[i], gtemp);
        //    double per = dgetValue(x, trainx[i], fcount) - trainy[i];
        //    for (int j = 0; j < g.size(); j++)
        //        g[j] += gtemp[j] * per;
        //}
        //for (int j = 0; j < (dimension + 2) * nodes; j++)
        //    g[j] *= 2.0;

    }

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





}
