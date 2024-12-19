#include "snake_model.hpp"

namespace s21 {

SnakeModel::SnakeModel() {
  game_info_.field = new int*[20];
  for (int i = 0; i < 20; i++) {
    game_info_.field[i] = new int[10];
    for (int j = 0; j < 10; j++) {
      game_info_.field[i][j] = 0;
    }
  }
}

SnakeModel::~SnakeModel() {
  for (int i = 0; i < 20; i++) {
    delete[] game_info_.field[i];
  }
  delete[] game_info_.field;
}

SnakeData& SnakeModel::getData() { return data_; }

void SnakeModel::setDefaultValue() {
  data_.dir = Direction::UP;
  data_.level = 1;
  data_.score = 0;
  data_.state = SnakeState::START;
  data_.best_score = loadBestScore("best_score_snake.txt");

  data_.snake.clear();

  data_.snake.push_back({5, 9});
  data_.snake.push_back({5, 10});
  data_.snake.push_back({5, 11});
  data_.snake.push_back({5, 12});

  updateApplePos();
}

void SnakeModel::userInput(UserAction_t action, bool hold) {
  data_.is_modified = true;
  SnakeData data_tmp = data_;

  Action_f act = TableSnakeActions[static_cast<int>(data_.state)]
                                  [static_cast<int>(action)];

  if (act) (this->*act)();

  if (data_tmp == data_) data_.is_modified = false;

  if (hold) {
  };
}

GameInfo_t SnakeModel::updateCurrentState() {
  current_moving_time_ = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      current_moving_time_ - last_moving_time_);

  if (duration.count() > GameConstans::kMsIntervals[data_.level - 1] &&
      data_.state == SnakeState::MOVING) {
    shift();
  }

  if (data_.state == SnakeState::MOVING) {
    snakeEatsApple();
  }

  game_info_.score = data_.score;
  game_info_.level = data_.level;
  game_info_.speed = data_.level;
  game_info_.high_score = data_.best_score;

  if (data_.state == SnakeState::START) {
    game_info_.level = -1;
  } else if (data_.state == SnakeState::GAMEOVER) {
    game_info_.level = -2;
  }

  if (data_.state == SnakeState::PAUSE) {
    game_info_.pause = 1;
  } else {
    game_info_.pause = 0;
  }

  if (data_.state == SnakeState::MOVING) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        game_info_.field[i][j] = 0;
      }
    }

    for (int i = 0; i < (int)data_.snake.size(); i++) {
      game_info_.field[data_.snake[i].y - 1][data_.snake[i].x - 1] = 1;
    }
    game_info_.field[data_.apple.y - 1][data_.apple.x - 1] = 2;
  }

  return game_info_;
}

void SnakeModel::move() {
  last_moving_time_ = std::chrono::high_resolution_clock::now();
  data_.state = SnakeState::MOVING;

  last_coord_snake_ = data_.snake[data_.snake.size() - 1];

  for (int i = data_.snake.size() - 1; i > 0; i--) {
    data_.snake[i] = data_.snake[i - 1];
  }
}

void SnakeModel::moveUp() {
  if (data_.dir != Direction::DOWN) {
    move();
    data_.snake[0].y -= 1;
    data_.dir = Direction::UP;
    if (!checkSnakeAlive()) {
      data_.state = SnakeState::GAMEOVER;
    }
  }
}

void SnakeModel::moveDown() {
  if (data_.dir != Direction::UP) {
    move();
    data_.snake[0].y += 1;
    data_.dir = Direction::DOWN;
    if (!checkSnakeAlive()) {
      data_.state = SnakeState::GAMEOVER;
    }
  }
}

void SnakeModel::moveLeft() {
  if (data_.dir != Direction::RIGHT) {
    move();
    data_.snake[0].x -= 1;
    data_.dir = Direction::LEFT;
    if (!checkSnakeAlive()) {
      data_.state = SnakeState::GAMEOVER;
    }
  }
}

void SnakeModel::moveRight() {
  if (data_.dir != Direction::LEFT) {
    move();
    data_.snake[0].x += 1;
    data_.dir = Direction::RIGHT;
    if (!checkSnakeAlive()) {
      data_.state = SnakeState::GAMEOVER;
    }
  }
}

void SnakeModel::shift() {
  if (data_.dir == Direction::UP)
    moveUp();
  else if (data_.dir == Direction::RIGHT)
    moveRight();
  else if (data_.dir == Direction::DOWN)
    moveDown();
  else
    moveLeft();
}

void SnakeModel::snakeEatsApple() {
  if (data_.snake[0] == data_.apple) {
    data_.score++;
    if (data_.level < 10 && data_.score >= 5) data_.level = 1 + data_.score / 5;
    updateApplePos();
    data_.snake.push_back(last_coord_snake_);

    if (data_.best_score < data_.score) data_.best_score = data_.score;
  }
}

void SnakeModel::updateApplePos() {
  do {
    int random_x = 1 + std::rand() % 10;
    int random_y = 1 + std::rand() % 20;
    data_.apple = {random_x, random_y};
  } while (checkMatchAppleSnake());
}

bool SnakeModel::checkMatchAppleSnake() {
  bool result = false;
  for (int i = 0; i < (int)data_.snake.size(); i++) {
    if (data_.snake[i] == data_.apple) {
      result = true;
      break;
    }
  }
  return result;
}

bool SnakeModel::checkSnakeAlive() {
  bool result = true;
  for (int i = data_.snake.size() - 1; i > 3; i--) {
    if (data_.snake[i] == data_.snake[0]) {
      result = false;
      break;
    }
  }
  if (result) {
    if (data_.snake[0].x == 11 || data_.snake[0].x == 0 ||
        data_.snake[0].y == 0 || data_.snake[0].y == 21) {
      result = false;
    }
  }
  return result;
}

void SnakeModel::pause() { data_.state = SnakeState::PAUSE; }

void SnakeModel::unpause() { data_.state = SnakeState::MOVING; }

void SnakeModel::exit() {
  if (data_.best_score == data_.score && data_.score > 0) {
    saveBestScore("best_score_snake.txt", data_.best_score);
  }
  setDefaultValue();
}

void SnakeModel::startGame() {
  setDefaultValue();
  data_.state = SnakeState::MOVING;
}

void SnakeModel::saveBestScore(std::string filename, int best_score) {
  std::ofstream file(filename);
  if (file.is_open()) {
    file << best_score;
    file.close();
  }
}

int SnakeModel::loadBestScore(std::string filename) {
  int best_score = 0;
  std::ifstream file(filename);
  if (file.is_open()) {
    file >> best_score;
    file.close();
  }
  return best_score;
}

}  // namespace s21
