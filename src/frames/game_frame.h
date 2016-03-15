#pragma once

#include "net/client.h"
#include "vessels/marauder.h"
#include "chip/human_control.h"

#include <QMainWindow>
#include <QKeyEvent>
#include <QTimer>

namespace Ui {
  class GameFrame;
}

class GameFrame : public QMainWindow {
  Q_OBJECT

public:
  explicit GameFrame(QWidget *parent = nullptr);

  ~GameFrame();

  void keyReleaseEvent(QKeyEvent *event) {
    ctl->handleKeyRelease(event->key());
  }

  void keyPressEvent(QKeyEvent *event) {
    ctl->handleKeyPress(event->key());
  }

private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

private:
  Ui::GameFrame *ui;
  Client *client = nullptr;
  Marauder *dr;
  HumanControl *ctl;
  QTimer *timer;
};
