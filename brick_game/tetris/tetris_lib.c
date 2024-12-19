#include "tetris_lib.h"


Action_f TableTetrisActions[4][8] = {
    {startGame, NULL, exitState, NULL, NULL, NULL, NULL, NULL},
    {NULL, pause_state, exitState, moveLeft, moveRight, rotate, moveDown, drop},
    {NULL, unpause, exitState, NULL, NULL, NULL, NULL, NULL},
    {startGame, NULL, exitState, NULL, NULL, NULL, NULL, NULL}};

static TetrisData tetris_data;
static GameInfo_t game_info;

void saveBestScore(const char *filename, int best_score) {
  FILE *file = fopen(filename, "w");
  if (file != NULL) {
    fprintf(file, "%d", best_score);
    fclose(file);
  }
}

int loadBestScore(const char *filename) {
  int best_score = 0;
  FILE *file = fopen(filename, "r");
  if (file != NULL) {
    fscanf(file, "%d", &best_score);
    fclose(file);
  }
  return best_score;
}

void setDefaultValue() {
  tetris_data.level = 1;
  tetris_data.score = 0;
  tetris_data.state = START;

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      tetris_data.field[i][j].is_fill = 0;
      tetris_data.field[i][j].shape = EMPTY;
    }
  }

  setRandomFigure(&tetris_data.cur_figure);
  setRandomFigure(&tetris_data.next_figure);

  tetris_data.projection_cur_figure =tetris_data.cur_figure;

  tetris_data.current_moving_time = current_time_millis();
  tetris_data.last_moving_time = tetris_data.current_moving_time;

  tetris_data.best_score = loadBestScore("best_score_tetris.txt");
}

void setRandomFigure(Tetromino *figure) {
  int num_figure = rand() % 7;

  Coordinate figures[7][4] = {
      {{4, 0}, {4, 1}, {5, 1}, {5, 0}},  // O
      {{2, 0}, {3, 0}, {4, 0}, {5, 0}},  // I
      {{3, 1}, {4, 1}, {4, 0}, {5, 0}},  // S
      {{3, 0}, {4, 0}, {4, 1}, {5, 1}},  // Z
      {{3, 1}, {4, 1}, {5, 1}, {4, 0}},  // T
      {{3, 1}, {3, 0}, {4, 0}, {5, 0}},  // L
      {{3, 0}, {3, 1}, {4, 1}, {5, 1}}   // J
  };

  for (int i = 0; i < 4; i++) {
    figure->coords[i] = figures[num_figure][i];
  }

  figure->shape = (ShapeFigure)(num_figure + 1);
}

void userInput(UserAction_t action, bool hold) {
  Action_f act = NULL;
  act = TableTetrisActions[tetris_data.state][action];

  if (act) {
    act(&tetris_data);
  }

  if (hold) {}
}

GameInfo_t convertTetrisDataToGameInfo(TetrisData *data) {
  game_info.score = data->score;
  game_info.level = data->level;
  game_info.speed = data->level;
  game_info.high_score = data->best_score;

  if (data->state == START) {
    game_info.level = -1;
  } else if (data->state == GAMEOVER) {
    game_info.level = -2;
  } 

  if (data->state == PAUSE) {
    game_info.pause = 1;
  } else {
    game_info.pause = 0;
  }

  if (data->state == MOVING) {

    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        game_info.field[i][j] = data->field[i][j].shape;
      }
    }

    for (int i = 0; i < 4; i++) {
      Tetromino cur_fig = data->cur_figure;
      Tetromino proj = data->projection_cur_figure;
      game_info.field[cur_fig.coords[i].y][cur_fig.coords[i].x] = cur_fig.shape;
      game_info.field[proj.coords[i].y - 1][proj.coords[i].x] = proj.shape;
    }


    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        game_info.next[i][j] = EMPTY;
      }
    }


    for (int i = 0; i < 4; i++) {
      Tetromino next_fig = data->next_figure;
      game_info.next[next_fig.coords[i].y][next_fig.coords[i].x - 2] = next_fig.shape;
    }
  }
  
  return game_info;
}

long long current_time_millis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

GameInfo_t updateCurrentState() {
  const int kMsIntervals[10] = {500, 480, 460, 440, 430,
                                420, 415, 410, 405, 400};

  tetris_data.current_moving_time = current_time_millis();

  long long duration_ms = tetris_data.current_moving_time - tetris_data.last_moving_time;

  if (duration_ms > kMsIntervals[tetris_data.level - 1] && (tetris_data.state == MOVING)) {
    moveDown(&tetris_data);
  }

  if (isCollided(&tetris_data) && (tetris_data.state == MOVING)) {
    attach(&tetris_data);
  }

  int count_filled_lines = deleteFilledLines(&tetris_data);
  if (count_filled_lines) updateScoreLevel(&tetris_data, count_filled_lines);


  return convertTetrisDataToGameInfo(&tetris_data);
}

void startGame(TetrisData *data) {

  if (data->state == GAMEOVER) {
    for (int i = 0; i < 20; i++) {
      free(game_info.field[i]);
    }
    free(game_info.field);
    game_info.field = NULL;

    for (int i = 0; i < 4; i++) {
      free(game_info.next[i]);
    }
    free(game_info.next);
    game_info.next = NULL;
  }

  game_info.field = (int**)calloc(20, sizeof(int*));
  for (int i = 0; i < 20; i++) {
    game_info.field[i] = (int*)calloc(10, sizeof(int));
  }

  game_info.next = (int**)calloc(4, sizeof(int*));
  for (int i = 0; i < 4; i++) {
    game_info.next[i] = (int*)calloc(4, sizeof(int));
  }


  setDefaultValue();
  spawn(data);
}

void spawn(TetrisData *data) {
  data->cur_figure = data->next_figure;
  setRandomFigure(&data->next_figure);

  setProjection(data);

  data->state = MOVING;

  if (isCollided(data)) data->state = GAMEOVER;
}

void exitState(TetrisData *data) {
  if (data->best_score == data->score && data->score > 0) {
    saveBestScore("best_score_tetris.txt", data->best_score);
  }
  setDefaultValue();

  if (game_info.field) {
    for (int i = 0; i < 20; i++) {
      free(game_info.field[i]);
    }
    free(game_info.field);
    game_info.field = NULL;
  }
  if (game_info.next) {
    for (int i = 0; i < 4; i++) {
      free(game_info.next[i]);
    }
    free(game_info.next);
    game_info.next = NULL;
  }

}

void pause_state(TetrisData *data) { data->state = PAUSE; }
void unpause(TetrisData *data) { data->state = MOVING; }

void rotate(TetrisData *data) {
  if (data->cur_figure.shape != O) {
    Tetromino tmp_figure = data->cur_figure;
    Coordinate center = tmp_figure.coords[1];

    for (int i = 0; i < 4; ++i) {
      int newX = tmp_figure.coords[i].y - center.y;
      int newY = tmp_figure.coords[i].x - center.x;

      tmp_figure.coords[i].x = center.x - newX;
      tmp_figure.coords[i].y = center.y + newY;
    }

    int can_rotate = 1;
    for (int i = 0; i < 4; i++) {
      if (tmp_figure.coords[i].x < 0 || tmp_figure.coords[i].x > 9 ||
          tmp_figure.coords[i].y > 19 || tmp_figure.coords[i].y < 0 ||
          data->field[tmp_figure.coords[i].y][tmp_figure.coords[i].x].is_fill) {
        can_rotate = 0;
        break;
      }
    }

    if (can_rotate) data->cur_figure = tmp_figure;
    setProjection(data);
  }

  data->state = MOVING;
}

void moveRight(TetrisData *data) {
  if (canMove(data, &data->cur_figure, RIGHT)) {
    for (int i = 0; i < 4; i++) {
      data->cur_figure.coords[i].x += 1;
    }
    setProjection(data);
  }

  data->state = MOVING;
}

void moveDown(TetrisData *data) {
  if (canMove(data, &data->cur_figure, DOWN)) {
    data->last_moving_time = current_time_millis();

    moveDownFigure(&data->cur_figure);
  }
  data->state = MOVING;
}

void moveDownFigure(Tetromino *figure) {
  for (int i = 0; i < 4; i++) {
    figure->coords[i].y += 1;
  }
}

void moveLeft(TetrisData *data) {
  if (canMove(data, &data->cur_figure, LEFT)) {
    for (int i = 0; i < 4; i++) {
      data->cur_figure.coords[i].x -= 1;
    }
    setProjection(data);
  }

  data->state = MOVING;
}

void drop(TetrisData *data) {
  while (canMove(data, &data->cur_figure, DOWN)) {
    moveDownFigure(&data->cur_figure);
  }
  attach(data);
}

void attach(TetrisData *data) {
  for (int i = 0; i < 4; i++) {
    data->field[data->cur_figure.coords[i].y - 1][data->cur_figure.coords[i].x]
        .is_fill = 1;
    data->field[data->cur_figure.coords[i].y - 1][data->cur_figure.coords[i].x]
        .shape = data->cur_figure.shape;
  }
  spawn(data);
}

int deleteFilledLines(TetrisData *data) {
  int count_filled_lines = 0;
  for (int i = 0; i < 20; i++) {
    int filled_line = 1;
    for (int j = 0; j < 10; j++) {
      if (data->field[i][j].is_fill == 0) {
        filled_line = 0;
        break;
      }
    }
    if (filled_line) {
      for (int k = i; k > 0; --k) {
        memcpy(data->field[k], data->field[k - 1], sizeof(Cell) * 10);
      }

      for (int f = 0; f < 10; ++f) {
        data->field[0][f].is_fill = 0;
        data->field[0][f].shape = EMPTY;
      }
      count_filled_lines++;
    }
  }

  return count_filled_lines;
}

void updateScoreLevel(TetrisData *data, int count_filled_lines) {
  if (count_filled_lines == 1)
    data->score += 100;
  else if (count_filled_lines == 2)
    data->score += 300;
  else if (count_filled_lines == 3)
    data->score += 700;
  else if (count_filled_lines == 4)
    data->score += 1500;
  else
    data->score += 3000;

  if (data->level < 10 && data->score >= 600)
    data->level = 1 + data->score / 600;

  if (data->best_score < data->score) data->best_score = data->score;
}

void setProjection(TetrisData *data) {
  data->projection_cur_figure = data->cur_figure;
  do {
    moveDownFigure(&data->projection_cur_figure);
  } while (canMove(data, &data->projection_cur_figure, DOWN));
}

int isCollided(TetrisData *data) {
  int isCollided = 0;
  for (int i = 0; i < 4; i++) {
    if (data->cur_figure.coords[i].y == 20 ||
        data->field[data->cur_figure.coords[i].y][data->cur_figure.coords[i].x]
            .is_fill) {
      isCollided = 1;
      break;
    }
  }
  return isCollided;
}

int canMove(TetrisData *data, Tetromino *figure, Direction dir) {
  int canMove = 1;

  if (dir == DOWN) {
    for (int i = 0; i < 4; i++) {
      if (figure->coords[i].y == 20 ||
          data->field[figure->coords[i].y][figure->coords[i].x].is_fill) {
            canMove = 0;
            break;
          }
    }
  } else if (dir == RIGHT) {
    for (int i = 0; i < 4; i++) {
      if (figure->coords[i].y == 20 || figure->coords[i].x >= 9 ||
          data->field[figure->coords[i].y][figure->coords[i].x + 1].is_fill) {
        canMove = 0;
        break;
      }
    }
  } else if (dir == LEFT) {
    for (int i = 0; i < 4; i++) {
      if (figure->coords[i].y == 20 || figure->coords[i].x <= 0 ||
          data->field[figure->coords[i].y][figure->coords[i].x - 1].is_fill) {
        canMove = 0;
        break;
      }
    }
  }

  return canMove;
}