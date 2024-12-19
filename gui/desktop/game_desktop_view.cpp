#include "game_desktop_view.hpp"

#include <time.h>
#include <unistd.h>

GameDesktopView::GameDesktopView(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::GameDesktopView),
      timer_(new QTimer(this)),
      action_((UserAction_t)-1),
      currentPosition_(0),
      data_() {
  ui->setupUi(this);
  connect(timer_, &QTimer::timeout, this, &GameDesktopView::updateModel);
}

GameDesktopView::~GameDesktopView() { delete ui; }

void GameDesktopView::keyPressEvent(QKeyEvent *event) {
  int key = event->key();
  hold_ = false;
  switch (key) {
    case Qt::Key_Left:
      action_ = UserAction_t::Left;
      break;
    case Qt::Key_Right:
      action_ = UserAction_t::Right;
      break;
    case Qt::Key_Up:
      action_ = UserAction_t::Up;
      break;
    case Qt::Key_Down:
      action_ = UserAction_t::Down;
      break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
      action_ = UserAction_t::Start;
      break;
    case Qt::Key_Escape:
      action_ = UserAction_t::Terminate;
      break;
    case Qt::Key_P:
      action_ = UserAction_t::Pause;
      break;
    case Qt::Key_Space:
      action_ = UserAction_t::Action;
      break;
    default:
      break;
  }
  if (event->isAutoRepeat()) {
    hold_ = true;
  }
}

void GameDesktopView::startTimer(const char *lib_name) {
  handle = dlopen(lib_name, RTLD_LAZY);
  userInput = (UserInputFunc)dlsym(handle, "userInput");
  updateCurrentState =
      (UpdateCurrentStateFunc)dlsym(handle, "updateCurrentState");
  timer_->start(50);
}

void GameDesktopView::updateModel() {
  if ((int)action_ != -1) {
    userInput(action_, hold_);
  }

  if (action_ == UserAction_t::Terminate) {
    timer_->stop();
    dlclose(handle);
    emit exit();
    data_.level = -1;
  } else {
    data_ = updateCurrentState();
    repaint();
  }

  ui->BestScore->setNum(data_.high_score);
  ui->CurrScore->setNum(data_.score);

  if (data_.level > 0) {
    ui->CurrLevel->setNum(data_.level);
    ui->Speed->setNum(data_.speed);
  }

  action_ = (UserAction_t)-1;
}

void GameDesktopView::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
  QPainter field(this);

  QColor fieldColor = QColor::fromHsv((currentPosition_) % 360, 255, 255, 180);
  currentPosition_++;

  QRect rect(0, 0, 300, 600);
  field.setBrush(fieldColor);
  field.drawRect(rect);

  QPen pen(QColor(0, 0, 0));
  pen.setWidth(3);
  field.setPen(pen);

  QFont font = field.font();
  font.setPointSize(20);
  field.setFont(font);

  if (data_.level == -1) {
    field.drawText(20, 300, "Press ENTER to start");
    field.drawText(50, 340, "Press ESC to exit");
  } else if (data_.pause) {
    field.drawText(120, 280, "Pause");
    field.drawText(80, 310, "Press ENTER");
    field.drawText(90, 340, "to continue");
  } else if (data_.level == -2) {
    field.drawText(80, 280, "GAMEOVER");
    field.drawText(10, 310, "Press ENTER to restart");
    field.drawText(50, 340, "Press ESC to exit");
  } else if (data_.level > 0) {
    if (data_.field) {
      for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
          if (data_.field[i][j]) {
            QColor ceilColor = QColor::fromHsv(
                (currentPosition_ + 30 * data_.field[i][j]) % 360, 255, 255,
                180);
            field.setBrush(ceilColor);
            field.drawRect(j * GameConstans::kDotSize,
                           i * GameConstans::kDotSize, GameConstans::kDotSize,
                           GameConstans::kDotSize);
          }
        }
      }
    }

    if (data_.next) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (data_.next[i][j]) {
            QColor ceilColor = QColor::fromHsv(
                (currentPosition_ + 30 * data_.field[i][j]) % 360, 255, 255,
                180);
            field.setBrush(ceilColor);
            field.drawRect((j + 11) * GameConstans::kDotSize,
                           (i + 10) * GameConstans::kDotSize,
                           GameConstans::kDotSize, GameConstans::kDotSize);
          }
        }
      }
    }
  }
  field.setPen(pen);
  field.drawLine(300, 0, 300, 605);
}
