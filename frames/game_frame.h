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
    switch (event->key()) {
    case Qt::Key_W:
      ctl->deactivateThrust();
      break;
    case Qt::Key_A:
      ctl->deactivateRotateLeft();
      break;
    case Qt::Key_D:
      ctl->deactivateRotateRight();
      break;
    case Qt::Key_BracketLeft:
      ctl->deactivateFire1();
      break;
    default:
      event->ignore();
    }
  }

  void keyPressEvent( QKeyEvent* event ) {
      switch ( event->key() ) {
      case Qt::Key_BracketLeft:
        ctl->activateFire1();
        break;

      case Qt::Key_W:
        ctl->activateThrust();
        break;
      case Qt::Key_A:
        ctl->activateRotateLeft();
        break;
      case Qt::Key_D:
        ctl->activateRotateRight();
        break;
      case Qt::Key_X:
        dr->left();
          break;
      case Qt::Key_Y:
        qDebug() << "y!";
          break;
      default:
          event->ignore();
          break;
      }
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
