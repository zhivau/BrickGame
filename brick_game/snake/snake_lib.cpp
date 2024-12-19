#include "snake_controller.hpp"

static s21::SnakeModel model;
static s21::SnakeController controller(&model);

extern "C" {
void userInput(UserAction_t action, bool hold) {
  controller.userInput(action, hold);
}

GameInfo_t updateCurrentState() { return controller.updateCurrentState(); }
}