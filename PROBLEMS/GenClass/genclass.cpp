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
#include "classprogram.h"
# include <qreplytimeout.h>
# include "interval.h"

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
    double leftMargin=0.0;
    double rightMargin=255.0;
    QString trainfile="";
    QString testfile="";
    //QString urlpath="http://itsoulos.teiep.gr/genclass/";
    QString urlpath="https://app-1525680166.000webhostapp.com/";
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
    ClassProgram program;
    vector<int> genome;

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
    file=file.mid(QString("file://").size());
    QFile fp(file);
   fp.open(QIODevice::ReadOnly|QIODevice::Text);
   QByteArray st=fp.readAll();
   fp.close();
   return st;
}

void    parseJson()
{
    //load nntest.json file if exists
    QByteArray bts=readNetworkFile(urlpath+QString("genclass.json"));
    if(bts.size()==0) return;
    QString contents(bts);
    QJsonDocument doc=QJsonDocument::fromJson(contents.toUtf8());
    QJsonObject obj=doc.object();
    trainfile=obj["trainfile"].toString();
    testfile=obj["testfile"].toString();
}


void    init(QJsonObject obj)
{
    if(obj.contains("urlpath")) urlpath=obj["urlpath"].toString();
    if(!obj.contains("trainfile") || !obj.contains("testfile"))
    parseJson();
    else
    {
        trainfile=obj["trainfile"].toString();
        testfile=obj["testfile"].toString();
    }
    if(obj.contains("leftmargin"))
        leftMargin=obj["leftmargin"].toString().toDouble();
    if(obj.contains("rightmargin"))
        rightMargin=obj["rightmargin"].toString().toDouble();
    if(obj.contains("chromosomesize"))
        chromosomeSize=obj["chromosomesize"].toString().toInt();

    QByteArray bts ;
    if(trainfile.startsWith("file://"))
        bts=readLocalFile(trainfile);
    else
       bts=readNetworkFile(urlpath+trainfile);
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
    program.setData(xdata,ydata);
    populationCount=chromosomeSize * (program.getClass()-1);
    genome.resize(populationCount);
}

int	getdimension()
{
   return populationCount;
}

QString replacePlus(QString s)
{
    return s.replace("+","#");
}

void 	done(double *node)
{
    double	funmin(double *x);
       for(int i=0;i<getdimension();i++)
           genome[i]=(int)fabs(node[i]);
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
    double trainError=funmin(node);
    double testError=program.getClassError(genome,testxdata,testydata);
    QString bestProgram=QString::fromStdString(program.printF(genome));
    	QEventLoop loop;


        QUrl serviceUrl = QUrl(urlpath+QString("upload_result.php"));
        QString name=QString(trainfile).split(".").at(0);

    QByteArray postData;

    /*     postData.addQueryItem(QString("name"),name);
        postData.addQueryItem(QString("trainerror"),QString::number(trainError));
        postData.addQueryItem(QString("testerror"),QString::number(testError));
        postData.addQueryItem(QString("bestprogram"),replacePlus(bestProgram));


    QNetworkAccessManager *networkManager = new QNetworkAccessManager();
    	QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
    	QNetworkRequest networkRequest(serviceUrl);
    	networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        QNetworkReply *rep=networkManager->post(networkRequest,postData.toString(QUrl::FullyEncoded).toUtf8());
*/

            postData.append(QString("name=")+name+QString("&"));
            postData.append(QString("trainerror=")+QString::number(trainError)+QString("&"));
            postData.append(QString("testerror=")+QString::number(testError)+QString("&"));
            postData.append(QString("bestprogram=123"));

            QNetworkAccessManager *networkManager = new QNetworkAccessManager();
            QObject::connect(networkManager, SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));

            QNetworkRequest networkRequest(serviceUrl);
            networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

            QNetworkReply *rep=networkManager->post(networkRequest,postData);
    //new QReplyTimeout(rep, 30000);
            loop.exec();
            rep->readAll();
}


void	getleftmargin(double *x)
{
        for(int i=0;i<getdimension();i++)
                x[i]=leftMargin;
}

void	getrightmargin(double *x)
{
        for(int i=0;i<getdimension();i++)
                x[i]=rightMargin;
}
void 	getmargins(vector<Interval> &x)
{
    for(int i=0;i<getdimension();i++)
    {
            x[i]=Interval(leftMargin,rightMargin);
    }
}

double	funmin(vector<double> &x)
{
		setlocale(LC_ALL,"C");
       for(int i=0;i<getdimension();i++)
           genome[i]=(int)fabs(x[i]);
       double f=program.fitness(genome);
       program.printF(genome);
       return -f;
}


void    granal(vector<double> &x,vector<double> &g)
{
    for(int i=0;i<x.size();i++)
         {
             double eps=pow(1e-18,1.0/3.0)*qMax(1.0,fabs(x[i]));
             x[i]+=eps;
             double v1=funmin(x);
             x[i]-=2.0 *eps;
             double v2=funmin(x);
             g[i]=(v1-v2)/(2.0 * eps);
             x[i]+=eps;
         }

}

}
