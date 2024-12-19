#ifndef BACKEND_H
#define BACKEND_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../library_specification.h"
#include "tetris_objects.h"

typedef void (*Action_f)(TetrisData *data);

void startGame(TetrisData *data);
void spawn(TetrisData *data);
void exitState(TetrisData *data);
void rotate(TetrisData *data);
void moveRight(TetrisData *data);
void moveDown(TetrisData *data);
void moveDownFigure(Tetromino *figure);
void moveLeft(TetrisData *data);
void pause_state(TetrisData *data);
void unpause(TetrisData *data);
void drop(TetrisData *data);
void attach(TetrisData *data);

void userInput(UserAction_t action, bool hold);
GameInfo_t convertTetrisDataToGameInfo(TetrisData *data);

GameInfo_t updateCurrentState();
TetrisData *getTetrisData();
GameInfo_t *getGameInfo();

void setRandomFigure(Tetromino *figure);
void setDefaultValue();
void setProjection(TetrisData *data);
int isCollided(TetrisData *data);
int canMove(TetrisData *data, Tetromino *figure, Direction dir);
int deleteFilledLines(TetrisData *data);
void updateScoreLevel(TetrisData *data, int count_filled_lines);

int loadBestScore(const char *filename);
void saveBestScore(const char *filename, int best_score);

long long current_time_millis();

#endif