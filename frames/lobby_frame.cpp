#include "frames/lobby_frame.h"
#include "ui_lobby_frame.h"
#include "ui/messenger.h"
#include "net/route.h"

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
  client = new Client{new QTcpSocket{}};
  client->getPlayer()->setName(name);
  client->bindEvents();

  client->joinServer(Route{ip, 8080});

  connect(client, SIGNAL(broadcast(QString)), this, SLOT(introduce(QString)));
}

void LobbyFrame::introduce(QString response) {
  auto idAndIp = response.split(":");
  QString id = idAndIp[0];
  QString ip = idAndIp[1];
  auto auth = id + ":" + client->getPlayer()->getName();

  disconnect(client, SIGNAL(broadcast(QString)), this, SLOT(introduce(QString)));
  connect(client, SIGNAL(broadcast(QString)), this, SLOT(updateChat(QString)));
  client->sendMessage(auth);

  client->auth(id.toInt(), client->getPlayer()->getName());
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

void LobbyFrame::updateChat(QString batch) {
  auto messages = batch.split(";");

  foreach (auto message, messages) {
    auto parts = message.split("|");
    qDebug() << message;
    if (parts[0] == "0") { // Новый участник
      addPlayer(parts[1], parts[2]);
    } else if (parts[0] == "1") { // Новое сообщение в чате
      ui->messages->addItem(parts[1]);
    }
  }
}

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

bool LobbyFrame::addPlayer(const QString& name, const QString& ip) {
  int emptySlot = findEmptyPlayerSlot();
  if (emptySlot == -1) {
    return false;
  }

  ui->players->item(emptySlot, 0)->setText(name);
  ui->players->item(emptySlot, 1)->setText(ip);

  return true;
}

int LobbyFrame::findEmptyPlayerSlot() const {
  for (int i = 0; i < ui->players->rowCount(); ++i) {
    if (ui->players->item(i, 0)->text().isEmpty()) {
      return i;
    }
  }

  return -1;
}

void LobbyFrame::on_sendMessageButton_clicked() {
  client->sendMessage(QString::number(client->getId()) + ":" + ui->messageInput->text());
}
