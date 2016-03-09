#include "frames/game_frame.h"
#include "ui_game_frame.h"

#include "net/route.h"
#include "net/player.h"
#include "vessels/dreadnought.h"
#include "core/game_scene.h"
#include "chip/random_control.h"

#include <QImage>
#include <QPointF>

GameFrame::GameFrame(QWidget *parent):
  QMainWindow{parent}, ui{new Ui::GameFrame} {
  ui->setupUi(this);

  auto scene = new QGraphicsScene{};
  GameScene::start(scene);
  scene->setSceneRect(0, 0, 480, 480);
  ui->graphicsView->setScene(scene);
  QImage pim("./sprites/space.jpg");
  scene->setBackgroundBrush(pim);

  ctl = new DirectControl{};
  dr = new Dreadnought{ctl};

  dr->setPos(32, 32);

  Dreadnought *dummy = new Dreadnought{new RandomControl{}};
  dummy->setPos(300, 300);
}

GameFrame::~GameFrame() {
  delete ui;

  if (client) {
    delete client;
  }
}

void GameFrame::on_pushButton_clicked() {
  /*
  if (!client) {
    // client = new Client{std::unique_ptr<Player>{new Player{"Iskander"}}};
    // client = new Client{new QTcpSocket{}};
    client->getPlayer()->setName(ui->);
    client->joinServer(Route{});
  }
  */
}

void GameFrame::on_pushButton_2_clicked() {
  qDebug() << "writing?";
  // client->sendText("!!!!!");
}
