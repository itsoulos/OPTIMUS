#include "qreplytimeout.h"

QReplyTimeout::QReplyTimeout(QNetworkReply* reply, const int timeout) : QObject(reply) {
    Q_ASSERT(reply);
    if (reply) {
      QTimer::singleShot(timeout, this, SLOT(timeout()));
    }
  }
 void QReplyTimeout::timeout()
 {
     QNetworkReply* reply = static_cast<QNetworkReply*>(parent());
     if (reply->isRunning()) {
       reply->close();
     }
   }
