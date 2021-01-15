#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class VersionChecker : public QObject
{
    Q_OBJECT
  public:
    explicit VersionChecker(QObject *parent = nullptr);
    void checkAgate();
    void checkqAgate();

  private:
    QNetworkRequest _request;
    QNetworkAccessManager _manager;

  signals:

};

#endif // VERSIONCHECKER_H
