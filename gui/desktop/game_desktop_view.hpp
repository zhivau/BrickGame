#ifndef GameDesktopView_H
#define GameDesktopView_H

#include <dlfcn.h>

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/library_specification.h"
#include "../../brick_game/snake/snake_objects.hpp"
#include "ui_game_desktop_view.h"

namespace Ui {
class GameDesktopView;
}

typedef GameInfo_t (*UpdateCurrentStateFunc)();
typedef void (*UserInputFunc)(UserAction_t, bool);

class GameDesktopView : public QWidget {
  Q_OBJECT

 public:
  explicit GameDesktopView(QWidget *parent = nullptr);
  ~GameDesktopView();

 protected:
  Ui::GameDesktopView *ui;
  QTimer *timer_;
  UserAction_t action_;
  bool hold_;
  int currentPosition_;
  GameInfo_t data_;

  void *handle;
  UserInputFunc userInput;
  UpdateCurrentStateFunc updateCurrentState;

  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void updateModel();

 signals:
  void exit();

 public slots:
  void startTimer(const char *lib_name);
};

#endif  // GameDesktopView_H
