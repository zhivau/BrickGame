#ifndef MAINDESKTOPVIEW_H
#define MAINDESKTOPVIEW_H

#include <QWidget>

#include "ui_menu_desktop_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MenuDesktopView;
}
QT_END_NAMESPACE

class MenuDesktopView : public QWidget {
  Q_OBJECT

 public:
  MenuDesktopView(QWidget *parent = nullptr);
  ~MenuDesktopView();

 signals:
  void startTimerGame(const char *lib_name);

 private slots:
  void on_snakeButton_clicked();
  void on_tetrisButton_clicked();
  void on_exitButton_clicked();
  void exit_main_menu();

  void on_raceButton_clicked();

 private:
  Ui::MenuDesktopView *ui;
};

#endif  // MAINDESKTOPVIEW_H
