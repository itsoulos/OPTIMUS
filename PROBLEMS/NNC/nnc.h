#ifndef NNC_H
#define NNC_H

#include "NNC_global.h"
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
# include <nncneuralprogram.h>
# include <QTimer>
class QReplyTimeout : public QObject {
  Q_OBJECT
public:
  QReplyTimeout(QNetworkReply* reply, const int timeout) : QObject(reply) {
    Q_ASSERT(reply);
    if (reply) {
      QTimer::singleShot(timeout, this, SLOT(timeout()));
    }
  }

private slots:
  void timeout() {
    QNetworkReply* reply = static_cast<QNetworkReply*>(parent());
    if (reply->isRunning()) {
      reply->close();
    }
  }
};

using namespace std;

#endif // NNC_H
