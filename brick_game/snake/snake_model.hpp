#ifndef SNAKEMODEL_H
#define SNAKEMODEL_H

#include "../library_specification.h"
#include "snake_objects.hpp"

namespace s21 {

enum class SnakeState { START = 0, MOVING, PAUSE, GAMEOVER, EXIT };

struct SnakeData {
  std::vector<Coordinate> snake;
  Coordinate apple;
  int score;
  int level;
  int best_score;
  Direction dir;
  SnakeState state;
  bool is_modified;

  SnakeData()
      : snake(),
        apple(),
        score(0),
        level(1),
        best_score(0),
        dir(Direction::UP),
        state(SnakeState::START),
        is_modified(true){};

  bool operator==(const SnakeData& other) const {
    return snake == other.snake && score == other.score &&
           level == other.level && best_score == other.best_score &&
           dir == other.dir && state == other.state;
  }

  bool operator!=(const SnakeData& other) const { return !(*this == other); }
};

class SnakeModel {
 public:
  SnakeModel();
  ~SnakeModel();

  void userInput(UserAction_t action, bool hold);
  GameInfo_t updateCurrentState();

 private:
  SnakeData data_;
  GameInfo_t game_info_;

  std::chrono::high_resolution_clock::time_point current_moving_time_;
  std::chrono::high_resolution_clock::time_point last_moving_time_;

  Coordinate last_coord_snake_;

  using Action_f = void (SnakeModel::*)();
  Action_f TableSnakeActions[4][8] = {
      {&SnakeModel::startGame, nullptr, &SnakeModel::exit, nullptr, nullptr,
       nullptr, nullptr, nullptr},

      {nullptr, &SnakeModel::pause, &SnakeModel::exit, &SnakeModel::moveLeft,
       &SnakeModel::moveRight, &SnakeModel::moveUp, &SnakeModel::moveDown,
       nullptr},

      {nullptr, &SnakeModel::unpause, &SnakeModel::exit, nullptr, nullptr,
       nullptr, nullptr},

      {&SnakeModel::startGame, nullptr, &SnakeModel::exit, nullptr, nullptr,
       nullptr, nullptr}};

  void startGame();
  void moveUp();
  void moveDown();
  void moveRight();
  void moveLeft();
  void pause();
  void unpause();
  void exit();

  void check();

  void move();
  void shift();

  void updateApplePos();
  bool checkMatchAppleSnake();

  bool checkSnakeAlive();
  void snakeEatsApple();

  void setDefaultValue();
  SnakeData& getData();

  void saveBestScore(std::string filename, int best_score);
  int loadBestScore(std::string filename);
};

}  // namespace s21

#endif  // SNAKEMODEL_H