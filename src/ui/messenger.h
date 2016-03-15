#pragma once

#include <QMessageBox>

class Messenger {
public:
  static void message(QString msg, QString title, QMessageBox::Icon type) {
    QMessageBox box{type, title, msg};
    box.setText(msg);
    box.exec();
  }

  static void warn(QString msg) {
    message(msg, "warning!", QMessageBox::Icon::Warning);
  }

  static void info(QString msg) {
    message(msg, "info", QMessageBox::Icon::Information);
  }

  static bool confirm(QString msg) {
    QMessageBox box;

    box.setWindowTitle("question");
    box.setText(msg);
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    return box.exec() == QMessageBox::Yes;
  }
};
