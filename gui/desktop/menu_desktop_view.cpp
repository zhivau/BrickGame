#include "menu_desktop_view.hpp"

MenuDesktopView::MenuDesktopView(QWidget *parent)
    : QWidget(parent), ui(new Ui::MenuDesktopView) {
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  connect(ui->game_desktop_view, &GameDesktopView::exit, this,
          &MenuDesktopView::exit_main_menu);
  connect(this, &MenuDesktopView::startTimerGame, ui->game_desktop_view,
          &GameDesktopView::startTimer);
}

MenuDesktopView::~MenuDesktopView() { delete ui; }

void MenuDesktopView::on_snakeButton_clicked() {
  ui->stackedWidget->setCurrentIndex(1);
  emit startTimerGame("./libsnake.so");
}

void MenuDesktopView::on_tetrisButton_clicked() {
  ui->stackedWidget->setCurrentIndex(1);
  emit startTimerGame("./libtetris.so");
}

void MenuDesktopView::on_raceButton_clicked() {
  ui->stackedWidget->setCurrentIndex(1);
  emit startTimerGame("./librace_client.so");
}

void MenuDesktopView::on_exitButton_clicked() { close(); }

void MenuDesktopView::exit_main_menu() {
  ui->stackedWidget->setCurrentIndex(0);
}
