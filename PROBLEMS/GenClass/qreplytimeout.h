#ifndef QREPLYTIMEOUT_H
#define QREPLYTIMEOUT_H

#include <QObject>
# include <QNetworkAccessManager>
# include <QNetworkReply>
# include <QTimer>
class QReplyTimeout : public QObject
{
    Q_OBJECT
public:
     QReplyTimeout(QNetworkReply* reply, const int timeout);

signals:

private slots:
  void timeout();
};

#endif // QREPLYTIMEOUT_H
