#ifndef SNAKECONTROLLER_H
#define SNAKECONTROLLER_H

#include "snake_model.hpp"

namespace s21 {

class SnakeController {
 public:
  SnakeController(SnakeModel *model) : snake_model_(model){};

  void userInput(UserAction_t action, bool hold) {
    snake_model_->userInput(action, hold);
  }

  GameInfo_t updateCurrentState() { return snake_model_->updateCurrentState(); }

 private:
  SnakeModel *snake_model_;
};

}  // namespace s21

#endif
