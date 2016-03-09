#pragma once

#include "net/client.h"
#include "net/server.h"

#include <QMainWindow>

namespace Ui {
class LobbyFrame;
}

class LobbyFrame : public QMainWindow
{
  Q_OBJECT

public:
  explicit LobbyFrame(QWidget *parent = 0);
  ~LobbyFrame();

private slots:
  void on_makeHostButton_clicked();

  void on_disconnectButton_clicked();

  void on_joinHostButton_clicked();

  // void updateChat(QByteArray newMessage);
  void introduce(QString id);
  void setupConnections();
  void onJoined(QVector<Player>);
  void onNewPlayer(Player);
  void onNewPublicText(QString);
  void onNewPrivateText(QString);

  void on_sendPublicMessageButton_clicked();

  void on_sendPrivateMessageButton_clicked();

  void on_comboBox_currentIndexChanged(int index);

private:
  Ui::LobbyFrame *ui;
  Server *server = nullptr;
  Client *client = nullptr;
  unsigned playerCount = 0;

  // bool addPlayer(const QString& name, const QString& ip);
  void addPlayer(const Player& player);
  int findEmptyPlayerSlot() const;
  void initClient(const QString& name, const QString& ip);
  void toggleUi();
  void appendChatMessage(const QString& text, Qt::GlobalColor color);
};

