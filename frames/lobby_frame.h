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

  void updateChat(QString newMessage);
  void introduce(QString id);

  void on_sendMessageButton_clicked();

private:
  Ui::LobbyFrame *ui;
  Server *server = nullptr;
  Client *client = nullptr;

  bool addPlayer(const QString& name, const QString& ip);
  int findEmptyPlayerSlot() const;
  void initClient(const QString& name, const QString& ip);
  void toggleUi();
};
