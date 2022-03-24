#include <math.h>
#include <stdio.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QEventLoop>
#include <QUrl>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
# include <QUrlQuery>
#include <QTimer>
# include "omp.h"
#include "classprogram.h"
# include <qreplytimeout.h>
# include "interval.h"
#include <adept.h>
#include <adept_arrays.h>
/*
 *  This problem file will be used as a fronted
 *  for the genetic classifier technique. All the
 *  data are located on a remote http server.
*/
extern "C"
{

/**
Usage:
new QReplyTimeout(yourReply, msec);

When the timeout is reached the given reply is closed if still running



QNetworkAccessManager networkAccessManger;
QNetworkReply* reply = networkAccessManger.get(QNetworkRequest(QUrl("https://www.google.com")));
new QReplyTimeout(r, 100);
*/

typedef QVector<double> Data;

    //parameters
    const int maxthreads=64;
    double leftMargin=0.0;
    double rightMargin=255.0;
    QString trainfile="";
    QString testfile="";
    QString urlpath="http://itsoulos.teiep.gr/genclass/";
    //QString urlpath="https://app-1525680166.000webhostapp.com/";
    int chromosomeSize=40;
    //end of parameters
    //names: leftmargin, rightmargin, trainfile, testfile, chromosomesize

    int populationCount=0;
    int dimension=1;
    QVector<Data> xdata;
    Data ydata;
    QVector<Data> testxdata;
    Data testydata;
    int dcount=0;
    vector<ClassProgram> program;

//This method reads a file from http location file
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

int thread()
{
    return omp_get_thread_num();
}

void    parseJson()
{
    //load nntest.json file if exists
    QByteArray bts=readNetworkFile(urlpath+QString("genclass.json"));
    if(bts.size()==0) return;
    QString contents(bts);
    QJsonDocument doc=QJsonDocument::fromJson(contents.toUtf8());
    QJsonObject obj=doc.object();
    trainfile=obj["trainName"].toString();
    testfile=obj["testName"].toString();
}


void    init(QJsonObject obj)
{
    if(obj.contains("urlpath")) urlpath=obj["urlpath"].toString();
    if(!obj.contains("trainName") || !obj.contains("testName"))
    parseJson();
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

    QByteArray bts ;
    if(trainfile.startsWith("file://"))
    {
        bts=readLocalFile(trainfile);
    }
    else
    {
       bts=readNetworkFile(urlpath+trainfile);
    }
    QTextStream in(&bts);
    in>>dimension;
    in>>dcount;
    xdata.resize(dcount);
    ydata.resize(dcount);
    for(int i=0;i<dcount;i++)
    {
     xdata[i].resize(dimension);
     for(int j=0;j<dimension;j++) in>>xdata[i][j];
     in>>ydata[i];
     }

    program.resize(maxthreads);
    for(int i=0;i<maxthreads;i++)
    program[i].setData(xdata,ydata);
    populationCount=chromosomeSize * (program[0].getClass()-1);
}

int	getdimension()
{
   return populationCount;
}

QString replacePlus(QString s)
{
    return s.replace("+","#");
}

QJsonObject 	done(vector<double> &x)
{
    double	funmin(vector<double> &x);

       QByteArray bts ;
       if(testfile.startsWith("file://"))
           bts=readLocalFile(testfile);
       else
          bts=readNetworkFile(urlpath+testfile);
	QTextStream in(&bts);
	int count;
	in>>dimension;
	in>>count;
	testydata.resize(count);
	testxdata.resize(count);
	for(int i=0;i<count;i++) 
	{
		testxdata[i].resize(dimension);
		for(int j=0;j<dimension;j++)
			in>>testxdata[i][j];
		in>>testydata[i];
	}
    double trainError=funmin(x);
    double testError=program[omp_get_thread_num()].getClassError(x,testxdata,testydata);
    QJsonObject result;
    QString bestProgram=QString::fromStdString(program[omp_get_thread_num()].printF(x));
    result["nodes"]=10;
    result["testError"]=testError;
    result["classError"]=testError;
    result["string"]=bestProgram;
    return result;
    /*
    	QEventLoop loop;


        QUrl serviceUrl = QUrl(urlpath+QString("upload_result.php"));
        QString name=QString(trainfile).split(".").at(0);

    QByteArray postData;

            postData.append(QString("name=")+name+QString("&"));
            postData.append(QString("trainerror=")+QString::number(trainError)+QString("&"));
            postData.append(QString("testerror=")+QString::number(testError)+QString("&"));
            postData.append(QString("bestprogram=123"));

            QNetworkAccessManager *networkManager = new QNetworkAccessManager();
            QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));

            QNetworkRequest networkRequest(serviceUrl);
            networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

            QNetworkReply *rep=networkManager->post(networkRequest,postData);
            loop.exec();
            rep->readAll();*/
}


void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(0.0,1.0);
    }
}

double	funmin(vector<double> &x)
{
       double f=program[omp_get_thread_num()].fitness(x);
       program[omp_get_thread_num()].printF(x);
       return -f;
}


static double dmax(double a,double b)
{
	return a>b?a:b;
}

adept::adouble afunmin(adept::aVector &x)
{
    setlocale(LC_ALL, "C");
    vector<double> xx;
    xx.resize(getdimension());
    for(int i=0;i<xx.size();i++)
        xx[i]=x.data()[i];

    adept::adouble f = program[thread()].fitness(xx);
    return -f;;

}
void granal1(vector<double> &x, vector<double> &g)
{

    adept::Stack stack;
    g.resize(x.size());
    for (int i = 0; i < g.size(); i++)
        g[i] = 0.0;
    Data gtemp;
    gtemp.resize(g.size());
    int fcount = 0;
    QJsonObject done(vector<double> & x);
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
             x[i]-=2*eps;
             double v2=funmin(x);
             g[i]=(v1-v2)/(2.0 * eps);
	     x[i]+=eps;
         }

}

}
