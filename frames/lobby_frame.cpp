#include "frames/lobby_frame.h"
#include "ui_lobby_frame.h"
#include "ui/messenger.h"
#include "net/route.h"
#include "net/socket.h"

LobbyFrame::LobbyFrame(QWidget *parent):
QMainWindow(parent), ui(new Ui::LobbyFrame) {
  ui->setupUi(this);
  ui->lobbyBox->hide();
}

LobbyFrame::~LobbyFrame() {
  delete ui;

  if (server) {
    delete server;
  }

  if (client) {
    delete client;
  }
}

void LobbyFrame::on_makeHostButton_clicked() {
  auto name = ui->nicknameInput->text();
  auto ip = ui->ipInput->text();

  if (name.isEmpty() || ip.isEmpty()) {
    Messenger::warn("Enter nickname and IP, please");
  } else {
    toggleUi();
    server = new Server{Route{ip, 8080}};
    initClient(name, ip);
  }
}

void LobbyFrame::toggleUi() {
  ui->conectionBox->setVisible(!ui->conectionBox->isVisible());
  ui->lobbyBox->setVisible(!ui->lobbyBox->isVisible());
}

void LobbyFrame::initClient(const QString& name, const QString& ip) {
  client = new Client{new Socket{}};
  client->getPlayer()->setName(name);

  client->joinServer(Route{ip, 8080});
  connect(client, SIGNAL(joined(QVector<Player>)), this, SLOT(onJoined(QVector<Player>)));
}

void LobbyFrame::onJoined(QVector<Player> players) {
  foreach (auto player, players) {
    addPlayer(player);
  }

  disconnect(client, SIGNAL(joined(QVector<Player>)), this, SLOT(onJoined(QVector<Player>)));

  connect(client, SIGNAL(gotNewPlayer(Player)), this, SLOT(onNewPlayer(Player)));
  connect(client, SIGNAL(gotPrivateText(QString)), this, SLOT(onNewPrivateText(QString)));
  connect(client, SIGNAL(gotPublicText(QString)), this, SLOT(onNewPublicText(QString)));
}

void LobbyFrame::onNewPrivateText(QString text) {
  appendChatMessage(text, Qt::darkBlue);
}

void LobbyFrame::onNewPublicText(QString text) {
  appendChatMessage(text, Qt::black);
}

void LobbyFrame::appendChatMessage(const QString& text, Qt::GlobalColor color) {
  auto wget = ui->messages;
  wget->addItem(text);
  wget->item(wget->count() - 1)->setForeground(color);
}

void LobbyFrame::onNewPlayer(Player player) {
  addPlayer(player);
}

void LobbyFrame::setupConnections() {
  qDebug() << "got auth!! yeah";
}

void LobbyFrame::introduce(QString response) {
  qDebug() << response;
  /*
  auto idAndIp = response.split(":");
  QString id = idAndIp[0];
  QString ip = idAndIp[1];
  auto auth = id + ":" + client->getPlayer()->getName();

  disconnect(client, SIGNAL(broadcast(QString)), this, SLOT(introduce(QString)));
  connect(client, SIGNAL(broadcast(QString)), this, SLOT(updateChat(QString)));
  client->sendMessage(auth);

  client->auth(id.toInt(), client->getPlayer()->getName());
  */
}

void LobbyFrame::on_joinHostButton_clicked() {
  auto name = ui->nicknameInput->text();
  auto ip = ui->ipInput->text();

  if (name.isEmpty() || ip.isEmpty()) {
    Messenger::warn("Enter nickname and IP, please");
  } else {
    toggleUi();
    initClient(name, ip);
  }
}

/*
void LobbyFrame::updateChat(QByteArray batch) {
  qDebug() << "text msg:" << QString{batch};

  auto messages = batch.split(';');

  foreach (auto message, messages) {
    auto parts = message.split("|");
    qDebug() << message;
    if (parts[0] == "0") { // Новый участник
      addPlayer(parts[1], parts[2]);
    } else if (parts[0] == "1") { // Новое сообщение в чате
      ui->messages->addItem(parts[1]);
    }
  }
}*/

void LobbyFrame::on_disconnectButton_clicked() {
  if (Messenger::confirm("Are you sure?")) {
    ui->conectionBox->show();
    ui->lobbyBox->hide();

    for (int i = 0; i < ui->players->rowCount(); ++i) {
      ui->players->item(i, 0)->setText("");
      ui->players->item(i, 1)->setText("");
    }

    if (server) {
      server->stop();
      delete server;
      server = nullptr;
    }

    if (client) {
      delete client;
      client = nullptr;
      // Нужно ли тут делать disconnect?
    }
  }
}

void LobbyFrame::addPlayer(const Player &player) {
  auto wget = ui->players;
  wget->item(playerCount, 0)->setText(player.getName());
  wget->item(playerCount, 1)->setText(player.getIp());
  wget->item(playerCount, 2)->setText(QString::number(player.getTeam()));
  playerCount += 1;
}

/*
bool LobbyFrame::addPlayer(const QString& name, const QString& ip) {
  int emptySlot = findEmptyPlayerSlot();
  if (emptySlot == -1) {
    return false;
  }

  ui->players->item(emptySlot, 0)->setText(name);
  ui->players->item(emptySlot, 1)->setText(ip);

  return true;
}*/

/*
int LobbyFrame::findEmptyPlayerSlot() const {
  for (int i = 0; i < ui->players->rowCount(); ++i) {
    if (ui->players->item(i, 0)->text().isEmpty()) {
      return i;
    }
  }

  return -1;
}
*/

void LobbyFrame::on_sendPublicMessageButton_clicked() {
  auto text = ui->messageInput->text();
  if (!text.isEmpty()) {
    client->sendPublicText(text);
  } else {
    Messenger::warn("Enter your message");
  }
}

void LobbyFrame::on_sendPrivateMessageButton_clicked() {
  auto text = ui->messageInput->text();
  if (!text.isEmpty()) {
    client->sendPrivateText(text);
  } else {
    Messenger::warn("Enter your message");
  }
}

void LobbyFrame::on_comboBox_currentIndexChanged(int index) {
  qDebug() << index;
}
