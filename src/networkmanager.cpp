#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDesktopServices>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkInterface>
#include <QEventLoop>

#include "networkmanager.h"
#include "client_info.h"

/********************************************************************
 *
 *  NetworkManager()
 *
 ********************************************************************/
NetworkManager::NetworkManager(
  const QString& host,
  const QString& port,
  QObject* parent)
    : QObject(parent)
    , _settings("Tachidesk-qtui", "Suwayomi")
    , _host(host)
    , _port(port)
{
}

/********************************************************************
 *
 * handleNetworkError
 *
 ********************************************************************/
bool NetworkManager::handleNetworkError(QNetworkReply* reply)
{
  if (reply->error() != QNetworkReply::NoError)
  {
    if (reply->error() >= 1 && reply->error() <= 199)
    {
      if (connectionOK == true)
      {
        connectionOK = false;
      }
    }

    qDebug() << reply->errorString();

    return false;
  }

  if (!connectionOK) {
    connectionOK = true;
  }

  return true;
}

// TODO: add different paths
//void NetworkManager::getManga(const Qstring& endpoint)
//{
//  get("/manga" + endpoint);
//}

/********************************************************************
 *
 *  get()
 *
 ********************************************************************/
void NetworkManager::get(const QString& endpoint)
{
  QNetworkRequest request;
  request.setUrl(QUrl(QStringLiteral("%1:%2/api/v1/%3").arg(_host).arg(_port).arg(endpoint)));

  QNetworkReply *reply = man.get(request);

  connect(
      reply,
      &QNetworkReply::finished,
      this,
      &NetworkManager::endpointReply);
}

/********************************************************************
 *
 *  get()
 *
 ********************************************************************/
void NetworkManager::deleteResource(const QString& endpoint)
{
  QNetworkRequest request;
  request.setUrl(QUrl(QStringLiteral("%1:%2/api/v1/%3").arg(_host).arg(_port).arg(endpoint)));

  QNetworkReply *reply = man.deleteResource(request);

  connect(
      reply,
      &QNetworkReply::finished,
      this,
      &NetworkManager::endpointReply);
}

/********************************************************************
 *
 *  userReply()
 *
 ********************************************************************/
void NetworkManager::endpointReply()
{
  QNetworkReply* reply = qobject_cast<QNetworkReply *>(sender());
  if (!handleNetworkError(reply)) {
      return;
  }
  QByteArray data = reply->readAll();

  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(data,&error);

  emit recievedReply(doc);
  reply->deleteLater();
}

