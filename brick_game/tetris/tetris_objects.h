#ifndef OBJECTCS_H
#define OBJECTCS_H

typedef enum { EMPTY = 0, O, I, S, Z, T, L, J } ShapeFigure;

typedef enum { UP = 0, RIGHT, DOWN, LEFT } Direction;

typedef enum { START = 0, MOVING, PAUSE, GAMEOVER, EXIT } TetrisState;

typedef struct {
  int x;
  int y;
} Coordinate;

typedef struct {
  int is_fill;
  ShapeFigure shape;
} Cell;

typedef struct {
  Coordinate coords[4];
  ShapeFigure shape;
} Tetromino;

typedef struct {
  int level;
  int score;
  int best_score;
  TetrisState state;

  Cell field[20][10];
  Tetromino cur_figure;
  Tetromino projection_cur_figure;
  Tetromino next_figure;

  long long current_moving_time;
  long long last_moving_time;

} TetrisData;

#endif