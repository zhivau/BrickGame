#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>
#include <stdlib.h>

#include "../../brick_game/library_specification.h"

#define ESCAPE 27
#define ENTER_KEY 10

typedef GameInfo_t (*UpdateCurrentStateFunc)();
typedef void (*UserInputFunc)(UserAction_t, bool);

UserAction_t getAction();

void printInteface(int level, int score, int best_score, int speed);
void printField(GameInfo_t* data);
void printRectangle(int top_y, int bottom_y, int left_x, int right_x);
void printStart();
void printPause();
void printGameover();

void menuRender(int selected);
void startMenuLoop();
void startGameLoop(const char* lib_name);

#endif