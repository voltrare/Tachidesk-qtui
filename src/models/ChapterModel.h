#pragma once

#include <QAbstractListModel>
#include <QQmlParserStatus>
#include <optional>

#include "networkmanager.h"

class ChapterModel : public QAbstractListModel, public QQmlParserStatus
{
  Q_OBJECT

  Q_PROPERTY(NetworkManager* nm READ getNetworkManager WRITE setNetworkManager NOTIFY networkManagerChanged)
  Q_PROPERTY(qint32 mangaNumber MEMBER _mangaNumber NOTIFY mangaNumberChanged)
  Q_PROPERTY(qint32 chapterNumber MEMBER _chapterNumber NOTIFY chapterNumberChanged)

  NetworkManager* _networkManager = nullptr;

  struct ChapterInfo {
    QString  url;
    QString  name;
    qint64   uploadDate;
    qint32   chapterNumber;
    bool     read;
    quint32  index;
    quint32  pageCount;
    quint32  chapterCount;
  };
  std::optional<ChapterInfo> _chapters;

  qint32 _mangaNumber;
  qint32 _chapterNumber;

protected:

  void classBegin() override;

  void componentComplete() override;

  virtual QHash<int, QByteArray> roleNames() const override;

public:

  enum Role {
    RoleUrl = Qt::UserRole + 1,
    RoleName,
    RoleChapterNumber,
    RoleRead,
    RoleIndex,
    RolePageCount,
    RoleChapterCount,
    RoleChapterUrl,
  };

  ChapterModel(QObject* parent = nullptr);

  virtual int rowCount(
     const QModelIndex &parent = QModelIndex()) const override;

  virtual QVariant data(
     const QModelIndex &index,
     int role = Qt::DisplayRole) const override;

  auto getNetworkManager() const
  {
    return _networkManager;
  }

  void setNetworkManager(NetworkManager* nm) {
    _networkManager = nm;
    networkManagerChanged();
  }

  void recievedReply(const QJsonDocument& reply);

signals:
   void networkManagerChanged();
   void mangaNumberChanged();
   void chapterNumberChanged();
};
